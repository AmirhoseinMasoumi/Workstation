#include "calibration.h"
#include "ui_calibration.h"

Calibration::Calibration(QWidget *parent, QString udpIp, int udpPort) :
    QDialog(parent),
    ui(new Ui::Calibration)
{
    ui->setupUi(this);

    UDP_ip = udpIp;
    UDP_port = udpPort;

    udpSocket = new QUdpSocket(this);

    bool result =  udpSocket->bind(QHostAddress::AnyIPv4, UDP_port);
    if(result)
    {
        qDebug() << "Connected to port " << UDP_port;
    }
    else
    {
        qDebug() << "Failed to connect to port " << UDP_port;
    }
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingData()),Qt::AutoConnection);

    ui->message_txt->setText("Please REMOVE the sheets above the Sensors");
    steps = 0;
    numPacketsReceived = 0;
}

Calibration::~Calibration()
{
    delete ui;
}

void Calibration::on_done_btn_clicked()
{
    switch (steps) {
    case 0:
        ui->message_txt->setText("Please wait ....");
        ui->done_btn->setEnabled(false);

        receiveReq();
        break;
    case 1:
        ui->message_txt->setText("Please wait ....");
        ui->done_btn->setEnabled(false);

        receiveReq();
        break;
    default:
        ui->message_txt->setText("*** Calibration finished ***");
        ui->done_btn->setEnabled(false);
        ui->ok_btn->setEnabled(true);
        break;
    }
}


void Calibration::on_cancel_btn_clicked()
{
    udpSocket->close();
    reject();
}


void Calibration::on_ok_btn_clicked()
{

    QString appDir = QCoreApplication::applicationDirPath();
    QString Cal_H = appDir + "/Cal_H.txt";
    QString Cal_L = appDir + "/Cal_L.txt";
    QFile file_H(Cal_H);
    QFile file_L(Cal_L);

    if(file_H.exists()){
        if (file_H.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file_H);
            stream << convertListToString(data_H);

            file_H.close();
        }
        else
        {
            qDebug() << "Data_H file is empty.";
        }
    }
    else{
        if (file_H.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file_H);
            stream << convertListToString(data_H);

            file_H.close();

            qDebug() << "File created and data_H written successfully.";
        }
        else
        {
            qDebug() << "Failed to create the file.";
        }
    }

    if(file_L.exists()){
        if (file_L.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file_L);
            stream << convertListToString(data_L);

            file_L.close();
        }
        else
        {
            qDebug() << "Data_L file is empty.";
        }
    }
    else{
        if (file_L.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file_L);
            stream << convertListToString(data_L);

            file_L.close();

            qDebug() << "File created and data_L written successfully.";
        }
        else
        {
            qDebug() << "Failed to create the file.";
        }
    }

    udpSocket->close();
    accept();
}

void Calibration::processPendingData()
{
    QByteArray datagram;
    QHostAddress sender;
    uint16_t port;
    QByteArray buffer;
    QHostAddress serverAddress(UDP_ip);

    datagram.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(datagram.data(),datagram.size(),&sender,&port);

    if(datagram.size() == 1200){
        if(datagram.count(header0) == 1200 || datagram.count(header1) == 1200 ||datagram.count(header2) == 1200 ||datagram.count(header3) == 1200 || datagram.count(header4) == 1200){
//            qDebug() << "Header Received";
            readyWrite = true;
            return;
        }
        if(datagram.count(footer) == 1200){
//            qDebug() << "Footer Received";
            readyWrite = false;

            QList<QString> data_str_list = QString(packet).split('$');
            packet.clear();

            QList<int> data_list;
            for (QString str : data_str_list) {
                data_list.append(str.toInt());
            }

            readValues(data_list);

            return;
        }
        if(readyWrite == true){
//            qDebug() << "Data Received";
            packet.append(datagram);
//            qDebug() << packet;
            return;
        }
    }
//    if(datagram.size() > 1){
//        QList<QString> data_str_list = QString(datagram).split('$');

//        QList<int> data_list;
//        for (QString str : data_str_list) {
//            data_list.append(str.toInt());
//        }
//        if(data_list.size() > 1){
//            if(numPacketsReceived < 5){
//                list.append(data_list);
//                receiveReq();
//                numPacketsReceived++;
//                qDebug() << numPacketsReceived << " Packets received";
//            }
//            else{
//                if(steps == 0){
//                    ui->message_txt->setText("Please PLACE the metal sheets above the Sensors");
//                    ui->done_btn->setEnabled(true);
//                    data_L = Avg_cal(list);

//                    steps++;
//                    numPacketsReceived = 0;
//                    list.clear();
//                }
//                else if(steps == 1){
//                    ui->message_txt->setText("Please REMOVE the metal sheets above the Sensors");
//                    ui->done_btn->setEnabled(true);
//                    data_H = Avg_cal(list);

//                    steps++;
//                    numPacketsReceived = 0;
//                    list.clear();
//                }
//            }
//        }
//    }
}

void Calibration::receiveReq()
{
    QHostAddress serverAddress(UDP_ip);
    QByteArray data = "Send";
    if (-1 == udpSocket->writeDatagram(data, serverAddress, UDP_port + 10)){
        // Data write failed, print out warning
        qWarning() << "Unable to write data to " << serverAddress.toString() << ":" << UDP_port + 10;
        reject();
    }
    else{
        qDebug() << "Request sent to "  << serverAddress.toString() << ":" << UDP_port + 10;
    }
}

void Calibration::readValues(QList<int> vals)
{
    if(numPacketsReceived < 2){
        list.append(vals);
        QTimer::singleShot(2000, [this]() {
            receiveReq();
        });
        numPacketsReceived++;
        qDebug() << numPacketsReceived << " Packets received";
    }
    else{
        if(steps == 0){
            ui->message_txt->setText("Please PLACE the metal sheets above the Sensors");
            ui->done_btn->setEnabled(true);
            data_H = Avg_cal(list);

            steps++;
            numPacketsReceived = 0;
            list.clear();
        }
        else if(steps == 1){
            ui->message_txt->setText("Please REMOVE the metal sheets above the Sensors");
            ui->done_btn->setEnabled(true);
            data_L = Avg_cal(list);

            steps++;
            numPacketsReceived = 0;
            list.clear();
        }
    }
}


QList<int> Calibration::Avg_cal(QList<QList<int>> inList)
{
    QList<int> avg;
    if(inList.isEmpty()){
        udpSocket->close();
        reject();
        return avg;
    }
    // Calculate average
    for (int i = 0; i < 800; i++) {
        int sum = 0;
        for (int j = 0; j < 2; j++) {
            sum += inList[j][i];
        }
        avg.append(sum / 2);
    }

    return avg;
}

QString Calibration::convertListToString(const QList<int>& list)
{
    QString result;

    for (int i = 0; i < list.size(); ++i)
    {
        result += QString::number(list[i]);

        if (i < list.size() - 1)
        {
            result += "$";
        }
    }
    return result;
}
