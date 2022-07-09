import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import Shamsi

TextField {
    id: field
    horizontalAlignment: Text.AlignRight
    placeholderText: 'انتخاب'

    property var popup: null
    property alias shamsi: shamsi
    property alias timestamp: shamsi.timestamp
    property string format: 'l j F Y ساعت H:i'


    onFocusChanged: {
        if(focus){
            field.popup.field = this
            field.popup.open()
            field.enabled = false
        }
    }
    onPressed: {
        field.popup.field = this
        field.popup.open()
        field.enabled = false
    }
    Shamsi {
        id: shamsi
        onChange: {
            field.text= shamsi.format(field.format)
        }
    }
}
