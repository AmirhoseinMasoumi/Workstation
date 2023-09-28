/****************************************************************************
** Meta object code from reading C++ file 'weatherapi.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../WeatherStatus_V1_0/weatherapi.h"
#include <QtNetwork/QSslPreSharedKeyAuthenticator>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'weatherapi.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSWeatherAPIENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSWeatherAPIENDCLASS = QtMocHelpers::stringData(
    "WeatherAPI",
    "weatherDataChanged",
    "",
    "weatherDayChanged",
    "refreshWeather",
    "managerFinished",
    "QNetworkReply*",
    "reply",
    "temperature",
    "humidity",
    "feelslike",
    "city",
    "date",
    "day",
    "windSpeed",
    "weatherIconUrl"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSWeatherAPIENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[11];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[15];
    char stringdata5[16];
    char stringdata6[15];
    char stringdata7[6];
    char stringdata8[12];
    char stringdata9[9];
    char stringdata10[10];
    char stringdata11[5];
    char stringdata12[5];
    char stringdata13[4];
    char stringdata14[10];
    char stringdata15[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSWeatherAPIENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSWeatherAPIENDCLASS_t qt_meta_stringdata_CLASSWeatherAPIENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "WeatherAPI"
        QT_MOC_LITERAL(11, 18),  // "weatherDataChanged"
        QT_MOC_LITERAL(30, 0),  // ""
        QT_MOC_LITERAL(31, 17),  // "weatherDayChanged"
        QT_MOC_LITERAL(49, 14),  // "refreshWeather"
        QT_MOC_LITERAL(64, 15),  // "managerFinished"
        QT_MOC_LITERAL(80, 14),  // "QNetworkReply*"
        QT_MOC_LITERAL(95, 5),  // "reply"
        QT_MOC_LITERAL(101, 11),  // "temperature"
        QT_MOC_LITERAL(113, 8),  // "humidity"
        QT_MOC_LITERAL(122, 9),  // "feelslike"
        QT_MOC_LITERAL(132, 4),  // "city"
        QT_MOC_LITERAL(137, 4),  // "date"
        QT_MOC_LITERAL(142, 3),  // "day"
        QT_MOC_LITERAL(146, 9),  // "windSpeed"
        QT_MOC_LITERAL(156, 14)   // "weatherIconUrl"
    },
    "WeatherAPI",
    "weatherDataChanged",
    "",
    "weatherDayChanged",
    "refreshWeather",
    "managerFinished",
    "QNetworkReply*",
    "reply",
    "temperature",
    "humidity",
    "feelslike",
    "city",
    "date",
    "day",
    "windSpeed",
    "weatherIconUrl"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWeatherAPIENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       8,   44, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x06,    9 /* Public */,
       3,    0,   39,    2, 0x06,   10 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    0,   40,    2, 0x0a,   11 /* Public */,
       5,    1,   41,    2, 0x0a,   12 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,

 // properties: name, type, flags
       8, QMetaType::Double, 0x00015001, uint(0), 0,
       9, QMetaType::Int, 0x00015001, uint(0), 0,
      10, QMetaType::Double, 0x00015001, uint(0), 0,
      11, QMetaType::QString, 0x00015001, uint(0), 0,
      12, QMetaType::QString, 0x00015001, uint(0), 0,
      13, QMetaType::QString, 0x00015001, uint(0), 0,
      14, QMetaType::Double, 0x00015001, uint(0), 0,
      15, QMetaType::QString, 0x00015001, uint(0), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject WeatherAPI::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSWeatherAPIENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSWeatherAPIENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSWeatherAPIENDCLASS_t,
        // property 'temperature'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'humidity'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'feelslike'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'city'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'date'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'day'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'windSpeed'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'weatherIconUrl'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<WeatherAPI, std::true_type>,
        // method 'weatherDataChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'weatherDayChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'refreshWeather'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'managerFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>
    >,
    nullptr
} };

void WeatherAPI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WeatherAPI *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->weatherDataChanged(); break;
        case 1: _t->weatherDayChanged(); break;
        case 2: _t->refreshWeather(); break;
        case 3: _t->managerFinished((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WeatherAPI::*)();
            if (_t _q_method = &WeatherAPI::weatherDataChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WeatherAPI::*)();
            if (_t _q_method = &WeatherAPI::weatherDayChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<WeatherAPI *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->temperature(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->humidity(); break;
        case 2: *reinterpret_cast< double*>(_v) = _t->feelslike(); break;
        case 3: *reinterpret_cast< QString*>(_v) = _t->city(); break;
        case 4: *reinterpret_cast< QString*>(_v) = _t->date(); break;
        case 5: *reinterpret_cast< QString*>(_v) = _t->day(); break;
        case 6: *reinterpret_cast< double*>(_v) = _t->windSpeed(); break;
        case 7: *reinterpret_cast< QString*>(_v) = _t->weatherIconUrl(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
}

const QMetaObject *WeatherAPI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WeatherAPI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSWeatherAPIENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WeatherAPI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void WeatherAPI::weatherDataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void WeatherAPI::weatherDayChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
