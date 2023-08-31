#include "headers/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE ( application ); /* 初始化qrc资源 */
    QApplication a(argc, argv);
    //qRegisterMetaType<std::string>("string");

    MainWindow w;

    w.show();
    return a.exec();
}
