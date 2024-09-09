import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


SplitView {
    id: notesContainer
    anchors.fill: parent
    orientation: Qt.Horizontal

    Rectangle {
        id: leftNavcon
        SplitView.preferredWidth: 250
        SplitView.minimumWidth: 150
        color: "#F7F7F7"

        LeftContainer {}
    }

    Rectangle {
        SplitView.fillWidth: true

        Rectangle {
            id: toolbar
            color: "transparent"
            height: 50
            width: parent.width

            RowLayout {
                spacing: 10
                Layout.alignment: Qt.AlignLeft

                EditorBtn {
                    iconSource: eventHandler.textFormat.heading ? "assets/headingActive.svg" : "assets/heading.svg"
                    onClicked: () => {
                        eventHandler.handleHeadingClick();
                    }
                }

                EditorBtn {
                    iconSource: eventHandler.textFormat.bold ? "assets/boldActive.svg" : "assets/bold.svg"
                    onClicked: () => {
                        eventHandler.handleBoldClick();
                    }
                }

                EditorBtn {
                    iconSource: eventHandler.textFormat.italic ? "assets/italicActive.svg" : "assets/italic.svg"
                    onClicked: () => {
                        eventHandler.handleItalicClick();
                    }
                }

                EditorBtn {
                    iconSource: "assets/underline.svg"
                    onClicked: () => {
                        eventHandler.handleUnderlineClick();
                    }
                }
            }
        }

        ScrollView {

            width: parent.width
            palette.mid: "#D3D3D3"

            anchors {
                top: toolbar.bottom
                left: parent.left
                bottom: parent.bottom
            }

            Connections {
                target: eventHandler
                onCurrentFileChanged: (isNew) => {
                    if (isNew) {
                        textarea.selectAll();
                        textarea.forceActiveFocus();
                        eventHandler.setSelection(textarea.selectionStart, textarea.selectionEnd);
                    } else {
                        textarea.text = eventHandler.readCurrentFileContent();
                    }
                }
            }

            TextArea {
                id: textarea
                selectionColor: "#cce5ff"
                selectedTextColor: "#000"
                textFormat: TextEdit.AutoText
                wrapMode: Text.Wrap
                readOnly: !eventHandler.currentFile
                font.pointSize: 14
                focus: true

                Timer {
                    id: debounceTimer
                    interval: 300
                    repeat: false
                    onTriggered: {
                        eventHandler.saveContentToFile();
                    }
                }


                Keys.onPressed: (event) => {
                    if (event.modifiers & Qt.ControlModifier) {
                        switch (event.key) {
                            case Qt.Key_B:
                                eventHandler.handleBoldClick();
                                break;
                            case Qt.Key_I:
                                eventHandler.handleItalicClick();
                                break;
                            case Qt.Key_U:
                                eventHandler.handleUnderlineClick();
                                break;
                            case Qt.Key_H:
                                eventHandler.handleHeadingClick();
                                break;
                            default:
                                break;
                        }
                    } else {
                        debounceTimer.restart();
                    }
                }

                onSelectionEndChanged: {
                    eventHandler.setSelection(textarea.selectionStart, textarea.selectionEnd);
                }

                onSelectionStartChanged: {
                    eventHandler.setSelection(textarea.selectionStart, textarea.selectionEnd);
                }

                Component.onCompleted: {
                    eventHandler.setTextDocument(textarea.textDocument);
                    Qt.callLater(() => {
                        if (eventHandler.currentFile) {
                            textarea.text = eventHandler.readCurrentFileContent();
                        }
                    });
                }
            }
        }

    }
}
