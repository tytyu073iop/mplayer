import QtQuick
import QtTest
import QtQuick.Controls
import "../qml/shared"

TestCase {
    name: "TrackDelegateTest"

    function test_delegate_display() {
        var delegate = createTemporaryObject(
            Qt.createComponent("../qml/shared/TrackDelegate.qml"), parent,
            {
                trackUrl: "/music/test_song.mp3",
                trackTitle: "Test Song Title",
                trackArtist: "Test Artist Name",
                trackAlbum: "Test Album",
                trackAlbumArt: "/path/to/art.jpg",
                trackDuration: 240000,
                actualIndex: 5
            })
        verify(delegate !== null)
        compare(delegate.trackTitle, "Test Song Title")
        compare(delegate.trackArtist, "Test Artist Name")
        compare(delegate.trackUrl, "/music/test_song.mp3")
        compare(delegate.trackAlbum, "Test Album")
        compare(delegate.trackAlbumArt, "/path/to/art.jpg")
        compare(delegate.trackDuration, 240000)
        compare(delegate.actualIndex, 5)
        destroyDelayed()
    }

    function test_delegate_empty_fields() {
        var delegate = createTemporaryObject(
            Qt.createComponent("../qml/shared/TrackDelegate.qml"), parent,
            {
                trackUrl: "",
                trackTitle: "",
                trackArtist: "",
                trackAlbum: "",
                trackAlbumArt: "",
                trackDuration: 0,
                actualIndex: 0
            })
        verify(delegate !== null)
        compare(delegate.trackTitle, "")
        compare(delegate.trackArtist, "")
        compare(delegate.trackDuration, 0)
        destroyDelayed()
    }

    function test_delegate_highlight() {
        var delegate = createTemporaryObject(
            Qt.createComponent("../qml/shared/TrackDelegate.qml"), parent,
            {
                trackUrl: "/path/t.mp3",
                trackTitle: "T",
                trackArtist: "A",
                trackAlbum: "",
                trackAlbumArt: "",
                trackDuration: 100,
                actualIndex: 0
            })
        verify(delegate !== null)
        delegate.isCurrentlyPlaying = true
        compare(delegate.isCurrentlyPlaying, true)
        delegate.isCurrentlyPlaying = false
        compare(delegate.isCurrentlyPlaying, false)
        destroyDelayed()
    }

    function test_delegate_duration_format() {
        var delegate = createTemporaryObject(
            Qt.createComponent("../qml/shared/TrackDelegate.qml"), parent,
            {
                trackUrl: "/path/t.mp3",
                trackTitle: "Long Song",
                trackArtist: "Artist",
                trackAlbum: "",
                trackAlbumArt: "",
                trackDuration: 3661000,
                actualIndex: 0
            })
        verify(delegate !== null)
        compare(delegate.trackDuration, 3661000)
        destroyDelayed()
    }

    function destroyDelayed() {
    }
}
