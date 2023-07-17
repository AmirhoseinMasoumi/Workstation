#ifndef LOGDATA_H
#define LOGDATA_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QTimer>

class LogData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString totalCount READ totalCount WRITE setTotalCount NOTIFY totalCountChanged)
    Q_PROPERTY(QString deterCount READ deterCount WRITE setDeterCount NOTIFY deterCountChanged)
    Q_PROPERTY(QString resetCount READ resetCount WRITE setResetCount NOTIFY resetCountChanged)
    Q_PROPERTY(int readCount READ readCount WRITE setReadCount NOTIFY readCountChanged)
    Q_PROPERTY(QString rDate READ rDate WRITE setRDate NOTIFY rDateChanged)
    Q_PROPERTY(QString rCount READ rCount WRITE setRCount NOTIFY rCountChanged)
    Q_PROPERTY(double maxCount READ maxCount WRITE setMaxCount NOTIFY maxCountChanged)
    Q_PROPERTY(QString startDate READ startDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QString endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
public:
    explicit LogData(QObject *parent = nullptr);

    QString totalCount() const;
    void setTotalCount(const QString &newTotalCount);
    int readCount() const;
    void setReadCount(int newReadCount);
    QString rDate() const;
    void setRDate(const QString &newRDate);
    QString rCount() const;
    void setRCount(const QString &newRCount);
    double maxCount() const;
    void setMaxCount(double newMaxCount);
    QString startDate() const;
    void setStartDate(const QString &newStartDate);
    QString endDate() const;
    void setEndDate(const QString &newEndDate);

    QString deterCount() const;
    void setDeterCount(const QString &newDeterCount);

    QString resetCount() const;
    void setResetCount(const QString &newResetCount);

private:
    QString m_totalCount;
    int m_readCount;
    QString m_rDate;
    QString m_rCount;
    double m_maxCount;
    QString m_startDate;
    QString m_endDate;
    ////////////////////////////////////////////
    QString loadFilePath;
    QStringList dateList;
    QStringList countList;

    QTimer *timer;
    bool readRdy = false;

    int radioS = 1;
    QString Sel_startD = "";
    QString Sel_EndD = "";


    QString m_deterCount;

    QString m_resetCount;

public slots:
    void getLoadPath(QString path);
    void readFile();
    void readSelectedFile();
    void readReady();
    void radioSelected(int select);
    void selectedStartDate(QString startD);
    void selectedEndDate(QString endD);

signals:
    void totalCountChanged();
    void readCountChanged();
    void rDateChanged();
    void rCountChanged();
    void maxCountChanged();
    void startDateChanged();
    void endDateChanged();
    void deterCountChanged();
    void resetCountChanged();
};

#endif // LOGDATA_H
