import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ItemDelegate {
    id: delegate

    required property string trackUrl
    required property string trackTitle
    required property string trackArtist
    required property string trackAlbum
    required property string trackAlbumArt
    required property int trackDuration
    required property int actualIndex

    property bool isCurrentlyPlaying: false

    implicitHeight: 64
    width: ListView.view.width

    highlighted: isCurrentlyPlaying

    contentItem: RowLayout {
        spacing: 12

        AlbumCover {
            source: trackAlbumArt
            implicitWidth: 48
            implicitHeight: 48
            radius: 4
        }

        ColumnLayout {
            spacing: 2
            Layout.fillWidth: true

            Text {
                text: trackTitle || qsTr("Unknown Track")
                font.weight: isCurrentlyPlaying ? Font.Bold : Font.Normal
                elide: Text.ElideRight
                color: isCurrentlyPlaying ? "#1DB954" : "#fff"
                Layout.fillWidth: true
            }

            Text {
                text: trackArtist || qsTr("Unknown Artist")
                font.pixelSize: 12
                color: "#999"
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
        }

        Text {
            text: {
                if (trackDuration <= 0) return "--:--"
                var m = Math.floor(trackDuration / 60000)
                var s = Math.floor((trackDuration % 60000) / 1000)
                return m + ":" + (s < 10 ? "0" : "") + s
            }
            font.pixelSize: 12
            color: "#666"
        }
    }

    onClicked: delegate.ListView.view.currentIndex = actualIndex
}
