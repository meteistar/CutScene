#pragma once
#include <QQuickImageProvider>
#include "MediaItemModel.h"
#include "MediaImageProvider.h"

class MediaImageProvider : public QQuickImageProvider {
public:
    explicit MediaImageProvider(MediaItemModel *model)
        : QQuickImageProvider(QQuickImageProvider::Image),
        m_model(model) {}

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        QImage img = m_model->requestImage(id);
        if (requestedSize.isValid())
            img = img.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        if (size) *size = img.size();
        return img;
    }

private:
    MediaItemModel *m_model;
};
