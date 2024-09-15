import QtQuick
import QtQuick.Layouts

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
                    wrapMode: Text.Wrap
                    text: modelData
                    anchors.left: folderIcon.right
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        folderList.currentIndex = index;
                        eventHandler.setCurrentFolder(modelData);
                    }
                }
            }
        }
    }
}
