import QtQuick 2.9
import QtQuick3D
import "."
import QtQuick.Layouts 2.15

Item {
    id: item1
    width: 1280
    height: 720
    // Make the root item fill the available space
    anchors.fill: parent

    GridLayout {
        id: gridLayout
        anchors.fill: parent

        SimulationView {
            id: simulationView
            anchors.fill: parent
            Layout.preferredHeight: 720
            Layout.preferredWidth: 1280
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }

    Item {
        id: __materialLibrary__
        x: 100
        y: 50
    }
}
