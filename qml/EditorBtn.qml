import QtQuick


Rectangle {
    property string label
    property string iconSource

    signal clicked()
    radius: 5
    color: "transparent"
    implicitWidth: label ? textElement.implicitWidth + 10 : 30
    implicitHeight: label ? textElement.implicitHeight + 10 : 30

    Text {
        id: textElement
        text: parent.label
        anchors.centerIn: parent
    }

    Image {
        source: parent.iconSource
        anchors.centerIn: parent
        width: 25
        height: 25
    }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: () => parent.clicked()
        cursorShape: Qt.PointingHandCursor
        onEntered: parent.color = "lightgray"
        onExited: parent.color = "transparent"
    }
}
