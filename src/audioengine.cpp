#include "audioengine.h"
#include <QUrl>
#include <QImage>
#include <QMediaMetaData>
#include <QStandardPaths>

AudioEngine::AudioEngine(QObject *parent)
    : QObject(parent)
    , m_player(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
{
    m_player->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.5);

    connect(m_player, &QMediaPlayer::positionChanged, this, &AudioEngine::positionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &AudioEngine::durationChanged);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        PlaybackState s = Stopped;
        if (state == QMediaPlayer::PlayingState) s = Playing;
        else if (state == QMediaPlayer::PausedState) s = Paused;
        emit playbackStateChanged(s);
    });
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &AudioEngine::onMediaStatusChanged);
    connect(m_player, &QMediaPlayer::errorOccurred, this, &AudioEngine::onError);
    connect(m_player, &QMediaPlayer::metaDataChanged, this, &AudioEngine::updateMetadata);
}

QString AudioEngine::currentTitle() const { return m_currentTitle; }

QString AudioEngine::currentArtist() const { return m_currentArtist; }

QString AudioEngine::currentAlbum() const { return m_currentAlbum; }

QString AudioEngine::currentAlbumArt() const { return m_currentAlbumArt; }

qint64 AudioEngine::position() const { return m_player->position(); }

qint64 AudioEngine::duration() const { return m_player->duration(); }

qreal AudioEngine::volume() const { return m_audioOutput->volume(); }

AudioEngine::PlaybackState AudioEngine::playbackState() const
{
    auto state = m_player->playbackState();
    if (state == QMediaPlayer::PlayingState) return Playing;
    if (state == QMediaPlayer::PausedState) return Paused;
    return Stopped;
}

QString AudioEngine::currentSource() const
{
    return m_player->source().toString();
}

void AudioEngine::play() { m_player->play(); }

void AudioEngine::pause() { m_player->pause(); }

void AudioEngine::stop() { m_player->stop(); }

void AudioEngine::setPosition(qint64 position) { m_player->setPosition(position); }

void AudioEngine::setVolume(qreal volume)
{
    m_audioOutput->setVolume(volume);
    emit volumeChanged(volume);
}

void AudioEngine::setCurrentSource(const QString &source)
{
    if (source != currentSource()) {
        m_player->setSource(QUrl::fromLocalFile(source));
        emit currentSourceChanged(source);
    }
}

void AudioEngine::seekForward(qint64 ms)
{
    qint64 newPos = qMin(position() + ms, duration());
    setPosition(newPos);
}

void AudioEngine::seekBackward(qint64 ms)
{
    qint64 newPos = qMax(position() - ms, 0LL);
    setPosition(newPos);
}

void AudioEngine::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia) {
        updateMetadata();
    } else if (status == QMediaPlayer::EndOfMedia) {
        stop();
    }
}

void AudioEngine::onError(QMediaPlayer::Error error, const QString &errorString)
{
    Q_UNUSED(error)
    qWarning() << "AudioEngine error:" << errorString;
    emit errorOccurred(errorString);
}

void AudioEngine::updateMetadata()
{
    auto meta = m_player->metaData();
    m_currentTitle = meta.value(QMediaMetaData::Title).toString();
    m_currentArtist = meta.value(QMediaMetaData::Author).toString();
    if (m_currentArtist.isEmpty())
        m_currentArtist = meta.value(QMediaMetaData::ContributingArtist).toString();
    m_currentAlbum = meta.value(QMediaMetaData::AlbumTitle).toString();

    auto cover = meta.value(QMediaMetaData::CoverArtImage);
    if (cover.isValid()) {
        QImage img = cover.value<QImage>();
        if (!img.isNull()) {
            QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
                           + "/album_cover.jpg";
            img.save(path);
            m_currentAlbumArt = path;
        }
    } else {
        m_currentAlbumArt.clear();
    }

    emit currentTrackChanged();
}
