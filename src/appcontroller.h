#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include "audioengine.h"
#include "playlistmanager.h"
#include "filebrowser.h"
#include "databasemanager.h"

class AppController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AudioEngine *audioEngine READ audioEngine CONSTANT)
    Q_PROPERTY(PlaylistManager *playlist READ playlist CONSTANT)
    Q_PROPERTY(FileBrowser *fileBrowser READ fileBrowser CONSTANT)
    Q_PROPERTY(DatabaseManager *database READ database CONSTANT)
    Q_PROPERTY(QString platform READ platform CONSTANT)

public:
    explicit AppController(QQmlApplicationEngine *engine, QObject *parent = nullptr);

    AudioEngine *audioEngine() const;
    PlaylistManager *playlist() const;
    FileBrowser *fileBrowser() const;
    DatabaseManager *database() const;
    QString platform() const;

    Q_INVOKABLE void openFolder(const QString &path);
    Q_INVOKABLE void playTrack(int index);
    Q_INVOKABLE void togglePlayPause();
    Q_INVOKABLE void nextTrack();
    Q_INVOKABLE void previousTrack();
    Q_INVOKABLE void setLocale(const QString &locale);

signals:
    void trackPlaybackStarted(const TrackInfo &track);
    void folderLoaded(const QString &path);
    void localeChanged(const QString &locale);

private slots:
    void onTracksFound(const QVector<TrackInfo> &tracks);
    void onPlaybackError(const QString &message);
    void onCurrentIndexChanged(int index);

private:
    AudioEngine *m_audioEngine;
    PlaylistManager *m_playlist;
    FileBrowser *m_fileBrowser;
    DatabaseManager *m_database;
    QQmlApplicationEngine *m_engine;
    QString m_platform;
};

#endif // APPCONTROLLER_H
