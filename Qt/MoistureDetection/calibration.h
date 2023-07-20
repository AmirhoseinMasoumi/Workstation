#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QDialog>
#include <QTimer>
#include <QUdpSocket>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QTextStream>

namespace Ui {
class Calibration;
}

class Calibration : public QDialog
{
    Q_OBJECT

public:
    explicit Calibration(QWidget *parent = nullptr, QString udpIp = "127.0.0.1", int udpPort = 2020);
    ~Calibration();

private slots:
    void on_done_btn_clicked();
    void on_cancel_btn_clicked();
    void on_ok_btn_clicked();

    void processPendingData();

private:
    Ui::Calibration *ui;

    int steps;

    QUdpSocket *udpSocket;
    QString UDP_ip;
    int UDP_port;
    void receiveReq(void);
    QByteArray header0 = QString("0").toLatin1();
    QByteArray header1 = QString("1").toLatin1();
    QByteArray header2 = QString("2").toLatin1();
    QByteArray header3 = QString("3").toLatin1();
    QByteArray header4 = QString("4").toLatin1();
    QByteArray footer = QString("*").toLatin1();
    bool readyWrite = false;
    bool waitForFooter = false;
    QString packet = "";
    void readValues(QList<int> vals);

    QList<QList<int>> list;
    int numPacketsReceived = 0;
    QList<int> Avg_cal(QList<QList<int>> inList);
    QList<int> data_H;
    QList<int> data_L;
    QSettings settings;

    QString convertListToString(const QList<int>& list);
};

#endif // CALIBRATION_H
