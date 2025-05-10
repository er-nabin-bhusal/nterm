import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    id: folderContainer
    anchors.fill: parent
    property bool isSearchActive: false

    Timer {
        id: searchDebounceTimer
        interval: 500
        onTriggered: {
            eventHandler.onSearchTextChange(searchTextField.text);
        }
    }

    Rectangle {
        id: toolbar
        color: "transparent"
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 50
        Layout.preferredWidth: parent.width

        EditorBtn {
            id: searchButton
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 10
            iconSource: "qrc:/assets/icons/search.svg"
            visible: !folderContainer.isSearchActive
            onClicked: {
                folderContainer.isSearchActive = true;
                searchTextField.forceActiveFocus();
            }
        }

        EditorBtn {
            id: addFolderButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 10
            iconSource: "qrc:/assets/icons/addFolder.svg"
            visible: !folderContainer.isSearchActive
            onClicked: {
                eventHandler.createNewFolder();
            }
        }

        TextField {
            id: searchTextField
            anchors.left: parent.left
            anchors.right: closeSearchButton.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            placeholderText: "Search folders..."
            focus: true
            visible: folderContainer.isSearchActive

            onTextChanged: {
                console.log("Text changed:", text);
                searchDebounceTimer.restart();
            }
        }

        EditorBtn {
            id: closeSearchButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 10
            iconSource: "qrc:/assets/icons/close.svg"
            visible: folderContainer.isSearchActive

            onClicked: {
                folderContainer.isSearchActive = false;
                searchTextField.text = "";
                eventHandler.onSearchTextChange("");
            }
        }
    }

    Rectangle {
        color: "transparent"
        Layout.preferredHeight: parent.height - 105
        Layout.preferredWidth: parent.width

        ListView {
            id: folderList
            anchors.fill: parent
            model: eventHandler ? eventHandler.allFolders : []
            focus: true
            clip: true
            spacing: 1

            delegate: Rectangle {
                id: folderRect
                color: (eventHandler && eventHandler.currentFolder === modelData.folderName) ? "#C0C0C0" : "transparent"
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
    // Trash section
    Rectangle {
        id: trashSection
        Layout.alignment: Qt.AlignBottom
        Layout.preferredHeight: 50
        Layout.preferredWidth: parent.width
        color: (eventHandler && eventHandler.currentFolder === "Trash") ? "#C0C0C0" : "transparent"

        Image {
            id: trashIcon
            source: "qrc:/assets/icons/trash.svg"
            width: 24
            height: 24
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectFit
            mipmap: true
        }

        Text {
            id: trashText
            text: "Trash"
            color: "#000000"  // Explicit black color
            font.pixelSize: 16
            anchors.left: trashIcon.right
            anchors.right: parent.right
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                eventHandler.setCurrentFolder("Trash");
            }
        }
    }
}
