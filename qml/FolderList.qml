import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    anchors.fill: parent

    Rectangle {
        color: "transparent"
        Layout.preferredHeight: parent.height
        Layout.preferredWidth: parent.width

        ListView {
            id: folderList
            anchors.fill: parent
            model: eventHandler.allFolders
            focus: true
            clip: true
            spacing: 1

            delegate: Rectangle {
                color: (folderList.currentIndex === index && eventHandler.currentFolder) ? "#C0C0C0" : "transparent"
                implicitHeight: text.implicitHeight + 5
                anchors {
                    left: parent.left
                    right: parent.right
                }

                Image {
                    id: folderIcon
                    source: "qrc:/assets/icons/folder.svg"
                    width: 20
                    height: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: text
                    padding: 5
                    wrapMode: Text.NoWrap
                    text: modelData
                    anchors.left: folderIcon.right
                    anchors.right: parent.right
                    elide: Text.ElideRight
                    anchors.verticalCenter: parent.verticalCenter
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    onClicked: function(mouse) {
                        if (mouse.button === Qt.RightButton) {
                            contextMenu.popup();
                        } else {
                            folderList.currentIndex = index;
                            eventHandler.setCurrentFolder(modelData);
                        }
                    }

                    Menu {
                        id: contextMenu
                        MenuItem { text: "Rename" }
                        MenuItem {
                            text: "Delete"
                            onTriggered: {
                                eventHandler.deleteFolder(index);
                            }
                        }
                    }
                }
            }
        }
    }
}
