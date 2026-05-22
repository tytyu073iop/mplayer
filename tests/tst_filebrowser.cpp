#include <QtTest>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QFile>
#include "filebrowser.h"
#include "playlistmanager.h"

class TestFileBrowser : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir *m_tempDir = nullptr;

    void createTestFile(const QString &name)
    {
        QFile file(m_tempDir->filePath(name));
        if (file.open(QIODevice::WriteOnly))
            file.write("test");
    }

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testIsValidAudioFile();
    void testSupportedFormats();
    void testScanEmptyDirectory();
    void testScanDirectoryWithFiles();
    void testScanNonExistentDirectory();
    void testScanSubdirectories();
};

void TestFileBrowser::initTestCase()
{
    m_tempDir = new QTemporaryDir();
    QVERIFY(m_tempDir->isValid());
}

void TestFileBrowser::cleanupTestCase()
{
    delete m_tempDir;
}

void TestFileBrowser::testIsValidAudioFile()
{
    FileBrowser fb;
    QVERIFY(fb.isValidAudioFile("/path/to/file.mp3"));
    QVERIFY(fb.isValidAudioFile("/path/to/file.flac"));
    QVERIFY(fb.isValidAudioFile("/path/to/file.wav"));
    QVERIFY(fb.isValidAudioFile("/path/to/file.ogg"));
    QVERIFY(!fb.isValidAudioFile("/path/to/file.txt"));
    QVERIFY(!fb.isValidAudioFile("/path/to/file.jpg"));
    QVERIFY(!fb.isValidAudioFile("/path/to/file"));
}

void TestFileBrowser::testSupportedFormats()
{
    FileBrowser fb;
    auto formats = fb.supportedFormats();
    QVERIFY(formats.contains("mp3"));
    QVERIFY(formats.contains("flac"));
    QVERIFY(formats.contains("wav"));
    QVERIFY(formats.contains("ogg"));
    QVERIFY(!formats.contains("txt"));
}

void TestFileBrowser::testScanEmptyDirectory()
{
    FileBrowser fb;
    QSignalSpy errorSpy(&fb, &FileBrowser::scanError);

    fb.scanDirectory(m_tempDir->path());

    QCOMPARE(errorSpy.count(), 1);
    QString msg = errorSpy.at(0).at(0).toString();
    QVERIFY(msg.contains("No audio files found"));
}

void TestFileBrowser::testScanDirectoryWithFiles()
{
    createTestFile("song1.mp3");
    createTestFile("song2.flac");
    createTestFile("song3.wav");
    createTestFile("notes.txt");
    createTestFile("image.jpg");

    FileBrowser fb;
    QSignalSpy finishedSpy(&fb, &FileBrowser::scanFinished);

    fb.scanDirectory(m_tempDir->path());

    QCOMPARE(finishedSpy.count(), 1);
    auto tracks = finishedSpy.at(0).at(0).value<QVector<TrackInfo>>();
    QCOMPARE(tracks.size(), 3);

    QStringList titles;
    for (const auto &t : tracks)
        titles.append(t.title);
    QVERIFY(titles.contains("song1"));
    QVERIFY(titles.contains("song2"));
    QVERIFY(titles.contains("song3"));
    QVERIFY(!titles.contains("notes"));

    // clean up
    QDir dir(m_tempDir->path());
    dir.remove("song1.mp3");
    dir.remove("song2.flac");
    dir.remove("song3.wav");
    dir.remove("notes.txt");
    dir.remove("image.jpg");
}

void TestFileBrowser::testScanNonExistentDirectory()
{
    FileBrowser fb;
    QSignalSpy errorSpy(&fb, &FileBrowser::scanError);

    fb.scanDirectory("/nonexistent/path/that/does/not/exist");

    QCOMPARE(errorSpy.count(), 1);
    QString msg = errorSpy.at(0).at(0).toString();
    QVERIFY(msg.contains("does not exist"));
}

void TestFileBrowser::testScanSubdirectories()
{
    QTemporaryDir subDir;
    QVERIFY(subDir.isValid());
    QString subPath = subDir.path();

    QDir().mkpath(subPath + "/sub");
    QFile file(subPath + "/sub/deep.mp3");
    if (file.open(QIODevice::WriteOnly))
        file.write("test");

    FileBrowser fb;
    QSignalSpy finishedSpy(&fb, &FileBrowser::scanFinished);

    fb.scanDirectory(subPath);

    QCOMPARE(finishedSpy.count(), 1);
    auto tracks = finishedSpy.at(0).at(0).value<QVector<TrackInfo>>();
    QVERIFY(tracks.size() >= 1);

    QDir(subPath).remove("sub/deep.mp3");
    QDir(subPath).rmdir("sub");
}

QTEST_MAIN(TestFileBrowser)
#include "tst_filebrowser.moc"
