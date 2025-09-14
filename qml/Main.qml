import QtQuick
import "components"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("CutScene")

    ToolBar {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        onOpenClicked: console.log("Open clicked")
        onPlayClicked: console.log("Save clicked")
    }
}
