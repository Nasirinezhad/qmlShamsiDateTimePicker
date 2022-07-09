import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import 'qrc:/DateTimePicker'


ApplicationWindow {
    id: app
    visible: true

    width: 640
    height: 860

    Column {
        DateTimeField {
            id: a
            //initial NOW
            shamsi.timestamp: 0
            popup: popdate
        }
        DateTimeField {
            id: b
            //initial by timestamp
            shamsi.timestamp: 1657386796
            popup: popdate
        }
        DateTimeField {
            id: c
            //initial by date time
            shamsi.day: 18
            shamsi.month: 4
            shamsi.year: 1401
            shamsi.hours: 22
            popup: popdate
        }
    }
    PopupDateTimePicker {
        id: popdate
        // !!one popup for all
    }
}
