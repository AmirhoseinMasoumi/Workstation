/********************************************************************************
** Form generated from reading UI file 'calibration.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBRATION_H
#define UI_CALIBRATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Calibration
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QLabel *message_txt;
    QSpacerItem *verticalSpacer;
    QPushButton *done_btn;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancel_btn;
    QPushButton *ok_btn;

    void setupUi(QDialog *Calibration)
    {
        if (Calibration->objectName().isEmpty())
            Calibration->setObjectName(QString::fromUtf8("Calibration"));
        Calibration->resize(400, 203);
        verticalLayout = new QVBoxLayout(Calibration);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 29, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        message_txt = new QLabel(Calibration);
        message_txt->setObjectName(QString::fromUtf8("message_txt"));
        QFont font;
        font.setFamily(QString::fromUtf8("B Tir"));
        font.setPointSize(10);
        message_txt->setFont(font);
        message_txt->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(message_txt);

        verticalSpacer = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        done_btn = new QPushButton(Calibration);
        done_btn->setObjectName(QString::fromUtf8("done_btn"));

        verticalLayout->addWidget(done_btn);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancel_btn = new QPushButton(Calibration);
        cancel_btn->setObjectName(QString::fromUtf8("cancel_btn"));

        horizontalLayout->addWidget(cancel_btn);

        ok_btn = new QPushButton(Calibration);
        ok_btn->setObjectName(QString::fromUtf8("ok_btn"));
        ok_btn->setEnabled(false);

        horizontalLayout->addWidget(ok_btn);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(Calibration);

        QMetaObject::connectSlotsByName(Calibration);
    } // setupUi

    void retranslateUi(QDialog *Calibration)
    {
        Calibration->setWindowTitle(QCoreApplication::translate("Calibration", "Dialog", nullptr));
        message_txt->setText(QCoreApplication::translate("Calibration", "Calibration", nullptr));
        done_btn->setText(QCoreApplication::translate("Calibration", "Done", nullptr));
        cancel_btn->setText(QCoreApplication::translate("Calibration", "Cancel", nullptr));
        ok_btn->setText(QCoreApplication::translate("Calibration", "Ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Calibration: public Ui_Calibration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBRATION_H
