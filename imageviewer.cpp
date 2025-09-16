#include "imageviewer.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

ImageViewer::ImageViewer(QWidget *parent)
    : QLabel(parent)
    , m_isPanning(false)
    , m_zoomFactor(1.0)
    , m_minZoom(0.1)
    , m_maxZoom(10.0)
    , m_imageLoaded(false)
{
    setAlignment(Qt::AlignCenter);
    setMouseTracking(true);
    setMinimumSize(400, 300);
    setStyleSheet("QLabel { background-color: white; border: 1px dashed #ccc; }");
    setText(tr("点击\"打开图片\"或拖拽图片到此处"));
}

void ImageViewer::setImage(const QPixmap &pixmap)
{
    if (pixmap.isNull()) {
        m_imageLoaded = false;
        setText(tr("无法加载图片"));
        return;
    }

    m_originalPixmap = pixmap;
    m_imageLoaded = true;
    m_zoomFactor = 1.0;
    m_panOffset = QPoint(0, 0);
    updatePixmap();
}

void ImageViewer::resetView()
{
    if (!m_imageLoaded) return;
    m_zoomFactor = 1.0;
    m_panOffset = QPoint(0, 0);
    updatePixmap();
}

void ImageViewer::mousePressEvent(QMouseEvent *event)
{
    if (!m_imageLoaded || event->button() != Qt::LeftButton) {
        QLabel::mousePressEvent(event);
        return;
    }

    m_isPanning = true;
    m_lastPanPoint = event->pos();
    setCursor(Qt::ClosedHandCursor);
    event->accept();
}

void ImageViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_imageLoaded || !m_isPanning) {
        QLabel::mouseMoveEvent(event);
        return;
    }

    QPoint delta = event->pos() - m_lastPanPoint;
    m_panOffset += delta;
    m_lastPanPoint = event->pos();
    updatePixmap();
    event->accept();
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isPanning) {
        m_isPanning = false;
        setCursor(Qt::OpenHandCursor);
        event->accept();
    } else {
        QLabel::mouseReleaseEvent(event);
    }
}

void ImageViewer::wheelEvent(QWheelEvent *event)
{
    if (!m_imageLoaded) {
        event->ignore();
        return;
    }

    const QPoint angle = event->angleDelta();
    if (angle.isNull()) {
        event->ignore();
        return;
    }

    const double scale = angle.y() > 0 ? 1.1 : 0.9;
    const double oldZoom = m_zoomFactor;
    m_zoomFactor = qBound(m_minZoom, m_zoomFactor * scale, m_maxZoom);

    if (oldZoom != m_zoomFactor) {
        const QPointF mousePos = event->position();
        const QPointF imgCenter = QRectF(rect()).center();
        const QPointF mouseOffset = mousePos - imgCenter;
        m_panOffset = (m_panOffset + mouseOffset.toPoint()) * (m_zoomFactor / oldZoom) - mouseOffset.toPoint();
        updatePixmap();
    }
    
    event->accept();
}

void ImageViewer::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    if (m_imageLoaded) {
        updatePixmap();
    }
}

void ImageViewer::updatePixmap()
{
    qDebug() << "ImageViewer::updatePixmap() - 开始更新显示图像";
    
    if (!m_imageLoaded || m_originalPixmap.isNull()) {
        qDebug() << "ImageViewer::updatePixmap() - 跳过更新：未加载图像或原图为空";
        return;
    }

    qDebug() << "ImageViewer::updatePixmap() - 原始图像尺寸:" << m_originalPixmap.size();
    qDebug() << "ImageViewer::updatePixmap() - 当前缩放因子:" << m_zoomFactor;
    qDebug() << "ImageViewer::updatePixmap() - 当前偏移量:" << m_panOffset;
    
    // 计算缩放后的尺寸
    QSize scaledSize = m_originalPixmap.size() * m_zoomFactor;
    qDebug() << "ImageViewer::updatePixmap() - 缩放后尺寸(计算):" << scaledSize;
    
    // 添加最大尺寸限制，避免处理过大图像导致内存问题
    const int MAX_PIXEL_COUNT = 100000000; // 1亿像素
    qint64 pixelCount = (qint64)scaledSize.width() * scaledSize.height();
    qDebug() << "ImageViewer::updatePixmap() - 计算像素数:" << pixelCount;
    
    if (pixelCount > MAX_PIXEL_COUNT) {
        qDebug() << "ImageViewer::updatePixmap() - 像素数超过限制，需要进一步缩放";
        double scaleDownFactor = sqrt((double)MAX_PIXEL_COUNT / pixelCount);
        qDebug() << "ImageViewer::updatePixmap() - 缩放因子:" << scaleDownFactor;
        scaledSize = scaledSize * scaleDownFactor;
        qDebug() << "ImageViewer::updatePixmap() - 缩放后尺寸(调整后):" << scaledSize;
    }
    
    // 使用更高效的转换模式
    Qt::TransformationMode transformMode = (scaledSize.width() > m_originalPixmap.width() || scaledSize.height() > m_originalPixmap.height()) 
                                          ? Qt::SmoothTransformation : Qt::FastTransformation;
    qDebug() << "ImageViewer::updatePixmap() - 使用转换模式:" << (transformMode == Qt::SmoothTransformation ? "SmoothTransformation" : "FastTransformation");
    
    // 验证缩放尺寸是否有效
    if (scaledSize.width() <= 0 || scaledSize.height() <= 0) {
        qWarning() << "ImageViewer::updatePixmap() - 无效的缩放尺寸:" << scaledSize;
        return;
    }
    
    qDebug() << "ImageViewer::updatePixmap() - 开始图像缩放操作";
    QPixmap scaled = m_originalPixmap.scaled(scaledSize, Qt::KeepAspectRatio, transformMode);
    
    // 验证缩放后的图像是否有效
    if (scaled.isNull()) {
        qWarning() << "ImageViewer::updatePixmap() - 图像缩放失败，结果为空";
        return;
    }
    qDebug() << "ImageViewer::updatePixmap() - 缩放后图像尺寸:" << scaled.size();

    qDebug() << "ImageViewer::updatePixmap() - 创建显示缓冲区，尺寸:" << size();
    QPixmap display(size());
    display.fill(Qt::white);
    
    QPoint drawPos = rect().center() - QPoint(scaled.width()/2, scaled.height()/2) + m_panOffset;
    qDebug() << "ImageViewer::updatePixmap() - 绘制位置:" << drawPos;
    
    // 检查绘制位置是否完全在视口外，如果是则不绘制
    if (drawPos.x() > width() || drawPos.y() > height() || 
        drawPos.x() + scaled.width() < 0 || drawPos.y() + scaled.height() < 0) {
        qDebug() << "ImageViewer::updatePixmap() - 图像完全在视口外，跳过绘制";
        setPixmap(display);
        return;
    }

    qDebug() << "ImageViewer::updatePixmap() - 开始绘制图像";
    QPainter painter(&display);
    painter.drawPixmap(drawPos, scaled);
    setPixmap(display);
    
    qDebug() << "ImageViewer::updatePixmap() - 图像更新完成";
}

void ImageViewer::zoomIn()
{
    if (!m_imageLoaded) return;
    m_zoomFactor = qMin(m_zoomFactor * 1.25, m_maxZoom);
    updatePixmap();
}

void ImageViewer::zoomOut()
{
    if (!m_imageLoaded) return;
    m_zoomFactor = qMax(m_zoomFactor * 0.8, m_minZoom);
    updatePixmap();
}

void ImageViewer::resetZoom()
{
    if (!m_imageLoaded) return;
    m_zoomFactor = 1.0;
    updatePixmap();
}

void ImageViewer::resetPan()
{
    if (!m_imageLoaded) return;
    m_panOffset = QPoint(0, 0);
    updatePixmap();
}

void ImageViewer::resetTransform()
{
    if (!m_imageLoaded) return;
    m_zoomFactor = 1.0;
    m_panOffset = QPoint(0, 0);
    updatePixmap();
}

void ImageViewer::fitInView()
{
    if (!m_imageLoaded || m_originalPixmap.isNull()) {
        return;
    }

    double widthRatio = (double)width() / m_originalPixmap.width();
    double heightRatio = (double)height() / m_originalPixmap.height();
    m_zoomFactor = qMin(widthRatio, heightRatio) * 0.9;
    resetPan();
    updatePixmap();
}