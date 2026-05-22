#include <QtTest>
#include <QSignalSpy>
#include "audioengine.h"

class TestAudioEngine : public QObject
{
    Q_OBJECT

private slots:
    void testInitialState();
    void testVolume();
    void testPlayPauseStop();
    void testSeek();
    void testPlaybackStateEnum();
    void testSetCurrentSource();
};

void TestAudioEngine::testInitialState()
{
    AudioEngine engine;
    QCOMPARE(engine.playbackState(), AudioEngine::Stopped);
    QCOMPARE(engine.volume(), 0.5);
    QCOMPARE(engine.position(), 0);
    QCOMPARE(engine.duration(), 0);
    QVERIFY(engine.currentTitle().isEmpty());
    QVERIFY(engine.currentArtist().isEmpty());
}

void TestAudioEngine::testVolume()
{
    AudioEngine engine;
    QSignalSpy spy(&engine, &AudioEngine::volumeChanged);

    engine.setVolume(0.75);
    QCOMPARE(engine.volume(), 0.75);
    QCOMPARE(spy.count(), 1);

    engine.setVolume(0.0);
    QCOMPARE(engine.volume(), 0.0);
    QCOMPARE(spy.count(), 2);

    engine.setVolume(1.0);
    QCOMPARE(engine.volume(), 1.0);
    QCOMPARE(spy.count(), 3);
}

void TestAudioEngine::testPlayPauseStop()
{
    AudioEngine engine;

    QCOMPARE(engine.playbackState(), AudioEngine::Stopped);

    engine.play();
    QCOMPARE(engine.playbackState(), AudioEngine::Stopped); // no source set, stays stopped

    engine.pause();
    QCOMPARE(engine.playbackState(), AudioEngine::Stopped);

    engine.stop();
    QCOMPARE(engine.playbackState(), AudioEngine::Stopped);
}

void TestAudioEngine::testSeek()
{
    AudioEngine engine;

    engine.setPosition(5000);
    QCOMPARE(engine.position(), 0); // no source loaded, position stays 0

    engine.setPosition(-100);
    QCOMPARE(engine.position(), 0);
}

void TestAudioEngine::testPlaybackStateEnum()
{
    AudioEngine engine;
    QCOMPARE(static_cast<int>(AudioEngine::Stopped), 0);
    QCOMPARE(static_cast<int>(AudioEngine::Playing), 1);
    QCOMPARE(static_cast<int>(AudioEngine::Paused), 2);
    QCOMPARE(engine.playbackState(), AudioEngine::Stopped);
}

void TestAudioEngine::testSetCurrentSource()
{
    AudioEngine engine;
    QSignalSpy spy(&engine, &AudioEngine::currentSourceChanged);

    engine.setCurrentSource("/nonexistent/test.mp3");
    QCOMPARE(spy.count(), 1);
    QVERIFY(!engine.currentSource().isEmpty());
}

QTEST_MAIN(TestAudioEngine)
#include "tst_audioengine.moc"
