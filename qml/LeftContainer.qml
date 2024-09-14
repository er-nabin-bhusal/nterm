import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

SplitView {
    anchors.fill: parent
    orientation: Qt.Horizontal

    Rectangle {
        SplitView.preferredWidth: 150
        SplitView.minimumWidth: 100
        FolderList {}
    }

    Rectangle {
        SplitView.fillWidth: true
        NoteList {}
    }
}
