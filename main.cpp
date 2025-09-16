#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("图片处理器");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("CodeBuddy");
    app.setApplicationDisplayName("Image Processor");
    
    // 创建主窗口
    MainWindow window;
    window.show();
    
    return app.exec();
}