#include "logdata.h"

LogData::LogData(QObject *parent)
    : QObject{parent},
    m_O2value("00"),
    m_CO2value("00"),
    m_COvalue("00"),
    m_TEMPvalue("00"),
    m_dataCounter(0)
{
    // create a timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loadData()));
    timer->start(1000);

    // create a timer
    timer_s = new QTimer(this);
    connect(timer_s, SIGNAL(timeout()), this, SLOT(secondCount()));
    timer_s->start(1000);

    O2AvgTime = 0, CO2AvgTime = 0, COAvgTime = 0, TempAvgTime = 0;
    O2_sc = 0, CO2_sc = 0, CO_sc = 0, Temp_sc = 0;
    O2Count = 0, CO2Count = 0, COCount = 0, TempCount = 0;
    O2A = 0, CO2A = 0, COA = 0, TempA = 0;

    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName("GasAnalayser_DB.sqlite");

    if(!dataBase.open()){
        qDebug()<<"Failed to open database";
    }else{
        qDebug()<<"Database has been opened in file class";
    }

    setDbTableList(dataBase.tables());
}

bool LogData::connection() const
{
    return m_connection;
}

void LogData::setConnection(bool newConnection)
{
    if (m_connection == newConnection)
        return;
    m_connection = newConnection;
    emit connectionChanged();
}

QString LogData::O2value() const
{
    return m_O2value;
}

void LogData::setO2value(const QString &newO2value)
{
    if (m_O2value == newO2value)
        return;
    m_O2value = newO2value;
    emit O2valueChanged();
}

QString LogData::CO2value() const
{
    return m_CO2value;
}

void LogData::setCO2value(const QString &newCO2value)
{
    if (m_CO2value == newCO2value)
        return;
    m_CO2value = newCO2value;
    emit CO2valueChanged();
}

QString LogData::COvalue() const
{
    return m_COvalue;
}

void LogData::setCOvalue(const QString &newCOvalue)
{
    if (m_COvalue == newCOvalue)
        return;
    m_COvalue = newCOvalue;
    emit COvalueChanged();
}

QString LogData::TEMPvalue() const
{
    return m_TEMPvalue;
}

void LogData::setTEMPvalue(const QString &newTEMPvalue)
{
    if (m_TEMPvalue == newTEMPvalue)
        return;
    m_TEMPvalue = newTEMPvalue;
    emit TEMPvalueChanged();
}

double LogData::dataCounter() const
{
    return m_dataCounter;
}

void LogData::setDataCounter(double newDataCounter)
{
    if (qFuzzyCompare(m_dataCounter, newDataCounter))
        return;
    m_dataCounter = newDataCounter;
    emit dataCounterChanged();
}

QString LogData::o2Avg() const
{
    return m_o2Avg;
}

void LogData::setO2Avg(const QString &newO2Avg)
{
    if (m_o2Avg == newO2Avg)
        return;
    m_o2Avg = newO2Avg;
    emit o2AvgChanged();
}

QString LogData::co2Avg() const
{
    return m_co2Avg;
}

void LogData::setCo2Avg(const QString &newCo2Avg)
{
    if (m_co2Avg == newCo2Avg)
        return;
    m_co2Avg = newCo2Avg;
    emit co2AvgChanged();
}

QString LogData::coAvg() const
{
    return m_coAvg;
}

void LogData::setCoAvg(const QString &newCoAvg)
{
    if (m_coAvg == newCoAvg)
        return;
    m_coAvg = newCoAvg;
    emit coAvgChanged();
}

QString LogData::tempAvg() const
{
    return m_tempAvg;
}

void LogData::setTempAvg(const QString &newTempAvg)
{
    if (m_tempAvg == newTempAvg)
        return;
    m_tempAvg = newTempAvg;
    emit tempAvgChanged();
}

QString LogData::currentTime() const
{
    return m_currentTime;
}

void LogData::setCurrentTime(const QString &newCurrentTime)
{
    if (m_currentTime == newCurrentTime)
        return;
    m_currentTime = newCurrentTime;
    emit currentTimeChanged();
}

QStringList LogData::dbTableList() const
{
    return m_dbTableList;
}

void LogData::setDbTableList(const QStringList &newDbTableList)
{
    if (m_dbTableList == newDbTableList)
        return;
    m_dbTableList = newDbTableList;
    emit dbTableListChanged();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LogData::createFile(QString path)
{
    qDebug() << path;
    filename = path.right(path.length() - 8) + "/" + QString("%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    qDebug() << filename;
    QFile file(filename);
    if ( file.open(QIODevice::WriteOnly | QIODevice::Append) )
    {
        QTextStream stream( &file );
    }
    file.close();
}

void LogData::logDataInFile(QString d1, QString d2, QString d3, QString d4)
{
    //    filename = "C://Users//Legend//Desktop//Temp//Project//Book1.csv"; //////////////////
    //    QFile file(filename);
    //    qDebug()<<filename;
    //    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
    //        QTextStream stream(&file);
    //        stream << QDateTime::currentDateTime().toString("ddMMyyyy-hh_mm_ss") << "," << d1 << "," << d2  << "," << d3  << "," << d4  << Qt::endl;

    //    }
    //    file.close();
    qDebug() << QDateTime::currentDateTime().toString("ddMMyyyy-hh_mm_ss") << "," << d1 << "," << d2  << "," << d3  << "," << d4  << Qt::endl;
}

void LogData::loadData()
{
    if(connection() == false && readyReadStatus == true){
        QFile file(loadFilePath);
        setDataCounter(0);       

//        if(firstRead == true){
//            if (file.open(QIODevice::ReadOnly)) {
//                QTextStream in(&file);
//                qDebug() << "Load file";
//                while (!in.atEnd())
//                {
//                    QString line = in.readLine();
//                    QStringList list = line.split(',');

//                    setO2value(list[1]);
//                    setCO2value(list[2]);
//                    setCOvalue(list[3]);
//                    setTEMPvalue(list[4]);

//                    Time_List.append(list[0].right(8));
//                    O2_List.append(list[1]);
//                    CO2_List.append(list[2]);
//                    CO_List.append(list[3]);
//                    Temp_List.append(list[4]);

//                    setCurrentTime(list[0].right(8));

//                    setDataCounter(dataCounter()+1);
//                    firstRead = false;
//                    //                    if(O2_sc < O2AvgTime*60){
//                    //                        O2A += O2value().toDouble();
//                    //                        O2Count++;
//                    //                    }else{
//                    //                        setO2Avg(QString::number(O2A/O2Count));
//                    //                        O2Count = 0;
//                    //                        O2_sc = 0;
//                    //                        O2A = 0;
//                    //                    }
//                    //                    if(CO2_sc < CO2AvgTime*60){
//                    //                        CO2A += CO2value().toDouble();
//                    //                        CO2Count++;
//                    //                    }else{
//                    //                        setCo2Avg(QString::number(CO2A/CO2Count));
//                    //                        CO2Count = 0;
//                    //                        CO2_sc = 0;
//                    //                        CO2A = 0;
//                    //                    }
//                    //                    if(CO_sc < COAvgTime*60){
//                    //                        COA += COvalue().toDouble();
//                    //                        COCount++;
//                    //                    }else{
//                    //                        setCoAvg(QString::number(COA/COCount));
//                    //                        COCount = 0;
//                    //                        CO_sc = 0;
//                    //                        COA = 0;
//                    //                    }
//                    //                    if(Temp_sc < TempAvgTime*60){
//                    //                        TempA += TEMPvalue().toDouble();
//                    //                        TempCount++;
//                    //                    }else{
//                    //                        setTempAvg(QString::number(TempA/TempCount));
//                    //                        TempCount = 0;
//                    //                        Temp_sc = 0;
//                    //                        TempA = 0;
//                    //                    }
//                }
//                file.close();
//            }
//        }
    }

}

void LogData::getO2AvgTime(QString O2AvgT)
{
    O2AvgTime = O2AvgT.toDouble();
}

void LogData::getCO2AvgTime(QString CO2AvgT)
{
    CO2AvgTime = CO2AvgT.toDouble();
}

void LogData::getCOAvgTime(QString COAvgT)
{
    COAvgTime = COAvgT.toDouble();
}

void LogData::getTempAvgTime(QString TempAvgT)
{
    TempAvgTime = TempAvgT.toDouble();
}

void LogData::secondCount()
{
    O2_sc++;
    CO2_sc++;
    CO_sc++;
    Temp_sc++;
}

void LogData::getLoadPath(QString path)
{
    loadFilePath = path.right(path.length() - 8);
    qDebug() << loadFilePath;
}

void LogData::readyRead(bool status)
{
    readyReadStatus = status;
    if(status == false){
        readCount = 0;
        firstRead = true;
    }else{
        loadData();
    }
}

void LogData::loadFromList()
{
    if(firstRead == false){
        readCount++;
        setO2value(O2_List.value(readCount));
        setCO2value(CO2_List.value(readCount));
        setCOvalue(CO_List.value(readCount));
        setTEMPvalue(Temp_List.value(readCount));

        setCurrentTime(Time_List.value(readCount));

        setDataCounter(readCount);
        if(readCount >= Time_List.length()){
            readCount = 0;
        }
    }
}

void LogData::loadSqlite(QString table)
{
    QString SelectTable = "SELECT * FROM " + table;
    QSqlQuery query(SelectTable);
    while (query.next())
    {
        readCount++;
        QString Time = query.value(0).toString();
        QString O2 = query.value(1).toString();
        QString CO2 = query.value(2).toString();
        QString CO = query.value(3).toString();
        QString Temp = query.value(4).toString();
        //qDebug() << Time << O2 << CO2 << CO << Temp;

        setO2value(O2);
        setCO2value(CO2);
        setCOvalue(CO);
        setTEMPvalue(Temp);
        setCurrentTime(Time);
        setDataCounter(readCount);
    }
}

void LogData::createSql()
{
    dataBase.close();
    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName("GasAnalayser_DB.sqlite");

    if(!dataBase.open()){
        qDebug()<<"Failed to open database";
    }else{
        qDebug()<<"Database has been opened in file class";
    }
    qDebug() << dataBase.tables();
    setDbTableList(dataBase.tables());
}



