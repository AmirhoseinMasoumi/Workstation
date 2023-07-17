#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

class TCPsocket : public QObject
{
    Q_OBJECT
public:
    explicit TCPsocket(QObject *parent = nullptr);

    void doConnect();

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void getTCPip(QString ip);
    void getTCPport(QString port);
    void doDisconnect();

private:
    QTcpSocket *socket;

    QString TCPip;
    QString TCPport;

signals:

};

#endif // TCPSOCKET_H
