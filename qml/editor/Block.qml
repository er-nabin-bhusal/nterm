import QtQuick

Item {
    id: root
    property var blockData: null
    property int blockIndex: -1
    property alias flowContent: flowContent
    width: parent.width
    height: Math.max(flowContent.height, 16)
    
    MouseArea {
        id: blockMouseArea
        anchors.fill: parent
        z: -1
        acceptedButtons: Qt.LeftButton
        onClicked: function(mouse) {
            // This MouseArea is behind children, so it only receives clicks on empty space
            // Set cursor to last position in this block
            blockLayoutEngineCtx.setCursorToLastPositionInBlock(root.blockIndex)
        }
        onPressed: function(mouse) {
            blockLayoutEngineCtx.setCursorPosition(root.blockIndex, root.itemIndex, root.cursorPosition);
        }
        onReleased: function(mouse) {
            blockLayoutEngineCtx.setCursorPosition(root.blockIndex, root.itemIndex, root.cursorPosition);
        }
    }
    
    Flow {
        id: flowContent
        width: parent.width
        
        Connections {
            target: blockLayoutEngineCtx
            function onBlockChanged(changedBlockIndex) {
                if (changedBlockIndex === root.blockIndex) {
                    root.blockData = blockLayoutEngineCtx.getBlock(changedBlockIndex)
                }
            }
        }
        
        Repeater {
            model: blockData ? (blockData.items || []) : []
            
            Loader {
                property var itemData: modelData
                property int itemIndex: index
                
                sourceComponent: {
                    if (modelData.typeStr === "bold") {
                        return boldComponent
                    } else if (modelData.typeStr === "normal") {
                        return normalComponent
                    }
                    return null
                }
                
                onItemChanged: {
                    if (item) {
                        item.componentText = itemData ? (itemData.text || "") : ""
                        item.componentFontSize = itemData ? (itemData.fontSize || 16) : 16
                        item.blockIndex = root.blockIndex
                        item.itemIndex = index
                    }
                }
            }
        }
    }
    
    Component {
        id: boldComponent
        Bold {}
    }
    
    Component {
        id: normalComponent
        Normal {}
    }
}
