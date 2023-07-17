import QtQuick 2.15
import QtQuick.Controls 1.4
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Item {
    id : rootId
    function openDialog(){
        inputDialog.open()
    }

    property string textColor: "#E8E8E8"

    Dialog {
        id: inputDialog

        anchors.centerIn: parent

        width: parent.width / 3.1 //Breaks the binding loop introduced in Qt 5.12.
        height: parent.height / 2.1
        parent: Overlay.overlay

        focus: true
        modal: true
        title: "Pick Date"
        standardButtons: Dialog.Close | Dialog.Select

        Label {
            id: fromLabel
            width: 161
            height: 47
            text: qsTr("From: ")
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 15
        }

        Label {
            id: toLabel
            width: 161
            height: 47
            text: qsTr("To: ")
            anchors.left: fromLabel.right
            anchors.leftMargin: 150
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 15
        }

        Calendar {
            id: calendar
            width: parent.width
            height: parent.height - 50
            frameVisible: true
            weekNumbersVisible: true
            anchors.top: fromLabel.bottom
            anchors.topMargin: 20
            // selectedDate: new Date(2014, 0, 1)
            focus: true
            property var startDate: Date.fromLocaleString(Qt.locale(), LogData.startDate, "yyyy-MM-dd")
            property var stopDate: Date.fromLocaleString(Qt.locale(), LogData.endDate, "yyyy-MM-dd")

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
                                // Date is within the valid range.
                                var strDate = new Date( calendar.startDate.getTime())
                                //console.log(Qt.formatDate(strDate, "dd/MM/yyyy"))
                                fromLabel.text = "From:  " + Qt.formatDate(strDate, "dd/MM/yyyy")

                                var stpDate = new Date( calendar.stopDate.getTime())
                                //console.log(Qt.formatDate(stpDate, "dd/MM/yyyy"))
                                toLabel.text = "To:  " + Qt.formatDate(stpDate, "dd/MM/yyyy")

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

        onAccepted: {
            console.log("Date picked")

            var strDate = new Date( calendar.startDate.getTime())
            console.log(Qt.formatDate(strDate, "yyyy-MM-dd"))

            var stpDate = new Date( calendar.stopDate.getTime())
            console.log(Qt.formatDate(stpDate, "yyyy-MM-dd"))

            LogData.readSelectedFile()
        }

        onRejected: {
            console.log("Closed Configuration")
        }
    }
}
