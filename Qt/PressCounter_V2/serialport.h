#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QDateTime>

class SerialPort : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool connection READ connection WRITE setConnection NOTIFY connectionChanged)

public:
    explicit SerialPort(QObject *parent = nullptr);

    bool connection() const;
    void setConnection(bool newConnection);
private:
    QSerialPort *serialPort;
    QSerialPortInfo portInfo;
    //////////////////////////////////////////////
    bool m_connection;
    QTimer *timer;
    QString saveFilePath = "C:/Users/Legend/Desktop/Temp/SaveData.txt";
    QString readFilePath = "";
    QFile file;
    void setPortConfig(QString portName);
    void readData();

public slots:
    void getPortNames();
    void disconnect();
    void readFromDevice();
    void createSaveFile(QString path);

signals:
    void connectionChanged();
};

#endif // SERIALPORT_H
