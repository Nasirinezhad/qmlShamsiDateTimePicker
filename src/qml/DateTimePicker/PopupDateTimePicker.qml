import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Popup {
    id: popup
    property var field: null
    property var years: []
    property real currentTime: 0
    property bool time: true
    property bool delimiters: true

    property alias year: year
    property alias month: month
    property alias day: day
    property alias hour: hour
    property alias minute: minute
    property alias second: second

    width: parent.width
    height: parent.height *.4
    y:parent.height-height

    modal: true
    focus: true
    closePolicy: popup.CloseOnReleaseOutside

    background: Rectangle {
        color: "#FFFFFF"
        anchors.fill: parent
    }
    onAboutToHide: {
        field.enabled = true
    }
    function go_today(){
        field.shamsi.timestamp = 0

        day.currentIndex = field.shamsi.day-1
        month.currentIndex = field.shamsi.month-1
        year.currentIndex = year.model.indexOf(field.shamsi.year)
        hour.currentIndex = field.shamsi.hours
        minute.currentIndex = field.shamsi.minutes
        second.currentIndex = field.shamsi.seconds
    }

    Column{
        anchors.fill: parent
        spacing: 10
        Label {
            id:dlbl
            width: parent.width
            horizontalAlignment: Qt.AlignHCenter
            text: field ? (field.shamsi.year, field.shamsi.format(field.format)) : ''
        }

        Row{
            id: row
            anchors.horizontalCenter: parent.horizontalCenter
//            anchors.verticalCenter: parent.verticalCenter
            Tumbler {
                id: second
                model: 60
                visible: time
                visibleItemCount: 3
                height: day.height
                onCurrentIndexChanged: {
                    if(field)field.shamsi.seconds = currentIndex
                }
            }
            Label{
                text: ':'
                visible: delimiters & time & second.visible
                anchors.verticalCenter: hour.verticalCenter
            }

            Tumbler {
                id: minute
                model: 60
                visible: time
                visibleItemCount: 3
                height: day.height
                onCurrentIndexChanged: {
                    if(field)field.shamsi.minutes = currentIndex
                }
            }
            Label{
                text: ':'
                visible: delimiters & time & minute.visible
                anchors.verticalCenter: hour.verticalCenter
            }
            Tumbler {
                id: hour
                model: 24
                visible: time
                visibleItemCount: 3
                height: day.height
                onCurrentIndexChanged: {
                    if(field)field.shamsi.hours = currentIndex
                }
            }
            ToolSeparator {
                height: parent.height
                anchors.verticalCenter: hour.verticalCenter
                visible: time
            }
            Tumbler {
                id: year
                model: years
                visibleItemCount: 3
                height: day.height
                onCurrentIndexChanged: {
                    if(field)field.shamsi.year = model[currentIndex]
                }
            }
            Label{
                text: '/  '
                visible: delimiters
                anchors.verticalCenter: month.verticalCenter
            }
            Tumbler {
                id: month
                model: ['فروردین','اردیبهشت','خرداد','تیر','اَمرداد','شهریور','مهر','آبان','آذر','دی','بهمن','اسفند']
                visibleItemCount: 3
                height: day.height
                onCurrentIndexChanged: {
                    console.log(currentIndex)
                    if(field)field.shamsi.month = currentIndex+1
                }
            }
            Label{
                text: '  /'
                visible: delimiters
                anchors.verticalCenter: day.verticalCenter
            }
            Tumbler {
                id: day
                model: [...Array(month.currentIndex < 6 ? 31: month.currentIndex===11 && year.model[year.currentIndex]%4==3 ? 30: 29).keys()].map(i => i +1)
                visibleItemCount: 3
                onCurrentIndexChanged: {
                    if(field)field.shamsi.day = currentIndex+1
                }
            }
        }
        Item {
            width: parent.width-20
            anchors.horizontalCenter: parent.horizontalCenter
            height: apl.height
            Button {
                id: apl
                text: 'انتخاب'
                anchors.right: parent.right
                Material.background: Material.Cyan
                Material.foreground: "#FFFFFF"
                onClicked: {
                    popup.close()
                }
            }
            Button {
                text: 'انصراف'
                anchors.right: apl.left
                anchors.rightMargin: 10
                onClicked: {
                    field.shamsi.timestamp = currentTime
                    popup.close()
                }
            }

            Button {
                text: 'امروز'
                anchors.left: parent.left
                onClicked: go_today()
            }
        }
    }

    onAboutToShow: {
        currentTime = field.shamsi.timestamp
        go_today()
        if(years.length < 1){
            years = [...Array(5).keys()].map(i => i +field.shamsi.year -2);
            year.currentIndex = 2
        }
    }
}
