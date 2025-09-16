#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
#include <QDateTime>

class ImageViewer : public QLabel
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    
    void setImage(const QPixmap &pixmap);
    void resetView();
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void resetPan();
    void resetTransform();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updatePixmap();
    void fitInView();
    
    QPixmap m_originalPixmap;
    QPixmap m_scaledPixmap;
    
    bool m_isPanning;
    QPoint m_lastPanPoint;
    QPoint m_panOffset;
    
    double m_zoomFactor;
    double m_minZoom;
    double m_maxZoom;
    
    bool m_imageLoaded;
    qint64 m_lastUpdateTime;
};

#endif // IMAGEVIEWER_H