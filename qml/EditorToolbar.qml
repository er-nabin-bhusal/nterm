import QtQuick
import QtQuick.Layouts


Rectangle {
    id: toolbar
    color: "transparent"
    height: 50

    RowLayout {
        spacing: 20
        anchors.fill: parent

        EditorBtn {
            Layout.leftMargin: 10
            iconSource: eventHandler.textFormat.paragraph ? "qrc:/assets/icons/paragraphActive.svg": "qrc:/assets/icons/paragraph.svg"
            onClicked: () => {
                eventHandler.handleParagraphClick();
            }
        }

        EditorBtn {
            iconSource: eventHandler.textFormat.heading ? "qrc:/assets/icons/headingActive.svg" : "qrc:/assets/icons/heading.svg"
            onClicked: () => {
                eventHandler.handleHeadingClick();
            }
        }

        EditorBtn {
            iconSource: eventHandler.textFormat.bold ? "qrc:/assets/icons/boldActive.svg" : "qrc:/assets/icons/bold.svg"
            onClicked: () => {
                eventHandler.handleBoldClick();
            }
        }

        EditorBtn {
            iconSource: eventHandler.textFormat.italic ? "qrc:/assets/icons/italicActive.svg" : "qrc:/assets/icons/italic.svg"
            onClicked: () => {
                eventHandler.handleItalicClick();
            }
        }

        EditorBtn {
            iconSource: eventHandler.textFormat.underline ? "qrc:/assets/icons/underlineActive.svg" : "qrc:/assets/icons/underline.svg"
            onClicked: () => {
                eventHandler.handleUnderlineClick();
            }
        }
        Rectangle { Layout.fillWidth: true }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 1
        color: "lightgray"
    }
}

