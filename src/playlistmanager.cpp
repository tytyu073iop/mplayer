#include "playlistmanager.h"
#include <QRandomGenerator>
#include <algorithm>

PlaylistManager::PlaylistManager(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PlaylistManager::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_tracks.size();
}

QVariant PlaylistManager::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_tracks.size())
        return {};

    const auto &track = m_tracks.at(index.row());
    switch (role) {
    case UrlRole:      return track.url;
    case TitleRole:    return track.title;
    case ArtistRole:   return track.artist;
    case AlbumRole:    return track.album;
    case AlbumArtRole: return track.albumArt;
    case DurationRole: return track.duration;
    case Qt::DisplayRole: return track.title;
    }
    return {};
}

QHash<int, QByteArray> PlaylistManager::roleNames() const
{
    return {
        {UrlRole, "trackUrl"},
        {TitleRole, "trackTitle"},
        {ArtistRole, "trackArtist"},
        {AlbumRole, "trackAlbum"},
        {AlbumArtRole, "trackAlbumArt"},
        {DurationRole, "trackDuration"}
    };
}

int PlaylistManager::currentIndex() const { return m_currentIndex; }

void PlaylistManager::setCurrentIndex(int index)
{
    if (index >= -1 && index < m_tracks.size() && index != m_currentIndex) {
        m_currentIndex = index;
        emit currentIndexChanged(m_currentIndex);
    }
}

void PlaylistManager::addTrack(const QString &url, const QString &title,
                                const QString &artist, const QString &album,
                                const QString &albumArt, qint64 duration)
{
    beginInsertRows(QModelIndex(), m_tracks.size(), m_tracks.size());
    m_tracks.append({url, title, artist, album, albumArt, duration});
    endInsertRows();
    emit countChanged(m_tracks.size());
}

void PlaylistManager::removeTrack(int index)
{
    if (index < 0 || index >= m_tracks.size()) return;
    beginRemoveRows(QModelIndex(), index, index);
    m_tracks.removeAt(index);
    endRemoveRows();
    if (m_currentIndex >= m_tracks.size())
        setCurrentIndex(m_tracks.size() - 1);
    emit countChanged(m_tracks.size());
}

void PlaylistManager::clear()
{
    if (m_tracks.isEmpty()) return;
    beginResetModel();
    m_tracks.clear();
    endResetModel();
    m_currentIndex = -1;
    emit countChanged(0);
    emit currentIndexChanged(-1);
}

void PlaylistManager::shuffle()
{
    if (m_tracks.size() < 2) return;
    beginResetModel();
    std::shuffle(m_tracks.begin(), m_tracks.end(),
                 *QRandomGenerator::global());
    endResetModel();
}

TrackInfo PlaylistManager::trackAt(int index) const
{
    if (index < 0 || index >= m_tracks.size()) return {};
    return m_tracks.at(index);
}

TrackInfo PlaylistManager::currentTrack() const
{
    return trackAt(m_currentIndex);
}

QStringList PlaylistManager::allUrls() const
{
    QStringList urls;
    urls.reserve(m_tracks.size());
    for (const auto &t : m_tracks)
        urls.append(t.url);
    return urls;
}

void PlaylistManager::setTracks(const QVector<TrackInfo> &tracks)
{
    beginResetModel();
    m_tracks = tracks;
    endResetModel();
    m_currentIndex = m_tracks.isEmpty() ? -1 : 0;
    emit countChanged(m_tracks.size());
    emit currentIndexChanged(m_currentIndex);
}
