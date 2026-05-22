#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

class AudioEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentTitle READ currentTitle NOTIFY currentTrackChanged)
    Q_PROPERTY(QString currentArtist READ currentArtist NOTIFY currentTrackChanged)
    Q_PROPERTY(QString currentAlbum READ currentAlbum NOTIFY currentTrackChanged)
    Q_PROPERTY(QString currentAlbumArt READ currentAlbumArt NOTIFY currentTrackChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(qreal volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(PlaybackState playbackState READ playbackState NOTIFY playbackStateChanged)
    Q_PROPERTY(QString currentSource READ currentSource WRITE setCurrentSource NOTIFY currentSourceChanged)

public:
    enum PlaybackState { Stopped = 0, Playing = 1, Paused = 2 };
    Q_ENUM(PlaybackState)

    explicit AudioEngine(QObject *parent = nullptr);
    ~AudioEngine() override = default;

    QString currentTitle() const;
    QString currentArtist() const;
    QString currentAlbum() const;
    QString currentAlbumArt() const;
    qint64 position() const;
    qint64 duration() const;
    qreal volume() const;
    PlaybackState playbackState() const;
    QString currentSource() const;

public slots:
    void play();
    void pause();
    void stop();
    void setPosition(qint64 position);
    void setVolume(qreal volume);
    void setCurrentSource(const QString &source);
    void seekForward(qint64 ms = 10000);
    void seekBackward(qint64 ms = 10000);

signals:
    void currentTrackChanged();
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void volumeChanged(qreal volume);
    void playbackStateChanged(PlaybackState state);
    void currentSourceChanged(const QString &source);
    void errorOccurred(const QString &message);

private slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onError(QMediaPlayer::Error error, const QString &errorString);
    void updateMetadata();

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    QString m_currentTitle;
    QString m_currentArtist;
    QString m_currentAlbum;
    QString m_currentAlbumArt;
};

#endif // AUDIOENGINE_H
