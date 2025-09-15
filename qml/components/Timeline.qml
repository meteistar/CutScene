import QtQuick
import QtQuick.Controls

Rectangle {
    id: timeline
    width: parent.width
    height: 30
    color: "#222222"

    property int totalSeconds: 10
    property int pixelsPerSecond: 160

    Canvas {
        id: ruler
        anchors.fill: parent

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            ctx.strokeStyle = "gray"
            ctx.fillStyle = "gray"
            ctx.lineWidth = 2

            for (var i = 0; i <= timeline.totalSeconds * 4; i++) {
                // her 0.25s
                var x = i * timeline.pixelsPerSecond / 4
                var lineHeight = 0
                if (i % 4 === 0) {
                    // 1 saniye → büyük
                    lineHeight = 30
                    ctx.fillText("00:00:" + String(i / 4).padStart(2, "0"),
                                 x + 2, 12)
                } else if (i % 2 === 0) {
                    // 0.5 saniye → orta
                    lineHeight = 20
                } else {
                    // 0.25 saniye → küçük
                    lineHeight = 10
                }

                ctx.beginPath()
                ctx.moveTo(x, height)
                ctx.lineTo(x, height - lineHeight)
                ctx.stroke()
            }
        }
    }

    // Altına kırmızı dummy video track
    Rectangle {
        id: dummyTrack
        anchors.top: ruler.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50
        color: "red"
        radius: 4
    }

    // Playhead / scrubber
    Rectangle {
        id: playhead
        width: 3
        height: 80 //parent.height
        color: "white"
        // anchors.left: parent.left
        // anchors.leftMargin: 5
        x: 0

        // MouseArea {
        //     anchors.fill: parent
        //     cursorShape: Qt.SizeHorCursor
        //     drag.target: parent
        //     drag.axis: Drag.XAxis
        //     drag.minimumX: 0
        //     drag.maximumX: timeline.width - playhead.width
        // }
    }

    Canvas {
        id: triangle
        width: 12
        height: 8
        anchors.bottom: playhead.top
        anchors.horizontalCenter: playhead.horizontalCenter

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            ctx.fillStyle = "black"
            ctx.beginPath()
            ctx.moveTo(0, 0) // sol üst
            ctx.lineTo(width, 0) // sağ üst
            ctx.lineTo(width / 2, height) // alt orta (tepesi aşağı)
            ctx.closePath()
            ctx.fill()
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.SizeHorCursor
        drag.target: playhead
        drag.axis: Drag.XAxis
        drag.minimumX: 0
        drag.maximumX: timeline.width - playhead.width
        onClicked: function(mouse) {
            playhead.x = Math.min(Math.max(mouse.x - playhead.width/2, 0), timeline.width - playhead.width)
        }
    }
}
