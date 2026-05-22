#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <QObject>
#include <QVector>
#include "playlistmanager.h"

class FileBrowser : public QObject
{
    Q_OBJECT

public:
    explicit FileBrowser(QObject *parent = nullptr);

    Q_INVOKABLE void scanDirectory(const QString &path);
    Q_INVOKABLE bool isValidAudioFile(const QString &filePath) const;
    Q_INVOKABLE QStringList supportedFormats() const;

signals:
    void scanFinished(const QVector<TrackInfo> &tracks);
    void scanError(const QString &message);

private:
    QStringList m_supportedFormats;
};

#endif // FILEBROWSER_H
