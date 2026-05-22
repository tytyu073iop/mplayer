#include "appcontroller.h"
#include <QGuiApplication>
#include <QDir>
#include <QTranslator>
#include <QDebug>

AppController::AppController(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent)
    , m_audioEngine(new AudioEngine(this))
    , m_playlist(new PlaylistManager(this))
    , m_fileBrowser(new FileBrowser(this))
    , m_database(new DatabaseManager(this))
    , m_engine(engine)
{
#if defined(Q_OS_ANDROID)
    m_platform = QStringLiteral("android");
#elif defined(Q_OS_IOS)
    m_platform = QStringLiteral("ios");
#elif defined(Q_OS_WASM)
    m_platform = QStringLiteral("wasm");
#else
    m_platform = QStringLiteral("linux");
#endif

    connect(m_fileBrowser, &FileBrowser::scanFinished,
            this, &AppController::onTracksFound);
    connect(m_fileBrowser, &FileBrowser::scanError,
            this, [this](const QString &msg) {
                qWarning() << "FileBrowser error:" << msg;
                emit trackPlaybackStarted({});
            });
    connect(m_audioEngine, &AudioEngine::playbackStateChanged,
            this, [this](AudioEngine::PlaybackState state) {
                Q_UNUSED(state)
            });
    connect(m_audioEngine, &AudioEngine::errorOccurred,
            this, &AppController::onPlaybackError);
    connect(m_playlist, &PlaylistManager::currentIndexChanged,
            this, &AppController::onCurrentIndexChanged);
    connect(m_audioEngine, &AudioEngine::currentTrackChanged,
            this, [this]() {
                auto track = m_playlist->currentTrack();
                if (!track.url.isEmpty())
                    m_database->addToHistory(track);
            });
}

AudioEngine *AppController::audioEngine() const { return m_audioEngine; }
PlaylistManager *AppController::playlist() const { return m_playlist; }
FileBrowser *AppController::fileBrowser() const { return m_fileBrowser; }
DatabaseManager *AppController::database() const { return m_database; }
QString AppController::platform() const { return m_platform; }

void AppController::openFolder(const QString &path)
{
    m_playlist->clear();
    m_fileBrowser->scanDirectory(path);
}

void AppController::playTrack(int index)
{
    if (index < 0 || index >= m_playlist->rowCount()) return;
    m_playlist->setCurrentIndex(index);
    auto track = m_playlist->currentTrack();
    if (track.url.isEmpty()) return;
    m_audioEngine->setCurrentSource(track.url);
    m_audioEngine->play();
    emit trackPlaybackStarted(track);
}

void AppController::togglePlayPause()
{
    if (m_audioEngine->playbackState() == AudioEngine::Playing) {
        m_audioEngine->pause();
    } else {
        if (m_playlist->currentIndex() < 0 && m_playlist->rowCount() > 0) {
            playTrack(0);
        } else {
            m_audioEngine->play();
        }
    }
}

void AppController::nextTrack()
{
    int next = m_playlist->currentIndex() + 1;
    if (next < m_playlist->rowCount()) {
        playTrack(next);
    }
}

void AppController::previousTrack()
{
    int prev = m_playlist->currentIndex() - 1;
    if (prev >= 0) {
        playTrack(prev);
    }
}

void AppController::setLocale(const QString &locale)
{
    qWarning() << "Locale change requested:" << locale;
    emit localeChanged(locale);
}

void AppController::onTracksFound(const QVector<TrackInfo> &tracks)
{
    m_playlist->setTracks(tracks);
    emit folderLoaded(QString());
}

void AppController::onPlaybackError(const QString &message)
{
    qWarning() << "Playback error:" << message;
}

void AppController::onCurrentIndexChanged(int index)
{
    if (index >= 0 && index < m_playlist->rowCount()) {
        auto track = m_playlist->trackAt(index);
        if (!track.url.isEmpty() && track.url != m_audioEngine->currentSource()) {
            m_audioEngine->setCurrentSource(track.url);
        }
    }
}
