import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Page {
    id: root

    property int currentTab: 0

    function openFolder() {
        folderDialog.open()
    }

    FolderDialog {
        id: folderDialog
        title: qsTr("Select Music Folder")
        onAccepted: {
            if (selectedFolder)
                appController.openFolder(selectedFolder)
        }
    }

    header: ToolBar {
        height: 50
        background: Rectangle { color: "#1a1a1a" }

        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: "\uD83D\uDCC2"
                font.pixelSize: 18
                onClicked: openFolder()
            }

            Label {
                text: qsTr("Music Player")
                font.pixelSize: 18
                font.weight: Font.Bold
                color: "#fff"
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Item { width: 44 }
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: currentTab
        interactive: true
        clip: true

        onCurrentIndexChanged: currentTab = currentIndex

        Page {
            background: Rectangle { color: "#121212" }

            PlaylistView {
                anchors.fill: parent
                model: appController.playlist
                currentTrackIndex: appController.playlist.currentIndex
                onTrackSelected: function(index) {
                    appController.playTrack(index)
                }
            }
        }

        NowPlayingView {
            trackTitle: appController.audioEngine.currentTitle
            trackArtist: appController.audioEngine.currentArtist
            trackAlbum: appController.audioEngine.currentAlbum
            albumArt: appController.audioEngine.currentAlbumArt
            playbackState: appController.audioEngine.playbackState
            position: appController.audioEngine.position
            duration: appController.audioEngine.duration

            onPlayPause: appController.togglePlayPause()
            onNext: appController.nextTrack()
            onPrevious: appController.previousTrack()
            onSeek: function(pos) { appController.audioEngine.position = pos }
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        background: Rectangle { color: "#1a1a1a" }

        TabButton {
            text: qsTr("Tracks")
            font.pixelSize: 12
            contentItem: Text {
                text: parent.text
                color: parent.checked ? "#1DB954" : "#666"
                font: parent.font
                horizontalAlignment: Text.AlignHCenter
            }
            background: Item {}
        }

        TabButton {
            text: qsTr("Now Playing")
            font.pixelSize: 12
            contentItem: Text {
                text: parent.text
                color: parent.checked ? "#1DB954" : "#666"
                font: parent.font
                horizontalAlignment: Text.AlignHCenter
            }
            background: Item {}
        }
    }

    ErrorDialog {
        id: errorDialog
    }

    Connections {
        target: appController.fileBrowser
        function onScanError(message) {
            errorDialog.errorMessage = message
            errorDialog.open()
        }
    }

    Connections {
        target: appController.audioEngine
        function onErrorOccurred(message) {
            errorDialog.errorMessage = message
            errorDialog.open()
        }
    }
}
