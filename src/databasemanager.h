#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QVector>
#include "playlistmanager.h"

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager() override;

    Q_INVOKABLE bool saveTrack(const TrackInfo &track);
    Q_INVOKABLE QVector<TrackInfo> loadAllTracks();
    Q_INVOKABLE bool savePlaylist(const QString &name, const QStringList &trackUrls);
    Q_INVOKABLE bool clearTracks();
    Q_INVOKABLE bool addToHistory(const TrackInfo &track);
    Q_INVOKABLE QVector<TrackInfo> loadHistory(int limit = 50);

signals:
    void databaseError(const QString &message);

private:
    void initDatabase();
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
