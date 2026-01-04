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
                z: -1
                acceptedButtons: Qt.LeftButton
                onClicked: function(mouse) {
                    // This MouseArea is behind blocks, so it only receives clicks on empty space
                    // Set cursor to last position in last block
                    blockLayoutEngineCtx.setCursorToLastPositionInLastBlock()
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