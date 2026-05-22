import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

ApplicationWindow {
    id: root

    title: qsTr("Music Player")
    visible: true

    minimumWidth: 800
    minimumHeight: 500

    function openFolder() {
        folderDialog.open()
    }

    FolderDialog {
        id: folderDialog
        title: qsTr("Select Music Folder")
        currentFolder: StandardPaths.standardLocations(StandardPaths.MusicLocation)[0] || ""
        onAccepted: {
            if (selectedFolder)
                appController.openFolder(selectedFolder)
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Open Folder...")
                onTriggered: root.openFolder()
                Shortcut {
                    sequence: StandardKey.Open
                    onActivated: root.openFolder()
                }
            }
            MenuSeparator {}
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit()
                Shortcut {
                    sequence: StandardKey.Quit
                    onActivated: Qt.quit()
                }
            }
        }

        Menu {
            title: qsTr("Help")
            MenuItem {
                text: qsTr("About Music Player")
                onTriggered: aboutDialog.open()
            }
        }
    }

    Dialog {
        id: aboutDialog
        title: qsTr("About")
        standardButtons: Dialog.Ok
        x: (parent.width - width) / 2
        y: (parent.height - height) / 3
        width: 300

        ColumnLayout {
            spacing: 8
            Text { text: qsTr("Music Player"); font.pixelSize: 18; font.weight: Font.Bold }
            Text { text: qsTr("Version 0.1"); color: "#666" }
            Text { text: qsTr("A cross-platform media player built with Qt 6 and QML."); wrapMode: Text.WordWrap }
        }
    }

    DropArea {
        id: dropArea
        anchors.fill: parent

        onDropped: function(drop) {
            if (drop.urls.length > 0) {
                var path = drop.urls[0]
                if (path.toString().startsWith("file://"))
                    path = path.toString().substring(7)
                appController.openFolder(path)
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ToolBar {
            Layout.fillWidth: true
            height: 48

            RowLayout {
                anchors.fill: parent
                spacing: 4

                Action {
                    id: openAction
                    icon.name: "document-open"
                    text: qsTr("Open Folder")
                    shortcut: StandardKey.Open
                    onTriggered: root.openFolder()
                }

                ToolButton { action: openAction }

                Item { Layout.fillWidth: true }

                ToolButton {
                    text: "\u23EE"
                    font.pixelSize: 18
                    onClicked: appController.previousTrack()
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Previous")
                }

                ToolButton {
                    id: playPauseBtn
                    text: appController.audioEngine.playbackState === 1 ? "\u23F8" : "\u25B6"
                    font.pixelSize: 22
                    implicitWidth: 40
                    implicitHeight: 40
                    onClicked: appController.togglePlayPause()
                    ToolTip.visible: hovered
                    ToolTip.text: appController.audioEngine.playbackState === 1 ? qsTr("Pause") : qsTr("Play")
                }

                ToolButton {
                    text: "\u23ED"
                    font.pixelSize: 18
                    onClicked: appController.nextTrack()
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Next")
                }

                Text {
                    text: appController.audioEngine.currentTitle || qsTr("No track")
                    color: "#fff"
                    font.pixelSize: 13
                    elide: Text.ElideRight
                    Layout.maximumWidth: 200
                }

                Text {
                    text: appController.audioEngine.currentArtist || ""
                    color: "#999"
                    font.pixelSize: 12
                    elide: Text.ElideRight
                    Layout.maximumWidth: 150
                }

                Slider {
                    id: volumeSlider
                    from: 0
                    to: 1
                    value: appController.audioEngine.volume
                    implicitWidth: 80
                    onMoved: appController.audioEngine.volume = value
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Volume")
                }
            }
        }

        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            PlaylistView {
                id: playlistView
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                SplitView.minimumWidth: 250
                model: appController.playlist
                currentTrackIndex: appController.playlist.currentIndex
                onTrackSelected: function(index) {
                    appController.playTrack(index)
                }
            }

            Rectangle {
                SplitView.minimumWidth: 300
                SplitView.preferredWidth: parent.width * 0.45
                color: "#1a1a1a"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 24
                    spacing: 16

                    Item { Layout.fillHeight: true }

                    AlbumCover {
                        id: albumCover
                        source: appController.audioEngine.currentAlbumArt
                        implicitWidth: Math.min(parent.width, 300)
                        implicitHeight: implicitWidth
                        radius: 8
                        Layout.alignment: Qt.AlignHCenter

                        OpacityAnimator {
                            target: albumCover
                            from: 0.0
                            to: 1.0
                            duration: 500
                            running: appController.audioEngine.currentAlbumArt !== ""
                        }
                    }

                    Text {
                        text: appController.audioEngine.currentTitle || qsTr("No track selected")
                        font.pixelSize: 20
                        font.weight: Font.Bold
                        color: "#fff"
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        text: appController.audioEngine.currentArtist || ""
                        font.pixelSize: 14
                        color: "#999"
                        Layout.fillWidth: true
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Slider {
                        id: seekSlider
                        Layout.fillWidth: true
                        from: 0
                        to: Math.max(1, appController.audioEngine.duration)
                        value: appController.audioEngine.position
                        onMoved: appController.audioEngine.position = value
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 40
                        spacing: 16
                        Layout.alignment: Qt.AlignHCenter

                        ToolButton {
                            text: "\u23EE"
                            font.pixelSize: 24
                            onClicked: appController.previousTrack()
                        }

                        ToolButton {
                            id: playBtn
                            text: appController.audioEngine.playbackState === 1 ? "\u23F8" : "\u25B6"
                            font.pixelSize: 32
                            implicitWidth: 56
                            implicitHeight: 56
                            onClicked: appController.togglePlayPause()

                            background: Rectangle {
                                radius: 28
                                color: "#1DB954"
                            }

                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: "#fff"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        ToolButton {
                            text: "\u23ED"
                            font.pixelSize: 24
                            onClicked: appController.nextTrack()
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 4
            color: "#1DB954"
            visible: appController.audioEngine.playbackState === 1
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
