import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../"

Rectangle {
    id: toolbar
    width: parent.width
    height: 50
    color: "#333"

    signal openClicked()
    signal playClicked()
    signal pauseClicked()
    signal saveClicked()

    RowLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 5

        // Open Button
        ToolButton {
            icon.source: "qrc:/qml/assets/icons/folder-open.svg"
            icon.color: "white"
            width: 24 ; height: 24
            Layout.alignment: Qt.AlignLeft
            onClicked: openClicked()
        }

        // Save Button
        ToolButton {
            icon.source: "qrc:/qml/assets/icons/save.svg"
            icon.color: "white"
            width: 24 ; height: 24
            Layout.alignment: Qt.AlignLeft
            onClicked: playClicked()
        }



        // // Pause Button
        // ToolButton {
        //     icon.source: "qrc:/icons/pause.png"
        //     onClicked: pauseClicked()
        // }

        // // Save Button
        // ToolButton {
        //     icon.source: "qrc:/icons/save.png"
        //     onClicked: saveClicked()
        // }
        Item {
            Layout.fillWidth: true
        }
    }

}
