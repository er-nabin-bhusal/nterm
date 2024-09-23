import QtQuick
import QtQuick.Controls


SplitView {
    id: notesContainer
    anchors.fill: parent
    orientation: Qt.Horizontal
    handle: Rectangle {
        implicitWidth: 0.5
        implicitHeight: 0.5
        color: SplitHandle.pressed ? "#3276C7" : "lightgray"
    }

    Rectangle {
        id: leftNavcon
        SplitView.preferredWidth: 400
        SplitView.minimumWidth: 300

        LeftContainer {}
    }
    Rectangle {
        SplitView.fillWidth: true

        EditorToolbar {
            id: toolbar
            width: parent.width
        }

        ScrollView {
            palette.mid: "#D3D3D3"

            anchors {
                top: toolbar.bottom
                left: parent.left
                bottom: parent.bottom
                right: parent.right

                topMargin: 0.05 * parent.height
                leftMargin: 0.1 * parent.width
                rightMargin: 0.1 * parent.width
            }

            TextArea {
                id: textarea
                color: constants.textColor
                selectionColor: constants.textSelectionColor
                selectedTextColor: constants.textSelectedColor
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
    }
}
