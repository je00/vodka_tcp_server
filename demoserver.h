#ifndef DEMOSERVER_H
#define DEMOSERVER_H

#include <QtCore>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QList>
#include <QMap>
#include <QTimer>
#define TIMER_TIMEOUT   (1)

class DemoServer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int port READ getPort WRITE setPort)
public:
    DemoServer();
    ~DemoServer();

    int getPort() const;
    void setPort(int value);
    Q_INVOKABLE void listen();

protected:
    void incomingConnection(qintptr handle);

signals:
    void newDataCome(QJsonObject data);

public slots:
    void newConnectionSlot();
    void disconnectedSlot();
    void readyReadSlot();
    void handleTimeout();

private:
    QTcpServer server;
    QMap<QTcpSocket*, QString> clients;
    int port;
    QTimer *m_pTimer;
    uint64_t m_pMs;
    int image_index_ = 0;
};

#endif // DEBUGSERVER_H
