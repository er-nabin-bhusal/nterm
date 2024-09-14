import QtQuick
import QtQuick.Controls


Window {
    id: root
    width: 1024
    height: 720
    visible: true
    title: qsTr("NTerm")

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
