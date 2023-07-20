/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../HumidityMeter/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[307];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 29), // "on_coefficient_Custom_toggled"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 7), // "checked"
QT_MOC_LITERAL(4, 50, 25), // "on_dm_Calibration_toggled"
QT_MOC_LITERAL(5, 76, 24), // "on_coefficient_8_clicked"
QT_MOC_LITERAL(6, 101, 25), // "on_coefficient_10_clicked"
QT_MOC_LITERAL(7, 127, 25), // "on_coefficient_12_clicked"
QT_MOC_LITERAL(8, 153, 21), // "on_select_btn_clicked"
QT_MOC_LITERAL(9, 175, 24), // "on_Calirbate_btn_clicked"
QT_MOC_LITERAL(10, 200, 21), // "on_connectBtn_clicked"
QT_MOC_LITERAL(11, 222, 10), // "readValues"
QT_MOC_LITERAL(12, 233, 10), // "QList<int>"
QT_MOC_LITERAL(13, 244, 4), // "vals"
QT_MOC_LITERAL(14, 249, 23), // "processPendingDatagrams"
QT_MOC_LITERAL(15, 273, 12), // "checkCalFile"
QT_MOC_LITERAL(16, 286, 20) // "on_dm_Normal_toggled"

    },
    "MainWindow\0on_coefficient_Custom_toggled\0"
    "\0checked\0on_dm_Calibration_toggled\0"
    "on_coefficient_8_clicked\0"
    "on_coefficient_10_clicked\0"
    "on_coefficient_12_clicked\0"
    "on_select_btn_clicked\0on_Calirbate_btn_clicked\0"
    "on_connectBtn_clicked\0readValues\0"
    "QList<int>\0vals\0processPendingDatagrams\0"
    "checkCalFile\0on_dm_Normal_toggled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x08 /* Private */,
       4,    1,   77,    2, 0x08 /* Private */,
       5,    0,   80,    2, 0x08 /* Private */,
       6,    0,   81,    2, 0x08 /* Private */,
       7,    0,   82,    2, 0x08 /* Private */,
       8,    0,   83,    2, 0x08 /* Private */,
       9,    0,   84,    2, 0x08 /* Private */,
      10,    0,   85,    2, 0x08 /* Private */,
      11,    1,   86,    2, 0x08 /* Private */,
      14,    0,   89,    2, 0x08 /* Private */,
      15,    0,   90,    2, 0x08 /* Private */,
      16,    1,   91,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_coefficient_Custom_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_dm_Calibration_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_coefficient_8_clicked(); break;
        case 3: _t->on_coefficient_10_clicked(); break;
        case 4: _t->on_coefficient_12_clicked(); break;
        case 5: _t->on_select_btn_clicked(); break;
        case 6: _t->on_Calirbate_btn_clicked(); break;
        case 7: _t->on_connectBtn_clicked(); break;
        case 8: _t->readValues((*reinterpret_cast< QList<int>(*)>(_a[1]))); break;
        case 9: _t->processPendingDatagrams(); break;
        case 10: { bool _r = _t->checkCalFile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->on_dm_Normal_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
