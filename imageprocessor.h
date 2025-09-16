#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QPixmap>
#include <QImage>
#include <QBuffer>
#include <QImageReader>
#include <QImageWriter>
#include <QFileInfo>
#include <QThread>
#include <QMutex>
#include <QTimer>

struct ImageInfo {
    QSize originalSize;
    QSize currentSize;
    qint64 originalFileSize;
    qint64 currentFileSize;
    QString format;
    int quality;
    double scaleRatio;
    qint64 pixelCount;
    qint64 memoryUsage;
};

class ImageProcessor : public QObject
{
    Q_OBJECT

public:
    explicit ImageProcessor(QObject *parent = nullptr);
    ~ImageProcessor();

    // 图片加载和保存
    bool loadImage(const QString &filePath);
    bool saveImage(const QString &filePath);
    
    // 图片处理
    void resizeImage(const QSize &newSize);
    void setTargetFileSize(int targetSizeKB);
    void setQuality(int quality);
    void resetToOriginal();
    
    // 获取图片信息
    ImageInfo getImageInfo() const;
    QPixmap getCurrentPixmap() const;
    QPixmap getOriginalPixmap() const;
    
    // 实用函数
    static qint64 estimateFileSize(const QImage &image, int quality, const QString &format = "JPEG");
    static int findOptimalQuality(const QImage &image, int targetSizeKB, const QString &format = "JPEG");
    static QString formatFileSize(qint64 bytes);
    
    bool isImageLoaded() const { return m_imageLoaded; }
    
signals:
    void imageProcessed();
    void processingStarted();
    void processingFinished();
    void errorOccurred(const QString &error);

private slots:
    void processImage();

private:
    void updateImageInfo();
    QImage scaleImageToSize(const QImage &image, const QSize &targetSize) const;
    
    // 图片数据
    QPixmap m_originalPixmap;
    QPixmap m_currentPixmap;
    QImage m_originalImage;
    QImage m_currentImage;
    
    // 图片信息
    ImageInfo m_imageInfo;
    QString m_currentFilePath;
    QString m_imageFormat;
    
    // 处理参数
    QSize m_targetSize;
    int m_targetFileSize; // KB
    int m_quality;
    
    // 状态
    bool m_imageLoaded;
    bool m_processing;
    
    // 线程安全
    QMutex m_mutex;
    QTimer *m_processTimer;
};

#endif // IMAGEPROCESSOR_H