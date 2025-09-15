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

    DropArea {
        anchors.fill: parent
        onDropped: {
            if (drop.hasUrls) {
                let filePath = drop.urls[0].toString().replace("file:///", "")
                console.log("Dropped file:", filePath)
                videoThumbnailLoader.loadFile(filePath)
            }
        }
    }

    Column {
            anchors.centerIn: parent
            spacing: 10

            Rectangle {
                id: previewBox
                width: 300
                height: 200
                color: "#444"
                radius: 8
                border.color: "#888"

                Image {
                    id: thumbnail
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    id: durationText
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    color: "white"
                    font.pixelSize: 14
                }
            }
        }

    Connections {
        target: videoThumbnailLoader
        function onThumbnailReady(image, duration) {
            console.log('temp ' + image)
            thumbnail.source = "file:///" + image.replace("\\","/")
            durationText.text = duration
        }
    }
}
