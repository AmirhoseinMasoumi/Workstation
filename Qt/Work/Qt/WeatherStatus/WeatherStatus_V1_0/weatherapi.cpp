#include "weatherapi.h"

WeatherAPI::WeatherAPI(QObject *parent)
    : QObject{parent}
{
    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(managerFinished(QNetworkReply*)));

    refreshWeather();

    QObject::connect(&timer, &QTimer::timeout, this, &WeatherAPI::refreshWeather);
    // Set the timer interval to 30 minutes (30 * 60,000 milliseconds)
    timer.setInterval(30 * 60 * 1000);
    timer.start();
}

void WeatherAPI::refreshWeather() {
    // Make API request
    request.setUrl(QUrl("http://api.weatherapi.com/v1/current.json?key=edc7ebfcc67f4533b86102301232809&q=Tehran&aqi=no"));
    manager->get(request);
}

void WeatherAPI::managerFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    // Read the JSON response
    QByteArray response = reply->readAll();
    qDebug() << "Response:" << response;

    // Parse JSON response
    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (doc.isNull()) {
        qDebug() << "Error: Failed to parse JSON response.";
        return;
    }

    QJsonObject weatherObj = doc.object();

    // Parse weather data
    parseWeatherData(weatherObj);
}

void WeatherAPI::parseWeatherData(const QJsonObject &weatherObj) {

    // Parse weather data from JSON
    QJsonObject locationObj = weatherObj["location"].toObject();
    QJsonObject currentObj = weatherObj["current"].toObject();

    m_temperature = currentObj["temp_c"].toDouble();
    m_humidity = currentObj["humidity"].toInt();
    m_feelslike = currentObj["feelslike_c"].toDouble();
    m_city = locationObj["name"].toString();
    m_windSpeed = currentObj["wind_kph"].toDouble();
    QDateTime dateTime = QDateTime::fromString(locationObj["localtime"].toString(), "yyyy-MM-dd hh:mm");
    m_date = dateTime.toString("dd/MM/yyyy");
    m_day = getDayOfWeek(locationObj["localtime"].toString());
    QString iconUrl = currentObj["condition"].toObject()["icon"].toString();
    iconUrl.replace("64x64", "128x128");
    m_weatherIconUrl = "http:" + iconUrl;

    // qDebug values
    qDebug() << "Temperature:" << m_temperature;
    qDebug() << "Humidity:" << m_humidity;
    qDebug() << "Feels like:" << m_feelslike;
    qDebug() << "City:" << m_city;
    qDebug() << "Date:" << m_date;
    qDebug() << "Wind speed:" << m_windSpeed;
    qDebug() << "Day of the week:" << m_day;
    qDebug() << "Icon url:" << m_weatherIconUrl;

    // Notify property changes
    emit weatherDataChanged();
}

QString WeatherAPI::getDayOfWeek(const QString &dateTimeStr) {
    // Create a QDateTime object from the input string
    QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd hh:mm");

    // Get the day of the week as an integer (1 = Monday, 7 = Sunday)
    int dayOfWeek = dateTime.date().dayOfWeek();

    // Convert the integer to the name of the day
    QString dayName;
    switch (dayOfWeek) {
    case Qt::Monday:
        dayName = "Monday";
        break;
    case Qt::Tuesday:
        dayName = "Tuesday";
        break;
    case Qt::Wednesday:
        dayName = "Wednesday";
        break;
    case Qt::Thursday:
        dayName = "Thursday";
        break;
    case Qt::Friday:
        dayName = "Friday";
        break;
    case Qt::Saturday:
        dayName = "Saturday";
        break;
    case Qt::Sunday:
        dayName = "Sunday";
        break;
    default:
        dayName = "Invalid Day";
    }

    return dayName;
}

double WeatherAPI::temperature() const
{
    return m_temperature;
}

int WeatherAPI::humidity() const
{
    return m_humidity;
}

QString WeatherAPI::date() const
{
    return m_date;
}

double WeatherAPI::windSpeed() const
{
    return m_windSpeed;
}

QString WeatherAPI::city() const
{
    return m_city;
}

QString WeatherAPI::day() const
{
    return m_day;
}

QString WeatherAPI::weatherIconUrl() const
{
    return m_weatherIconUrl;
}

double WeatherAPI::feelslike() const
{
    return m_feelslike;
}
