#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_imageProcessor(nullptr)
    , m_updateTimer(nullptr)
    , m_imageLoaded(false)
    , m_lockAspectRatio(true)
    , m_isGenerating(false)
    , m_showProcessedSize(false)
{
    ui->setupUi(this);
    
    // 初始化核心组件
    m_imageProcessor = new ImageProcessor(this);
    m_updateTimer = new QTimer(this);
    m_updateTimer->setSingleShot(true);
    m_updateTimer->setInterval(300); // 300ms防抖动
    
    // 设置UI
    setupUI();
    connectSignals();
    
    // 连接错误信号
    connect(m_imageProcessor, &ImageProcessor::errorOccurred, this, &MainWindow::onErrorOccurred);
    
    // 初始状态
    setControlsEnabled(false);
    
    // 状态栏
    statusBar()->showMessage("准备就绪 - 请打开一张图片开始处理");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // 创建图像查看器
    m_imageViewer = new ImageViewer;
    
    // 替换原来的QLabel
    QLayout *layout = ui->scrollAreaWidgetContents->layout();
    QLabel *oldLabel = ui->imageLabel;
    layout->removeWidget(oldLabel);
    delete oldLabel;
    layout->addWidget(m_imageViewer);
    
    // 设置分割器比例
    ui->splitter->setStretchFactor(0, 3); // 图片区域占3/4
    ui->splitter->setStretchFactor(1, 1); // 控制面板占1/4
    
    // 进度条
    m_progressBar = new QProgressBar;
    m_progressBar->setVisible(false);
    statusBar()->addPermanentWidget(m_progressBar);
}



void MainWindow::connectSignals()
{
    // 按钮信号
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::openImage);
    connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::generateImage);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::resetImage);
    
    // 参数调整信号
    connect(ui->widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onWidthChanged);
    connect(ui->heightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onHeightChanged);
    connect(ui->targetSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onTargetSizeChanged);
    connect(ui->lockAspectRatioCheckBox, &QCheckBox::toggled, this, &MainWindow::onLockAspectRatioChanged);
    
    // 图片处理器信号
    connect(m_imageProcessor, &ImageProcessor::imageProcessed, this, &MainWindow::onImageProcessed);
    connect(m_imageProcessor, &ImageProcessor::processingStarted, this, [this]() {
        m_progressBar->setVisible(true);
        m_progressBar->setRange(0, 0); // 不确定进度
    });
    connect(m_imageProcessor, &ImageProcessor::processingFinished, this, [this]() {
        m_progressBar->setVisible(false);
    });
    connect(m_imageProcessor, &ImageProcessor::errorOccurred, this, [this](const QString &error) {
        // 统一由onErrorOccurred决定是否弹窗，这里仅处理进度条
        Q_UNUSED(error);
        m_progressBar->setVisible(false);
    });
    
    // 更新定时器
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateImagePreview);
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "打开图片", "",
        "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif *.tiff);;所有文件 (*.*)");
    
    if (!fileName.isEmpty()) {
        if (m_imageProcessor->loadImage(fileName)) {
            m_currentImagePath = fileName;
            m_imageLoaded = true;
            setControlsEnabled(true);
            m_showProcessedSize = false; // 加载后先不显示处理后大小
            
            // 更新控件值
            ImageInfo info = m_imageProcessor->getImageInfo();
            ui->widthSpinBox->setValue(info.originalSize.width());
            ui->heightSpinBox->setValue(info.originalSize.height());
            ui->targetSizeSpinBox->setValue(info.originalFileSize / 1024); // 转换为KB
            
            // 设置图像到查看器
            m_imageViewer->setImage(m_imageProcessor->getOriginalPixmap());
            
            updatePropertyInfo();
            
            statusBar()->showMessage(QString("已加载图片: %1").arg(QFileInfo(fileName).fileName()));
        } else {
            // 加载失败不弹窗，由errorOccurred统一处理，但此处确保状态栏提示
            statusBar()->showMessage("无法加载图片文件！");
        }
    }
}

void MainWindow::saveImage()
{
    if (!m_imageLoaded) return;
    
    // 创建默认文件名：原文件名-resized
    QFileInfo originalFileInfo(m_currentImagePath);
    QString defaultFileName = originalFileInfo.path() + "/" + 
                             originalFileInfo.completeBaseName() + "-resized." + 
                             originalFileInfo.suffix();
    
    QString fileName = QFileDialog::getSaveFileName(this,
        "保存图片", defaultFileName,
        "JPEG 图片 (*.jpg);;PNG 图片 (*.png);;BMP 图片 (*.bmp);;所有文件 (*.*)");
    
    if (!fileName.isEmpty()) {
        if (m_imageProcessor->saveImage(fileName)) {
            statusBar()->showMessage(QString("图片已保存: %1").arg(QFileInfo(fileName).fileName()));
        } else {
            QMessageBox::warning(this, "错误", "保存图片失败！");
        }
    }
}

void MainWindow::resetImage()
{
    if (!m_imageLoaded) return;
    
    m_imageProcessor->resetToOriginal();
    m_showProcessedSize = false;
    
    // 重置控件值
    ImageInfo info = m_imageProcessor->getImageInfo();
    ui->widthSpinBox->setValue(info.originalSize.width());
    ui->heightSpinBox->setValue(info.originalSize.height());
    ui->lockAspectRatioCheckBox->setChecked(true);
    m_lockAspectRatio = true;
    
    // 重置图像查看器
    m_imageViewer->resetView();
    
    updateImageDisplay();
    updatePropertyInfo();
    
    statusBar()->showMessage("图片已复原到原始状态");
}

void MainWindow::generateImage()
{
    if (!m_imageLoaded) return;
    
    // 获取用户设置的参数
    QSize newSize(ui->widthSpinBox->value(), ui->heightSpinBox->value());
    int targetSize = ui->targetSizeSpinBox->value();
    
    // 应用参数
    m_imageProcessor->resizeImage(newSize);
    m_imageProcessor->setTargetFileSize(targetSize);
    // 质量由 ImageProcessor 内部按目标大小自动二分决定
    
    m_isGenerating = true;
    statusBar()->showMessage("正在生成图片...");
}

void MainWindow::onWidthChanged()
{
    if (!m_imageLoaded) return;
    m_showProcessedSize = false;
    
    if (m_lockAspectRatio) {
        // 保持宽高比
        ImageInfo info = m_imageProcessor->getImageInfo();
        double ratio = (double)info.originalSize.height() / info.originalSize.width();
        int newHeight = qRound(ui->widthSpinBox->value() * ratio);
        
        ui->heightSpinBox->blockSignals(true);
        ui->heightSpinBox->setValue(newHeight);
        ui->heightSpinBox->blockSignals(false);
    }
    
    m_updateTimer->start(); // 启动防抖动定时器
}

void MainWindow::onHeightChanged()
{
    if (!m_imageLoaded) return;
    m_showProcessedSize = false;
    
    if (m_lockAspectRatio) {
        // 保持宽高比
        ImageInfo info = m_imageProcessor->getImageInfo();
        double ratio = (double)info.originalSize.width() / info.originalSize.height();
        int newWidth = qRound(ui->heightSpinBox->value() * ratio);
        
        ui->widthSpinBox->blockSignals(true);
        ui->widthSpinBox->setValue(newWidth);
        ui->widthSpinBox->blockSignals(false);
    }
    
    m_updateTimer->start();
}

void MainWindow::onTargetSizeChanged()
{
    if (!m_imageLoaded) return;
    m_showProcessedSize = false;
    m_updateTimer->start();
}



void MainWindow::onLockAspectRatioChanged()
{
    m_lockAspectRatio = ui->lockAspectRatioCheckBox->isChecked();
}

void MainWindow::updateImagePreview()
{
    qDebug() << "MainWindow::updateImagePreview() - 开始更新预览";
    
    if (!m_imageLoaded) {
        qDebug() << "MainWindow::updateImagePreview() - 跳过更新：未加载图像";
        return;
    }
    
    // 实时更新预览（不保存，只是预览效果）
    QSize newSize(ui->widthSpinBox->value(), ui->heightSpinBox->value());
    qDebug() << "MainWindow::updateImagePreview() - 预览尺寸:" << newSize;
    
    // 创建临时处理器进行预览
    QPixmap originalPixmap = m_imageProcessor->getOriginalPixmap();
    
    if (originalPixmap.isNull()) {
        qWarning() << "MainWindow::updateImagePreview() - 原始图像为空";
        return;
    }
    
    // 添加尺寸限制
    const int MAX_PREVIEW_PIXELS = 10000000; // 1千万像素
    qint64 pixelCount = (qint64)newSize.width() * newSize.height();
    
    if (pixelCount > MAX_PREVIEW_PIXELS) {
        double scaleDownFactor = sqrt((double)MAX_PREVIEW_PIXELS / pixelCount);
        newSize = newSize * scaleDownFactor;
        qDebug() << "MainWindow::updateImagePreview() - 预览尺寸过大，调整为:" << newSize;
    }
    
    QPixmap previewPixmap = originalPixmap.scaled(newSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    
    if (previewPixmap.isNull()) {
        qWarning() << "MainWindow::updateImagePreview() - 预览图像生成失败";
        return;
    }
    
    // 更新显示 - 使用m_imageViewer而不是已删除的ui->imageLabel
    qDebug() << "MainWindow::updateImagePreview() - 更新预览显示";
    m_imageViewer->setImage(previewPixmap);
    
    // 更新属性信息（预估）
    qDebug() << "MainWindow::updateImagePreview() - 更新属性信息";
    updatePropertyInfo();
    
    qDebug() << "MainWindow::updateImagePreview() - 预览更新完成";
}

void MainWindow::onImageProcessed()
{
    qDebug() << "MainWindow::onImageProcessed() - 图像处理完成";
    
    // 确保在主线程中执行UI更新
    if (QThread::currentThread() != thread()) {
        qDebug() << "MainWindow::onImageProcessed() - 跨线程调用，切换到主线程";
        QMetaObject::invokeMethod(this, "onImageProcessed", Qt::QueuedConnection);
        return;
    }
    
    updateImageDisplay();
    // 仅在点击生成后，显示处理后大小
    if (m_isGenerating) {
        m_showProcessedSize = true;
    }
    updatePropertyInfo();
    statusBar()->showMessage("图片处理完成");
    
    // 仅在生成流程中弹出成功提示
    if (m_isGenerating) {
        QMessageBox::information(this, "成功", "图片已成功生成！");
        m_isGenerating = false;
    }
    
    qDebug() << "MainWindow::onImageProcessed() - UI更新完成";
}

void MainWindow::onErrorOccurred(const QString &errorMessage)
{
    // 仅在生成流程中弹出失败提示
    if (m_isGenerating) {
        QMessageBox::critical(this, "错误", errorMessage);
        m_isGenerating = false;
    }
    statusBar()->showMessage("图片处理失败");
}

void MainWindow::updateImageDisplay()
{
    qDebug() << "MainWindow::updateImageDisplay() - 开始更新图像显示";
    
    if (!m_imageLoaded) {
        qDebug() << "MainWindow::updateImageDisplay() - 跳过更新：未加载图像";
        return;
    }
    
    // 检查m_imageViewer是否有效
    if (!m_imageViewer) {
        qWarning() << "MainWindow::updateImageDisplay() - 图像查看器为空";
        return;
    }
    
    QPixmap pixmap = m_imageProcessor->getCurrentPixmap();
    
    if (pixmap.isNull()) {
        qWarning() << "MainWindow::updateImageDisplay() - 获取当前图像失败";
        return;
    }
    
    qDebug() << "MainWindow::updateImageDisplay() - 设置图像到查看器，尺寸:" << pixmap.size();
    m_imageViewer->setImage(pixmap);
    
    qDebug() << "MainWindow::updateImageDisplay() - 图像显示更新完成";
}

void MainWindow::updatePropertyInfo()
{
    if (!m_imageLoaded) return;
    
    ImageInfo info = m_imageProcessor->getImageInfo();
    
    // 更新属性显示
    ui->originalSizeLabel->setText(QString("%1 × %2").arg(info.originalSize.width()).arg(info.originalSize.height()));
    ui->currentSizeLabel->setText(QString("%1 × %2").arg(info.currentSize.width()).arg(info.currentSize.height()));
    ui->scaleRatioLabel->setText(QString("%1%").arg(info.scaleRatio, 0, 'f', 1));
    
    // 原始文件大小改为显示当前大小（即加载的文件大小）
    ui->originalFileSizeLabel->setText(ImageProcessor::formatFileSize(info.originalFileSize));
    
    // 处理后大小：仅在生成后显示
    if (m_showProcessedSize && info.currentFileSize > 0) {
        ui->currentFileSizeLabel->setText(ImageProcessor::formatFileSize(info.currentFileSize));
    } else {
        ui->currentFileSizeLabel->setText("--");
    }
    
    // 不再显示压缩质量
    // ui->qualityLabel->setText("--");
    
    ui->pixelCountLabel->setText(QString("%L1").arg(info.pixelCount));
    ui->formatLabel->setText(info.format);
}

void MainWindow::setControlsEnabled(bool enabled)
{
    ui->editGroup->setEnabled(enabled);
    ui->generateButton->setEnabled(enabled);
    ui->saveButton->setEnabled(enabled);
    ui->resetButton->setEnabled(enabled);
}

void MainWindow::resetControls()
{
    ui->widthSpinBox->setValue(0);
    ui->heightSpinBox->setValue(0);
    ui->targetSizeSpinBox->setValue(0);
    
    // 清空属性显示
    ui->originalSizeLabel->setText("--");
    ui->currentSizeLabel->setText("--");
    ui->scaleRatioLabel->setText("--");
    ui->originalFileSizeLabel->setText("--");
    ui->currentFileSizeLabel->setText("--");

    ui->pixelCountLabel->setText("--");
    ui->formatLabel->setText("--");
}