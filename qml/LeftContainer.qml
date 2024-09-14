import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

SplitView {
    anchors.fill: parent
    orientation: Qt.Horizontal

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
