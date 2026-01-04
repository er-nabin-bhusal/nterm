import QtQuick
import QtQuick.Controls
    

Rectangle {
    id: container
    property int componentWidth: 0
    property int componentFontSize: 16
    property string componentText: ""
    property int blockIndex: -1
    property int itemIndex: -1
    property int cursorPosition: -1

    border.color: "red"
    border.width: 1
    width: Math.max(componentWidth, displayText.implicitWidth)
    height: displayText.implicitHeight

    Text {
        id: displayText
        font.weight: Font.Bold
        font.pixelSize: componentFontSize
        color: "#000000"
        text: componentText
        wrapMode: Text.NoWrap
        visible: true
    }

    TextArea {
        id: textArea
        /* Make all padding 0 */
        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0

        font.pixelSize: componentFontSize
        font.weight: Font.Bold
        color: "transparent"
        text: componentText
        wrapMode: TextArea.NoWrap
        background: null
        selectByMouse: true

        onTextChanged: {
            displayText.text = text
            if (text !== componentText && blockIndex >= 0 && itemIndex >= 0) {
                blockLayoutEngineCtx.textChanged(text, blockIndex, itemIndex, cursorPosition);
            }
        }

        Keys.onPressed: function(event) {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                event.accepted = true;
                blockLayoutEngineCtx.handleEnterKey(blockIndex, itemIndex, cursorPosition);
            }

            if (event.key === Qt.Key_Left || event.key === Qt.Key_Right || 
                event.key === Qt.Key_Up || event.key === Qt.Key_Down) {
                blockLayoutEngineCtx.handleArrowKeys(blockIndex, itemIndex, cursorPosition, event.key);
            }
        }

        Connections {
            target: blockLayoutEngineCtx
            function onCursorPositionChanged(changedBlockIndex, newNodeIndex, newCursorPos) {
                if (textArea && textArea.visible && changedBlockIndex === blockIndex && newNodeIndex === itemIndex) {
                    textArea.forceActiveFocus();
                    textArea.cursorPosition = newCursorPos;
                }
            }
        }
    }
}
