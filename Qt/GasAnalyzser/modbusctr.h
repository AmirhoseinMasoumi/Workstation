#ifndef MODBUSCTR_H
#define MODBUSCTR_H

#include <QObject>
#include <qmodbus.h>
#include <QTimer>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtGlobal>

class ModbusCTR : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString O2_tcp_val READ O2_tcp_val WRITE setO2_tcp_val NOTIFY O2_tcp_valChanged)
    Q_PROPERTY(QString CO2_tcp_val READ CO2_tcp_val WRITE setCO2_tcp_val NOTIFY CO2_tcp_valChanged)
    Q_PROPERTY(QString CO_tcp_val READ CO_tcp_val WRITE setCO_tcp_val NOTIFY CO_tcp_valChanged)
    Q_PROPERTY(QString Temp_tcp_val READ Temp_tcp_val WRITE setTemp_tcp_val NOTIFY Temp_tcp_valChanged)
    Q_PROPERTY(double counter READ counter WRITE setCounter NOTIFY counterChanged)

    Q_PROPERTY(QString o2Avg READ o2Avg WRITE setO2Avg NOTIFY o2AvgChanged)
    Q_PROPERTY(QString co2Avg READ co2Avg WRITE setCo2Avg NOTIFY co2AvgChanged)
    Q_PROPERTY(QString coAvg READ coAvg WRITE setCoAvg NOTIFY coAvgChanged)
    Q_PROPERTY(QString tempAvg READ tempAvg WRITE setTempAvg NOTIFY tempAvgChanged)

    Q_PROPERTY(QString systemStatus READ systemStatus WRITE setSystemStatus NOTIFY systemStatusChanged)

    Q_PROPERTY(QString currentTime READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged)

    Q_PROPERTY(QString maxValue READ maxValue WRITE setMaxValue NOTIFY maxValueChanged)

    qmodbus* mbus;
public:
    explicit ModbusCTR(QObject *parent = nullptr);
    ~ModbusCTR();
    void start();

    QString O2_tcp_val() const;
    void setO2_tcp_val(const QString &newO2_tcp_val);
    QString CO2_tcp_val() const;
    void setCO2_tcp_val(const QString &newCO2_tcp_val);
    QString CO_tcp_val() const;
    void setCO_tcp_val(const QString &newCO_tcp_val);
    QString Temp_tcp_val() const;
    void setTemp_tcp_val(const QString &newTemp_tcp_val);
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

    QString systemStatus() const;
    void setSystemStatus(const QString &newSystemStatus);

    QString currentTime() const;
    void setCurrentTime(const QString &newCurrentTime);

    QString maxValue() const;
    void setMaxValue(const QString &newMaxValue);

public slots:
    void readValues(int O2_val, int CO2_val, int CO_val, int Temp_val);
    void resetCounter();
    void secondCount();
    void getO2AvgTime(QString O2AvgT);
    void getCO2AvgTime(QString CO2AvgT);
    void getCOAvgTime(QString COAvgT);
    void getTempAvgTime(QString TempAvgT);
    void getSaveFilePath(QString path);
    void getConnectionStatus(bool status);
    void getTCPip(QString ip);
    void getTCPport(QString port);
    void checkSystemStatus();
    void createNewDb();

signals:
    void sendValues(int O2_val, int CO2_val, int CO_val, int Temp_val);

    void O2_tcp_valChanged();
    void CO2_tcp_valChanged();
    void CO_tcp_valChanged();
    void Temp_tcp_valChanged();
    void counterChanged();
    void o2AvgChanged();
    void co2AvgChanged();
    void coAvgChanged();
    void tempAvgChanged();

    void systemStatusChanged();

    void currentTimeChanged();

    void maxValueChanged();

private:
    QString m_O2_tcp_val;
    QString m_CO2_tcp_val;
    QString m_CO_tcp_val;
    QString m_Temp_tcp_val;
    double m_counter;
    QString m_o2Avg;
    QString m_co2Avg;
    QString m_coAvg;
    QString m_tempAvg;
    /////////////////////////////
    QTimer *timer_s;
    QTimer *timer_p;
    double O2AvgTime, CO2AvgTime, COAvgTime, TempAvgTime;
    double O2_sc, CO2_sc, CO_sc, Temp_sc;
    int O2Count, CO2Count, COCount, TempCount;
    double O2A, CO2A, COA, TempA;
    QString filePath;
    QFile file;

    QString TCP_Ip = "";
    QString TCP_Port = "";

    double writeCount = 0;
    int discCount = 0;
    QProcess *pingProcess;
    QString m_systemStatus;
    QString m_currentTime;

    bool connection = false;

    QSqlDatabase dataBase;
    bool addNewValue(const QString& Time,const QString& O2,const QString& CO2, const QString& CO, const QString& Temp);
    QString table;
    QString m_maxValue;
};

#endif // MODBUSCTR_H
