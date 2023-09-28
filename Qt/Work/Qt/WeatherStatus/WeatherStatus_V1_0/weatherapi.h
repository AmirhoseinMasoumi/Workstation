#ifndef WEATHERAPI_H
#define WEATHERAPI_H

#include <QObject>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QString>
#include <QTimer>
#include <QDebug>

class WeatherAPI : public QObject {
    Q_OBJECT

    Q_PROPERTY(double temperature READ temperature NOTIFY weatherDataChanged)
    Q_PROPERTY(int humidity READ humidity NOTIFY weatherDataChanged)
    Q_PROPERTY(double feelslike READ feelslike NOTIFY weatherDataChanged)
    Q_PROPERTY(QString city READ city NOTIFY weatherDataChanged)
    Q_PROPERTY(QString date READ date NOTIFY weatherDataChanged)
    Q_PROPERTY(QString day READ day NOTIFY weatherDataChanged)
    Q_PROPERTY(double windSpeed READ windSpeed NOTIFY weatherDataChanged)
    Q_PROPERTY(QString weatherIconUrl READ weatherIconUrl NOTIFY weatherDataChanged)

public:
    explicit WeatherAPI(QObject *parent = nullptr);

    double temperature() const;
    int humidity() const;
    QString date() const;
    double windSpeed() const;
    QString city() const;
    QString day() const;
    QString weatherIconUrl() const;
    double feelslike() const;

signals:
    void weatherDataChanged();
    void weatherDayChanged();

public slots:
    void refreshWeather();
    void managerFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;

    QTimer timer;

    QString getDayOfWeek(const QString &dateTimeStr);

    void setWeatherData();
    void parseWeatherData(const QJsonObject& weatherObj);
    double m_temperature;
    int m_humidity;
    QString m_date;
    double m_windSpeed;
    QString m_city;
    QString m_day;
    QString m_weatherIconUrl;
    double m_feelslike;
};

#endif // WEATHERAPI_H
