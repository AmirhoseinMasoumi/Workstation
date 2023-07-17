#include "serialport.h"

SerialPort::SerialPort(QObject *parent)
    : QObject{parent}
{
    // create a timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getPortNames()));
    timer->start(1000);

    connect(serialPort, &QSerialPort::readyRead, this, &SerialPort::readData);
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

/////////////////////////////////////////////////////////

void SerialPort::disconnect()
{
    serialPort->close();
    qDebug()<<"Disconnected";
    setConnection(false);
}

void SerialPort::getPortNames()
{
    foreach(QSerialPortInfo port, portInfo.availablePorts()){
        //qInfo() << port.portName() << "-" << port.description();
        if(port.description() == "STM32")
            setPortConfig(port.portName());
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
}

void SerialPort::readFromDevice()
{
    serialPort->write("0x1D");

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"Opening "+saveFilePath+" was Successful!";
    }
    else
    {
        qDebug()<<"Failed to open "+saveFilePath;
    }
}

void SerialPort::createSaveFile(QString path)
{
    saveFilePath = path.right(path.length() - 8) + "/" + QString("%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
    QFile file(saveFilePath);
    if (file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"Opening " + saveFilePath + " was Successful!";
    }
    else
    {
        qDebug()<<"Failed to open " + saveFilePath;
    }
    file.close();
}

void SerialPort::readData()
{
    const QByteArray data = serialPort->readAll();

    QTextStream stream(&file);
    stream << data << Qt::endl;
}
