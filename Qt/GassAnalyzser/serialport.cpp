#include "serialport.h"

SerialPort::SerialPort(QObject *parent)
    : QObject{parent},
      m_connection(false),
      m_counter(0),
      m_o2Avg("0"),
      m_co2Avg("0"),
      m_coAvg("0"),
      m_tempAvg("0")
{
    // create a timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getPortNames()));
    timer->start(1000);

    // create a timer
    timer_s = new QTimer(this);
    connect(timer_s, SIGNAL(timeout()), this, SLOT(secondCount()));
    timer_s->start(1000);

    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, this, &SerialPort::readData);

    O2_sc = 0, CO2_sc = 0, CO_sc = 0, Temp_sc = 0;
    O2Count = 0, CO2Count = 0, COCount = 0, TempCount = 0;
    O2A = 0, CO2A = 0, COA = 0, TempA = 0;

    QDateTime currentDateTime = QDateTime::currentDateTime();
    setCurrentTime(currentDateTime.toString("hh:mm:ss"));
}

QStringList SerialPort::portList() const
{
    return m_portList;
}

void SerialPort::setPortList(const QStringList &newPortList)
{
    if (m_portList == newPortList)
        return;
    m_portList = newPortList;
    emit portListChanged();
}

bool SerialPort::connection() const
{
    return m_connection;
}

void SerialPort::setConnection(bool newConnection)
{
    if (m_connection == newConnection)
        return;
    m_connection = newConnection;
    emit connectionChanged();
}

QString SerialPort::o2Val() const
{
    return m_o2Val;
}

void SerialPort::setO2Val(const QString &newO2Val)
{
    if (m_o2Val == newO2Val)
        return;
    m_o2Val = newO2Val;
    emit o2ValChanged();
}

QString SerialPort::co2Val() const
{
    return m_co2Val;
}

void SerialPort::setCo2Val(const QString &newCo2Val)
{
    if (m_co2Val == newCo2Val)
        return;
    m_co2Val = newCo2Val;
    emit co2ValChanged();
}

QString SerialPort::coVal() const
{
    return m_coVal;
}

void SerialPort::setCoVal(const QString &newCoVal)
{
    if (m_coVal == newCoVal)
        return;
    m_coVal = newCoVal;
    emit coValChanged();
}

QString SerialPort::tempVal() const
{
    return m_tempVal;
}

void SerialPort::setTempVal(const QString &newTempVal)
{
    if (m_tempVal == newTempVal)
        return;
    m_tempVal = newTempVal;
    emit tempValChanged();
}

double SerialPort::counter() const
{
    return m_counter;
}

void SerialPort::setCounter(double newCounter)
{
    if (qFuzzyCompare(m_counter, newCounter))
        return;
    m_counter = newCounter;
    emit counterChanged();
}


QString SerialPort::o2Avg() const
{
    return m_o2Avg;
}

void SerialPort::setO2Avg(const QString &newO2Avg)
{
    if (m_o2Avg == newO2Avg)
        return;
    m_o2Avg = newO2Avg;
    emit o2AvgChanged();
}

QString SerialPort::co2Avg() const
{
    return m_co2Avg;
}

void SerialPort::setCo2Avg(const QString &newCo2Avg)
{
    if (m_co2Avg == newCo2Avg)
        return;
    m_co2Avg = newCo2Avg;
    emit co2AvgChanged();
}

QString SerialPort::coAvg() const
{
    return m_coAvg;
}

void SerialPort::setCoAvg(const QString &newCoAvg)
{
    if (m_coAvg == newCoAvg)
        return;
    m_coAvg = newCoAvg;
    emit coAvgChanged();
}

QString SerialPort::tempAvg() const
{
    return m_tempAvg;
}

void SerialPort::setTempAvg(const QString &newTempAvg)
{
    if (m_tempAvg == newTempAvg)
        return;
    m_tempAvg = newTempAvg;
    emit tempAvgChanged();
}

QString SerialPort::currentTime() const
{
    return m_currentTime;
}

void SerialPort::setCurrentTime(const QString &newCurrentTime)
{
    if (m_currentTime == newCurrentTime)
        return;
    m_currentTime = newCurrentTime;
    emit currentTimeChanged();
}

//////////////////////////////////////////////////////////

void SerialPort::disconnect()
{
    serialPort->close();
    qDebug()<<"Disconnected";
    setConnection(false);
    file.close();
    setCounter(0);
}

void SerialPort::getPortNames()
{
    if(readyConnect == true && connection() == true){
        foreach(QSerialPortInfo port, portInfo.availablePorts()){
            //qInfo() << port.portName() << "-" << port.description();
            if(port.description() == "Silicon Labs CP210x USB to UART Bridge"){
//            if(port.description() == "Silicon Labs CP210x USB to UART Bridge"){
                setPortConfig(port.portName());
            }
        }
    }
}

void SerialPort::setPortConfig(QString portName)
{
    serialPort->setPortName(portName);
    serialPort->setBaudRate(9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if(serialPort->open(QSerialPort::ReadWrite)){
        qDebug()<<"Connected to "<< portName;
        setConnection(true);
    }
    else{
        qCritical()<<"Serial Port error: " << serialPort->errorString();
        setConnection(false);
    }

    if(file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug()<<"Opening "+filePath+" was Successful!";
    }
    else
    {
        qDebug()<<"Failed to open "+filePath;
    }
}

void SerialPort::getSaveFilePath(QString path)
{
    filePath = path.right(path.length() - 8) + "/" + QString("%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    file.setFileName(filePath);

}

void SerialPort::changeConnectionStatus(bool status)
{
    readyConnect = status;
    if(status == false && connection() == true){
        disconnect();
    }
}

void SerialPort::connectionReady(bool status)
{
    setConnection(status);
}

void SerialPort::getO2AvgTime(QString O2AvgT)
{
    O2AvgTime = O2AvgT.toDouble();
}

void SerialPort::getCO2AvgTime(QString CO2AvgT)
{
    CO2AvgTime = CO2AvgT.toDouble();
}

void SerialPort::getCOAvgTime(QString COAvgT)
{
    COAvgTime = COAvgT.toDouble();
}

void SerialPort::getTempAvgTime(QString TempAvgT)
{
    TempAvgTime = TempAvgT.toDouble();
}

void SerialPort::secondCount()
{
    O2_sc++;
    CO2_sc++;
    CO_sc++;
    Temp_sc++;
}

void SerialPort::readData()
{
    QByteArray data = serialPort->readAll();
    inputData.append(data);
    if(inputData.length() == 16){
        inputData.remove(15,1);
        QString value = QString(inputData);

        QStringList splitedData = value.split(QLatin1Char(','));
        inputData = "";

        writeCount++;
        if(writeCount == 200){
            writeCount = 0;
            file.close();
            if(file.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                qDebug()<<"Opening " + filePath + " was Successful!";
            }
            else
            {
                qDebug()<<"Failed to open " + filePath;
            }
        }
        QTextStream stream(&file);
        stream << QDateTime::currentDateTime().toString("ddMMyyyy-hh_mm_ss") << "," << QString::number(splitedData[0].toInt()) << "," << QString::number(splitedData[1].toInt())  << "," << QString::number(splitedData[2].toInt())  << "," << QString::number(splitedData[3].toInt())  << Qt::endl;

        setO2Val(QString::number(splitedData[0].toInt()));
        setCo2Val(QString::number(splitedData[1].toInt()));
        setCoVal(QString::number(splitedData[2].toInt()));
        setTempVal(QString::number(splitedData[3].toInt()));

        QDateTime currentDateTime = QDateTime::currentDateTime();
        setCurrentTime(currentDateTime.toString("hh:mm:ss"));

        if(O2_sc < O2AvgTime*60){
            O2A += o2Val().toDouble();
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
            CO2A += co2Val().toDouble();
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
            COA += coVal().toDouble();
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
            TempA += tempVal().toDouble();
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

        setCounter(counter()+1);
    }

}


