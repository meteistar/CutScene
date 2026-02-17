#pragma once

#include <QObject>
#include <QImage>

class VideoThumbnailLoader : public QObject {
    Q_OBJECT
public:
    explicit VideoThumbnailLoader(QObject *parent = nullptr);

    Q_INVOKABLE void loadFile(const QString &filePath);
    QImage generateTimelineImage(const QString &filePath);

signals:
    void thumbnailReady(const QString &filePath, const QString &imagePath, const QString &duration);
};
