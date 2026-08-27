#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Qt 应用入口：创建应用对象并显示主窗口。
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
