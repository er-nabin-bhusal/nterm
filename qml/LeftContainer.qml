import QtQuick
import QtQuick.Controls

SplitView {
    anchors.fill: parent
    orientation: Qt.Horizontal
    handle: Rectangle {
        implicitWidth: 0.5
        implicitHeight: 0.5
        color: SplitHandle.pressed ? "#3276C7" : "lightgray"
    }

    Rectangle {
        color: "#F7F7F7"
        SplitView.preferredWidth: 150
        SplitView.minimumWidth: 100
        FolderList {}
    }

    Rectangle {
        color: "#F7F7F7"
        SplitView.fillWidth: true
        NoteList {}
    }
}
