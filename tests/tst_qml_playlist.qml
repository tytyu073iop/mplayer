import QtQuick
import QtTest
import QtQuick.Controls
import "../qml/shared"

TestCase {
    name: "PlaylistViewTest"

    function test_playlistview_exists() {
        var view = createTemporaryObject(
            Qt.createComponent("../qml/shared/PlaylistView.qml"), parent)
        verify(view !== null, "PlaylistView component should be created")
        verify(view.count !== undefined, "count property should exist")
        compare(view.count, 0, "Default count should be 0")
        verify(view.trackSelected !== undefined, "trackSelected signal should exist")
        destroyDelayed()
    }

    function test_playlistview_properties() {
        var view = createTemporaryObject(
            Qt.createComponent("../qml/shared/PlaylistView.qml"), parent)
        verify(view !== null)
        compare(view.currentTrackIndex, -1, "Default currentTrackIndex should be -1")
        destroyDelayed()
    }

    function test_track_delegate_click() {
        var delegate = createTemporaryObject(
            Qt.createComponent("../qml/shared/TrackDelegate.qml"), parent,
            {
                trackUrl: "/test/track.mp3",
                trackTitle: "Click Test",
                trackArtist: "Artist",
                trackAlbum: "Album",
                trackAlbumArt: "",
                trackDuration: 100000,
                actualIndex: 2
            })
        verify(delegate !== null)
        compare(delegate.actualIndex, 2)
        compare(delegate.trackTitle, "Click Test")
        delegate.clicked()
        destroyDelayed()
    }

    function destroyDelayed() {
    }
}
