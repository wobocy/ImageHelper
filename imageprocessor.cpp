#include "imageprocessor.h"
#include <QDebug>
#include <QApplication>
#include <cmath>

ImageProcessor::ImageProcessor(QObject *parent)
    : QObject(parent)
    , m_targetFileSize(0)
    , m_quality(90)
    , m_imageLoaded(false)
    , m_processing(false)
{
    m_processTimer = new QTimer(this);
    m_processTimer->setSingleShot(true);
    m_processTimer->setInterval(100);
    connect(m_processTimer, &QTimer::timeout, this, &ImageProcessor::processImage);
}

ImageProcessor::~ImageProcessor()
{
}

bool ImageProcessor::loadImage(const QString &filePath)
{
    QMutexLocker locker(&m_mutex);
    
    QImageReader reader(filePath);
    if (!reader.canRead()) {
        emit errorOccurred("无法读取图片文件");
        return false;
    }
    
    m_originalImage = reader.read();
    if (m_originalImage.isNull()) {
        emit errorOccurred("图片文件格式不支持或已损坏");
        return false;
    }
    
    // 保存原始数据
    m_originalPixmap = QPixmap::fromImage(m_originalImage);
    m_currentImage = m_originalImage;
    m_currentPixmap = m_originalPixmap;
    
    // 获取文件信息
    QFileInfo fileInfo(filePath);
    m_currentFilePath = filePath;
    m_imageFormat = fileInfo.suffix().toUpper();
    
    // 初始化参数
    m_targetSize = m_originalImage.size();
    m_quality = 90;
    m_targetFileSize = fileInfo.size() / 1024; // 转换为KB
    
    m_imageLoaded = true;
    updateImageInfo();
    
    emit imageProcessed();
    return true;
}

bool ImageProcessor::saveImage(const QString &filePath)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_imageLoaded) {
        emit errorOccurred("没有加载图片");
        return false;
    }
    
    // 依据当前选择的保存格式与质量写出
    QString fmt = m_imageFormat.isEmpty() ? QString("JPEG") : m_imageFormat.toUpper();
    QImageWriter writer(filePath, fmt.toUtf8());
    writer.setQuality(qBound(1, m_quality, 100));
    
    if (!writer.write(m_currentImage)) {
        emit errorOccurred(QString("保存失败: %1").arg(writer.errorString()));
        return false;
    }
    
    return true;
}

void ImageProcessor::resizeImage(const QSize &newSize)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_imageLoaded || newSize.isEmpty()) {
        return;
    }
    
    m_targetSize = newSize;
    m_processTimer->start(); // 延迟处理
}

void ImageProcessor::setTargetFileSize(int targetSizeKB)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_imageLoaded || targetSizeKB <= 0) {
        return;
    }
    
    m_targetFileSize = targetSizeKB;
    m_processTimer->start();
}

void ImageProcessor::setQuality(int quality)
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_imageLoaded) {
        return;
    }
    
    m_quality = qBound(1, quality, 100);
    m_processTimer->start();
}

void ImageProcessor::resetToOriginal()
{
    QMutexLocker locker(&m_mutex);
    
    if (!m_imageLoaded) {
        return;
    }
    
    m_currentImage = m_originalImage;
    m_currentPixmap = m_originalPixmap;
    m_targetSize = m_originalImage.size();
    m_quality = 90;
    
    updateImageInfo();
    emit imageProcessed();
}

void ImageProcessor::processImage()
{
    qDebug() << "ImageProcessor::processImage() - 开始处理";
    
    // 首先获取锁以避免竞态条件
    QMutexLocker locker(&m_mutex);
    
    if (m_processing || !m_imageLoaded) {
        qDebug() << "ImageProcessor::processImage() - 跳过处理：处理中或未加载图片";
        return;
    }
    
    m_processing = true;
    qDebug() << "ImageProcessor::processImage() - 设置处理标志为true";
    
    // 在锁外发射信号，避免死锁
    locker.unlock();
    emit processingStarted();
    locker.relock();
    
    try {
        qDebug() << "ImageProcessor::processImage() - 原始图片尺寸:" << m_originalImage.size();
        qDebug() << "ImageProcessor::processImage() - 目标图片尺寸:" << m_targetSize;
        
        // 1. 首先调整尺寸
        if (m_targetSize != m_originalImage.size()) {
            qDebug() << "ImageProcessor::processImage() - 调整图片尺寸";
            m_currentImage = scaleImageToSize(m_originalImage, m_targetSize);
            qDebug() << "ImageProcessor::processImage() - 调整后尺寸:" << m_currentImage.size();
        } else {
            qDebug() << "ImageProcessor::processImage() - 尺寸相同，无需调整";
            m_currentImage = m_originalImage;
        }
        
        // 验证图像是否有效
        if (m_currentImage.isNull()) {
            qWarning() << "ImageProcessor::processImage() - 调整后图像为空";
            throw std::runtime_error("图像调整失败，结果为空");
        }
        
        // 2. 按目标文件大小压缩：仅调节质量，不自动缩小分辨率
        if (m_targetFileSize > 0) {
            const qint64 targetBytes = static_cast<qint64>(m_targetFileSize) * 1024;

            // 始终基于目标尺寸的新图进行压缩估算
            QImage workingImage = (m_targetSize != m_originalImage.size())
                                  ? scaleImageToSize(m_originalImage, m_targetSize)
                                  : m_originalImage;

            // 只使用 JPEG 进行目标大小控制（可预期、可控）
            const QString format = "JPEG";

            // 二分查找质量
            int lo = 1, hi = 100;
            int bestQ = 1;
            qint64 bestSize = 0;
            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                qint64 est = estimateFileSize(workingImage, mid, format);
                if (est == 0) break;

                if (est <= targetBytes) {
                    bestQ = mid;
                    bestSize = est;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }

            // 如果最低质量依然超标，给出提示并采用最低质量结果
            qint64 minSizeAtQ1 = estimateFileSize(workingImage, 1, format);
            if (minSizeAtQ1 > 0 && minSizeAtQ1 > targetBytes) {
                // 生成质量=1的结果用于展示
                m_currentImage = workingImage;
                m_quality = 1;
                m_imageFormat = "JPEG";

                // 在锁外提示，避免阻塞
                locker.unlock();
                const qint64 kb = (minSizeAtQ1 + 1023) / 1024;
                emit errorOccurred(QString("在当前长宽像素下，最小可压缩到约 %1 KB，无法达到目标大小。请降低分辨率或提高目标大小。").arg(kb));
                locker.relock();
            } else {
                // 正常采用二分结果
                m_currentImage = workingImage;
                m_quality = qBound(1, bestQ, 100);
                m_imageFormat = "JPEG";
            }
        }
        
        // 3. 更新pixmap
        qDebug() << "ImageProcessor::processImage() - 更新pixmap";
        m_currentPixmap = QPixmap::fromImage(m_currentImage);
        
        // 验证pixmap是否有效
        if (m_currentPixmap.isNull()) {
            qWarning() << "ImageProcessor::processImage() - pixmap转换失败";
            throw std::runtime_error("pixmap转换失败");
        }
        
        // 4. 更新信息
        qDebug() << "ImageProcessor::processImage() - 更新图像信息";
        updateImageInfo();
        
        m_processing = false;
        qDebug() << "ImageProcessor::processImage() - 设置处理标志为false";
        
        // 在锁外发射信号，避免死锁
        locker.unlock();
        qDebug() << "ImageProcessor::processImage() - 发射processingFinished信号";
        emit processingFinished();
        qDebug() << "ImageProcessor::processImage() - 发射imageProcessed信号";
        emit imageProcessed();
        
    } catch (const std::exception &e) {
        m_processing = false;
        qWarning() << "ImageProcessor::processImage() - 捕获异常:" << e.what();
        
        // 在锁外发射信号，避免死锁
        locker.unlock();
        emit processingFinished();
        emit errorOccurred(QString("处理图片时发生错误: %1").arg(e.what()));
    }
    
    qDebug() << "ImageProcessor::processImage() - 处理完成";
}

ImageInfo ImageProcessor::getImageInfo() const
{
    return m_imageInfo;
}

QPixmap ImageProcessor::getCurrentPixmap() const
{
    return m_currentPixmap;
}

QPixmap ImageProcessor::getOriginalPixmap() const
{
    return m_originalPixmap;
}

void ImageProcessor::updateImageInfo()
{
    if (!m_imageLoaded) {
        return;
    }
    
    m_imageInfo.originalSize = m_originalImage.size();
    m_imageInfo.currentSize = m_currentImage.size();
    m_imageInfo.format = m_imageFormat;
    m_imageInfo.quality = m_quality;
    
    // 计算缩放比例
    double scaleX = (double)m_imageInfo.currentSize.width() / m_imageInfo.originalSize.width();
    double scaleY = (double)m_imageInfo.currentSize.height() / m_imageInfo.originalSize.height();
    m_imageInfo.scaleRatio = (scaleX + scaleY) / 2.0 * 100.0; // 转换为百分比
    
    // 计算像素数量
    m_imageInfo.pixelCount = (qint64)m_imageInfo.currentSize.width() * m_imageInfo.currentSize.height();
    
    // 更准确地计算内存占用，根据图像格式和位深度
    int bytesPerPixel = 0;
    switch (m_currentImage.format()) {
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
        bytesPerPixel = 0.125; // 每像素1位
        break;
    case QImage::Format_Indexed8:
        bytesPerPixel = 1; // 每像素8位
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
        bytesPerPixel = 4; // 每像素32位
        break;
    case QImage::Format_RGB16:
        bytesPerPixel = 2; // 每像素16位
        break;
    default:
        bytesPerPixel = 3; // 默认RGB格式，每像素24位
        break;
    }
    m_imageInfo.memoryUsage = static_cast<qint64>(m_imageInfo.pixelCount * bytesPerPixel);
    
    // 获取原始文件大小
    if (!m_currentFilePath.isEmpty()) {
        QFileInfo fileInfo(m_currentFilePath);
        m_imageInfo.originalFileSize = fileInfo.size();
    }
    
    // 估算当前文件大小 - 优化估算逻辑
    QString format = m_imageFormat.toLower() == "png" ? "PNG" : "JPEG";
    m_imageInfo.currentFileSize = estimateFileSize(m_currentImage, m_quality, format);
    
    qDebug() << "ImageProcessor::updateImageInfo() - 更新图像信息";
    qDebug() << "  - 原始尺寸:" << m_imageInfo.originalSize;
    qDebug() << "  - 当前尺寸:" << m_imageInfo.currentSize;
    qDebug() << "  - 缩放比例:" << m_imageInfo.scaleRatio;
    qDebug() << "  - 像素数量:" << m_imageInfo.pixelCount;
    qDebug() << "  - 内存占用:" << m_imageInfo.memoryUsage << "字节";
    qDebug() << "  - 原始文件大小:" << m_imageInfo.originalFileSize << "字节";
    qDebug() << "  - 当前文件大小(估算):" << m_imageInfo.currentFileSize << "字节";
    qDebug() << "  - 图像格式:" << m_imageInfo.format;
    qDebug() << "  - 质量:" << m_imageInfo.quality;
}

QImage ImageProcessor::scaleImageToSize(const QImage &image, const QSize &targetSize) const
{
    if (image.isNull() || targetSize.isEmpty()) {
        return image;
    }
    
    return image.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

qint64 ImageProcessor::estimateFileSize(const QImage &image, int quality, const QString &format)
{
    if (image.isNull()) {
        return 0;
    }
    
    // 如果图像很小，直接返回估算值以提高性能
    qint64 pixelCount = (qint64)image.width() * image.height();
    if (pixelCount < 1000000) { // 小于100万像素
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        
        QImageWriter writer(&buffer, format.toUtf8());
        writer.setQuality(quality);
        
        if (writer.write(image)) {
            qDebug() << "ImageProcessor::estimateFileSize() - 直接写入计算大小:" << buffer.size() << "字节";
            return buffer.size();
        }
        
        return 0;
    }
    
    // 对于大图像，使用采样方式估算以避免内存问题
    // 创建一个缩小版本的图像用于估算
    QSize sampleSize;
    if (image.width() > image.height()) {
        sampleSize = QSize(1000, static_cast<int>(image.height() * 1000.0 / image.width()));
    } else {
        sampleSize = QSize(static_cast<int>(image.width() * 1000.0 / image.height()), 1000);
    }
    
    QImage sampleImage = image.scaled(sampleSize, Qt::KeepAspectRatio, Qt::FastTransformation);
    
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    
    QImageWriter writer(&buffer, format.toUtf8());
    writer.setQuality(quality);
    
    if (writer.write(sampleImage)) {
        qint64 sampleSizeBytes = buffer.size();
        // 根据像素比例估算原始大小
        double scaleFactor = static_cast<double>(pixelCount) / (sampleSize.width() * sampleSize.height());
        qint64 estimatedSize = static_cast<qint64>(sampleSizeBytes * scaleFactor);
        
        qDebug() << "ImageProcessor::estimateFileSize() - 采样估算大小:" << estimatedSize << "字节";
        qDebug() << "  - 采样图像尺寸:" << sampleSize;
        qDebug() << "  - 采样大小字节:" << sampleSizeBytes;
        qDebug() << "  - 缩放因子:" << scaleFactor;
        
        return estimatedSize;
    }
    
    return 0;
}

int ImageProcessor::findOptimalQuality(const QImage &image, int targetSizeKB, const QString &format)
{
    if (image.isNull() || targetSizeKB <= 0) {
        return -1;
    }
    
    qint64 targetBytes = targetSizeKB * 1024;
    
    // 二分查找最优质量
    int minQuality = 1;
    int maxQuality = 100;
    int bestQuality = 90;
    qint64 bestDiff = LLONG_MAX;
    
    while (minQuality <= maxQuality) {
        int midQuality = (minQuality + maxQuality) / 2;
        qint64 estimatedSize = estimateFileSize(image, midQuality, format);
        
        if (estimatedSize == 0) {
            break; // 估算失败
        }
        
        qint64 diff = qAbs(estimatedSize - targetBytes);
        if (diff < bestDiff) {
            bestDiff = diff;
            bestQuality = midQuality;
        }
        
        if (estimatedSize > targetBytes) {
            maxQuality = midQuality - 1;
        } else if (estimatedSize < targetBytes) {
            minQuality = midQuality + 1;
        } else {
            return midQuality; // 完美匹配
        }
    }
    
    return bestQuality;
}

QString ImageProcessor::formatFileSize(qint64 bytes)
{
    const qint64 KB = 1024;
    const qint64 MB = KB * 1024;
    const qint64 GB = MB * 1024;
    
    if (bytes >= GB) {
        return QString("%1 GB").arg(bytes / (double)GB, 0, 'f', 2);
    } else if (bytes >= MB) {
        return QString("%1 MB").arg(bytes / (double)MB, 0, 'f', 2);
    } else if (bytes >= KB) {
        return QString("%1 KB").arg(bytes / (double)KB, 0, 'f', 1);
    } else {
        return QString("%1 B").arg(bytes);
    }
}