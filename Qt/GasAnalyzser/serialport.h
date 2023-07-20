#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QDateTime>

class SerialPort : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool connection READ connection WRITE setConnection NOTIFY connectionChanged)
    Q_PROPERTY(QStringList portList READ portList WRITE setPortList NOTIFY portListChanged)

    Q_PROPERTY(QString o2Val READ o2Val WRITE setO2Val NOTIFY o2ValChanged)
    Q_PROPERTY(QString co2Val READ co2Val WRITE setCo2Val NOTIFY co2ValChanged)
    Q_PROPERTY(QString coVal READ coVal WRITE setCoVal NOTIFY coValChanged)
    Q_PROPERTY(QString tempVal READ tempVal WRITE setTempVal NOTIFY tempValChanged)
    Q_PROPERTY(double counter READ counter WRITE setCounter NOTIFY counterChanged)

    Q_PROPERTY(QString o2Avg READ o2Avg WRITE setO2Avg NOTIFY o2AvgChanged)
    Q_PROPERTY(QString co2Avg READ co2Avg WRITE setCo2Avg NOTIFY co2AvgChanged)
    Q_PROPERTY(QString coAvg READ coAvg WRITE setCoAvg NOTIFY coAvgChanged)
    Q_PROPERTY(QString tempAvg READ tempAvg WRITE setTempAvg NOTIFY tempAvgChanged)

    Q_PROPERTY(QString currentTime READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged)

public:
    QStringList portList() const;
    void setPortList(const QStringList &newPortList);
    bool connection() const;
    void setConnection(bool newConnection);
    QString o2Val() const;
    void setO2Val(const QString &newO2Val);
    QString co2Val() const;
    void setCo2Val(const QString &newCo2Val);
    QString coVal() const;
    void setCoVal(const QString &newCoVal);
    QString tempVal() const;
    void setTempVal(const QString &newTempVal);
    double counter() const;
    void setCounter(double newCounter);
    QString o2Avg() const;
    void setO2Avg(const QString &newO2Avg);
    QString co2Avg() const;
    void setCo2Avg(const QString &newCo2Avg);
    QString coAvg() const;
    void setCoAvg(const QString &newCoAvg);
    QString tempAvg() const;
    void setTempAvg(const QString &newTempAvg);
    //////////////////////////////
    explicit SerialPort(QObject *parent = nullptr);




    QString currentTime() const;
    void setCurrentTime(const QString &newCurrentTime);

private:
    QStringList m_portList;
    bool m_connection;
    QString m_o2Val;
    QString m_co2Val;
    QString m_coVal;
    QString m_tempVal;
    double m_counter;
    QString m_o2Avg;
    QString m_co2Avg;
    QString m_coAvg;
    QString m_tempAvg;
    //////////////////////////////
    QSerialPort *serialPort;
    QSerialPortInfo portInfo;
    QTimer *timer;
    QTimer *timer_s;
    void readData();
    QString filePath = "";
    QFile file;
    QString inputData;
    bool readyConnect = false;

    double O2AvgTime, CO2AvgTime, COAvgTime, TempAvgTime;
    double O2_sc, CO2_sc, CO_sc, Temp_sc;
    int O2Count, CO2Count, COCount, TempCount;
    double O2A, CO2A, COA, TempA;

    double writeCount = 0;


    QString m_currentTime;

public slots:
    void getPortNames();
    void disconnect();
    void setPortConfig(QString portName);
    void getSaveFilePath(QString path);
    void changeConnectionStatus(bool status);
    void connectionReady(bool status);

    void getO2AvgTime(QString O2AvgT);
    void getCO2AvgTime(QString CO2AvgT);
    void getCOAvgTime(QString COAvgT);
    void getTempAvgTime(QString TempAvgT);
    void secondCount();

signals:
    void portListChanged();
    void connectionChanged();
    void o2ValChanged();
    void co2ValChanged();
    void coValChanged();
    void tempValChanged();
    //////////////////////////////
    QStringList availablePorts(QStringList ports);
    void counterChanged();
    void o2AvgChanged();
    void co2AvgChanged();
    void coAvgChanged();
    void tempAvgChanged();
    void currentTimeChanged();
};

#endif // SERIALPORT_H
