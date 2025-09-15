// MediaItemModel.h
#pragma once
#include <QAbstractListModel>
#include <QVector>
#include "VideoThumbnailLoader.h"

struct MediaItem {
    QString id;
    QString fileName;
    QString duration;
    QImage  thumbnail;
};

class MediaItemModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        FileNameRole,
        DurationRole
    };
    Q_ENUM(Roles)

    explicit MediaItemModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addMedia(const QString &filePath);

    QImage requestImage(const QString &id) const;

private slots:
    void onThumbnailReady(const QString &imageId, const QString &duration);

private:
    QVector<MediaItem> m_items;
    QString m_pendingFilePath;
};
