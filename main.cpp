#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(nullptr);

    w.resize(800, 600);  // 设置窗口大小
    w.show();
    return a.exec();
}
