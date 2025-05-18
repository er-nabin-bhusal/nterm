import QtQuick
import QtQuick.Controls

Button {
    id: root
    width: 40
    height: 40
    text: "+"
    font.pixelSize: 24
    visible: true
    z: 1000
    
    background: Item {
        Rectangle {
            id: shadow
            anchors.fill: parent
            anchors.margins: -1
            radius: 20.5
            color: "#40000000"
            y: 3
            opacity: 0.5
        }
        
        Rectangle {
            id: buttonRect
            anchors.fill: parent
            color: root.pressed ? "#e0e0e0" : "#ffffff"
            radius: 20
            border.width: 1
            border.color: "#e0e0e0"
        }
    }
    
    contentItem: Text {
        text: root.text
        font: root.font
        color: "#404040"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
} 