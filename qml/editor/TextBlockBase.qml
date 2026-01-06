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
    
    // Style properties that can be customized
    property color borderColor: "#4caf50"
    property int fontWeight: Font.Normal
    property color textColor: "blue"

    border.color: borderColor
    border.width: 1
    width: Math.max(componentWidth, displayText.implicitWidth)
    height: displayText.implicitHeight

    Text {
        id: displayText
        font.weight: fontWeight
        font.pixelSize: componentFontSize
        color: textColor
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
        font.weight: fontWeight
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
            if (event.key === Qt.Key_Backspace) {
                if (cursorPosition === 0) {
                    event.accepted = true;
                    blockLayoutEngineCtx.handleBackspaceKey(blockIndex, itemIndex, cursorPosition);
                }
            }
        }

        Connections {
            target: blockLayoutEngineCtx
            function onCursorPositionChanged(changedBlockIndex, newNodeIndex, newCursorPos) {
                /*
                Because these nodes are replaced when a text is changed, we need to focus the text area again
                Plus when refreshed old components are removed but not immediately, so we need to make sure 
                we don't update the ones that are about to be removed(thus use visible)
                */
                if (textArea && textArea.visible && changedBlockIndex === blockIndex && newNodeIndex === itemIndex) {
                    textArea.forceActiveFocus();
                    textArea.cursorPosition = newCursorPos;
                }
            }
        }
    }
}

