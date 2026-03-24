import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: root
    background: Rectangle {
        color: "#ffffff"
    }

    Component.onCompleted: {
        blockLayoutEngineCtx.initializeSampleData();
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        Label {
            text: "New Note"
            font.pixelSize: 24
            font.weight: Font.Bold
            Layout.fillWidth: true
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f5f5f5"
            border.color: "#cccccc"
            border.width: 1

            MouseArea {
                id: editorMouseArea
                anchors.fill: parent
                z: 100
                acceptedButtons: Qt.LeftButton
                
                function getBlockAndNodeIndexAtCoordinates(x, y) {
                    var pointInBlocksContainer = editorMouseArea.mapToItem(blocksContainer, x, y);
                    var clickedBlock = blocksContainer.childAt(pointInBlocksContainer.x, pointInBlocksContainer.y);
                    
                    if (!clickedBlock) {
                        return {blockIndex: -1, nodeIndex: -1, x: -1};
                    }
                    
                    var blockIndex = clickedBlock.blockIndex;
                    var flowContent = clickedBlock.flowContent;
                    var pointInFlowContent = editorMouseArea.mapToItem(flowContent, x, y);
                    var clickedNode = flowContent.childAt(pointInFlowContent.x, pointInFlowContent.y);
                    if (!clickedNode) {
                        return {blockIndex: blockIndex, nodeIndex: -1, x: -1};
                    }
                    var pointInNode = editorMouseArea.mapToItem(clickedNode, x, y);
                    return {blockIndex: blockIndex, nodeIndex: clickedNode.itemIndex, x: pointInNode.x };
                }
                
                onPressed: function(mouse) {
                    var pos = getBlockAndNodeIndexAtCoordinates(mouse.x, mouse.y);
                    if (pos.blockIndex !== -1 && pos.nodeIndex !== -1 && pos.x !== -1) {
                        blockLayoutEngineCtx.setSelectionStart(pos.blockIndex, pos.nodeIndex, pos.x);
                        blockLayoutEngineCtx.toggleSelecting(true);
                    }
                }
                onReleased: function(mouse) {
                    var pos = getBlockAndNodeIndexAtCoordinates(mouse.x, mouse.y);
                    if (pos.blockIndex !== -1 && pos.nodeIndex !== -1 && pos.x !== -1) {
                        blockLayoutEngineCtx.setSelectionEnd(pos.blockIndex, pos.nodeIndex, pos.x);
                        blockLayoutEngineCtx.toggleSelecting(false);
                    }
                }
                onPositionChanged: function(mouse) {
                    var pos = getBlockAndNodeIndexAtCoordinates(mouse.x, mouse.y);
                    if (pos.blockIndex !== -1 && pos.nodeIndex !== -1 && pos.x !== -1) {
                        blockLayoutEngineCtx.onPositionChanged(pos.blockIndex, pos.nodeIndex, pos.x);
                    }
                }
                
            }

            Column {
                id: blocksContainer
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 16
                
                onWidthChanged: {
                    blockLayoutEngineCtx.availableWidth = width;
                }

                Connections {
                    target: blockLayoutEngineCtx
                    function onBlocksChanged() {
                        blocksRepeater.model = blockLayoutEngineCtx.blocks;
                    }
                }
                
                Repeater {
                    id: blocksRepeater
                    model: blockLayoutEngineCtx ? blockLayoutEngineCtx.blocks : []
                    
                    Block {
                        blockData: modelData
                        blockIndex: index
                        width: parent.width
                    }
                }
            }
        }
    }
} 