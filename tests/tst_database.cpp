#include <QtTest>
#include <QSignalSpy>
#include "databasemanager.h"

class TestDatabase : public QObject
{
    Q_OBJECT

private:
    DatabaseManager *m_db = nullptr;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testSaveAndLoadTrack();
    void testClearTracks();
    void testSavePlaylist();
    void testAddAndLoadHistory();
    void testLoadEmptyHistory();
};

void TestDatabase::initTestCase()
{
    m_db = new DatabaseManager(this);
}

void TestDatabase::cleanupTestCase()
{
    m_db->clearTracks();
}

void TestDatabase::testSaveAndLoadTrack()
{
    TrackInfo track;
    track.url = "/test/path/song.mp3";
    track.title = "Test Song";
    track.artist = "Test Artist";
    track.album = "Test Album";
    track.duration = 300000;

    QVERIFY(m_db->saveTrack(track));

    auto tracks = m_db->loadAllTracks();
    QVERIFY(tracks.size() > 0);

    bool found = false;
    for (const auto &t : tracks) {
        if (t.url == track.url) {
            found = true;
            QCOMPARE(t.title, track.title);
            QCOMPARE(t.artist, track.artist);
            QCOMPARE(t.album, track.album);
            break;
        }
    }
    QVERIFY(found);
}

void TestDatabase::testClearTracks()
{
    TrackInfo track;
    track.url = "/test/clear.mp3";
    track.title = "ToClear";
    m_db->saveTrack(track);

    QVERIFY(m_db->clearTracks());

    auto tracks = m_db->loadAllTracks();
    for (const auto &t : tracks)
        QVERIFY(t.url != "/test/clear.mp3");
}

void TestDatabase::testSavePlaylist()
{
    QStringList urls = {"/test/p1.mp3", "/test/p2.mp3", "/test/p3.mp3"};
    QVERIFY(m_db->savePlaylist("TestPlaylist", urls));
}

void TestDatabase::testAddAndLoadHistory()
{
    TrackInfo track;
    track.url = "/test/history.mp3";
    track.title = "History Track";
    track.artist = "History Artist";
    track.album = "";

    QVERIFY(m_db->addToHistory(track));

    auto history = m_db->loadHistory(10);
    QVERIFY(history.size() > 0);

    bool found = false;
    for (const auto &h : history) {
        if (h.url == track.url) {
            found = true;
            QCOMPARE(h.title, track.title);
            break;
        }
    }
    QVERIFY(found);
}

void TestDatabase::testLoadEmptyHistory()
{
    auto history = m_db->loadHistory(0);
    QCOMPARE(history.size(), 0);
}

QTEST_MAIN(TestDatabase)
#include "tst_database.moc"
