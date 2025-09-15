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

        delegate: Row {
            spacing: 8
            height: 80
            Image {
                width: 80; height: 60
                source: "image://media/" + id
                fillMode: Image.PreserveAspectFit
            }
            Column {
                Text { text: fileName; font.pixelSize: 14 }
                Text { text: duration; font.pixelSize: 12; color: "gray" }
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
