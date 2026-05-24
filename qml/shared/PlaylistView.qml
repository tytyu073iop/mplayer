import QtQuick
import QtQuick.Controls

ListView {
    id: root

    property int currentTrackIndex: -1

    signal trackSelected(int index)

    clip: true
    spacing: 1

    model: null

    Rectangle {
        anchors.fill: parent
        z: -1
        color: "#121212"
    }

    currentIndex: currentTrackIndex

    delegate: TrackDelegate {
        required property int index
        required property var model

        actualIndex: index
        trackUrl: model.trackUrl
        trackTitle: model.trackTitle
        trackArtist: model.trackArtist
        trackAlbum: model.trackAlbum
        trackAlbumArt: model.trackAlbumArt
        trackDuration: model.trackDuration
        isCurrentlyPlaying: ListView.isCurrentItem

        width: root.width

        onClicked: {
            root.currentIndex = actualIndex
            root.trackSelected(actualIndex)
        }
    }

    ScrollBar.vertical: ScrollBar {
        active: true
        policy: ScrollBar.AsNeeded
    }

    Text {
        anchors.centerIn: parent
        text: qsTr("No tracks found.\nOpen a folder to get started.")
        horizontalAlignment: Text.AlignHCenter
        color: "#888888"
        font.pixelSize: 14
        visible: root.count === 0
    }
}
