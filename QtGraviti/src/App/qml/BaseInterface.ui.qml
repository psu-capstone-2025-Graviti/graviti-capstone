

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 1280
    height: 720

    Button {
        id: button
        x: 22
        y: 22
        text: qsTr("Button")
        autoExclusive: true
        checked: true
        checkable: true
        display: AbstractButton.TextUnderIcon
        state: ""
    }
}
