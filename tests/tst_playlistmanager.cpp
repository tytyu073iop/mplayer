#include <QtTest>
#include <QSignalSpy>
#include "playlistmanager.h"

class TestPlaylistManager : public QObject
{
    Q_OBJECT

private slots:
    void testInitialState();
    void testAddTrack();
    void testRemoveTrack();
    void testClear();
    void testShuffle();
    void testCurrentIndex();
    void testTrackAt();
    void testAllUrls();
};

void TestPlaylistManager::testInitialState()
{
    PlaylistManager pm;
    QCOMPARE(pm.rowCount(), 0);
    QCOMPARE(pm.currentIndex(), -1);
    QVERIFY(pm.currentTrack().url.isEmpty());
    QVERIFY(pm.allUrls().isEmpty());
}

void TestPlaylistManager::testAddTrack()
{
    PlaylistManager pm;
    QSignalSpy countSpy(&pm, &PlaylistManager::countChanged);

    pm.addTrack("/path/track1.mp3", "Song One", "Artist A", "Album X", "", 200000);
    QCOMPARE(pm.rowCount(), 1);
    QCOMPARE(countSpy.count(), 1);

    pm.addTrack("/path/track2.mp3", "Song Two", "Artist B", "Album Y", "", 180000);
    QCOMPARE(pm.rowCount(), 2);
    QCOMPARE(countSpy.count(), 2);

    auto track = pm.trackAt(0);
    QCOMPARE(track.title, "Song One");
    QCOMPARE(track.artist, "Artist A");
    QCOMPARE(track.url, "/path/track1.mp3");
    QCOMPARE(track.duration, 200000);
}

void TestPlaylistManager::testRemoveTrack()
{
    PlaylistManager pm;
    pm.addTrack("/path/1.mp3", "Track 1", "A", "", "", 100);
    pm.addTrack("/path/2.mp3", "Track 2", "B", "", "", 200);
    pm.addTrack("/path/3.mp3", "Track 3", "C", "", "", 300);

    pm.setCurrentIndex(1);
    pm.removeTrack(0);
    QCOMPARE(pm.rowCount(), 2);
    QCOMPARE(pm.trackAt(0).title, "Track 2");
}

void TestPlaylistManager::testClear()
{
    PlaylistManager pm;
    pm.addTrack("/path/1.mp3", "T1", "A", "", "", 100);
    pm.addTrack("/path/2.mp3", "T2", "B", "", "", 200);
    pm.setCurrentIndex(0);

    pm.clear();
    QCOMPARE(pm.rowCount(), 0);
    QCOMPARE(pm.currentIndex(), -1);
}

void TestPlaylistManager::testShuffle()
{
    PlaylistManager pm;
    pm.addTrack("/path/1.mp3", "T1", "A", "", "", 100);
    pm.addTrack("/path/2.mp3", "T2", "B", "", "", 200);
    pm.addTrack("/path/3.mp3", "T3", "C", "", "", 300);
    pm.addTrack("/path/4.mp3", "T4", "D", "", "", 400);

    QStringList before;
    for (int i = 0; i < pm.rowCount(); ++i)
        before.append(pm.trackAt(i).title);

    pm.shuffle();

    QStringList after;
    for (int i = 0; i < pm.rowCount(); ++i)
        after.append(pm.trackAt(i).title);

    QCOMPARE(after.size(), before.size());
    QVERIFY(after != before); // unlikely to be identical
}

void TestPlaylistManager::testTrackAt()
{
    PlaylistManager pm;
    pm.addTrack("/path/1.mp3", "Song One", "Artist A", "", "", 200);

    auto track = pm.trackAt(0);
    QCOMPARE(track.title, "Song One");
    QCOMPARE(track.artist, "Artist A");

    track = pm.trackAt(5);
    QVERIFY(track.url.isEmpty());

    track = pm.trackAt(-1);
    QVERIFY(track.url.isEmpty());
}

void TestPlaylistManager::testCurrentIndex()
{
    PlaylistManager pm;
    QSignalSpy spy(&pm, &PlaylistManager::currentIndexChanged);

    pm.addTrack("/path/1.mp3", "T1", "A", "", "", 100);
    pm.addTrack("/path/2.mp3", "T2", "B", "", "", 200);

    pm.setCurrentIndex(0);
    QCOMPARE(pm.currentIndex(), 0);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(pm.currentTrack().title, "T1");

    pm.setCurrentIndex(1);
    QCOMPARE(pm.currentIndex(), 1);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(pm.currentTrack().title, "T2");
}

void TestPlaylistManager::testAllUrls()
{
    PlaylistManager pm;
    pm.addTrack("/path/a.mp3", "A", "", "", "", 0);
    pm.addTrack("/path/b.mp3", "B", "", "", "", 0);

    auto urls = pm.allUrls();
    QCOMPARE(urls.size(), 2);
    QCOMPARE(urls[0], "/path/a.mp3");
    QCOMPARE(urls[1], "/path/b.mp3");
}

QTEST_MAIN(TestPlaylistManager)
#include "tst_playlistmanager.moc"
