/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QGroupBox *Right_Image;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *deviceIp;
    QLineEdit *devicePort;
    QLabel *label;
    QPushButton *connectBtn;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *dm_Normal;
    QRadioButton *dm_Calibration;
    QPushButton *Calirbate_btn;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *coefficient_8;
    QRadioButton *coefficient_10;
    QRadioButton *coefficient_12;
    QRadioButton *coefficient_Custom;
    QLineEdit *coefficient_input;
    QPushButton *select_btn;
    QGroupBox *left_Image;
    QLabel *tab_lbl;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1920, 1080);
        MainWindow->setMaximumSize(QSize(1920, 1080));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        Right_Image = new QGroupBox(centralwidget);
        Right_Image->setObjectName(QString::fromUtf8("Right_Image"));
        Right_Image->setMinimumSize(QSize(400, 700));
        Right_Image->setMaximumSize(QSize(16777215, 900));

        gridLayout_3->addWidget(Right_Image, 0, 1, 3, 1);

        groupBox_5 = new QGroupBox(centralwidget);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setMaximumSize(QSize(200, 16777215));
        gridLayout_2 = new QGridLayout(groupBox_5);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(groupBox_5);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        deviceIp = new QLineEdit(groupBox_5);
        deviceIp->setObjectName(QString::fromUtf8("deviceIp"));
        deviceIp->setEnabled(true);
        deviceIp->setMaximumSize(QSize(75, 16777215));
        QFont font;
        font.setPointSize(8);
        deviceIp->setFont(font);
        deviceIp->setReadOnly(false);

        gridLayout->addWidget(deviceIp, 0, 1, 1, 1);

        devicePort = new QLineEdit(groupBox_5);
        devicePort->setObjectName(QString::fromUtf8("devicePort"));
        devicePort->setEnabled(true);
        devicePort->setMaximumSize(QSize(75, 16777215));
        devicePort->setFont(font);
        devicePort->setReadOnly(false);

        gridLayout->addWidget(devicePort, 1, 1, 1, 1);

        label = new QLabel(groupBox_5);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        connectBtn = new QPushButton(groupBox_5);
        connectBtn->setObjectName(QString::fromUtf8("connectBtn"));

        gridLayout->addWidget(connectBtn, 2, 0, 1, 2);


        verticalLayout_3->addLayout(gridLayout);

        groupBox = new QGroupBox(groupBox_5);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        dm_Normal = new QRadioButton(groupBox);
        dm_Normal->setObjectName(QString::fromUtf8("dm_Normal"));
        dm_Normal->setChecked(true);

        verticalLayout->addWidget(dm_Normal);

        dm_Calibration = new QRadioButton(groupBox);
        dm_Calibration->setObjectName(QString::fromUtf8("dm_Calibration"));

        verticalLayout->addWidget(dm_Calibration);

        Calirbate_btn = new QPushButton(groupBox);
        Calirbate_btn->setObjectName(QString::fromUtf8("Calirbate_btn"));
        Calirbate_btn->setEnabled(false);

        verticalLayout->addWidget(Calirbate_btn);


        verticalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(groupBox_5);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        coefficient_8 = new QRadioButton(groupBox_2);
        coefficient_8->setObjectName(QString::fromUtf8("coefficient_8"));
        coefficient_8->setChecked(true);

        verticalLayout_2->addWidget(coefficient_8);

        coefficient_10 = new QRadioButton(groupBox_2);
        coefficient_10->setObjectName(QString::fromUtf8("coefficient_10"));

        verticalLayout_2->addWidget(coefficient_10);

        coefficient_12 = new QRadioButton(groupBox_2);
        coefficient_12->setObjectName(QString::fromUtf8("coefficient_12"));

        verticalLayout_2->addWidget(coefficient_12);

        coefficient_Custom = new QRadioButton(groupBox_2);
        coefficient_Custom->setObjectName(QString::fromUtf8("coefficient_Custom"));

        verticalLayout_2->addWidget(coefficient_Custom);

        coefficient_input = new QLineEdit(groupBox_2);
        coefficient_input->setObjectName(QString::fromUtf8("coefficient_input"));
        coefficient_input->setEnabled(false);
        coefficient_input->setInputMethodHints(Qt::ImhNone);

        verticalLayout_2->addWidget(coefficient_input);

        select_btn = new QPushButton(groupBox_2);
        select_btn->setObjectName(QString::fromUtf8("select_btn"));
        select_btn->setEnabled(false);

        verticalLayout_2->addWidget(select_btn);


        verticalLayout_3->addWidget(groupBox_2);


        gridLayout_2->addLayout(verticalLayout_3, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_5, 0, 2, 1, 1);

        left_Image = new QGroupBox(centralwidget);
        left_Image->setObjectName(QString::fromUtf8("left_Image"));
        left_Image->setMinimumSize(QSize(400, 700));
        left_Image->setMaximumSize(QSize(16777215, 900));

        gridLayout_3->addWidget(left_Image, 0, 0, 3, 1);

        tab_lbl = new QLabel(centralwidget);
        tab_lbl->setObjectName(QString::fromUtf8("tab_lbl"));
        tab_lbl->setMaximumSize(QSize(200, 16777215));
        QFont font1;
        font1.setPointSize(30);
        tab_lbl->setFont(font1);

        gridLayout_3->addWidget(tab_lbl, 1, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1920, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Right_Image->setTitle(QCoreApplication::translate("MainWindow", "Right", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Device Port", nullptr));
        deviceIp->setText(QCoreApplication::translate("MainWindow", "192.168.0.92", nullptr));
        deviceIp->setPlaceholderText(QCoreApplication::translate("MainWindow", "192.168.0.92", nullptr));
        devicePort->setText(QCoreApplication::translate("MainWindow", "2020", nullptr));
        devicePort->setPlaceholderText(QCoreApplication::translate("MainWindow", "2020", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Device IP", nullptr));
        connectBtn->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Device Mode", nullptr));
        dm_Normal->setText(QCoreApplication::translate("MainWindow", "Normal", nullptr));
        dm_Calibration->setText(QCoreApplication::translate("MainWindow", "Calibration", nullptr));
        Calirbate_btn->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Coefficient", nullptr));
        coefficient_8->setText(QCoreApplication::translate("MainWindow", "10 Coefficient", nullptr));
        coefficient_10->setText(QCoreApplication::translate("MainWindow", "12 Coefficient", nullptr));
        coefficient_12->setText(QCoreApplication::translate("MainWindow", "14 Coefficient", nullptr));
        coefficient_Custom->setText(QCoreApplication::translate("MainWindow", "Custom Coefficient", nullptr));
        select_btn->setText(QCoreApplication::translate("MainWindow", "Select", nullptr));
        left_Image->setTitle(QCoreApplication::translate("MainWindow", "Left ", nullptr));
        tab_lbl->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
