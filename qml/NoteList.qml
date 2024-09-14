import QtQuick
import QtQuick.Layouts

ColumnLayout {
    anchors.fill: parent

    Rectangle {
        id: toolbar
        color: "transparent"
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 50
        Layout.preferredWidth: parent.width

        EditorBtn {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 10
            iconSource: eventHandler.currentFile ? "qrc:/assets/icons/deleteActive.svg" : "qrc:/assets/icons/delete.svg"

            onClicked: {
                eventHandler.deleteNote(noteListView.currentIndex);
            }
        }

        EditorBtn {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 10
            iconSource: "qrc:/assets/icons/create.svg"

            onClicked: {
                eventHandler.createNewNote();
            }
        }
    }
    Rectangle {
        color: "transparent"
        Layout.preferredHeight: parent.height - toolbar.height
        Layout.preferredWidth: parent.width

        ListView {
            id: noteListView
            anchors.fill: parent
            model: eventHandler.allNotes
            focus: true
            clip: true
            spacing: 1

            delegate: Rectangle {
                color: (noteListView.currentIndex === index && eventHandler.currentFile) ? "#C0C0C0" : "transparent"
                implicitHeight: text.implicitHeight + 5

                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: 5
                    rightMargin: 5
                }


                Text {
                    id: text
                    padding: 5
                    wrapMode: Text.Wrap
                    text: modelData.title
                    anchors.centerIn: parent
                    width: parent.width
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        noteListView.currentIndex = index;
                        eventHandler.setCurrentFile(modelData.fileName);
                    }
                }
                Rectangle {
                    height: 1
                    width: parent.width
                    color: "#C0C0C0"
                    anchors {
                        bottom: parent.bottom
                        left: parent.left
                        right: parent.right
                        leftMargin: 5
                        rightMargin: 5
                    }
                    visible: index + 1 != noteListView.count
                }
            }
        }
    }
}
