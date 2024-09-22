import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    anchors.fill: parent

    Rectangle {
        id: toolbar
        color: "transparent"
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 50
        Layout.preferredWidth: parent.width

        EditorBtn {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 10
            iconSource: "qrc:/assets/icons/addNote.svg"

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
                    acceptedButtons: Qt.LeftButton | Qt.RightButton

                    onClicked: function (mouse) {
                        if (mouse.button === Qt.RightButton) {
                            contextMenu.popup();
                        } else {
                            noteListView.currentIndex = index;
                            eventHandler.setCurrentFile(modelData.fileName);
                        }
                    }

                    Menu {
                        id: contextMenu
                        MenuItem {
                            text: "Delete"
                            onTriggered: {
                                eventHandler.deleteNote(index);
                            }
                        }
                    }
                }
                Rectangle {
                    height: 1
                    width: parent.width
                    color: "#E8E8E8"
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
