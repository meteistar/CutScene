// MediaItemModel.cpp
#include "MediaItemModel.h"
#include <QFileInfo>
#include <QUuid>

MediaItemModel::MediaItemModel(QObject *parent)
    : QAbstractListModel(parent) {}

int MediaItemModel::rowCount(const QModelIndex &) const {
    return m_items.size();
}

QVariant MediaItemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_items.size())
        return {};

    const auto &item = m_items.at(index.row());
    switch (role) {
    case IdRole: return item.id;
    case FileNameRole: return item.fileName;
    case DurationRole: return item.duration;
    case FileSizeRole: return item.fileSize;
    }
    return {};
}

QHash<int, QByteArray> MediaItemModel::roleNames() const {
    return {
        {IdRole, "id"},
        {FileNameRole, "fileName"},
        {DurationRole, "duration"},
        {FileSizeRole, "fileSize"}
    };
}

void MediaItemModel::addMedia(const QString &filePath) {
    QFileInfo info(filePath);
    QString ext = info.suffix().toLower();

    QStringList supported = {
        "mp4","mov","avi","mkv","flv","wmv","ts",
        "mp3","wav","aac","flac","ogg"
    };

    if (!supported.contains(ext)) {
        qWarning() << "Unsupported file type:" << ext;
        return;
    }
    m_pendingFilePath = filePath;

    auto loader = new VideoThumbnailLoader(this);
    connect(loader, &VideoThumbnailLoader::thumbnailReady,
            this, &MediaItemModel::onThumbnailReady);

    loader->loadFile(filePath);
}

void MediaItemModel::onThumbnailReady(const QString &imageId, const QString &duration) {
    QFileInfo info(m_pendingFilePath);

    MediaItem item;
    item.id = QUuid::createUuid().toString(QUuid::Id128);
    item.fileName = info.fileName();
    item.duration = duration;

    qint64 bytes = info.size();
    if (bytes < 1024)
        item.fileSize = QString::number(bytes) + " B";
    else if (bytes < 1024 * 1024)
        item.fileSize = QString::number(bytes / 1024.0, 'f', 1) + " KB";
    else
        item.fileSize = QString::number(bytes / (1024.0 * 1024.0), 'f', 1) + " MB";

    item.thumbnail = QImage(imageId); // imageId path gibi geldiyse QImage yükle

    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.push_back(item);
    endInsertRows();
}

QImage MediaItemModel::requestImage(const QString &id) const {
    for (const auto &item : m_items) {
        if (item.id == id)
            return item.thumbnail;
    }
    return {};
}
