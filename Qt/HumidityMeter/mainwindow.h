#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QGroupBox>
#include <QListView>
#include <QStandardItemModel>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <QUdpSocket>
#include <QTextCodec>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QRandomGenerator>
#include "calibration.h"
#include <QThread>
#include <QDateTime>
#include <QRandomGenerator>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QUdpSocket *socket = nullptr;
    QByteArray header0 = QString("0").toLatin1();
    QByteArray header1 = QString("1").toLatin1();
    QByteArray header2 = QString("2").toLatin1();
    QByteArray header3 = QString("3").toLatin1();
    QByteArray header4 = QString("4").toLatin1();
    QByteArray footer = QString("*").toLatin1();
    bool readyWrite = false;
    bool waitForFooter = false;
    QString packet = "";

    static const int ROWS = 25;
    static const int COLS = 16;
    void initialImages(void);

    int sheet_coefficient;

    QColor Right_colorMatrix[ROWS][COLS];
    QColor Left_colorMatrix[ROWS][COLS];
    QColor Clear_colorMatrix[ROWS][COLS];
    bool calibration = false;
    void createRectangleGrid(QGroupBox *groupBox, QColor colorMatrix[ROWS][COLS]);
    void createTemperatureRect(QGroupBox *groupBox, int x, int y);
    QList<int> calibrateMoistureValues(const QList<int>& data_h, const QList<int>& data_L, const QList<int>& vals);

    QColor getColor(int val, double min1, double max1);

    bool connectionStatus = false;
    QList<int> data_H;
    QList<int> data_L;
    bool calibrated = false;

    class Sleeper : public QThread
    {
    public:
        static void usleep(unsigned long usecs){QThread::usleep(usecs);}
        static void msleep(unsigned long msecs){QThread::msleep(msecs);}
        static void sleep(unsigned long secs){QThread::sleep(secs);}
    };

    void smoothColumnsAroundLowest(QVector<QVector<int>>& matrix);
    void clearCheck(QList<int>& matrix);

private slots:
    void on_coefficient_Custom_toggled(bool checked);
    void on_dm_Calibration_toggled(bool checked);
    void on_coefficient_8_clicked();
    void on_coefficient_10_clicked();
    void on_coefficient_12_clicked();
    void on_select_btn_clicked();
    void on_Calirbate_btn_clicked();
    void on_connectBtn_clicked();

    void readValues(QList<int> vals);
    void processPendingDatagrams();
    bool checkCalFile(void);
    void on_dm_Normal_toggled(bool checked);
};

#endif // MAINWINDOW_H
