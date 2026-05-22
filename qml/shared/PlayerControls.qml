import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property int playbackState: 0
    property real position: 0
    property real duration: 0
    property real volume: 0.5
    property string trackTitle: ""
    property string trackArtist: ""
    property string albumArt: ""

    signal playPause()
    signal stop()
    signal next()
    signal previous()
    signal seek(real position)

    implicitHeight: 160

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 16
            spacing: 12

            AlbumCover {
                source: root.albumArt
                implicitWidth: 56
                implicitHeight: 56
                radius: 6

                ScaleAnimator {
                    target: parent
                    from: 0.8
                    to: 1.0
                    duration: 300
                    running: root.albumArt !== ""
                    easing.type: Easing.OutBack
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2

                Text {
                    text: root.trackTitle || qsTr("No track selected")
                    font.weight: Font.Bold
                    elide: Text.ElideRight
                    color: "#fff"
                    Layout.fillWidth: true
                }

                Text {
                    text: root.trackArtist || ""
                    font.pixelSize: 12
                    color: "#999"
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
            }
        }

        Slider {
            id: seekSlider
            Layout.fillWidth: true
            Layout.margins: 16
            Layout.topMargin: 0
            from: 0
            to: Math.max(1, root.duration)
            value: root.position
            enabled: root.duration > 0

            onMoved: root.seek(value)

            background: Rectangle {
                implicitHeight: 4
                radius: 2
                color: "#333"

                Rectangle {
                    width: seekSlider.visualPosition * parent.width
                    height: parent.height
                    radius: 2
                    color: "#1DB954"
                }
            }

            handle: Rectangle {
                x: seekSlider.leftPadding + seekSlider.visualPosition * (seekSlider.availableWidth - width)
                y: seekSlider.topPadding + seekSlider.availableHeight / 2 - height / 2
                width: 16
                height: 16
                radius: 8
                color: "#1DB954"
                visible: seekSlider.pressed || seekSlider.hovered
                Behavior on visible { NumberAnimation { duration: 100 } }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 16
            Layout.topMargin: 0
            spacing: 2

            Text {
                text: {
                    if (root.duration <= 0) return "--:--"
                    var totalSec = Math.floor(root.position / 1000)
                    var m = Math.floor(totalSec / 60)
                    var s = totalSec % 60
                    return m + ":" + (s < 10 ? "0" : "") + s
                }
                font.pixelSize: 11
                color: "#666"
            }

            Item { Layout.fillWidth: true }

            Text {
                text: {
                    if (root.duration <= 0) return "--:--"
                    var totalSec = Math.floor(root.duration / 1000)
                    var m = Math.floor(totalSec / 60)
                    var s = totalSec % 60
                    return m + ":" + (s < 10 ? "0" : "") + s
                }
                font.pixelSize: 11
                color: "#666"
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 16
            Layout.topMargin: 0
            spacing: 8

            Item { Layout.fillWidth: true }

            ToolButton {
                icon.source: "qrc:/qt/qml/mplayer/icons/skip-back.svg"
                text: "\u23EE"
                font.pixelSize: 20
                onClicked: root.previous()
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Previous")
            }

            ToolButton {
                id: playBtn
                text: root.playbackState === 1 ? "\u23F8" : "\u25B6"
                font.pixelSize: 28
                implicitWidth: 56
                implicitHeight: 56
                onClicked: root.playPause()

                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: "#fff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    radius: 28
                    color: playBtn.hovered ? "#1ed760" : "#1DB954"
                    Behavior on color { ColorAnimation { duration: 150 } }
                }

                ScaleAnimator {
                    target: playBtn
                    from: 0.9
                    to: 1.0
                    duration: 150
                    running: playBtn.pressed
                }
            }

            ToolButton {
                icon.source: "qrc:/qt/qml/mplayer/icons/skip-forward.svg"
                text: "\u23ED"
                font.pixelSize: 20
                onClicked: root.next()
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Next")
            }

            Item { Layout.fillWidth: true }
        }
    }
}
