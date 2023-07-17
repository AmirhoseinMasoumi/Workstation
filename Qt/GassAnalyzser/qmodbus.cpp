#include "qmodbus.h"
#include <QTimer>
#include <QVariant>
#include <QDebug>

void qmodbus::run(){
    QModbusTcpClient mc;
    QTimer tim_event_timer;
    tim_event_timer.setSingleShot(true);
    QVector<quint16> tab(4);
    QModbusDataUnit du(QModbusDataUnit::HoldingRegisters,1,tab);
    QModbusReply *reply;
    //define modbus reply function ....................................................................[1]
    auto rpl_redy =[&](){
        if(!reply->error()){
            //qDebug() << "Data receiver";
            emit updateData(reply->result().value(0), reply->result().value(1), reply->result().value(2), reply->result().value(3));
            tim_event_timer.start(1000); //next modbus Request after one second
        }else{
            exit();
        }
        reply->deleteLater();
    };
    //define timer timeout function........................................................................[2]
    auto timeout= [&](){
        reply=mc.sendReadRequest(du,1); //new Request modbus adress 255
        if(reply->isFinished()){
            delete reply;        
        }else{
            connect(reply,&QModbusReply::finished,rpl_redy); //set callback function when reply ready.  call[1]
        }
    };
    //define modbus device state change function...........................................................[3] 
    auto stateChange = [&](QModbusClient::State state){
        if (state==QModbusClient::ConnectedState){
            tim_event_timer.start(1000);
        }else if(state==QModbusClient::UnconnectedState) {
            exit();
        }
    };
    connect(&tim_event_timer,&QTimer::timeout,timeout);           //set callback function ->[2] as timeut
    connect(&mc, &QModbusClient::stateChanged,stateChange);       //set callback function ->[3] when qmodbus state change
    //qDebug() << TCP_Addr << TCP_Port << "***";
    mc.setConnectionParameter(QModbusDevice::NetworkAddressParameter, TCP_Addr);
    mc.setConnectionParameter(QModbusDevice::NetworkPortParameter, TCP_Port);
    mc.setTimeout(500);
    if (!mc.connectDevice()){
        qDebug() << "Modbus error";
        exit();
        return;
    }
    exec(); //qt event loop 
}

qmodbus::qmodbus(QObject *parent) : QThread(parent)
{

}

QString qmodbus::getTcpAddr()
{
    return TCP_Addr;
}

void qmodbus::setTcpAddr(QString Addr)
{
    TCP_Addr = Addr;
}

void qmodbus::setTcpPort(QString Port)
{
    TCP_Port = Port;
}
