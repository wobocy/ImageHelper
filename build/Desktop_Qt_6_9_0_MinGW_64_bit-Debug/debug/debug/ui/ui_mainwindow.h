/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_main;
    QSplitter *splitter;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_image;
    QLabel *imageLabel;
    QWidget *controlPanel;
    QVBoxLayout *verticalLayout_control;
    QGroupBox *propertyGroup;
    QGridLayout *gridLayout_property;
    QLabel *label_originalSize;
    QLabel *originalSizeLabel;
    QLabel *label_currentSize;
    QLabel *currentSizeLabel;
    QLabel *label_scaleRatio;
    QLabel *scaleRatioLabel;
    QFrame *line_1;
    QLabel *label_originalFileSize;
    QLabel *originalFileSizeLabel;
    QLabel *label_currentFileSize;
    QLabel *currentFileSizeLabel;
    QFrame *line_2;
    QLabel *label_pixelCount;
    QLabel *pixelCountLabel;
    QLabel *label_format;
    QLabel *formatLabel;
    QGroupBox *editGroup;
    QGridLayout *gridLayout_edit;
    QLabel *label_width;
    QSpinBox *widthSpinBox;
    QLabel *label_height;
    QSpinBox *heightSpinBox;
    QLabel *label_targetSize;
    QSpinBox *targetSizeSpinBox;
    QCheckBox *lockAspectRatioCheckBox;
    QWidget *buttonPanel;
    QVBoxLayout *verticalLayout_buttons;
    QPushButton *openButton;
    QPushButton *generateButton;
    QPushButton *saveButton;
    QPushButton *resetButton;
    QSpacerItem *verticalSpacer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 800);
        MainWindow->setMinimumSize(QSize(1000, 700));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_main = new QHBoxLayout(centralwidget);
        horizontalLayout_main->setSpacing(5);
        horizontalLayout_main->setObjectName("horizontalLayout_main");
        horizontalLayout_main->setContentsMargins(5, 5, 5, 5);
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        scrollArea = new QScrollArea(splitter);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setStyleSheet(QString::fromUtf8("QScrollArea { background-color: #f0f0f0; border: 1px solid #ccc; }"));
        scrollArea->setWidgetResizable(true);
        scrollArea->setAlignment(Qt::AlignCenter);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 896, 770));
        verticalLayout_image = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_image->setObjectName("verticalLayout_image");
        imageLabel = new QLabel(scrollAreaWidgetContents);
        imageLabel->setObjectName("imageLabel");
        imageLabel->setMinimumSize(QSize(400, 300));
        imageLabel->setStyleSheet(QString::fromUtf8("QLabel { background-color: white; border: 1px dashed #ccc; }"));
        imageLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_image->addWidget(imageLabel);

        scrollArea->setWidget(scrollAreaWidgetContents);
        splitter->addWidget(scrollArea);
        controlPanel = new QWidget(splitter);
        controlPanel->setObjectName("controlPanel");
        controlPanel->setMinimumSize(QSize(300, 0));
        controlPanel->setMaximumSize(QSize(300, 16777215));
        controlPanel->setStyleSheet(QString::fromUtf8("QWidget { background-color: #fafafa; }"));
        verticalLayout_control = new QVBoxLayout(controlPanel);
        verticalLayout_control->setSpacing(10);
        verticalLayout_control->setObjectName("verticalLayout_control");
        verticalLayout_control->setContentsMargins(10, 10, 10, 10);
        propertyGroup = new QGroupBox(controlPanel);
        propertyGroup->setObjectName("propertyGroup");
        propertyGroup->setStyleSheet(QString::fromUtf8("QGroupBox { font-weight: bold; }"));
        gridLayout_property = new QGridLayout(propertyGroup);
        gridLayout_property->setSpacing(8);
        gridLayout_property->setObjectName("gridLayout_property");
        label_originalSize = new QLabel(propertyGroup);
        label_originalSize->setObjectName("label_originalSize");

        gridLayout_property->addWidget(label_originalSize, 0, 0, 1, 1);

        originalSizeLabel = new QLabel(propertyGroup);
        originalSizeLabel->setObjectName("originalSizeLabel");
        originalSizeLabel->setStyleSheet(QString::fromUtf8("QLabel { font-weight: normal; color: #333; }"));

        gridLayout_property->addWidget(originalSizeLabel, 0, 1, 1, 1);

        label_currentSize = new QLabel(propertyGroup);
        label_currentSize->setObjectName("label_currentSize");

        gridLayout_property->addWidget(label_currentSize, 1, 0, 1, 1);

        currentSizeLabel = new QLabel(propertyGroup);
        currentSizeLabel->setObjectName("currentSizeLabel");
        currentSizeLabel->setStyleSheet(QString::fromUtf8("QLabel { font-weight: normal; color: #333; }"));

        gridLayout_property->addWidget(currentSizeLabel, 1, 1, 1, 1);

        label_scaleRatio = new QLabel(propertyGroup);
        label_scaleRatio->setObjectName("label_scaleRatio");

        gridLayout_property->addWidget(label_scaleRatio, 2, 0, 1, 1);

        scaleRatioLabel = new QLabel(propertyGroup);
        scaleRatioLabel->setObjectName("scaleRatioLabel");
        scaleRatioLabel->setStyleSheet(QString::fromUtf8("QLabel { font-weight: normal; color: #333; }"));

        gridLayout_property->addWidget(scaleRatioLabel, 2, 1, 1, 1);

        line_1 = new QFrame(propertyGroup);
        line_1->setObjectName("line_1");
        line_1->setFrameShape(QFrame::Shape::HLine);
        line_1->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_property->addWidget(line_1, 3, 0, 1, 2);

        label_originalFileSize = new QLabel(propertyGroup);
        label_originalFileSize->setObjectName("label_originalFileSize");

        gridLayout_property->addWidget(label_originalFileSize, 4, 0, 1, 1);

        originalFileSizeLabel = new QLabel(propertyGroup);
        originalFileSizeLabel->setObjectName("originalFileSizeLabel");
        originalFileSizeLabel->setStyleSheet(QString::fromUtf8("QLabel { font-weight: normal; color: #333; }"));

        gridLayout_property->addWidget(originalFileSizeLabel, 4, 1, 1, 1);

        label_currentFileSize = new QLabel(propertyGroup);
        label_currentFileSize->setObjectName("label_currentFileSize");

        gridLayout_property->addWidget(label_currentFileSize, 5, 0, 1, 1);

        currentFileSizeLabel = new QLabel(propertyGroup);
        currentFileSizeLabel->setObjectName("currentFileSizeLabel");
        currentFileSizeLabel->setStyleSheet(QString::fromUtf8("QLabel { font-weight: normal; color: #333; }"));

        gridLayout_property->addWidget(currentFileSizeLabel, 5, 1, 1, 1);

        line_2 = new QFrame(propertyGroup);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::Shape::HLine);
        line_2->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_property->addWidget(line_2, 7, 0, 1, 2);

        label_pixelCount = new QLabel(propertyGroup);
        label_pixelCount->setObjectName("label_pixelCount");

        gridLayout_property->addWidget(label_pixelCount, 8, 0, 1, 1);

        pixelCountLabel = new QLabel(propertyGroup);
        pixelCountLabel->setObjectName("pixelCountLabel");
        pixelCountLabel->setStyleSheet(QString::fromUtf8("QLabel { font-weight: normal; color: #333; }"));

        gridLayout_property->addWidget(pixelCountLabel, 8, 1, 1, 1);

        label_format = new QLabel(propertyGroup);
        label_format->setObjectName("label_format");

        gridLayout_property->addWidget(label_format, 10, 0, 1, 1);

        formatLabel = new QLabel(propertyGroup);
        formatLabel->setObjectName("formatLabel");
        formatLabel->setStyleSheet(QString::fromUtf8("QLabel { font-weight: normal; color: #333; }"));

        gridLayout_property->addWidget(formatLabel, 10, 1, 1, 1);


        verticalLayout_control->addWidget(propertyGroup);

        editGroup = new QGroupBox(controlPanel);
        editGroup->setObjectName("editGroup");
        editGroup->setStyleSheet(QString::fromUtf8("QGroupBox { font-weight: bold; }"));
        gridLayout_edit = new QGridLayout(editGroup);
        gridLayout_edit->setSpacing(8);
        gridLayout_edit->setObjectName("gridLayout_edit");
        label_width = new QLabel(editGroup);
        label_width->setObjectName("label_width");

        gridLayout_edit->addWidget(label_width, 0, 0, 1, 1);

        widthSpinBox = new QSpinBox(editGroup);
        widthSpinBox->setObjectName("widthSpinBox");
        widthSpinBox->setMinimum(1);
        widthSpinBox->setMaximum(10000);

        gridLayout_edit->addWidget(widthSpinBox, 0, 1, 1, 1);

        label_height = new QLabel(editGroup);
        label_height->setObjectName("label_height");

        gridLayout_edit->addWidget(label_height, 1, 0, 1, 1);

        heightSpinBox = new QSpinBox(editGroup);
        heightSpinBox->setObjectName("heightSpinBox");
        heightSpinBox->setMinimum(1);
        heightSpinBox->setMaximum(10000);

        gridLayout_edit->addWidget(heightSpinBox, 1, 1, 1, 1);

        label_targetSize = new QLabel(editGroup);
        label_targetSize->setObjectName("label_targetSize");

        gridLayout_edit->addWidget(label_targetSize, 2, 0, 1, 1);

        targetSizeSpinBox = new QSpinBox(editGroup);
        targetSizeSpinBox->setObjectName("targetSizeSpinBox");
        targetSizeSpinBox->setMinimum(1);
        targetSizeSpinBox->setMaximum(100000);

        gridLayout_edit->addWidget(targetSizeSpinBox, 2, 1, 1, 1);

        lockAspectRatioCheckBox = new QCheckBox(editGroup);
        lockAspectRatioCheckBox->setObjectName("lockAspectRatioCheckBox");
        lockAspectRatioCheckBox->setChecked(true);

        gridLayout_edit->addWidget(lockAspectRatioCheckBox, 4, 0, 1, 2);


        verticalLayout_control->addWidget(editGroup);

        buttonPanel = new QWidget(controlPanel);
        buttonPanel->setObjectName("buttonPanel");
        verticalLayout_buttons = new QVBoxLayout(buttonPanel);
        verticalLayout_buttons->setSpacing(10);
        verticalLayout_buttons->setObjectName("verticalLayout_buttons");
        verticalLayout_buttons->setContentsMargins(0, 0, 0, 0);
        openButton = new QPushButton(buttonPanel);
        openButton->setObjectName("openButton");
        openButton->setMinimumSize(QSize(0, 35));

        verticalLayout_buttons->addWidget(openButton);

        generateButton = new QPushButton(buttonPanel);
        generateButton->setObjectName("generateButton");
        generateButton->setMinimumSize(QSize(0, 35));

        verticalLayout_buttons->addWidget(generateButton);

        saveButton = new QPushButton(buttonPanel);
        saveButton->setObjectName("saveButton");
        saveButton->setMinimumSize(QSize(0, 35));

        verticalLayout_buttons->addWidget(saveButton);

        resetButton = new QPushButton(buttonPanel);
        resetButton->setObjectName("resetButton");
        resetButton->setMinimumSize(QSize(0, 35));

        verticalLayout_buttons->addWidget(resetButton);


        verticalLayout_control->addWidget(buttonPanel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_control->addItem(verticalSpacer);

        splitter->addWidget(controlPanel);

        horizontalLayout_main->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\345\233\276\347\211\207\345\244\204\347\220\206\345\231\250 - Image Processor", nullptr));
        imageLabel->setText(QCoreApplication::translate("MainWindow", "\347\202\271\345\207\273\"\346\211\223\345\274\200\345\233\276\347\211\207\"\346\210\226\346\213\226\346\213\275\345\233\276\347\211\207\345\210\260\346\255\244\345\244\204", nullptr));
        propertyGroup->setTitle(QCoreApplication::translate("MainWindow", "\345\233\276\347\211\207\345\261\236\346\200\247\344\277\241\346\201\257", nullptr));
        label_originalSize->setText(QCoreApplication::translate("MainWindow", "\345\216\237\345\247\213\345\260\272\345\257\270:", nullptr));
        originalSizeLabel->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        label_currentSize->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\345\260\272\345\257\270:", nullptr));
        currentSizeLabel->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        label_scaleRatio->setText(QCoreApplication::translate("MainWindow", "\347\274\251\346\224\276\346\257\224\344\276\213:", nullptr));
        scaleRatioLabel->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        label_originalFileSize->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\345\244\247\345\260\217:", nullptr));
        originalFileSizeLabel->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        label_currentFileSize->setText(QCoreApplication::translate("MainWindow", "\345\244\204\347\220\206\345\220\216\345\244\247\345\260\217:", nullptr));
        currentFileSizeLabel->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        label_pixelCount->setText(QCoreApplication::translate("MainWindow", "\345\203\217\347\264\240\346\200\273\346\225\260:", nullptr));
        pixelCountLabel->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        label_format->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\346\240\274\345\274\217:", nullptr));
        formatLabel->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        editGroup->setTitle(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221\345\217\202\346\225\260", nullptr));
        label_width->setText(QCoreApplication::translate("MainWindow", "\345\256\275\345\272\246 (\345\203\217\347\264\240):", nullptr));
        widthSpinBox->setSuffix(QCoreApplication::translate("MainWindow", " px", nullptr));
        label_height->setText(QCoreApplication::translate("MainWindow", "\351\253\230\345\272\246 (\345\203\217\347\264\240):", nullptr));
        heightSpinBox->setSuffix(QCoreApplication::translate("MainWindow", " px", nullptr));
        label_targetSize->setText(QCoreApplication::translate("MainWindow", "\347\233\256\346\240\207\345\244\247\345\260\217 (KB):", nullptr));
        targetSizeSpinBox->setSuffix(QCoreApplication::translate("MainWindow", " KB", nullptr));
        lockAspectRatioCheckBox->setText(QCoreApplication::translate("MainWindow", "\351\224\201\345\256\232\345\256\275\351\253\230\346\257\224", nullptr));
        openButton->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\345\233\276\347\211\207", nullptr));
        generateButton->setText(QCoreApplication::translate("MainWindow", "\347\224\237\346\210\220\345\233\276\347\211\207", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\345\233\276\347\211\207", nullptr));
        resetButton->setText(QCoreApplication::translate("MainWindow", "\345\244\215\345\216\237\345\233\276\347\211\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
