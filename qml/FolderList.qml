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
            iconSource: "qrc:/assets/icons/addFolder.svg"

            onClicked: {
                eventHandler.createNewFolder();
            }
        }
    }

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
                id: folderRect
                color: (folderList.currentIndex === index && eventHandler.currentFolder) ? "#C0C0C0" : "transparent"
                implicitHeight: 40
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

                TextField {
                    id: editField
                    anchors.left: folderIcon.right
                    anchors.right: parent.right
                    anchors.leftMargin: 10
                    visible: false
                    wrapMode: Text.NoWrap
                    readOnly: false
                    text: modelData.folderName
                    anchors.verticalCenter: parent.verticalCenter

                    function editComplete() {
                        if (!editField.visible) return;
                        const res = eventHandler.renameFolder(index, editField.text);
                        if (res) {
                            folderText.text = editField.text;
                        } else {
                            editField.text = modelData.folderName;
                        }

                        folderContent.visible = true;
                        editField.visible = false;
                    }

                    onFocusChanged: function (event) {
                        if (!event) {
                            editComplete();
                        }
                    }

                    Keys.onPressed: function (event) {
                        if (event.key == Qt.Key_Enter || event.key == Qt.Key_Return) {
                            editComplete();
                        }
                    }
                }

                Item {
                    id: folderContent
                    visible: true
                    anchors.left: folderIcon.right
                    anchors.right: parent.right
                    anchors.leftMargin: 5
                    height: parent.height

                    Text {
                        id: folderText
                        padding: 5
                        wrapMode: Text.NoWrap
                        text: modelData.folderName
                        elide: Text.ElideRight
                        anchors.left: parent.left
                        anchors.right: parent.right
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
                                eventHandler.setCurrentFolder(modelData.folderName);
                            }
                        }

                        Menu {
                            id: contextMenu
                            MenuItem {
                                text: "Rename"
                                onTriggered: {
                                    folderContent.visible = false;
                                    editField.visible = true;
                                    editField.forceActiveFocus();
                                }
                            }
                            MenuItem {
                                text: "Delete"
                                onTriggered: {
                                    eventHandler.deleteFolder(index);
                                }
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    if (modelData.editAble) {
                        folderContent.visible = false;
                        editField.visible = true;
                        Qt.callLater(function() {
                            editField.forceActiveFocus();
                        });
                    }
                }
            }
        }
    }
}
