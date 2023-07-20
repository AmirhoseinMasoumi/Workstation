#include "modbusctr.h"

ModbusCTR::ModbusCTR(QObject *parent)
    : QObject{parent},
    m_counter(0),
    m_maxValue("0")
{
    mbus = new qmodbus(this);
    connect(mbus,&qmodbus::finished,this,[this](){
        QTimer::singleShot(2000,mbus,SLOT(start()));//reconect after 1s after modbus thread finisch
    });
    connect(mbus,SIGNAL(updateData(int,int,int,int)),this,SLOT(readValues(int,int,int,int))); //modbus data update emit signal. coreObj re_emit signal

    // create a timer
    timer_s = new QTimer(this);
    connect(timer_s, SIGNAL(timeout()), this, SLOT(secondCount()));
    timer_s->start(1000);

    // create a timer
    timer_p = new QTimer(this);
    connect(timer_p, SIGNAL(timeout()), this, SLOT(checkSystemStatus()));
    timer_p->start(500);

    pingProcess = new QProcess;

    O2_sc = 0, CO2_sc = 0, CO_sc = 0, Temp_sc = 0;
    O2Count = 0, CO2Count = 0, COCount = 0, TempCount = 0;
    O2A = 0, CO2A = 0, COA = 0, TempA = 0;

    QDateTime currentDateTime = QDateTime::currentDateTime();
    setCurrentTime(currentDateTime.toString("hh:mm:ss"));


    dataBase = QSqlDatabase::addDatabase("QSQLITE");
    dataBase.setDatabaseName("GasAnalayser_DB.sqlite");

    if(!dataBase.open()){
        qDebug()<<"Failed to open database";
    }else{
        qDebug()<<"Database has been opened";
    }
}

void ModbusCTR::getSaveFilePath(QString path)
{
    filePath = path.right(path.length() - 8) + "/" + QString("%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    file.setFileName(filePath);
}

void ModbusCTR::getConnectionStatus(bool status)
{
    if(status == true){
//        if(file.open(QIODevice::WriteOnly | QIODevice::Append))
//        {
//            qDebug()<<"Opening " + filePath + " was Successful!";
//        }
//        else
//        {
//            qDebug()<<"Failed to open " + filePath;
//        }
        connection = true;
    }
    else{
        qDebug() << "Close";
//        file.close();
        dataBase.close();
        connection = false;
    }
}

void ModbusCTR::getTCPip(QString ip)
{
    mbus->setTcpAddr(ip);
}

void ModbusCTR::getTCPport(QString port)
{
    mbus->setTcpPort(port);
}

ModbusCTR::~ModbusCTR()
{
    mbus->exit();
    mbus->wait();
}

void ModbusCTR::start()
{
    mbus->start();
}

void ModbusCTR::readValues(int O2_val, int CO2_val, int CO_val, int Temp_val)
{
    if(connection == true){
        //qDebug() << "Slot: " << O2_val << CO2_val << CO_val << Temp_val;

        setO2_tcp_val(QString::number(O2_val));
        setCO2_tcp_val(QString::number(CO2_val));
        setCO_tcp_val(QString::number(CO_val));
        setTemp_tcp_val(QString::number(Temp_val));

        if(CO2_val > CO_val){
            if(CO2_val > maxValue().toFloat())
                setMaxValue(QString::number(CO2_val));
        }
        else{
            if(CO_val > maxValue().toFloat())
                setMaxValue(QString::number(CO2_val));
        }

        QDateTime currentDateTime = QDateTime::currentDateTime();
        setCurrentTime(currentDateTime.toString("hh:mm:ss"));

        addNewValue(currentDateTime.toString("hh:mm:ss"), QString::number(O2_val), QString::number(CO2_val), QString::number(CO_val), QString::number(Temp_val));

//        writeCount++;
//        if(writeCount == 200){
//            writeCount = 0;
//            file.close();
//            if(file.open(QIODevice::WriteOnly | QIODevice::Append))
//            {
//                qDebug()<<"Opening " + filePath + " was Successful!";
//            }
//            else
//            {
//                qDebug()<<"Failed to open " + filePath;
//            }
//        }
//        QTextStream stream(&file);
//        stream << QDateTime::currentDateTime().toString("MM/dd/yyyy_hh:mm:ss") << "," << QString::number(O2_val) << "," << QString::number(CO2_val)  << "," <<  QString::number(CO_val)  << "," <<  QString::number(Temp_val)  << Qt::endl;

        if(O2_sc < O2AvgTime*60){
            O2A += O2_tcp_val().toDouble();
            O2Count++;
        }else if (O2_sc > O2AvgTime*60) {
            O2Count = 0;
            O2_sc = 0;
            O2A = 0;
        }else{
            setO2Avg(QString::number(O2A/O2Count));
            O2Count = 0;
            O2_sc = 0;
            O2A = 0;
        }
        if(CO2_sc < CO2AvgTime*60){
            CO2A += CO2_tcp_val().toDouble();
            CO2Count++;
        }else if (CO2_sc > CO2AvgTime*60) {
            CO2Count = 0;
            CO2_sc = 0;
            CO2A = 0;
        }else{
            setCo2Avg(QString::number(CO2A/CO2Count));
            CO2Count = 0;
            CO2_sc = 0;
            CO2A = 0;
        }
        if(CO_sc < COAvgTime*60){
            COA += CO_tcp_val().toDouble();
            COCount++;
        }else if (CO_sc > COAvgTime*60) {
            COCount = 0;
            CO_sc = 0;
            COA = 0;
        }else{
            setCoAvg(QString::number(COA/COCount));
            COCount = 0;
            CO_sc = 0;
            COA = 0;
        }
        if(Temp_sc < TempAvgTime*60){
            TempA += Temp_tcp_val().toDouble();
            TempCount++;
        }else if (Temp_sc > TempAvgTime*60) {
            TempCount = 0;
            Temp_sc = 0;
            TempA = 0;
        }else{
            setTempAvg(QString::number(TempA/TempCount));
            TempCount = 0;
            Temp_sc = 0;
            TempA = 0;
        }
        setCounter(counter() + 1);
    }
}

void ModbusCTR::resetCounter()
{
    setCounter(0);
}

void ModbusCTR::secondCount()
{
    O2_sc++;
    CO2_sc++;
    CO_sc++;
    Temp_sc++;
}

void ModbusCTR::checkSystemStatus()
{
    QString nParameter = "-n";
    QString pingCount = "1"; //(int)
    QString wParameter = "-w";
    QString pingWaitTime = "10"; //(ms)
    TCP_Ip = mbus->getTcpAddr();
    pingProcess->start("ping",QStringList()<<TCP_Ip<<nParameter<<pingCount<<wParameter<<pingWaitTime);
    pingProcess->waitForFinished();
    if(pingProcess->readAllStandardOutput().contains("1ms") && TCP_Ip != ""){
        setSystemStatus("Connected");
        discCount = 0;
//        if(!file.open(QIODevice::WriteOnly | QIODevice::Append)){
//            if(file.open(QIODevice::WriteOnly | QIODevice::Append))
//            {
//                //qDebug()<<"Opening " + filePath + " was Successful!";
//            }
//            else
//            {
//                //qDebug()<<"Failed to open " + filePath;
//            }
//        }
    }
    else{
        discCount++;
        //qDebug() << discCount;
        if(discCount > 5){
            setSystemStatus("Disconnected");
            discCount = 0;
//            file.close();
            //getConnectionStatus(false);
        }
    }
}

void ModbusCTR::getO2AvgTime(QString O2AvgT)
{
    O2AvgTime = O2AvgT.toDouble();
}

void ModbusCTR::getCO2AvgTime(QString CO2AvgT)
{
    CO2AvgTime = CO2AvgT.toDouble();
}

void ModbusCTR::getCOAvgTime(QString COAvgT)
{
    COAvgTime = COAvgT.toDouble();
}

void ModbusCTR::getTempAvgTime(QString TempAvgT)
{
    TempAvgTime = TempAvgT.toDouble();
}

bool ModbusCTR::addNewValue(const QString& Time,const QString& O2,const QString& CO2, const QString& CO, const QString& Temp)
{
    bool success = false;
    // you should check if args are ok first...
    QSqlQuery query;
    QString TempInsert = "INSERT INTO " + table + " VALUES (:Time,:O2,:CO2,:CO,:Temperature)";
    query.prepare(TempInsert);
    query.bindValue(":Time", Time);
    query.bindValue(":O2", O2);
    query.bindValue(":CO2", CO2);
    query.bindValue(":CO", CO);
    query.bindValue(":Temperature", Temp);
    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "add value error";
    }

    return success;
}

void ModbusCTR::createNewDb()
{
    table = "DB_" + QDateTime::currentDateTime().toString("MMddyyyy_hhmmss");
    QString Temp = "create table " + table + " (Time TEXT, O2 TEXT, CO2 TEXT, CO TEXT, Temperature TEXT)";
    qDebug() << Temp;
    QSqlQuery query;
    if(query.exec(Temp)){
        qDebug() << "Table created";
    }
    else{
        qDebug() << "Failed to create table";
    }
}

///////////////////////////////////////////////////////////

QString ModbusCTR::O2_tcp_val() const
{
    return m_O2_tcp_val;
}

void ModbusCTR::setO2_tcp_val(const QString &newO2_tcp_val)
{
    if (m_O2_tcp_val == newO2_tcp_val)
        return;
    m_O2_tcp_val = newO2_tcp_val;
    emit O2_tcp_valChanged();
}

QString ModbusCTR::CO2_tcp_val() const
{
    return m_CO2_tcp_val;
}

void ModbusCTR::setCO2_tcp_val(const QString &newCO2_tcp_val)
{
    if (m_CO2_tcp_val == newCO2_tcp_val)
        return;
    m_CO2_tcp_val = newCO2_tcp_val;
    emit CO2_tcp_valChanged();
}

QString ModbusCTR::CO_tcp_val() const
{
    return m_CO_tcp_val;
}

void ModbusCTR::setCO_tcp_val(const QString &newCO_tcp_val)
{
    if (m_CO_tcp_val == newCO_tcp_val)
        return;
    m_CO_tcp_val = newCO_tcp_val;
    emit CO_tcp_valChanged();
}

QString ModbusCTR::Temp_tcp_val() const
{
    return m_Temp_tcp_val;
}

void ModbusCTR::setTemp_tcp_val(const QString &newTemp_tcp_val)
{
    if (m_Temp_tcp_val == newTemp_tcp_val)
        return;
    m_Temp_tcp_val = newTemp_tcp_val;
    emit Temp_tcp_valChanged();
}

double ModbusCTR::counter() const
{
    return m_counter;
}

void ModbusCTR::setCounter(double newCounter)
{
    if (qFuzzyCompare(m_counter, newCounter))
        return;
    m_counter = newCounter;
    emit counterChanged();
}

QString ModbusCTR::o2Avg() const
{
    return m_o2Avg;
}

void ModbusCTR::setO2Avg(const QString &newO2Avg)
{
    if (m_o2Avg == newO2Avg)
        return;
    m_o2Avg = newO2Avg;
    emit o2AvgChanged();
}

QString ModbusCTR::co2Avg() const
{
    return m_co2Avg;
}

void ModbusCTR::setCo2Avg(const QString &newCo2Avg)
{
    if (m_co2Avg == newCo2Avg)
        return;
    m_co2Avg = newCo2Avg;
    emit co2AvgChanged();
}

QString ModbusCTR::coAvg() const
{
    return m_coAvg;
}

void ModbusCTR::setCoAvg(const QString &newCoAvg)
{
    if (m_coAvg == newCoAvg)
        return;
    m_coAvg = newCoAvg;
    emit coAvgChanged();
}

QString ModbusCTR::tempAvg() const
{
    return m_tempAvg;
}

void ModbusCTR::setTempAvg(const QString &newTempAvg)
{
    if (m_tempAvg == newTempAvg)
        return;
    m_tempAvg = newTempAvg;
    emit tempAvgChanged();
}

QString ModbusCTR::systemStatus() const
{
    return m_systemStatus;
}

void ModbusCTR::setSystemStatus(const QString &newSystemStatus)
{
    if (m_systemStatus == newSystemStatus)
        return;
    m_systemStatus = newSystemStatus;
    emit systemStatusChanged();
}

QString ModbusCTR::currentTime() const
{
    return m_currentTime;
}

void ModbusCTR::setCurrentTime(const QString &newCurrentTime)
{
    if (m_currentTime == newCurrentTime)
        return;
    m_currentTime = newCurrentTime;
    emit currentTimeChanged();
}

QString ModbusCTR::maxValue() const
{
    return m_maxValue;
}

void ModbusCTR::setMaxValue(const QString &newMaxValue)
{
    if (m_maxValue == newMaxValue)
        return;
    m_maxValue = newMaxValue;
    emit maxValueChanged();
}
