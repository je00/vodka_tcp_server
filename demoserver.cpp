#include "demoserver.h"

DemoServer::DemoServer()
{
    qDebug() << "DebugServer";
    /*
    connect(&m_pServer, SIGNAL(newConnection()), this, SLOT(newConnect()));
    m_pServer.listen(QHostAddress::Any, 1345);
    */
    connect(&server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    m_pTimer = new QTimer(this);
    m_pMs = 0;
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    m_pTimer->start(TIMER_TIMEOUT);
}

DemoServer::~DemoServer()
{
    m_pTimer->stop();
    while (m_pTimer->isActive());

    QMap<QTcpSocket*, QString>::Iterator it;
    for (it = clients.begin(); it != clients.end(); it++) {
        it.key()->close();
        clients.erase(it);
    }
}

// FireWater
//void DemoServer::handleTimeout()
//{
////    qDebug() << "tick";
//    m_pMs += TIMER_TIMEOUT;
//    QMap<QTcpSocket*, QString>::Iterator it;
//    for (it = clients.begin(); it != clients.end(); it++) {
//        QString data;
//        double t_s = static_cast<double>(m_pMs)/1000.0;
//        data.sprintf("d0:%f,%f,%f,%f\r\n"
//                     , sin(2*M_PI*t_s) + sin(M_PI*t_s), sin(M_PI*t_s),sin(M_PI/2*t_s),sin(M_PI/4*t_s));
////        , 1.0, 2.0,3.0,4.0);
//        it.key()->write(data.toStdString().data());
//        if (m_pMs % 80 == 0) {
//            QString data;
//            QString file_name;
//            image_index_ = (image_index_ + 1) % 24 + 1;
//            file_name.sprintf(":/images/img%d.jpg", image_index_);
//            QFile file;
//            file.setFileName(file_name);
//            file.open(QFile::ReadOnly);
//            QByteArray image_data = file.readAll();
//            data.sprintf("image:0,%d,jpg\n", image_data.length());
//            it.key()->write(data.toStdString().data());
//            it.key()->write(image_data.data(), image_data.length());


//            QString data2;
//            QString file_name2;
//            file_name2.sprintf(":/images/img2-%d.jpg", image_index_ + 19);
//            QFile file2;
//            file2.setFileName(file_name2);
//            file2.open(QFile::ReadOnly);
//            QByteArray image_data2 = file2.readAll();
//            data2.sprintf("image:1,%d,jpg\n", image_data2.length());
//            it.key()->write(data2.toStdString().data());
//            it.key()->write(image_data2.data(), image_data2.length());
//        }

//    }
//}


// JustFloat
void DemoServer::handleTimeout()
{
    //    qDebug() << "tick";

    m_pMs += TIMER_TIMEOUT;
    QMap<QTcpSocket*, QString>::Iterator it;
    for (it = clients.begin(); it != clients.end(); it++) {
        QString data;
        double t_s = static_cast<double>(m_pMs)/1000.0;
        float d[13] = {0.0, 1.1, 2.2, 3.3, 0x7F800000};
//        d[0] = static_cast<float>(sin(2*M_PI*t_s) + sin(M_PI*t_s));
//        d[1] = static_cast<float>(sin(M_PI*t_s));
//        d[2] = static_cast<float>(sin(M_PI/2*t_s));
//        d[3] = static_cast<float>(sin(M_PI/4*t_s));
        d[0] = qrand() % 10;
        d[1] = qrand() % 20;
        d[2] = qrand() % 30;
        d[3] = qrand() % 40;

//        d[4] = static_cast<float>(sin(M_PI/4*t_s));
//        d[5] = static_cast<float>(sin(M_PI/4*t_s));
//        d[6] = static_cast<float>(sin(M_PI/4*t_s));
//        d[7] = static_cast<float>(sin(M_PI/4*t_s));
//        d[8] = static_cast<float>(sin(M_PI/4*t_s));
//        d[9] = static_cast<float>(sin(M_PI/4*t_s));
//        d[10] = static_cast<float>(sin(M_PI/4*t_s));
//        d[11] = static_cast<float>(sin(M_PI/4*t_s));
        *((int*)&d[4]) = 0x7F800000;
        QByteArray array((char*)&d[0], 5*sizeof (float));
//        qDebug() << array.toHex(' ');
        it.key()->write(array);
        if (m_pMs % 80 == 0) {
            char format[4] = { 'j', 'p', 'g', '\0'};

            QString file_name;
            image_index_ = (image_index_ + 1) % 24 + 1;
            file_name.sprintf(":/images/img%d.jpg", image_index_);
            QFile file;
            file.setFileName(file_name);
            file.open(QFile::ReadOnly);
            QByteArray image_data = file.readAll();

            int d2[5] = {0, 0, 0, 0x7F800000, 0x7F800000};
            memcpy((char*)&d2[2], format, 4);
            d2[1] = image_data.length();
            it.key()->write((char*)&d2[0], 5 * sizeof(float));
            it.key()->write(image_data.data(), image_data.length());


            QString file_name2;
            file_name2.sprintf(":/images/img2-%d.jpg", image_index_ + 19);
            QFile file2;
            file2.setFileName(file_name2);
            file2.open(QFile::ReadOnly);
            QByteArray image_data2 = file2.readAll();

            int d22[5] = {1, 0, 0, 0x7F800000, 0x7F800000};
            memcpy((char*)&d22[2], format, 4);
            d22[1] = image_data2.length();
            it.key()->write((char*)&d22[0], 5 * sizeof(float));
            it.key()->write(image_data2.data(), image_data2.length());
        }
    }
}

void DemoServer::newConnectionSlot()
{
    qDebug() << "client connected" << endl;
    QTcpSocket *client = server.nextPendingConnection();
    connect(client, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
    clients[client] = "NONE";
    qDebug() << clients.keys().length();
    if (!m_pTimer->isActive()) {
        m_pTimer->start();
    }
}

void DemoServer::disconnectedSlot()
{
    qDebug() << "client disconnected" << endl;
    QMap<QTcpSocket*, QString>::Iterator it;
    for (it = clients.begin(); it != clients.end(); it++) {
        if (it.key()->state() == QTcpSocket::UnconnectedState) {
            clients.erase(it);
            break;
        }
    }
    qDebug() << clients.keys().length();
    if (clients.keys().length() == 0) {
        m_pTimer->stop();
        while (m_pTimer->isActive());
    }
}

void DemoServer::readyReadSlot() {
    QMap<QTcpSocket*, QString>::iterator it;
    for (it = clients.begin(); it != clients.end(); it++) {
        while (it.key()->bytesAvailable()) {
            QByteArray vTemp = it.key()->readLine();
            qDebug() << vTemp << ", " << qstrlen(vTemp.data());
            it.key()->write(vTemp.data(), qstrlen(vTemp.data()));
        }
    }
}

void DemoServer::listen()
{
    qDebug() << "listen" << endl;
    server.listen(QHostAddress::Any, 1346);
}

int DemoServer::getPort() const
{
    return port;
}

void DemoServer::setPort(int value)
{
    port = value;
}

