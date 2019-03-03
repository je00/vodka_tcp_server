#include <QCoreApplication>
#include <QDebug>
#include <QVariant>

#include <debugserver.h>

int main(int argc, char *argv[])
{
    QVariantList list;//借书记录

    QCoreApplication a(argc, argv);
    a.setApplicationName("伏特加测试服务器——FireWater");
    qDebug() << "FireWater";
    DebugServer server;
    server.listen();
    return a.exec();
}
