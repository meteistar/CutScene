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
                for (var i = 0; i < drop.urls.length; ++i) {
                    mediaModel.addMedia(drop.urls[i].toString().replace("file:///", ""))
                }
            }
        }
    }

    ListView {
        id: mediaList
        anchors.fill: parent
        model: mediaModel

        delegate: Rectangle {
            width: parent.width
            height: 80
            radius: 8
            color: "#f0f0f0"
            border.color: "#cccccc"

            Row {
                spacing: 8
                anchors.verticalCenter: parent.verticalCenter
                // anchors.fill: parent
                anchors.margins: 8

                Image {
                    width: 60
                    height: 40
                    source: "image://media/" + id
                    fillMode: Image.PreserveAspectFit
                }

                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 2

                    Text { text: fileName; font.pixelSize: 14; font.bold: true }
                    Text { text: "Duration: " + duration; font.pixelSize: 12; color: "gray" }
                    Text { text: "Size: " + fileSize; font.pixelSize: 12; color: "gray" }
                }
            }
        }
    }

    Timeline {
        id: timeline
        width: parent.width
        y:100
        // anchors.bottom: parent.bottom
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
