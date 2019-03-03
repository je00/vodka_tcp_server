#include "debugserver.h"

DebugServer::DebugServer()
{
    qDebug() << "DebugServer";
    /*
    connect(&m_pServer, SIGNAL(newConnection()), this, SLOT(newConnect()));
    m_pServer.listen(QHostAddress::Any, 1345);
    */
    connect(&server, SIGNAL(newConnection()), this, SLOT(newConnectionSlot()));
    m_pTimer = new QTimer(this);
    m_pMs = 0;
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    m_pTimer->start(TIMER_TIMEOUT);
}

DebugServer::~DebugServer()
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
//void DebugServer::handleTimeout()
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
//    }
//}


// JustFloat
void DebugServer::handleTimeout()
{
    //    qDebug() << "tick";
    m_pMs += TIMER_TIMEOUT;
    QMap<QTcpSocket*, QString>::Iterator it;
    for (it = clients.begin(); it != clients.end(); it++) {
        QString data;
        double t_s = static_cast<double>(m_pMs)/1000.0;
        float d[5] = {0.0, 1.1, 2.2, 3.3, 0x7F800000};
        d[0] = static_cast<float>(sin(2*M_PI*t_s) + sin(M_PI*t_s));
        d[1] = static_cast<float>(sin(M_PI*t_s));
        d[2] = static_cast<float>(sin(M_PI/2*t_s));
        d[3] = static_cast<float>(sin(M_PI/4*t_s));
        *((int*)&d[4]) = 0x7F800000;
        QByteArray array((char*)&d[0], 5*sizeof (float));
//        qDebug() << array.toHex(' ');
        it.key()->write(array);
    }
}

void DebugServer::newConnectionSlot()
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

void DebugServer::disconnectedSlot()
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

void DebugServer::readyReadSlot() {
    QMap<QTcpSocket*, QString>::iterator it;
    for (it = clients.begin(); it != clients.end(); it++) {
        while (it.key()->bytesAvailable()) {
            QByteArray vTemp = it.key()->readLine();
            qDebug() << vTemp << ", " << qstrlen(vTemp.data());
            it.key()->write(vTemp.data(), qstrlen(vTemp.data()));
        }
    }
}

void DebugServer::listen()
{
    qDebug() << "listen" << endl;
    server.listen(QHostAddress::Any, 1346);
}

int DebugServer::getPort() const
{
    return port;
}

void DebugServer::setPort(int value)
{
    port = value;
}

