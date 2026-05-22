#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    initDatabase();
}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isOpen())
        m_db.close();
}

void DatabaseManager::initDatabase()
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir().mkpath(dataDir);
    QString dbPath = dataDir + QStringLiteral("/mplayer.db");

    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qWarning() << "Failed to open database:" << m_db.lastError().text();
        emit databaseError(QStringLiteral("Failed to open database: %1").arg(m_db.lastError().text()));
        return;
    }

    QSqlQuery query(m_db);
    query.exec(QStringLiteral("PRAGMA journal_mode=WAL"));
    query.exec(QStringLiteral("PRAGMA foreign_keys=ON"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS tracks ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  url TEXT UNIQUE NOT NULL,"
        "  title TEXT,"
        "  artist TEXT,"
        "  album TEXT,"
        "  album_art TEXT,"
        "  duration INTEGER DEFAULT 0"
        ")"
    ));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS playlists ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name TEXT NOT NULL,"
        "  created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")"
    ));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS playlist_tracks ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  playlist_id INTEGER NOT NULL,"
        "  track_url TEXT NOT NULL,"
        "  position INTEGER DEFAULT 0,"
        "  FOREIGN KEY (playlist_id) REFERENCES playlists(id) ON DELETE CASCADE"
        ")"
    ));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS play_history ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  url TEXT NOT NULL,"
        "  title TEXT,"
        "  artist TEXT,"
        "  album TEXT,"
        "  played_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")"
    ));
}

bool DatabaseManager::saveTrack(const TrackInfo &track)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "INSERT OR REPLACE INTO tracks (url, title, artist, album, album_art, duration) "
        "VALUES (:url, :title, :artist, :album, :album_art, :duration)"
    ));
    query.bindValue(QStringLiteral(":url"), track.url);
    query.bindValue(QStringLiteral(":title"), track.title);
    query.bindValue(QStringLiteral(":artist"), track.artist);
    query.bindValue(QStringLiteral(":album"), track.album);
    query.bindValue(QStringLiteral(":album_art"), track.albumArt);
    query.bindValue(QStringLiteral(":duration"), track.duration);

    if (!query.exec()) {
        qWarning() << "Failed to save track:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<TrackInfo> DatabaseManager::loadAllTracks()
{
    QVector<TrackInfo> tracks;
    QSqlQuery query(m_db);
    query.exec(QStringLiteral("SELECT url, title, artist, album, album_art, duration FROM tracks ORDER BY title"));

    while (query.next()) {
        TrackInfo t;
        t.url = query.value(0).toString();
        t.title = query.value(1).toString();
        t.artist = query.value(2).toString();
        t.album = query.value(3).toString();
        t.albumArt = query.value(4).toString();
        t.duration = query.value(5).toLongLong();
        tracks.append(t);
    }
    return tracks;
}

bool DatabaseManager::clearTracks()
{
    QSqlQuery query(m_db);
    if (!query.exec(QStringLiteral("DELETE FROM tracks"))) {
        qWarning() << "Failed to clear tracks:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::savePlaylist(const QString &name, const QStringList &trackUrls)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral("INSERT INTO playlists (name) VALUES (:name)"));
    query.bindValue(QStringLiteral(":name"), name);
    if (!query.exec()) {
        qWarning() << "Failed to save playlist:" << query.lastError().text();
        return false;
    }
    int playlistId = query.lastInsertId().toInt();

    for (int i = 0; i < trackUrls.size(); ++i) {
        query.prepare(QStringLiteral(
            "INSERT INTO playlist_tracks (playlist_id, track_url, position) "
            "VALUES (:pid, :url, :pos)"
        ));
        query.bindValue(QStringLiteral(":pid"), playlistId);
        query.bindValue(QStringLiteral(":url"), trackUrls.at(i));
        query.bindValue(QStringLiteral(":pos"), i);
        if (!query.exec()) {
            qWarning() << "Failed to save playlist track:" << query.lastError().text();
            return false;
        }
    }
    return true;
}

bool DatabaseManager::addToHistory(const TrackInfo &track)
{
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "INSERT INTO play_history (url, title, artist, album) "
        "VALUES (:url, :title, :artist, :album)"
    ));
    query.bindValue(QStringLiteral(":url"), track.url);
    query.bindValue(QStringLiteral(":title"), track.title);
    query.bindValue(QStringLiteral(":artist"), track.artist);
    query.bindValue(QStringLiteral(":album"), track.album);

    if (!query.exec()) {
        qWarning() << "Failed to add to history:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<TrackInfo> DatabaseManager::loadHistory(int limit)
{
    QVector<TrackInfo> tracks;
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
        "SELECT url, title, artist, album FROM play_history ORDER BY played_at DESC LIMIT :limit"
    ));
    query.bindValue(QStringLiteral(":limit"), limit);
    query.exec();

    while (query.next()) {
        TrackInfo t;
        t.url = query.value(0).toString();
        t.title = query.value(1).toString();
        t.artist = query.value(2).toString();
        t.album = query.value(3).toString();
        tracks.append(t);
    }
    return tracks;
}
