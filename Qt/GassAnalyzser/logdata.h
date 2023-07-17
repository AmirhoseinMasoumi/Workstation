#ifndef LOGDATA_H
#define LOGDATA_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QQmlContext>
#include <QList>
#include <QStringList>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>

class LogData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool connection READ connection WRITE setConnection NOTIFY connectionChanged)
    Q_PROPERTY(QString O2value READ O2value WRITE setO2value NOTIFY O2valueChanged)
    Q_PROPERTY(QString CO2value READ CO2value WRITE setCO2value NOTIFY CO2valueChanged)
    Q_PROPERTY(QString COvalue READ COvalue WRITE setCOvalue NOTIFY COvalueChanged)
    Q_PROPERTY(QString TEMPvalue READ TEMPvalue WRITE setTEMPvalue NOTIFY TEMPvalueChanged)
    Q_PROPERTY(double dataCounter READ dataCounter WRITE setDataCounter NOTIFY dataCounterChanged)

    Q_PROPERTY(QString o2Avg READ o2Avg WRITE setO2Avg NOTIFY o2AvgChanged)
    Q_PROPERTY(QString co2Avg READ co2Avg WRITE setCo2Avg NOTIFY co2AvgChanged)
    Q_PROPERTY(QString coAvg READ coAvg WRITE setCoAvg NOTIFY coAvgChanged)
    Q_PROPERTY(QString tempAvg READ tempAvg WRITE setTempAvg NOTIFY tempAvgChanged)

    Q_PROPERTY(QString currentTime READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged)

    Q_PROPERTY(QStringList dbTableList READ dbTableList WRITE setDbTableList NOTIFY dbTableListChanged)

public:
    bool connection() const;
    void setConnection(bool newConnection);
    QString O2value() const;
    void setO2value(const QString &newO2value);
    QString CO2value() const;
    void setCO2value(const QString &newCO2value);
    QString COvalue() const;
    void setCOvalue(const QString &newCOvalue);
    QString TEMPvalue() const;
    void setTEMPvalue(const QString &newTEMPvalue);
    double dataCounter() const;
    void setDataCounter(double newDataCounter);
    QString o2Avg() const;
    void setO2Avg(const QString &newO2Avg);
    QString co2Avg() const;
    void setCo2Avg(const QString &newCo2Avg);
    QString coAvg() const;
    void setCoAvg(const QString &newCoAvg);
    QString tempAvg() const;
    void setTempAvg(const QString &newTempAvg);
    ////////////////////////////////////////////////
    explicit LogData(QObject *parent = nullptr);
    void logDataInFile(QString d1 , QString d2 , QString d3, QString d4);



    QString currentTime() const;
    void setCurrentTime(const QString &newCurrentTime);

    QStringList dbTableList() const;
    void setDbTableList(const QStringList &newDbTableList);

private:
    bool m_connection;
    QString m_O2value;
    QString m_CO2value;
    QString m_COvalue;
    QString m_TEMPvalue;
    double m_dataCounter;
    QString m_O2Avg;
    QString m_CO2Avg;
    QString m_COAvg;
    QString m_TEMPAvg;
    QString m_o2Avg;
    QString m_co2Avg;
    QString m_coAvg;
    QString m_tempAvg;
    QString m_currentTime;
    ////////////////////////////////////////////////
    QString filename;
    QString loadFilePath;
    QTimer *timer;
    QTimer *timer_s;
    double O2AvgTime, CO2AvgTime, COAvgTime, TempAvgTime;

    double O2_sc, CO2_sc, CO_sc, Temp_sc;
    int O2Count, CO2Count, COCount, TempCount;
    double O2A, CO2A, COA, TempA;

    bool readyReadStatus = false;

    QStringList Time_List, O2_List, CO2_List, CO_List, Temp_List;
    bool firstRead = true;
    double readCount = 0;

    QSqlDatabase dataBase;

    QStringList m_dbTableList;

public slots:
    void createFile(QString path);
    void loadData();
    void getO2AvgTime(QString O2AvgT);
    void getCO2AvgTime(QString CO2AvgT);
    void getCOAvgTime(QString COAvgT);
    void getTempAvgTime(QString TempAvgT);
    void secondCount();
    void getLoadPath(QString path);
    void readyRead(bool status);
    void loadFromList();
    void loadSqlite(QString table);
    void createSql();

signals:
    void connectionChanged();
    void O2valueChanged();
    void CO2valueChanged();
    void COvalueChanged();
    void TEMPvalueChanged();
    void dataCounterChanged();
    void o2AvgChanged();
    void co2AvgChanged();
    void coAvgChanged();
    void tempAvgChanged();
    void currentTimeChanged();
    void dbTableListChanged();
};

#endif // LOGDATA_H
