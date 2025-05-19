import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import NTerm 1.0

Page {
    id: root
    background: Rectangle {
        color: "#ffffff"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        Button {
            text: "← Back"
            onClicked: stackView.pop()
        }

        Label {
            text: "New Note"
            font.pixelSize: 24
            font.weight: Font.Bold
            Layout.fillWidth: true
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            border.color: "#cccccc"
            border.width: 1
            radius: 4

            Rectangle {
                anchors.fill: parent
                anchors.margins: 8
                color: "#f5f5f5"  // Light gray background

                Editor { id: editor }

                ListView {
                    anchors.fill: parent
                    model: editor.lines
                    focus: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: editor.onClicked()
                    }

                    Keys.onPressed: function(event) {
                        editor.onKeyPressed(event.text, currentIndex)
                        event.accepted = true
                    }

                    delegate: EditorItem {
                        index: index
                        text: modelData.text
                        fontSize: 16
                        fontWeight: 500
                        fontColor: "#333333"
                        backgroundColor: "#ffffff"
                        borderColor: "#cccccc"
                        borderWidth: 1
                        borderRadius: 4
                        padding: 8
                    }
                }
            }
        }
    }
} 