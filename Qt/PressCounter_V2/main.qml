import QtQuick 2.15
import QtQuick.Window 2.15
import QtCharts 2.3
import QtQuick.Controls 1.4
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Styles 1.1
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

Window {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Press Counter")

    Material.theme: Material.Dark
    Material.accent: Material.Orange

    property bool readStatus: false
    property bool determined: false

    Rectangle {
        id: background
        color: "#ffffff"
        anchors.fill: parent
        Rectangle {
            id: rectangle
            width: 1405
            color: "#30304c"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 34

            ChartView {
                id: spline
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.rightMargin: 0
                anchors.leftMargin: 0
                anchors.bottomMargin: 0
                anchors.topMargin: 0

                Connections {
                    id: logChartValueId
                    target: LogData

                    property int dayCount: 0
                    property int weekCount: 0
                    property int monthCount: 0
                    property string date: ""
                    property double checkStart: 0
                    property double count: 0

                    function onReadCountChanged(){
                        if(readStatus === true){
                            valueChart.append(Date.fromLocaleString(Qt.locale(), LogData.rDate, "yyyy-MM-dd"), parseFloat(LogData.rCount))

                            if(checkStart === 0){
                                dateAxis.min = Date.fromLocaleString(Qt.locale(), LogData.rDate, "yyyy-MM-dd")

                                calendar.startDate = Date.fromLocaleString(Qt.locale(), LogData.rDate, "yyyy-MM-dd")
                                checkStart = 1
                            }

                            if(LogData.dataCounter === 1){
                                valueChart.clear();
                            }
                        }
                    }
                }

//                Connections {
//                    target: LogData

//                    function onStartDateChanged(){
//                        dateAxis.min = Date.fromLocaleString(Qt.locale(), LogData.startDate, "yyyy-MM-dd")
//                    }
//                }

                Connections {
                    target: LogData

                    function onEndDateChanged(){
                        dateAxis.max = Date.fromLocaleString(Qt.locale(), LogData.endDate, "yyyy-MM-dd")
                        calendar.enabled = Date.fromLocaleString(Qt.locale(), LogData.endDate, "yyyy-MM-dd")
                    }
                }

                ValueAxis {
                    id: countAxis
                    min: 0
                    max: 1000
                    tickCount: 1
                }

                DateTimeAxis{
                    id: dateAxis
                    format: "yyyy-MM-dd"
                    min : Date.fromLocaleString(Qt.locale(), LogData.startDate, "yyyy-MM-dd")
                    max : Date.fromLocaleString(Qt.locale(), LogData.endDate, "yyyy-MM-dd")
                    tickCount: 1
                }

//                ValueAxis {
//                    id: dateAxis
//                    min: 0
//                    //max: 100
//                    tickCount: 1
//                }

                LineSeries {
                    id: valueChart
                    name: "Day"
                    color: "#28b3ff"
                    width: 3
                    axisX: dateAxis
                    axisY: countAxis
                }

                Rectangle {
                    id: rectangle1
                    x: 26
                    y: 20
                    width: 374
                    height: 53
                    color: "#9756529e"
                    radius: 10
                    border.width: 0
                    smooth: false

                    RadioButton{
                        id: dayRd
                        x: 10
                        y: 7
                        width: 86
                        height: 40
                        text: qsTr("Day")
                        font.family: "Arial"
                        checked: true

                        onCheckedChanged: {
                            if(checked){
                                logChartValueId.count = 0
                                logChartValueId.checkStart = 0
                                valueChart.clear();

                                LogData.radioSelected(1);

                                if(determined == false){
                                    LogData.readFile();
                                    readStatus = true;
                                    LogData.readReady();
                                }
                                else{
                                    LogData.readSelectedFile();
                                }
                            }
                        }
                    }

                    RadioButton{
                        id: weekRd
                        x: 100
                        y: 7
                        width: 86
                        height: 40
                        text: qsTr("Week")
                        font.family: "Arial"

                        onCheckedChanged: {
                            if(checked){
                                logChartValueId.count = 0
                                logChartValueId.checkStart = 0
                                valueChart.clear();

                                LogData.radioSelected(2);

                                if(determined == false){
                                    LogData.readFile();
                                    readStatus = true;
                                    LogData.readReady();
                                }
                                else{
                                    LogData.readSelectedFile();
                                }
                            }
                        }
                    }

                    RadioButton{
                        id: monthRd
                        x: 190
                        y: 7
                        width: 86
                        height: 40
                        text: qsTr("Month")
                        font.family: "Arial"

                        onCheckedChanged: {
                            if(checked){
                                logChartValueId.count = 0
                                logChartValueId.checkStart = 0
                                valueChart.clear();

                                LogData.radioSelected(3);

                                if(determined == false){
                                    LogData.readFile();
                                    readStatus = true;
                                    LogData.readReady();
                                }
                                else{
                                    LogData.readSelectedFile();
                                }
                            }
                        }
                    }

                    RadioButton{
                        id: yearRd
                        x: 280
                        y: 7
                        width: 80
                        height: 40
                        text: qsTr("Year")
                        font.family: "Arial"

                        onCheckedChanged: {
                            if(checked){
                                logChartValueId.count = 0
                                logChartValueId.checkStart = 0
                                valueChart.clear();

                                LogData.radioSelected(4);

                                if(determined == false){
                                    LogData.readFile();
                                    readStatus = true;
                                    LogData.readReady();
                                }
                                else{
                                    LogData.readSelectedFile();
                                }
                            }
                        }
                    }
                }
            }
            anchors.leftMargin: 50
            anchors.topMargin: 50
        }

        GroupBox {
            id: groupBox
            x: 1461
            width: 436
            height: 390
            anchors.right: parent.right
            anchors.top: parent.top
            title: qsTr("SETTING")
            font.family: "Arial"
            font.pointSize: 20
            anchors.topMargin: 50
            anchors.rightMargin: 23

            Label {
                id: uartLabel
                width: 161
                height: 47
                text: qsTr("System")
                anchors.left: parent.left
                anchors.top: parent.top
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: 24
                anchors.topMargin: 25
                font.pointSize: 15
            }

            Label {
                id: connectionStatus
                width: 161
                height: 47
                font.pointSize: 12
                text: qsTr("NOT CONNECTED")
                color: "red"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.left: uartLabel.right
                anchors.top: parent.top
                anchors.leftMargin: 30
                anchors.topMargin: 27
            }

            Label {
                id: savePathLabel
                width: 161
                height: 47
                text: qsTr("Save Path")
                anchors.left: parent.left
                anchors.top: uartLabel.bottom
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: 24
                anchors.topMargin: 30
                font.pointSize: 15
            }

            Label {
                id: loadPathLabel
                width: 161
                height: 47
                text: qsTr("Load Path")
                anchors.left: parent.left
                anchors.top: savePathLabel.bottom
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: 24
                anchors.topMargin: 32
                font.pointSize: 15
            }

            Button {
                id: savePath_btn
                width: 183
                height: 56
                text: qsTr("Select")
                anchors.left: savePathLabel.right
                anchors.top: connectionStatus.bottom
                anchors.leftMargin: 20
                anchors.topMargin: 25
                font.family: "Arial"
                font.pointSize: 16
                icon.width: 21

                FileDialog {
                    id: saveFileDialog
                    title: "Choose a path"
                    folder: shortcuts.desktop
                    selectFolder: true
                    onAccepted: {
                        console.log("You have chosen: " + saveFileDialog.fileUrl)
                        SerialPort.createSaveFile(saveFileDialog.fileUrl);
                    }
                    onRejected: {
                        console.log("Canceled")
                    }
                }

                onClicked: {
                    saveFileDialog.open();
                }
            }

            Button {
                id: loadPath_btn
                width: 183
                height: 56
                text: qsTr("Select")
                anchors.left: savePathLabel.right
                anchors.top: savePath_btn.bottom
                anchors.leftMargin: 20
                anchors.topMargin: 19
                font.family: "Arial"
                font.pointSize: 16
                icon.width: 21

                FileDialog {
                    id: loadFileDialog
                    title: "Choose a path"
                    folder: shortcuts.desktop
                    onAccepted: {
                        console.log("You chose: " + loadFileDialog.fileUrl)
                        LogData.getLoadPath(loadFileDialog.fileUrl)
                        valueChart.clear();
                        load_btn.enabled = true
                        datePicker_btn.enabled = true
                        calendar.enabled = true
                        determined = false
                    }
                    onRejected: {
                        console.log("Canceled")
                        load_btn.enabled = false
                    }
                }

                onClicked: {
                    loadFileDialog.open();
                }
            }

            Button {
                id: load_btn
                text: qsTr("Load")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.rightMargin: 211
                anchors.leftMargin: 7
                anchors.bottomMargin: 10
                anchors.topMargin: 250
                font.pointSize: 14
                enabled: false

                onClicked: {
                    if(dayRd.checked){
                        LogData.radioSelected(1);
                    }
                    if(weekRd.checked){
                        LogData.radioSelected(2);
                    }
                    if(monthRd.checked){
                        LogData.radioSelected(3);
                    }
                    if(yearRd.checked){
                        LogData.radioSelected(4);
                    }

                    LogData.readFile();
                    readStatus = true;
                    LogData.readReady();
                }
            }

            Button {
                id: save_btn
                text: qsTr("SAVE")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.rightMargin: 0
                anchors.leftMargin: 214
                anchors.bottomMargin: 10
                anchors.topMargin: 250
                font.pointSize: 14
            }
        }

        Label {
            id: totalCountLabel
            width: 161
            height: 47
            text: qsTr("Total Count since 0000-00-00 is: NAN")
            anchors.left: groupBox.left
            anchors.top: groupBox.bottom
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 10
            anchors.topMargin: 20
            font.pointSize: 10

            Connections {
                target: LogData

                function onTotalCountChanged(){
                    if(readStatus === true){
                        totalCountLabel.text= "Total Count since " + LogData.startDate + " is: " + LogData.totalCount
                    }
                }
            }
        }

        Label {
            id: totalCountResetLabel
            width: 161
            height: 47
            text: qsTr("Total Count since reseted in 0000-00-00 is: NAN")
            anchors.left: groupBox.left
            anchors.top: totalCountLabel.bottom
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 10
            anchors.topMargin: 0
            font.pointSize: 10

            Connections {
                target: LogData

                function onResetCountChanged(){
                    if(readStatus === true){
                        totalCountResetLabel.text= "Total Count since reseted in " + LogData.startDate + " is: " + LogData.resetCount
                        console.log(LogData.resetCount)
                    }
                }
            }
        }

        Label {
            id: toFromCountLabel
            width: 161
            height: 47
            text: qsTr("Total Count from 0000-00-00 to 0000-00-00 is: NAN")
            anchors.left: groupBox.left
            anchors.top: totalCountResetLabel.bottom
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 10
            anchors.topMargin: 0
            font.pointSize: 10

            Connections {
                target: LogData

                function onTotalCountChanged(){
                    if(readStatus === true){
                        totalCountLabel.text= "Total Count from " + LogData.startDate + " to " +  LogData.endDate + " is: " + LogData.totalCount
                    }
                }
            }
        }

        Calendar {
            id: calendar
            width: 400
            height: 280
            frameVisible: true
            weekNumbersVisible: true
            anchors.top: toFromCountLabel.bottom
            anchors.topMargin: 10
            anchors.left: groupBox.left
            anchors.leftMargin: 10
            //selectedDate: new Date(2014, 0, 1)
            focus: true
            enabled: false
            property var startDate: undefined
            property var stopDate: undefined

            style: CalendarStyle {
                dayDelegate: Item {
                    readonly property color sameMonthDateTextColor: "#444"
                    readonly property color selectedDateColor: "#3778d0"
                    readonly property color selectedDateTextColor: "white"
                    readonly property color differentMonthDateTextColor: "#bbb"
                    readonly property color invalidDatecolor: "#dddddd"
                    property var dateOnFocus: styleData.date



                    Rectangle {
                        anchors.fill: parent
                        border.color: "transparent"
                        color: styleData.date !== undefined && styleData.selected ? selectedDateColor : "transparent"

                    }

                    Rectangle{
                        id:fl
                        anchors.fill: parent
                        property bool flag: false
                        color: ((dateOnFocus>calendar.startDate) && (dateOnFocus< calendar.stopDate))?"#55555555":
                               (calendar.startDate !==undefined && dateOnFocus.getTime()===calendar.startDate.getTime())?"#3778d0":"transparent"
                    }


                    MouseArea{
                        anchors.fill: parent
                        propagateComposedEvents: true
                        onPressed: {

                            if(calendar.startDate===undefined){
                                calendar.startDate=styleData.date
                            }
                            else if(calendar.stopDate=== undefined){
                                calendar.stopDate=styleData.date
                            }
                            else{
                                calendar.startDate=styleData.date
                                calendar.stopDate= undefined
                            }

                            if(calendar.stopDate<=calendar.startDate){
                                calendar.startDate=styleData.date
                                calendar.stopDate= undefined
                            }

                            mouse.accepted = false
                        }
                    }


                    Label {
                        id: dayDelegateText
                        text: styleData.date.getDate()
                        anchors.centerIn: parent
                        color: {
                            var color = invalidDatecolor;
                            if (styleData.valid) {

                                color = styleData.visibleMonth ? sameMonthDateTextColor : differentMonthDateTextColor;
                                if (styleData.selected) {
                                    color = selectedDateTextColor;
                                }
                                else if (dateOnFocus.getTime()===calendar.startDate.getTime()) {
                                    color = selectedDateTextColor;
                                }
                            }
                            color;
                        }
                    }
                }
            }
        }

        Button {
            id: datePicker_btn
            width: 400
            anchors.left: groupBox.left
            anchors.top: calendar.bottom
            anchors.leftMargin: 10
            anchors.topMargin: 10
            text: qsTr("Select Date")
            enabled: false

            onClicked: {
                //inputPannel.openDialog()
                valueChart.clear();
                var strDate = new Date( calendar.startDate.getTime())
                var stpDate = new Date( calendar.stopDate.getTime())
                LogData.selectedStartDate(Qt.formatDate(strDate, "yyyy-MM-dd"))
                LogData.selectedEndDate(Qt.formatDate(stpDate, "yyyy-MM-dd"))
                LogData.readSelectedFile()
                determined = true
                toFromCountLabel.text = qsTr("Total Count from " + Qt.formatDate(strDate, "yyyy-MM-dd") + " to " + Qt.formatDate(strDate, "yyyy-MM-dd") + " is: " + LogData.deterCount)
            }

            DatePickerDialog {
                id : inputPannel
            }
        }


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
    }
}

