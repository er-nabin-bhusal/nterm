import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


SplitView {
    id: notesContainer
    anchors.fill: parent
    orientation: Qt.Horizontal

    Rectangle {
        id: leftNavcon
        SplitView.preferredWidth: 400
        SplitView.minimumWidth: 300

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
                function onCurrentFileChanged (isNew) {
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
                    }
                    if (event.key == Qt.Key_Enter || event.key == Qt.Key_Return) {
                        const response = eventHandler.enterPressed();
                        if (response) { event.accepted = true; }
                    }
                    if (event.key == Qt.Key_Space) {
                        eventHandler.detectLink();
                    }

                    debounceTimer.restart();
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

                onLinkActivated: (exLink) => {
                    Qt.openUrlExternally(exLink);
                }
            }
        }

    }
}
