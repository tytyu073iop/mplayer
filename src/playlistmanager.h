#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include <QVector>

struct TrackInfo {
    QString url;
    QString title;
    QString artist;
    QString album;
    QString albumArt;
    qint64 duration = 0;
};

class PlaylistManager : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        UrlRole = Qt::UserRole + 1,
        TitleRole,
        ArtistRole,
        AlbumRole,
        AlbumArtRole,
        DurationRole
    };

    explicit PlaylistManager(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    int currentIndex() const;
    void setCurrentIndex(int index);

    Q_INVOKABLE void addTrack(const QString &url, const QString &title,
                              const QString &artist, const QString &album,
                              const QString &albumArt, qint64 duration);
    Q_INVOKABLE void removeTrack(int index);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void shuffle();
    Q_INVOKABLE TrackInfo trackAt(int index) const;
    Q_INVOKABLE TrackInfo currentTrack() const;
    Q_INVOKABLE QStringList allUrls() const;

    void setTracks(const QVector<TrackInfo> &tracks);

signals:
    void currentIndexChanged(int index);
    void countChanged(int count);

private:
    QVector<TrackInfo> m_tracks;
    int m_currentIndex = -1;
};

#endif // PLAYLISTMANAGER_H
