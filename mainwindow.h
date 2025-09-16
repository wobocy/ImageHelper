#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QSplitter>
#include <QTimer>
#include <QProgressBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QPixmap>
#include <QResizeEvent>

#include "imageprocessor.h"
#include "imageviewer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openImage();
    void saveImage();
    void resetImage();
    void generateImage();
    void onWidthChanged();
    void onHeightChanged();
    void onTargetSizeChanged();
    void onLockAspectRatioChanged();
    void updateImagePreview();
    void onImageProcessed();
    void onErrorOccurred(const QString &errorMessage);

private:
    void setupUI();
    void connectSignals();
    void updateImageDisplay();
    void updatePropertyInfo();
    void resetControls();
    void setControlsEnabled(bool enabled);
    
    // UI对象
    Ui::MainWindow *ui;
    ImageViewer *m_imageViewer;
    
    // 状态栏
    QProgressBar *m_progressBar;
    
    // 核心处理类
    ImageProcessor *m_imageProcessor;
    
    // 更新定时器（防抖动）
    QTimer *m_updateTimer;
    
    // 状态变量
    bool m_imageLoaded;
    bool m_lockAspectRatio;
    QString m_currentImagePath;
    bool m_isGenerating;
    bool m_showProcessedSize;
};

#endif // MAINWINDOW_H