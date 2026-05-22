#include "filebrowser.h"
#include <QDir>
#include <QFileInfo>
#include <QDirIterator>
#include <QMediaPlayer>
#include <QMediaMetaData>

FileBrowser::FileBrowser(QObject *parent)
    : QObject(parent)
{
    m_supportedFormats = {"mp3", "wav", "flac", "m4a", "ogg", "wma", "aac", "aiff"};
}

void FileBrowser::scanDirectory(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << path;
        emit scanError(QStringLiteral("Directory does not exist: %1").arg(path));
        return;
    }

    QVector<TrackInfo> tracks;
    QDirIterator it(path, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString filePath = it.next();
        QFileInfo fi(filePath);
        if (!isValidAudioFile(filePath)) continue;

        TrackInfo track;
        track.url = filePath;
        track.title = fi.completeBaseName();
        track.artist = QStringLiteral("Unknown Artist");
        track.album = QStringLiteral("Unknown Album");

        tracks.append(track);
    }

    if (tracks.isEmpty()) {
        emit scanError(QStringLiteral("No audio files found in: %1").arg(path));
    } else {
        emit scanFinished(tracks);
    }
}

bool FileBrowser::isValidAudioFile(const QString &filePath) const
{
    QFileInfo fi(filePath);
    QString ext = fi.suffix().toLower();
    return m_supportedFormats.contains(ext);
}

QStringList FileBrowser::supportedFormats() const
{
    return m_supportedFormats;
}
