QT += core widgets gui

CONFIG += c++17

TARGET = ImageProcessor
TEMPLATE = app

# 定义应用程序信息
VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "CodeBuddy"
QMAKE_TARGET_PRODUCT = "Image Processor"
QMAKE_TARGET_DESCRIPTION = "Qt Image Processing Application"
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2025 CodeBuddy"

# 源文件
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    imageprocessor.cpp \
    imageviewer.cpp

# 头文件
HEADERS += \
    mainwindow.h \
    imageprocessor.h \
    imageviewer.h

# UI文件
FORMS += \
    mainwindow.ui

# 资源文件（如果需要的话）
# RESOURCES += resources.qrc

# Windows特定设置
win32 {
    # RC_ICONS = app_icon.ico
    CONFIG += windows
}

# 输出目录
CONFIG(debug, debug|release) {
    DESTDIR = debug
    OBJECTS_DIR = debug/obj
    MOC_DIR = debug/moc
    RCC_DIR = debug/rcc
    UI_DIR = debug/ui
}

CONFIG(release, debug|release) {
    DESTDIR = release
    OBJECTS_DIR = release/obj
    MOC_DIR = release/moc
    RCC_DIR = release/rcc
    UI_DIR = release/ui
}

# 编译器标志
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS_RELEASE += -O2

# 链接库
LIBS += 

# 包含路径
INCLUDEPATH += .

# 定义宏
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000