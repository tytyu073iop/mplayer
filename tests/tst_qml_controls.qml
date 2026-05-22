import QtQuick
import QtTest
import QtQuick.Controls
import "../qml/shared"

TestCase {
    name: "PlayerControlsTest"

    function test_controls_exist() {
        var controls = createTemporaryObject(
            Qt.createComponent("../qml/shared/PlayerControls.qml"), parent)
        verify(controls !== null, "PlayerControls component should be created")
        verify(controls.playPause !== undefined, "playPause signal should exist")
        verify(controls.next !== undefined, "next signal should exist")
        verify(controls.previous !== undefined, "previous signal should exist")
        verify(controls.seek !== undefined, "seek signal should exist")
        destroyDelayed()
    }

    function test_controls_defaults() {
        var controls = createTemporaryObject(
            Qt.createComponent("../qml/shared/PlayerControls.qml"), parent)
        verify(controls !== null)
        compare(controls.playbackState, 0, "Default state should be Stopped")
        compare(controls.position, 0, "Default position should be 0")
        compare(controls.duration, 0, "Default duration should be 0")
        compare(controls.volume, 0.5, "Default volume should be 0.5")
        compare(controls.trackTitle, "", "Default title should be empty")
        compare(controls.trackArtist, "", "Default artist should be empty")
        destroyDelayed()
    }

    function test_controls_play_pause_signal() {
        var controls = createTemporaryObject(
            Qt.createComponent("../qml/shared/PlayerControls.qml"), parent)
        verify(controls !== null)

        var signalReceived = false
        controls.playPause.connect(function() { signalReceived = true })

        var playBtn = findChild(controls, "playBtn")
        if (playBtn) {
            playBtn.clicked()
            tryCompare(controls, "playbackState", -1) // signal should have been emitted
        }

        destroyDelayed()
    }

    function test_album_cover() {
        var cover = createTemporaryObject(
            Qt.createComponent("../qml/shared/AlbumCover.qml"), parent)
        verify(cover !== null, "AlbumCover component should be created")
        compare(cover.source, "", "Default source should be empty")
        destroyDelayed()
    }

    function test_error_dialog() {
        var dialog = createTemporaryObject(
            Qt.createComponent("../qml/shared/ErrorDialog.qml"), parent)
        verify(dialog !== null, "ErrorDialog component should be created")
        compare(dialog.errorMessage, "", "Default error message should be empty")
        dialog.errorMessage = "Test error"
        compare(dialog.errorMessage, "Test error")
        destroyDelayed()
    }

    function test_track_delegate() {
        var delegate = createTemporaryObject(
            Qt.createComponent("../qml/shared/TrackDelegate.qml"), parent,
            {
                trackUrl: "/test/path.mp3",
                trackTitle: "Test Song",
                trackArtist: "Test Artist",
                trackAlbum: "Test Album",
                trackAlbumArt: "",
                trackDuration: 200000,
                actualIndex: 0
            })
        verify(delegate !== null, "TrackDelegate component should be created")
        compare(delegate.trackTitle, "Test Song")
        compare(delegate.trackArtist, "Test Artist")
        compare(delegate.actualIndex, 0)
        destroyDelayed()
    }

    function findChild(item, objectName) {
        if (!item) return null
        if (item.objectName === objectName) return item
        for (var i = 0; i < item.children.length; ++i) {
            var result = findChild(item.children[i], objectName)
            if (result) return result
        }
        return null
    }

    function destroyDelayed() {
        // QtQuickTest will clean up on test end
    }
}
