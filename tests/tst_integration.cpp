#include <QtTest>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QFile>
#include <QDir>
#include "audioengine.h"
#include "playlistmanager.h"
#include "filebrowser.h"
#include "databasemanager.h"

class TestIntegration : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir *m_tempDir = nullptr;

    void createTestFile(const QString &name)
    {
        QFile file(m_tempDir->filePath(name));
        if (file.open(QIODevice::WriteOnly))
            file.write("fake audio data");
    }

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testFileScanToPlayback();
    void testDatabasePersistence();
    void testPlaylistNavigation();
    void testFileBrowserToPlaylistIntegration();
    void testAudioEngineStateTransitions();
};

void TestIntegration::initTestCase()
{
    m_tempDir = new QTemporaryDir();
    QVERIFY(m_tempDir->isValid());

    createTestFile("track01.mp3");
    createTestFile("track02.flac");
    createTestFile("track03.wav");
    createTestFile("track04.ogg");
    createTestFile("readme.txt");
}

void TestIntegration::cleanupTestCase()
{
    delete m_tempDir;
}

void TestIntegration::testFileScanToPlayback()
{
    FileBrowser fb;
    PlaylistManager pm;

    QSignalSpy scanSpy(&fb, &FileBrowser::scanFinished);

    fb.scanDirectory(m_tempDir->path());

    QCOMPARE(scanSpy.count( ), 1);
    auto tracks = scanSpy.at(0).at(0).value<QVector<TrackInfo>>();
    QCOMPARE(tracks.size(), 4);

    pm.setTracks(tracks);
    QCOMPARE(pm.rowCount(), 4);
    QCOMPARE(pm.currentIndex(), 0);
}

void TestIntegration::testDatabasePersistence()
{
    DatabaseManager db;

    TrackInfo t1, t2;
    t1.url = "/db/test1.mp3";
    t1.title = "DB Track 1";
    t1.artist = "Artist 1";
    t1.album = "";

    t2.url = "/db/test2.mp3";
    t2.title = "DB Track 2";
    t2.artist = "Artist 2";
    t2.album = "Album 2";

    QVERIFY(db.saveTrack(t1));
    QVERIFY(db.saveTrack(t2));

    auto loaded = db.loadAllTracks();
    QVERIFY(loaded.size() >= 2);

    int found = 0;
    for (const auto &t : loaded) {
        if (t.url == t1.url) found++;
        if (t.url == t2.url) found++;
    }
    QVERIFY(found >= 2);
}

void TestIntegration::testPlaylistNavigation()
{
    PlaylistManager pm;
    pm.addTrack("/nav/1.mp3", "Track 1", "A", "", "", 100);
    pm.addTrack("/nav/2.mp3", "Track 2", "B", "", "", 200);
    pm.addTrack("/nav/3.mp3", "Track 3", "C", "", "", 300);

    pm.setCurrentIndex(0);
    QCOMPARE(pm.currentTrack().title, "Track 1");

    pm.setCurrentIndex(1);
    QCOMPARE(pm.currentTrack().title, "Track 2");

    pm.removeTrack(0);
    QCOMPARE(pm.rowCount(), 2);

    pm.shuffle();
    QCOMPARE(pm.rowCount(), 2);
}

void TestIntegration::testFileBrowserToPlaylistIntegration()
{
    FileBrowser fb;
    PlaylistManager pm;

    QSignalSpy scanSpy(&fb, &FileBrowser::scanFinished);
    fb.scanDirectory(m_tempDir->path());
    QCOMPARE(scanSpy.count(), 1);

    auto tracks = scanSpy.at(0).at(0).value<QVector<TrackInfo>>();
    pm.setTracks(tracks);

    QStringList urls = pm.allUrls();
    for (const auto &url : urls) {
        QVERIFY(fb.isValidAudioFile(url));
    }

    QCOMPARE(urls.size(), 4);
}

void TestIntegration::testAudioEngineStateTransitions()
{
    AudioEngine engine;

    QCOMPARE(engine.playbackState(), AudioEngine::Stopped);

    engine.setVolume(0.5);
    QCOMPARE(engine.volume(), 0.5);

    engine.setVolume(0.0);
    QCOMPARE(engine.volume(), 0.0);

    engine.setVolume(1.0);
    QCOMPARE(engine.volume(), 1.0);
}

QTEST_MAIN(TestIntegration)
#include "tst_integration.moc"
