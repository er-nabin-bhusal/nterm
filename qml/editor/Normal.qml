import QtQuick
import QtQuick.Controls
    

Rectangle {
    id: container
    property int componentWidth: 0
    property int componentFontSize: 16
    property string componentText: ""
    property int blockIndex: -1
    property int itemIndex: -1

    border.color: "#4caf50"
    border.width: 1
    width: Math.max(componentWidth, displayText.implicitWidth)
    height: displayText.implicitHeight

    Text {
        id: displayText
        font.weight: Font.Normal
        font.pixelSize: componentFontSize
        color: "blue"
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
        font.weight: Font.Normal
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
