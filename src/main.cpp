#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "VideoThumbnailLoader.h"
#include "MediaItemModel.h"
#include "MediaImageProvider.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    VideoThumbnailLoader videoLoader;
    engine.rootContext()->setContextProperty("videoThumbnailLoader", &videoLoader);

    MediaItemModel model;
    engine.rootContext()->setContextProperty("mediaModel", &model);

    engine.addImageProvider("media", new MediaImageProvider(&model));

    const QUrl url(QStringLiteral("qrc:/CutScene/qml/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
