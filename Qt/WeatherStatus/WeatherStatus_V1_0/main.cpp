#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "weatherapi.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    WeatherAPI mWeatherAPI;

    QQmlApplicationEngine engine;

    // Register the WeatherAPI class to be available in QML
    qmlRegisterType<WeatherAPI>("WeatherAPI", 1, 0, "WeatherAPI");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("WeatherStatus_V1_0", "Main");

    return app.exec();
}
