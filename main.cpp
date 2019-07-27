#include <QCoreApplication>
#include <QDebug>
#include <QVariant>

#include <demoserver.h>

int main(int argc, char *argv[])
{
    QVariantList list;//借书记录

    QCoreApplication a(argc, argv);
    a.setApplicationName("TCP Demo Server");
    qDebug() << "listening port: 1346, Protocol: FireWater";
    DemoServer server;
    server.listen();
    return a.exec();
}
