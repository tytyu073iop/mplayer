import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Page {
    id: root

    property string trackTitle: ""
    property string trackArtist: ""
    property string trackAlbum: ""
    property string albumArt: ""
    property int playbackState: 0
    property real position: 0
    property real duration: 0

    signal playPause()
    signal next()
    signal previous()
    signal seek(real pos)

    background: Rectangle { color: "#1a1a1a" }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Item { Layout.fillHeight: true }

        AlbumCover {
            id: albumCover
            source: root.albumArt
            implicitWidth: Math.min(root.width, 400) * 0.8
            implicitHeight: implicitWidth
            radius: 12
            Layout.alignment: Qt.AlignHCenter

            OpacityAnimator {
                target: albumCover
                from: 0.0
                to: 1.0
                duration: 400
                running: root.albumArt !== ""
            }
        }

        Item { Layout.fillHeight: true }

        Text {
            text: root.trackTitle || qsTr("No Track Selected")
            font.pixelSize: 22
            font.weight: Font.Bold
            color: "#fff"
            elide: Text.ElideRight
            Layout.fillWidth: true
            Layout.leftMargin: 24
            Layout.rightMargin: 24
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            text: root.trackArtist || ""
            font.pixelSize: 16
            color: "#999"
            elide: Text.ElideRight
            Layout.fillWidth: true
            Layout.leftMargin: 24
            Layout.rightMargin: 24
            Layout.topMargin: 4
            horizontalAlignment: Text.AlignHCenter
        }

        Item { Layout.preferredHeight: 32 }

        Slider {
            id: seekSlider
            Layout.fillWidth: true
            Layout.leftMargin: 24
            Layout.rightMargin: 24
            from: 0
            to: Math.max(1, root.duration)
            value: root.position

            onMoved: root.seek(value)

            background: Rectangle {
                implicitHeight: 4
                radius: 2
                color: "#333"
                Rectangle {
                    width: seekSlider.visualPosition * parent.width
                    height: parent.height
                    radius: 2
                    color: "#fff"
                }
            }

            handle: Rectangle {
                x: seekSlider.leftPadding + seekSlider.visualPosition * (seekSlider.availableWidth - width)
                y: seekSlider.topPadding + seekSlider.availableHeight / 2 - height / 2
                width: 12
                height: 12
                radius: 6
                color: "#fff"
                visible: seekSlider.pressed || seekSlider.hovered
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 24
            Layout.rightMargin: 24
            Layout.topMargin: 4

            Text {
                text: {
                    if (root.duration <= 0) return "--:--"
                    var s = Math.floor(root.position / 1000)
                    var m = Math.floor(s / 60)
                    return m + ":" + (s % 60 < 10 ? "0" : "") + (s % 60)
                }
                font.pixelSize: 11
                color: "#666"
            }

            Item { Layout.fillWidth: true }

            Text {
                text: {
                    if (root.duration <= 0) return "--:--"
                    var s = Math.floor(root.duration / 1000)
                    var m = Math.floor(s / 60)
                    return m + ":" + (s % 60 < 10 ? "0" : "") + (s % 60)
                }
                font.pixelSize: 11
                color: "#666"
            }
        }

        Item { Layout.preferredHeight: 24 }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 64
            spacing: 32
            Layout.alignment: Qt.AlignHCenter

            ToolButton {
                text: "\u23EE"
                font.pixelSize: 28
                onClicked: root.previous()
                contentItem: Text {
                    text: parent.text
                    color: "#fff"
                    font: parent.font
                }
            }

            ToolButton {
                id: playBtn
                text: root.playbackState === 1 ? "\u23F8" : "\u25B6"
                font.pixelSize: 40
                implicitWidth: 72
                implicitHeight: 72
                onClicked: root.playPause()

                contentItem: Text {
                    text: parent.text
                    color: "#fff"
                    font: parent.font
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    radius: 36
                    color: "#1DB954"
                }

                ScaleAnimator {
                    target: playBtn
                    from: 0.85
                    to: 1.0
                    duration: 100
                    running: playBtn.down
                }
            }

            ToolButton {
                text: "\u23ED"
                font.pixelSize: 28
                onClicked: root.next()
                contentItem: Text {
                    text: parent.text
                    color: "#fff"
                    font: parent.font
                }
            }
        }

        Item { Layout.preferredHeight: 48 }
    }

    MouseArea {
        id: swipeArea
        anchors.fill: parent
        property real startX: 0

        onPressed: function(mouse) {
            startX = mouse.x
        }

        onReleased: function(mouse) {
            var dx = mouse.x - startX
            if (Math.abs(dx) > 80) {
                if (dx < 0) root.next()
                else root.previous()
            }
        }
    }
}
