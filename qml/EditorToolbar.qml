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
            iconSource: (eventHandler && eventHandler.textFormat && eventHandler.textFormat.paragraph) ? "qrc:/assets/icons/paragraphActive.svg": "qrc:/assets/icons/paragraph.svg"
            onClicked: () => {
                if (eventHandler) eventHandler.handleParagraphClick();
            }
        }

        EditorBtn {
            iconSource: (eventHandler && eventHandler.textFormat && eventHandler.textFormat.heading) ? "qrc:/assets/icons/headingActive.svg" : "qrc:/assets/icons/heading.svg"
            onClicked: () => {
                if (eventHandler) eventHandler.handleHeadingClick();
            }
        }

        EditorBtn {
            iconSource: (eventHandler && eventHandler.textFormat && eventHandler.textFormat.bold) ? "qrc:/assets/icons/boldActive.svg" : "qrc:/assets/icons/bold.svg"
            onClicked: () => {
                if (eventHandler) eventHandler.handleBoldClick();
            }
        }

        EditorBtn {
            iconSource: (eventHandler && eventHandler.textFormat && eventHandler.textFormat.italic) ? "qrc:/assets/icons/italicActive.svg" : "qrc:/assets/icons/italic.svg"
            onClicked: () => {
                if (eventHandler) eventHandler.handleItalicClick();
            }
        }

        EditorBtn {
            iconSource: (eventHandler && eventHandler.textFormat && eventHandler.textFormat.underline) ? "qrc:/assets/icons/underlineActive.svg" : "qrc:/assets/icons/underline.svg"
            onClicked: () => {
                if (eventHandler) eventHandler.handleUnderlineClick();
            }
        }

        EditorBtn {
            iconSource: (eventHandler && eventHandler.textFormat && eventHandler.textFormat.codeBlock) ? "qrc:/assets/icons/codeBlockActive.svg" : "qrc:/assets/icons/codeBlock.svg"
            onClicked: () => {
                if (eventHandler) eventHandler.handleCodeBlockClick();
            }
        }

        Rectangle { Layout.fillWidth: true }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        implicitHeight: 1
        color: "lightgray"
    }
}

