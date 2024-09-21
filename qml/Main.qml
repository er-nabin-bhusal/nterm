import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    width: 1200
    height: 800
    visible: true
    title: qsTr("NTerm")

    Popup {
        id: errorDialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        background: Rectangle {
            implicitHeight: contentLayout.implicitHeight
            implicitWidth: contentLayout.implicitWidth
            border.color: "#CCCCCC"
        }
        ColumnLayout {
            id: contentLayout
            spacing: 10
            anchors.topMargin: 10
            Text {
                Layout.margins: 10
                Layout.alignment: Qt.AlignHCenter
                text: popupHandler.message
            }
            Button {
                Layout.bottomMargin: 10
                Layout.alignment: Qt.AlignHCenter
                text: "Ok"
                onClicked: {
                    errorDialog.close();
                }
            }
        }
    }

    Connections {
        target: popupHandler
        function onMessageChanged () {
            errorDialog.open();
        }
    }

    NoteView {
        Keys.onPressed: (event) => {
            if (event.modifiers & Qt.ControlModifier) {
                switch (event.key) {
                    case Qt.Key_N:
                        eventHandler.createNewNote();
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
