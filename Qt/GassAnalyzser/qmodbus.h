#ifndef QMODBUS_H
#define QMODBUS_H

#include <QThread>
#include<QModbusTcpClient>
class qmodbus : public QThread
{
    Q_OBJECT
    int id=1;
    void run();
public:
    explicit qmodbus(QObject *parent = nullptr);
    QString getTcpAddr();

private:
    QString TCP_Addr = "";
    QString TCP_Port = "";

public slots:
    void setTcpAddr(QString Addr);
    void setTcpPort(QString Port);

signals:
    void updateData(int O2_val, int CO2_val, int CO_val, int Temp_val);
};

#endif // QMODBUS_H
