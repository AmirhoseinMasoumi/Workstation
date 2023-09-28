import QtQuick
import QtQuick.Window
import WeatherAPI 1.0

Window {
    width: 500
    maximumWidth: 500
    minimumWidth: 500
    height: 250
    maximumHeight : 250
    minimumHeight: 250
    visible: true
    title: qsTr("Weather forecast")

    WeatherAPI {
        id: weatherAPI

        onWeatherDataChanged: {
            // Update UI elements
            temperatureTxt_id.text = weatherAPI.temperature + "°";
            feelslikeTxt_id.text = "Feels like " + weatherAPI.feelslike + "°";
            dayTxt_id.text = weatherAPI.day;
            dateTxt_id.text = weatherAPI.date;
            cityTxt_id.text = weatherAPI.city;
            wind_speedTxt_id.text = "Wind " + weatherAPI.windSpeed + "km/h";
            humidityTxt_id.text = weatherAPI.humidity + "%";

            status_img_id.source = weatherAPI.weatherIconUrl;
        }
    }

    FontLoader {
        id: customFontLoader
        source: "Resources/Fonts/msgothic.ttc"
    }

    FontLoader {
        id: customFontLoader_bold
        source: "Resources/Fonts/BebasNeueBold.ttf"
    }

    FontLoader {
        id: customFontLoader_book
        source: "Resources/Fonts/BebasNeueBook.ttf"
    }

    Component.onCompleted: {
        customFontLoader.statusChanged.connect(function() {
            if (customFontLoader.status === FontLoader.Ready) {
                temperatureTxt_id.font.family = customFontLoader.name;
                feelslikeTxt_id.font.family = customFontLoader.name;
                dateTxt_id.font.family = customFontLoader.name;
                humidityTxt_id.font.family = customFontLoader.name;
            }
        });

        customFontLoader_bold.statusChanged.connect(function() {
            if (customFontLoader_bold.status === FontLoader.Ready) {
                cityTxt_id.font.family = customFontLoader_bold.name;
            }
        });

        customFontLoader_book.statusChanged.connect(function() {
            if (customFontLoader_book.status === FontLoader.Ready) {
                dayTxt_id.font.family = customFontLoader_book.name;
            }
        });
    }

    Rectangle {
        id: rectangle
        width: parent.width
        height: parent.height

        Rectangle {
            id: rectangle1
            x: 0
            y: 0
            width: 500
            height: 300
            color: "#5f9eaf"
        }
        states: [
            State {
                name: "clicked"
            }
        ]
        Rectangle {
            id: rectangle2
            x: 0
            y: 0
            width: 1018
            height: 652
            color: "#5298ab"
            radius: 326

            Image {
                id: status_img_id
                x: 140
                y: 40
                width: 200
                height: 200
                fillMode: Image.PreserveAspectFit
            }

            Text {
                id: temperatureTxt_id
                x: 350
                y: 70
                width: 158
                height: 151
                color: "#ffffff"
                text: qsTr("00°")
                font.pixelSize: 90
            }

            Text {
                id: cityTxt_id
                x: 16
                y: 9
                color: "#ffffff"
                text: qsTr("CITY")
                font.pixelSize: 50
            }

            Text {
                id: feelslikeTxt_id
                x: 300
                y: 23
                color: "#ffffff"
                text: qsTr("Feels like 00°")
                font.pixelSize: 30
            }

            Text {
                id: dayTxt_id
                x: 16
                y: 70
                color: "#ffffff"
                text: qsTr("Day")
                font.pixelSize: 30
            }

            Text {
                id: dateTxt_id
                x: 16
                y: 107
                width: 82
                height: 33
                color: "#ffffff"
                text: qsTr("00/00/0000")
                font.pixelSize: 20
            }

            Text {
                id: wind_speedTxt_id
                x: 16
                y: 136
                color: "#ffffff"
                text: qsTr("Wind 00km/h")
                font.pixelSize: 15
            }

            BorderImage {
                id: borderImage
                x: 16
                y: 162
                width: 20
                height: 30
                source: "Resources/Icons/drop.png"
            }

            Text {
                id: humidityTxt_id
                x: 60
                y: 168
                color: "#ffffff"
                text: qsTr("00%")
                font.pixelSize: 15
            }
        }
    }
}
