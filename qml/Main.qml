import QtQuick
import QtQuick.Controls


Window {
    id: root
    width: 1200
    height: 800
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
