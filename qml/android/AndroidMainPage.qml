import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import mplayer

Page {
    id: root

    property bool isPlayerExpanded: false

    function openFolder() {
        folderDialog.open()
    }

    FolderDialog {
        id: folderDialog
        title: qsTr("Select Music Folder")
        onAccepted: {
            if (selectedFolder) {
                appController.openFolder(selectedFolder)
            }
        }
    }

    NavigationDrawer {
        id: drawer
        onViewSelected: function(view) {
            if (view === qsTr("Now Playing")) {
                isPlayerExpanded = true
            } else {
                isPlayerExpanded = false
            }
        }
    }

    header: ToolBar {
        Material.elevation: 2
        height: 56

        RowLayout {
            anchors.fill: parent
            spacing: 8

            ToolButton {
                text: "\u2630"
                font.pixelSize: 22
                onClicked: drawer.open()
            }

            Label {
                text: qsTr("Music Player")
                font.pixelSize: 20
                font.weight: Font.Bold
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            ToolButton {
                text: "\uD83D\uDCC2"
                font.pixelSize: 18
                onClicked: openFolder()
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Open folder")
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        PlaylistView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: appController.playlist
            currentTrackIndex: appController.playlist.currentIndex
            onTrackSelected: function(index) {
                appController.playTrack(index)
            }
        }
    }

    footer: Rectangle {
        height: isPlayerExpanded ? parent.height * 0.35 : 72
        color: "#1a1a1a"
        Behavior on height { NumberAnimation { duration: 250 } }

        clip: true

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            PlayerControls {
                Layout.fillWidth: true
                Layout.fillHeight: true
                playbackState: appController.audioEngine.playbackState
                position: appController.audioEngine.position
                duration: appController.audioEngine.duration
                volume: appController.audioEngine.volume
                trackTitle: appController.audioEngine.currentTitle
                trackArtist: appController.audioEngine.currentArtist
                albumArt: appController.audioEngine.currentAlbumArt

                onPlayPause: appController.togglePlayPause()
                onStop: appController.audioEngine.stop()
                onNext: appController.nextTrack()
                onPrevious: appController.previousTrack()
                onSeek: function(pos) { appController.audioEngine.position = pos }
            }
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
