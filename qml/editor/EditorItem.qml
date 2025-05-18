import QtQuick

Rectangle {
    property string text
    property int fontSize
    property int fontWeight
    property string fontColor
    property string backgroundColor
    property string borderColor
    property int borderWidth
    property int borderRadius
    property int padding
    property int index

    color: backgroundColor
    border.color: borderColor
    border.width: borderWidth
    radius: borderRadius

    Text {
        id: lineText
        text: parent.text
        font.pixelSize: parent.fontSize
        font.weight: parent.fontWeight
        color: parent.fontColor
        anchors.left: parent.left
    }

    Rectangle {
        visible: true
        width: 4
        height: 20
        color: "#000000"
        opacity: 1.0
        z: 1000
        anchors.right: lineText.right
        anchors.verticalCenter: lineText.verticalCenter
       

        Timer {
            running: parent.visible
            repeat: true
            interval: 500
            onTriggered: parent.opacity = parent.opacity === 1.0 ? 0.0 : 1.0
        }
    }
}