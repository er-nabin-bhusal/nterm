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
    property alias textInput: textInput
    
    property color borderColor: "#4caf50"
    property int fontWeight: Font.Normal
    property color textColor: "blue"

    border.color: borderColor
    border.width: 1
    width: Math.max(componentWidth, textInput.implicitWidth)
    height: textInput.implicitHeight

    TextInput {
        id: textInput
        /* Make all padding 0 */
        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0

        font.pixelSize: componentFontSize
        font.weight: fontWeight
        color: textColor
        text: componentText
        selectByMouse: false

        onTextChanged: {
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

            if (event.modifiers & Qt.ControlModifier && event.key === Qt.Key_A) {
                console.log("Keys.onPressed: event.key:", event.key);
                event.accepted = true;
                if (blockLayoutEngineCtx) {
                    blockLayoutEngineCtx.selectAll();
                }
            }
        }

        Connections {
            target: blockLayoutEngineCtx
            function onCursorPositionChanged(changedBlockIndex, newNodeIndex, newCursorPos) {
                /*
                Because these nodes are replaced when a text is changed, we need to focus the text input again
                Plus when refreshed old components are removed but not immediately, so we need to make sure 
                we don't update the ones that are about to be removed(thus use visible)
                */
                if (textInput && textInput.visible && changedBlockIndex === blockIndex && newNodeIndex === itemIndex) {
                    var selStart = textInput.selectionStart;
                    var selEnd = textInput.selectionEnd;
                    textInput.forceActiveFocus();
                    textInput.cursorPosition = newCursorPos;
                    if (selStart !== selEnd) {
                        textInput.select(selStart, selEnd);
                    }
                }
            }
            function onSelectionChanged(block, item, start, end) {
                if (textInput && textInput.visible) {
                    if (block === blockIndex && item === itemIndex) {
                        textInput.select(start, end);
                    }
                }
            }
        }
    }
}

