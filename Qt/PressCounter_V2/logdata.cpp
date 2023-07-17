#include "logdata.h"

LogData::LogData(QObject *parent)
    : QObject{parent},
    m_readCount(0),
    m_maxCount(0),
    m_startDate("2023-01-01"),
    m_endDate("2050-01-01")
{
    //    timer = new QTimer(this);
    //    connect(timer, SIGNAL(timeout()), this, SLOT(readFile()));
    //    timer->start(5000);
}

void LogData::readFile()
{
    dateList.clear();
    countList.clear();
    if(readRdy){
        QFile file(loadFilePath);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            double count = 0;
            while (!in.atEnd())
            {
                QString line = in.readLine();
                QStringList list = line.split(',');
                //qDebug() << list;
                dateList.append(list[0]);
                countList.append(list[1]);
                if(!list[0].contains("#")){
                    //                    setRDate(list[0]);
                    //                    setRCount(list[1]);
                    //                    setReadCount(readCount()+1);

                    count++;
                    switch(radioS) {
                    case 1:
                        setRDate(list[0]);
                        setRCount(list[1]);
                        setReadCount(readCount()+1);
                        break;
                    case 2:
                        if(count == 6){
                            setRDate(list[0]);
                            setRCount(list[1]);
                            setReadCount(readCount()+1);
                            count = 0;
                        }
                        break;
                    case 3:
                        if(count == 29){
                            setRDate(list[0]);
                            setRCount(list[1]);
                            setReadCount(readCount()+1);
                            count = 0;
                        }

                        break;
                    default:
                        if(count == 364){
                            setRDate(list[0]);
                            setRCount(list[1]);
                            setReadCount(readCount()+1);
                            count = 0;
                        }
                        break;
                    }
                }
            }
            setRDate(dateList.value(dateList.size() - 1));
            setRCount(countList.value(dateList.size() - 1));
            setReadCount(readCount()+1);
            file.close();

            setTotalCount(countList.value(0));
            setResetCount(countList.value(countList.size() - 1));
            setStartDate(dateList.value(1));
            setEndDate(dateList.value(dateList.size() - 1));
            setMaxCount(countList.value(0).toDouble());
        }
    }
}

void LogData::selectedStartDate(QString startD)
{
    Sel_startD = startD;
}

void LogData::selectedEndDate(QString endD)
{
    Sel_EndD = endD;
}

void LogData::readSelectedFile()
{
    dateList.clear();
    countList.clear();
    QFile file(loadFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        double count = 0;
        bool readStatus = 0;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList list = line.split(',');
            dateList.append(list[0]);
            countList.append(list[1]);
            if(list[0] == Sel_startD){
                readStatus = true;
            }
            if(list[0] == Sel_EndD){
                readStatus = false;
                file.close();

                setRDate(dateList.value(dateList.size() - 1));
                setRCount(countList.value(dateList.size() - 1));
                setReadCount(readCount()+1);
                file.close();

                setDeterCount(countList.value(count));
                setStartDate(dateList.value(1));
                setEndDate(dateList.value(dateList.size() - 1));
                setMaxCount(countList.value(dateList.size() - 1).toDouble());
                break;
            }
            if(readStatus == true){
                count++;
                switch(radioS) {
                case 1:
                    setRDate(list[0]);
                    setRCount(list[1]);
                    setReadCount(readCount()+1);
                    break;
                case 2:
                    if(count == 6){
                        setRDate(list[0]);
                        setRCount(list[1]);
                        setReadCount(readCount()+1);
                        count = 0;
                    }
                    break;
                case 3:
                    if(count == 29){
                        setRDate(list[0]);
                        setRCount(list[1]);
                        setReadCount(readCount()+1);
                        count = 0;
                    }

                    break;
                default:
                    if(count == 364){
                        setRDate(list[0]);
                        setRCount(list[1]);
                        setReadCount(readCount()+1);
                        count = 0;
                    }
                    break;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void LogData::getLoadPath(QString path)
{
    loadFilePath = path.right(path.length() - 8);
    qDebug() << loadFilePath;
}

QString LogData::totalCount() const
{
    return m_totalCount;
}

void LogData::setTotalCount(const QString &newTotalCount)
{
    if (m_totalCount == newTotalCount)
        return;
    m_totalCount = newTotalCount;
    emit totalCountChanged();
}

int LogData::readCount() const
{
    return m_readCount;
}

void LogData::setReadCount(int newReadCount)
{
    if (m_readCount == newReadCount)
        return;
    m_readCount = newReadCount;
    emit readCountChanged();
}

QString LogData::rDate() const
{
    return m_rDate;
}

void LogData::setRDate(const QString &newRDate)
{
    if (m_rDate == newRDate)
        return;
    m_rDate = newRDate;
    emit rDateChanged();
}

QString LogData::rCount() const
{
    return m_rCount;
}

void LogData::setRCount(const QString &newRCount)
{
    if (m_rCount == newRCount)
        return;
    m_rCount = newRCount;
    emit rCountChanged();
}

double LogData::maxCount() const
{
    return m_maxCount;
}

void LogData::setMaxCount(double newMaxCount)
{
    if (qFuzzyCompare(m_maxCount, newMaxCount))
        return;
    m_maxCount = newMaxCount;
    emit maxCountChanged();
}

QString LogData::startDate() const
{
    return m_startDate;
}

void LogData::setStartDate(const QString &newStartDate)
{
    if (m_startDate == newStartDate)
        return;
    m_startDate = newStartDate;
    emit startDateChanged();
}

QString LogData::endDate() const
{
    return m_endDate;
}

void LogData::setEndDate(const QString &newEndDate)
{
    if (m_endDate == newEndDate)
        return;
    m_endDate = newEndDate;
    emit endDateChanged();
}

void LogData::readReady()
{
    readRdy = true;
}

void LogData::radioSelected(int select)
{
    radioS = select;
}

QString LogData::deterCount() const
{
    return m_deterCount;
}

void LogData::setDeterCount(const QString &newDeterCount)
{
    if (m_deterCount == newDeterCount)
        return;
    m_deterCount = newDeterCount;
    emit deterCountChanged();
}

QString LogData::resetCount() const
{
    return m_resetCount;
}

void LogData::setResetCount(const QString &newResetCount)
{
    if (m_resetCount == newResetCount)
        return;
    m_resetCount = newResetCount;
    emit resetCountChanged();
}
