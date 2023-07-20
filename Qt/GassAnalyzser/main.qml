import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtCharts 2.13
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.3

Window {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Sensor Reader")

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    property bool connection: false
    //visibility: Window.FullScreen

    Connections {
        target: Modbus

        function onSystemStatusChanged(){
            if(Modbus.systemStatus === "Disconnected"){
                text8.text= "0°C";
                circularProgressBar3.value = 1;
                text12.text = qsTr("Status: OFFLINE");
                text7.text= "0ppm";
                circularProgressBar2.value = 1;
                text11.text = qsTr("Status: OFFLINE");
                text6.text= "0ppm";
                circularProgressBar1.value = 1;
                text10.text = qsTr("Status: OFFLINE");
                text5.text= "0%";
                circularProgressBar.value = 1;
                text9.text = qsTr("Status: OFFLINE");

                o2_at_txf.clear();
                text22.text = qsTr("O2 avg: NAN");
                co2_at_txf.clear();
                text23.text = qsTr("CO2 avg: NAN");
                co_at_txf.clear();
                text24.text = qsTr("CO avg: NAN");
                temp_at_txf.clear();
                text25.text = qsTr("TEMP avg: NAN");
            }
        }
    }

    Rectangle {
        id: background
        color: "#ffffff"
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#22223c"
            }

            GradientStop {
                position: 1
                color: "#272846"
            }
            orientation: Gradient.Vertical
        }

        Rectangle {
            id: chartContainer
            width: background.width / 1.44
            height: background.height / 1.7
            color: "#30304c"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 50
            anchors.topMargin: 50

            ChartView {
                id: line
                opacity: 1
                visible: true
                anchors.fill: parent
                clip: false
                theme: ChartView.ChartThemeBlueCerulean

                Connections {
                    id: logDataCon_id
                    target: LogData

                    property bool frTime: true
                    property double maxValue: 0

                    function onDataCounterChanged(){
                        if(!connection){
                            if(frTime === true){
                                time1.min = Date.fromLocaleString(Qt.locale(), LogData.currentTime, "hh:mm:ss");
                                print(time1.min)
                                frTime = false;
                            }
                            time1.max = Date.fromLocaleString(Qt.locale(), LogData.currentTime, "hh:mm:ss");
                            o2_id.append(Date.fromLocaleString(Qt.locale(), LogData.currentTime, "hh:mm:ss"), parseFloat(LogData.O2value))
                            co2_id.append(Date.fromLocaleString(Qt.locale(), LogData.currentTime, "hh:mm:ss"), parseFloat(LogData.CO2value))
                            co_id.append(Date.fromLocaleString(Qt.locale(), LogData.currentTime, "hh:mm:ss"), parseFloat(LogData.COvalue))
                            tmp_id.append(Date.fromLocaleString(Qt.locale(), LogData.currentTime, "hh:mm:ss"), parseFloat(LogData.TEMPvalue))

                            if(checkBox2.checked && checkBox1.checked){
                                if(parseFloat(LogData.COvalue) > parseFloat(LogData.CO2value)){
                                    if(parseFloat(LogData.COvalue) > logDataCon_id.maxValue){
                                        logDataCon_id.maxValue = parseFloat(LogData.COvalue);
                                        percentage.max = logDataCon_id.maxValue + 100;
                                    }
                                }
                                else{
                                    if(parseFloat(LogData.CO2value) > logDataCon_id.maxValue){
                                        logDataCon_id.maxValue = parseFloat(LogData.CO2value);
                                        percentage.max = logDataCon_id.maxValue + 100;
                                    }
                                }
                            }
                            else if(checkBox1.checked)
                                percentage.max = logDataCon_id.maxValue + 100;
                            else if(checkBox2.checked)
                                percentage.max = logDataCon_id.maxValue + 100;
                            else{
                                percentage.max = 100;
                            }
                        }
                    }
                }

                Connections {
                    id: logModbus_id
                    target: Modbus

                    property double maxValue: 0

                    function onCounterChanged(){
                        if(connection && switch1.checked === true){
                            time1.max = Date.fromLocaleString(Qt.locale(), Modbus.currentTime, "hh:mm:ss");
                            o2_id.append(Date.fromLocaleString(Qt.locale(), Modbus.currentTime, "hh:mm:ss"), parseFloat(Modbus.O2_tcp_val))
                            co2_id.append(Date.fromLocaleString(Qt.locale(), Modbus.currentTime, "hh:mm:ss"), parseFloat(Modbus.CO2_tcp_val))
                            co_id.append(Date.fromLocaleString(Qt.locale(), Modbus.currentTime, "hh:mm:ss"), parseFloat(Modbus.CO_tcp_val))
                            tmp_id.append(Date.fromLocaleString(Qt.locale(), Modbus.currentTime, "hh:mm:ss"), parseFloat(Modbus.Temp_tcp_val))

                            if(checkBox2.checked && checkBox1.checked){
                                if(parseFloat(Modbus.CO_tcp_val) > parseFloat(Modbus.CO2_tcp_val)){
                                    if(parseFloat(Modbus.CO_tcp_val) > logModbus_id.maxValue){
                                        logModbus_id.maxValue = parseFloat(Modbus.CO_tcp_val);
                                        percentage.max = logModbus_id.maxValue + 100;
                                    }
                                }
                                else{
                                    if(parseFloat(Modbus.CO2_tcp_val) > logModbus_id.maxValue){
                                        logModbus_id.maxValue = parseFloat(Modbus.CO2_tcp_val);
                                        percentage.max = logModbus_id.maxValue + 100;
                                    }
                                }
                            }
                            else if(checkBox1.checked)
                                percentage.max = logDataCon_id.maxValue + 100;
                            else if(checkBox2.checked)
                                percentage.max = logDataCon_id.maxValue + 100;
                            else{
                                percentage.max = 100;
                            }
                        }
                    }
                }

                Connections {
                    id: logSerial_id
                    target: SerialPort

                    property double maxValue: 0

                    function onCounterChanged(){
                        if(connection && switch1.checked === false){
                            time1.max = Date.fromLocaleString(Qt.locale(), SerialPort.currentTime, "hh:mm:ss");
                            o2_id.append(Date.fromLocaleString(Qt.locale(), SerialPort.currentTime, "hh:mm:ss"), parseFloat(SerialPort.o2Val))
                            co2_id.append(Date.fromLocaleString(Qt.locale(), SerialPort.currentTime, "hh:mm:ss"), parseFloat(SerialPort.co2Val))
                            co_id.append(Date.fromLocaleString(Qt.locale(), SerialPort.currentTime, "hh:mm:ss"), parseFloat(SerialPort.coVal))
                            tmp_id.append(Date.fromLocaleString(Qt.locale(), SerialPort.currentTime, "hh:mm:ss"), parseFloat(SerialPort.tempVal))

                            if(checkBox2.checked && checkBox1.checked){
                                if(parseFloat(SerialPort.coVal) > parseFloat(SerialPort.co2Val)){
                                    if(parseFloat(SerialPort.coVal) > logSerial_id.maxValue){
                                        logSerial_id.maxValue = parseFloat(SerialPort.coVal);
                                        percentage.max = logSerial_id.maxValue + 100;
                                    }
                                }
                                else{
                                    if(parseFloat(SerialPort.co2Val) > logSerial_id.maxValue){
                                        logSerial_id.maxValue = parseFloat(SerialPort.co2Val);
                                        percentage.max = logSerial_id.maxValue + 100;
                                    }
                                }
                            }
                            else if(checkBox1.checked)
                                percentage.max = parseFloat(SerialPort.CO2_tcp_val) + 100;
                            else if(checkBox2.checked)
                                percentage.max = parseFloat(SerialPort.CO_tcp_val) + 100;
                            else{
                                percentage.max = 100;
                            }
                        }
                    }
                }

                ValueAxis {
                    id: percentage
                    min: 0
                    //max: 100
                    tickCount: 5
                }

                DateTimeAxis{
                    id: time1
                    format: "hh:mm:ss"
                    min : Date.fromLocaleString(Qt.locale(), Modbus.currentTime, "hh:mm:ss")
                    tickCount: 1
                }

                SplineSeries {
                    id: o2_id
                    name: "O2"
                    color: "#28b3ff"
                    width: 3
                    axisX: time1
                    axisY: percentage
                }

                SplineSeries {
                    id: co2_id
                    name: "CO2"
                    color: "#5dfc64"
                    width: 3
                    axisX: time1
                    axisY: percentage
                }

                SplineSeries {
                    id: co_id
                    name: "CO"
                    color: "#ff5858"
                    width: 3
                    axisX: time1
                    axisY: percentage
                }

                SplineSeries {
                    id: tmp_id
                    name: "Temperature"
                    color: "#cb49ff"
                    width: 3
                    axisX: time1
                    axisY: percentage
                }

                Rectangle {
                    id: rectangle
                    x: 26
                    y: 18
                    width: chartContainer.width / 3.5
                    height: 51
                    color: "#187ac6"
                    radius: 5
                    border.color: "#00000000"

                    CheckBox {
                        id: checkBox
                        y: 5
                        width: 86
                        height: 40
                        text: qsTr("O2")
                        anchors.left: parent.left
                        font.family: "Arial"
                        anchors.leftMargin: 7
                        checked: true;

                        onCheckedChanged: {
                            if(checked){
                                o2_id.visible = true;
                            }
                            else{
                                o2_id.visible = false;
                            }
                        }
                    }

                    CheckBox {
                        id: checkBox1
                        x: 86
                        y: 5
                        width: 86
                        height: 40
                        text: qsTr("CO2")
                        anchors.left: checkBox.right
                        font.family: "Arial"
                        anchors.leftMargin: -12
                        checked: true;

                        onCheckedChanged: {
                            if(checked){
                                co2_id.visible = true;
                            }
                            else{
                                co2_id.visible = false;
                            }
                        }
                    }

                    CheckBox {
                        id: checkBox2
                        x: 178
                        y: 5
                        width: 86
                        height: 40
                        text: qsTr("CO")
                        anchors.left: checkBox1.right
                        font.family: "Arial"
                        anchors.leftMargin: -1
                        checked: true;

                        onCheckedChanged: {
                            if(checked){
                                co_id.visible = true;
                            }
                            else{
                                co_id.visible = false;
                            }
                        }
                    }

                    CheckBox {
                        id: checkBox3
                        x: 255
                        y: 5
                        width: 126
                        height: 40
                        text: qsTr("Temperature")
                        anchors.left: checkBox2.right
                        font.family: "Arial"
                        anchors.leftMargin: -5
                        checked: true;

                        onCheckedChanged: {
                            if(checked){
                                tmp_id.visible = true;
                            }
                            else{
                                tmp_id.visible = false;
                            }
                        }
                    }
                }
            }
        }

        Rectangle{
            id: container1
            y: 691
            width: background.width / 6
            height: background.height / 3.5
            color: "#30304c"
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 50
            anchors.bottomMargin: 50

            CircularProgressBar{
                id: circularProgressBar
                y: -11
                height: 251
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: 30
                anchors.leftMargin: 30
                anchors.bottomMargin: 15
                secondaryColor: "#28b3ff"
                primaryColor: "#525264"
                value: 1
                size: 200
                lineWidth: 15

                Text {
                    id: text5
                    text: qsTr("0%")
                    color: "white"
                    anchors.fill: parent
                    font.pixelSize: 30
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.rightMargin: 50
                    anchors.leftMargin: 50
                    anchors.bottomMargin: 50
                    anchors.topMargin: 50

                    Connections {
                        target: LogData

                        function onDataCounterChanged(){
                            if(!connection){
                                text5.text= LogData.O2value + "%";
                                circularProgressBar.value = 1 - (parseFloat(LogData.O2value)*0.01);
                                text9.text = qsTr("Status: OFFLINE");
                            }
                        }
                    }
                    Connections {
                        target: Modbus

                        function onCounterChanged(){
                            if(connection && switch1.checked === true){
                                text5.text= Modbus.O2_tcp_val + "%";
                                circularProgressBar.value = 1 - (parseFloat(Modbus.O2_tcp_val)*0.01);
                                text9.text = qsTr("Status: ONLINE");
                            }
                        }
                    }
                    Connections {
                        target: SerialPort

                        function onCounterChanged(){
                            if(connection && switch1.checked === false){
                                text5.text= SerialPort.o2Val + "%";
                                circularProgressBar.value = 1 - (parseFloat(SerialPort.o2Val)*0.01);
                                text9.text = qsTr("Status: ONLINE");
                            }
                        }
                    }
                }
            }

            Text {
                id: text1
                width: 120
                height: 49
                color: "#c9c9c9"
                text: qsTr("O2")
                anchors.left: parent.left
                anchors.top: parent.top
                font.pixelSize: 40
                font.family: "Arial"
                anchors.topMargin: 5
                anchors.leftMargin: 10
            }

            Text {
                id: text9
                y: 275
                width: 120
                height: 49
                color: "#c9c9c9"
                text: qsTr("Status: OFFLINE")
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                font.pixelSize: 20
                anchors.bottomMargin: -16
                anchors.leftMargin: 10
            }
        }

        Rectangle {
            id: container2
            y: 691
            width: background.width / 6
            height: background.height / 3.5
            color: "#30304c"
            anchors.left: container1.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            anchors.leftMargin: 20

            CircularProgressBar{
                id: circularProgressBar1
                y: -11
                height: 251
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: 30
                anchors.leftMargin: 30
                anchors.bottomMargin: 15
                primaryColor: "#525264"
                secondaryColor: "#5dfc64"
                size: 200
                value: 1
                lineWidth: 15

                Text {
                    id: text6
                    text: qsTr("0ppm")
                    anchors.fill: parent
                    font.pixelSize: 30
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.topMargin: 50
                    anchors.leftMargin: 50
                    anchors.bottomMargin: 50
                    anchors.rightMargin: 50

                    Connections {
                        target: LogData

                        function onDataCounterChanged(){
                            if(!connection){
                                text6.text= LogData.CO2value + "ppm";
                                circularProgressBar1.value = 1 - (parseFloat(LogData.CO2value)*0.0001);
                                text10.text = qsTr("Status: OFFLINE");
                            }
                        }
                    }
                    Connections {
                        target: Modbus

                        function onCounterChanged(){
                            if(connection && switch1.checked === true){
                                text6.text= Modbus.CO2_tcp_val + "ppm";
                                circularProgressBar1.value = 1 - (parseFloat(Modbus.CO2_tcp_val)*0.0001);
                                text10.text = qsTr("Status: ONLINE");
                            }
                        }
                    }
                    Connections {
                        target: SerialPort

                        function onCounterChanged(){
                            if(connection && switch1.checked === false){
                                text6.text= SerialPort.co2Val + "ppm";
                                circularProgressBar1.value = 1 - (parseFloat(SerialPort.co2Val)*0.0001);
                                text10.text = qsTr("Status: ONLINE");
                            }
                        }
                    }

                }
            }

            Text {
                id: text2
                width: 120
                height: 49
                color: "#c9c9c9"
                text: qsTr("CO2")
                anchors.left: parent.left
                anchors.top: parent.top
                font.pixelSize: 40
                font.family: "Arial"
                anchors.topMargin: 5
                anchors.leftMargin: 10
            }

            Text {
                id: text10
                y: 275
                width: 120
                height: 49
                color: "#c9c9c9"
                text: qsTr("Status: OFFLINE")
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                font.pixelSize: 20
                anchors.leftMargin: 10
                anchors.bottomMargin: -16
            }
        }

        Rectangle {
            id: container3
            y: 691
            width: background.width / 6
            height: background.height / 3.5
            color: "#30304c"
            anchors.left: container2.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            anchors.leftMargin: 20

            CircularProgressBar{
                id: circularProgressBar2
                y: -11
                height: 251
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: 30
                anchors.leftMargin: 30
                anchors.bottomMargin: 15
                primaryColor: "#525264"
                secondaryColor: "#ff5858"
                size: 200
                value: 1
                lineWidth: 15

                Text {
                    id: text7
                    text: qsTr("0ppm")
                    anchors.fill: parent
                    font.pixelSize: 30
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.topMargin: 50
                    anchors.leftMargin: 50
                    anchors.bottomMargin: 50
                    anchors.rightMargin: 50

                    Connections {
                        target: LogData

                        function onDataCounterChanged(){
                            if(!connection){
                                text7.text= LogData.COvalue + "ppm";
                                circularProgressBar2.value = 1 - (parseFloat(LogData.COvalue)*0.001);
                                text11.text = qsTr("Status: OFFLINE");
                            }
                        }
                    }
                    Connections {
                        target: Modbus

                        function onCounterChanged(){
                            if(connection && switch1.checked === true){
                                text7.text= Modbus.CO_tcp_val + "ppm";
                                circularProgressBar2.value = 1 - (parseFloat(Modbus.CO_tcp_val)*0.001);
                                text11.text = qsTr("Status: ONLINE");
                            }
                        }
                    }
                    Connections {
                        target: SerialPort

                        function onCounterChanged(){
                            if(connection && switch1.checked === false){
                                text7.text= SerialPort.coVal + "ppm";
                                circularProgressBar2.value = 1 - (parseFloat(SerialPort.coVal)*0.001);
                                text11.text = qsTr("Status: ONLINE");
                            }
                        }
                    }
                }
            }

            Text {
                id: text3
                width: 120
                height: 49
                color: "#c9c9c9"
                text: qsTr("CO")
                anchors.left: parent.left
                anchors.top: parent.top
                font.pixelSize: 40
                font.family: "Arial"
                anchors.topMargin: 5
                anchors.leftMargin: 10
            }

            Text {
                id: text11
                y: 275
                width: 120
                height: 49
                color: "#c9c9c9"
                text: qsTr("Status: OFFLINE")
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                font.pixelSize: 20
                anchors.leftMargin: 10
                anchors.bottomMargin: -16
            }
        }

        Rectangle {
            id: container4
            y: 691
            width: background.width / 6
            height: background.height / 3.5
            color: "#30304c"
            anchors.left: container3.right
            anchors.bottom: parent.bottom
            anchors.leftMargin: 20
            anchors.bottomMargin: 50

            CircularProgressBar{
                id: circularProgressBar3
                y: -11
                height: 251
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: 30
                anchors.leftMargin: 30
                anchors.bottomMargin: 15
                primaryColor: "#525264"
                secondaryColor: "#cb49ff"
                size: 200
                value: 1
                lineWidth: 15

                Text {
                    id: text8
                    text: qsTr("0°C")
                    anchors.fill: parent
                    font.pixelSize: 30
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    anchors.topMargin: 50
                    anchors.leftMargin: 50
                    anchors.bottomMargin: 50
                    anchors.rightMargin: 50

                    Connections {
                        target: LogData

                        function onDataCounterChanged(){
                            if(!connection){
                                text8.text= LogData.TEMPvalue + "°C";
                                circularProgressBar3.value = 1 - (parseFloat(LogData.TEMPvalue)*0.01);
                                text12.text = qsTr("Status: OFFLINE");
                            }
                        }
                    }
                    Connections {
                        target: Modbus

                        function onCounterChanged(){
                            if(connection && switch1.checked === true){
                                text8.text= Modbus.Temp_tcp_val + "°C";
                                circularProgressBar3.value = 1 - (parseFloat(Modbus.Temp_tcp_val)*0.01);
                                text12.text = qsTr("Status: ONLINE");
                            }
                        }
                    }
                    Connections {
                        target: SerialPort

                        function onCounterChanged(){
                            if(connection && switch1.checked === false){
                                text8.text= SerialPort.tempVal + "°C";
                                circularProgressBar3.value = 1 - (parseFloat(SerialPort.tempVal)*0.01);
                                text12.text = qsTr("Status: ONLINE");
                            }
                        }
                    }
                }
            }

            Text {
                id: text4
                width: 224
                height: 49
                color: "#c9c9c9"
                text: qsTr("Temperature")
                anchors.left: parent.left
                anchors.top: parent.top
                font.pixelSize: 40
                font.family: "Arial"
                anchors.topMargin: 5
                anchors.leftMargin: 10
            }

            Text {
                id: text12
                y: 275
                width: 120
                height: 49
                color: "#c9c9c9"
                text: qsTr("Status: OFFLINE")
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                font.pixelSize: 20
                anchors.leftMargin: 10
                anchors.bottomMargin: -16
            }
        }

        GroupBox {
            id: groupBox
            x: 1399
            width: 498
            height: 413
            anchors.left: chartContainer.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.rightMargin: 23
            anchors.leftMargin: 16
            font.pointSize: 20
            font.family: "Arial"
            title: qsTr("SETTING")

            Text {
                id: text13
                x: 23
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("System status:")
                anchors.top: parent.top
                font.pixelSize: 20
                anchors.topMargin: 45
                font.family: "Arial"
            }

            Switch {
                id: switch1
                x: 367
                text: switch1.checked ? "TCP" : "USART"
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.rightMargin: 0
                checked: true
                font.pointSize: 7
                onCheckedChanged: {
                    text: checked ? "TCP" : "USART"
                    SerialPort.changeConnectionStatus(!checked);
                }
            }

            Text {
                id: systemStatus_txt
                y: 45
                width: 148
                height: 24
                color: "#ffffff"
                text: Modbus.systemStatus
                anchors.left: text13.right
                font.pixelSize: 20
                horizontalAlignment: Text.AlignHCenter
                anchors.leftMargin: 95
                font.family: "Arial"

                Connections {
                    target: Modbus

                    function onSystemStatusChanged(){
                        systemStatus_txt.text =  Modbus.systemStatus;
                    }
                }
            }

            Text {
                id: tcpIp_txt
                x: 23
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("TCP IP:")
                anchors.top: text13.bottom
                font.pixelSize: 20
                anchors.topMargin: 16
                font.family: "Arial"
            }

            Text {
                id: tcpPort_txt
                x: 23
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("TCP Port:")
                anchors.top: tcpIp_txt.bottom
                font.pixelSize: 20
                anchors.topMargin: 15
                font.family: "Arial"
            }

            TextField {
                id: tcpIP_txf
                y: 74
                width: 160
                height: 47
                text: ""
                anchors.left: tcpIp_txt.right
                horizontalAlignment: Text.AlignHCenter
                anchors.leftMargin: 89
                font.pointSize: 12
                placeholderText: qsTr("192.168.1.92")
            }

            TextField {
                id: tcpPort_txf
                y: 113
                width: 119
                height: 47
                text: ""
                anchors.left: tcpPort_txt.right
                horizontalAlignment: Text.AlignHCenter
                anchors.leftMargin: 110
                font.pointSize: 12
                placeholderText: qsTr("502")
            }

            Text {
                id: text16
                x: 23
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("Load Data PATH:")
                anchors.top: tcpPort_txt.bottom
                font.pixelSize: 20
                anchors.topMargin: 20
                font.family: "Arial"
            }

            ComboBox{
                id: databaseList_id
                y: 158
                width: 200
                height: 50
                anchors.left: text16.right
                anchors.leftMargin: 70
                font.pointSize: 12
                model: LogData.dbTableList
            }

            Button {
                id: selectSavePath_btn
                y: 203
                width: parent.width - 50
                height: 50
                anchors.top: databaseList_id.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 10
                text: qsTr("Load Data")
                font.pointSize: 9
                icon.width: 24

                onClicked: {
                    o2_id.clear();
                    co2_id.clear();
                    co_id.clear();
                    tmp_id.clear();
                    LogData.loadSqlite(databaseList_id.currentText)
                }
            }

            Button {
                id: button
                y: 335
                height: 54
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: 131
                anchors.leftMargin: 131
                anchors.bottomMargin: 10
                font.pointSize: 15
                text: connection ? qsTr("DISCONNECT"):qsTr("CONNECT")

                onClicked: {
                    Modbus.resetCounter();
                    LogData.readyRead(false);
                    o2_id.clear();
                    co2_id.clear();
                    co_id.clear();
                    tmp_id.clear();
                    if(connection === false){
                        time1.min = Date.fromLocaleString(Qt.locale(), Modbus.currentTime, "hh:mm:ss")
                        if(switch1.checked){

                            if(tcpIP_txf.text !== "" && tcpPort_txf.text !== ""){
                                Modbus.getTCPip(tcpIP_txf.text);
                                Modbus.getTCPport(tcpPort_txf.text);
                                Modbus.getConnectionStatus(true);
                                Modbus.createNewDb();
                                LogData.createSql();
                                connection = true;
                            }
                        }
                        else{
                            SerialPort.connectionReady(true);
                            connection = true;
                        }
                    }else{
                        if(switch1.checked){
                            //TcpSocket.doDisconnect();
                            Modbus.getConnectionStatus(false);
                        }
                        else{
                            SerialPort.connectionReady(false);
                            SerialPort.disconnect();
                        }
                        connection = false;

                        text8.text= "0°C";
                        circularProgressBar3.value = 1;
                        text12.text = qsTr("Status: OFFLINE");
                        text7.text= "0ppm";
                        circularProgressBar2.value = 1;
                        text11.text = qsTr("Status: OFFLINE");
                        text6.text= "0ppm";
                        circularProgressBar1.value = 1;
                        text10.text = qsTr("Status: OFFLINE");
                        text5.text= "0%";
                        circularProgressBar.value = 1;
                        text9.text = qsTr("Status: OFFLINE");

                        o2_at_txf.clear();
                        text22.text = qsTr("O2 avg: NAN");
                        co2_at_txf.clear();
                        text23.text = qsTr("CO2 avg: NAN");
                        co_at_txf.clear();
                        text24.text = qsTr("CO avg: NAN");
                        temp_at_txf.clear();
                        text25.text = qsTr("TEMP avg: NAN");
                    }
                }
            }
        }

        GroupBox {
            id: groupBox1
            x: 1399
            y: 486
            width: 498
            height: 470
            anchors.bottom: background.bottom
            anchors.bottomMargin: 50
            font.pointSize: 20
            font.family: "Arial"
            title: qsTr("CALCULATION PARAM")

            Text {
                id: text18
                x: 23
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("O2 avg Time:")
                anchors.top: parent.top
                font.pixelSize: 20
                anchors.topMargin: 20
                font.family: "Arial"
            }

            TextField {
                id: o2_at_txf
                width: 160
                height: 47
                text: " "
                anchors.left: text18.right
                anchors.top: parent.top
                anchors.topMargin: 15
                horizontalAlignment: Text.AlignHCenter
                anchors.leftMargin: 81
                font.pointSize: 12
                placeholderText: qsTr("minute")

                onEditingFinished: {
                    LogData.getO2AvgTime(o2_at_txf.text);
                    Modbus.getO2AvgTime(o2_at_txf.text);
                    SerialPort.getO2AvgTime(o2_at_txf.text);
                }
            }

            Text {
                id: text19
                x: 23
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("CO2 avg Time:")
                anchors.top: text18.bottom
                font.pixelSize: 20
                anchors.topMargin: 29
                font.family: "Arial"
            }

            TextField {
                id: co2_at_txf
                y: 104
                width: 160
                height: 47
                text: " "
                anchors.left: text19.right
                anchors.top: o2_at_txf.bottom
                anchors.topMargin: 5
                horizontalAlignment: Text.AlignHCenter
                anchors.leftMargin: 81
                font.pointSize: 12
                placeholderText: qsTr("minute")

                onEditingFinished: {
                    LogData.getCO2AvgTime(co2_at_txf.text);
                    Modbus.getCO2AvgTime(co2_at_txf.text);
                    SerialPort.getCO2AvgTime(co2_at_txf.text);
                }
            }

            Text {
                id: text20
                x: 23
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("CO avg Time:")
                anchors.top: text19.bottom
                font.pixelSize: 20
                anchors.topMargin: 29
                font.family: "Arial"
            }

            TextField {
                id: co_at_txf
                y: 157
                width: 160
                height: 47
                text: " "
                anchors.left: text20.right
                anchors.top: co2_at_txf.bottom
                anchors.topMargin: 5
                horizontalAlignment: Text.AlignHCenter
                anchors.leftMargin: 81
                font.pointSize: 12
                //enabled: connection ? true:false
                placeholderText: qsTr("minute")

                onEditingFinished: {
                    LogData.getCOAvgTime(co_at_txf.text);
                    Modbus.getCOAvgTime(co_at_txf.text);
                    SerialPort.getCOAvgTime(co_at_txf.text);
                }

            }

            Text {
                id: text21
                x: 23
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("TEMP avg Time:")
                anchors.top: text20.bottom
                font.pixelSize: 20
                anchors.topMargin: 29
                font.family: "Arial"
            }

            TextField {
                id: temp_at_txf
                y: 157
                width: 160
                height: 47
                text: " "
                anchors.left: text21.right
                anchors.top: co_at_txf.bottom
                anchors.topMargin: 5
                horizontalAlignment: Text.AlignHCenter
                anchors.leftMargin: 81
                font.pointSize: 12
                placeholderText: qsTr("minute")

                onEditingFinished: {
                    LogData.getTempAvgTime(temp_at_txf.text);
                    Modbus.getTempAvgTime(temp_at_txf.text);
                    SerialPort.getTempAvgTime(temp_at_txf.text);
                }
            }

            Text {
                id: text22
                x: 170
                y: 350
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("O2 avg: NAN")
                anchors.top: text19.bottom
                font.pixelSize: 20
                anchors.topMargin: 140
                anchors.leftMargin: 150
                font.family: "Arial"

//                Connections {
//                    target: LogData

//                    function onO2AvgChanged(){
//                        text22.text = "O2 avg: " + LogData.o2Avg;
//                    }
//                }
                Connections {
                    target: Modbus

                    function onO2AvgChanged(){
                        if(connection === true && switch1.checked === true){
                            text22.text = "O2 avg: " + Modbus.o2Avg;
                        }
                    }
                }
                Connections {
                    target: SerialPort

                    function onO2AvgChanged(){
                        if(connection === true && switch1.checked === false){
                            text22.text = "O2 avg: " + SerialPort.o2Avg;
                        }
                    }
                }
            }

            Text {
                id: text23
                x: 170
                y: 400
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("CO2 avg: NAN")
                anchors.top: text19.bottom
                font.pixelSize: 20
                anchors.topMargin: 180
                anchors.leftMargin: 150
                font.family: "Arial"

//                Connections {
//                    target: LogData

//                    function onCo2AvgChanged(){
//                        text23.text = "CO2 avg: " + LogData.co2Avg;
//                    }
//                }
                Connections {
                    target: Modbus

                    function onCo2AvgChanged(){
                        if(connection === true && switch1.checked === true){
                            text23.text = "CO2 avg: " + Modbus.co2Avg;
                        }
                    }
                }
                Connections {
                    target: SerialPort

                    function onCo2AvgChanged(){
                        if(connection === true && switch1.checked === false){
                            text23.text = "CO2 avg: " + SerialPort.co2Avg;
                        }
                    }
                }
            }

            Text {
                id: text24
                x: 170
                y: 450
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("CO avg: NAN")
                anchors.top: text19.bottom
                font.pixelSize: 20
                anchors.topMargin: 220
                anchors.leftMargin: 150
                font.family: "Arial"

//                Connections {
//                    target: LogData

//                    function onCoAvgChanged(){
//                        text24.text = "CO avg: " + LogData.coAvg;
//                    }
//                }
                Connections {
                    target: Modbus

                    function onCoAvgChanged(){
                        if(connection === true && switch1.checked === true){
                            text24.text = "CO avg: " + Modbus.coAvg;
                        }
                    }
                }
                Connections {
                    target: SerialPort

                    function onCoAvgChanged(){
                        if(connection === true && switch1.checked === false){
                            text24.text = "CO avg: " + SerialPort.coAvg;
                        }
                    }
                }
            }

            Text {
                id: text25
                x: 170
                y: 500
                width: 148
                height: 24
                color: "#ffffff"
                text: qsTr("TEMP avg: NAN")
                anchors.top: text19.bottom
                font.pixelSize: 20
                anchors.topMargin: 260
                anchors.leftMargin: 150
                font.family: "Arial"

//                Connections {
//                    target: LogData

//                    function onTempAvgChanged(){
//                        text25.text = "TEMP avg: " + LogData.tempAvg;
//                    }
//                }
                Connections {
                    target: Modbus

                    function onTempAvgChanged(){
                        if(connection === true && switch1.checked === true){
                            text25.text = "TEMP avg: " + Modbus.tempAvg;
                        }
                    }
                }
                Connections {
                    target: SerialPort

                    function onTempAvgChanged(){
                        if(connection === true && switch1.checked === false){
                            text25.text = "TEMP avg: " + SerialPort.tempAvg;
                        }
                    }
                }
            }
        }
    }

}
