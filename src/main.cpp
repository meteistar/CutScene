#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QDir>
#include <QDebug>
#include <QPixmap>
#include <QFileInfo>
#include <QTime>
#include <QEvent>
#include <QUrl>
#include "VideoThumbnailLoader.h"
#include "HeaderBarWidget.h"
#include "LeftTabBarWidget.h"
#include "RightSidebarWidget.h"
#include "MainContentWidget.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>  // av_image_get_buffer_size, av_image_fill_arrays
#include <libavutil/avutil.h>
}

class AppDropFilter : public QObject
{
public:
    explicit AppDropFilter(RightSidebarWidget *sidebar, QObject *parent = nullptr)
        : QObject(parent)
        , m_sidebar(sidebar)
    {
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        Q_UNUSED(watched);
        if (!m_sidebar) {
            return false;
        }

        if (event->type() == QEvent::DragEnter) {
            auto *dragEvent = static_cast<QDragEnterEvent *>(event);
            if (firstSupportedFile(dragEvent->mimeData()).isEmpty()) {
                return false;
            }
            dragEvent->acceptProposedAction();
            return true;
        }

        if (event->type() == QEvent::Drop) {
            auto *dropEvent = static_cast<QDropEvent *>(event);
            const QString path = firstSupportedFile(dropEvent->mimeData());
            if (path.isEmpty()) {
                return false;
            }
            m_sidebar->importMediaFile(path);
            dropEvent->acceptProposedAction();
            return true;
        }

        return false;
    }

private:
    static QString firstSupportedFile(const QMimeData *mimeData)
    {
        if (!mimeData || !mimeData->hasUrls()) {
            return {};
        }

        const QList<QUrl> urls = mimeData->urls();
        for (const QUrl &url : urls) {
            if (!url.isLocalFile()) {
                continue;
            }

            const QString path = url.toLocalFile();
            const QFileInfo info(path);
            if (!info.exists() || !info.isFile()) {
                continue;
            }

            if (info.suffix().compare("mp4", Qt::CaseInsensitive) == 0) {
                return path;
            }
        }

        return {};
    }

    RightSidebarWidget *m_sidebar;
};

// class VideoThumbnailLoader : public QObject {
//     Q_OBJECT
// public:
//     explicit VideoThumbnailLoader(QObject *parent = nullptr) : QObject(parent) {}

//     QImage generateThumbnail(const QString &filePath, QString &durationStr) {
//         AVFormatContext *fmtCtx = nullptr;
//         if (avformat_open_input(&fmtCtx, filePath.toStdString().c_str(), nullptr, nullptr) < 0) {
//             qWarning() << "Could not open file:" << filePath;
//             return QImage();
//         }

//         if (avformat_find_stream_info(fmtCtx, nullptr) < 0) {
//             qWarning() << "Could not find stream info";
//             avformat_close_input(&fmtCtx);
//             return QImage();
//         }

//         // Duration
//         qint64 durationMs = (fmtCtx->duration / AV_TIME_BASE) * 1000;
//         durationStr = QTime(0,0).addMSecs(durationMs).toString("mm:ss");

//         int videoStream = av_find_best_stream(fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
//         if (videoStream < 0) {
//             qWarning() << "No video stream found";
//             avformat_close_input(&fmtCtx);
//             return QImage();
//         }

//         AVCodecParameters *codecPar = fmtCtx->streams[videoStream]->codecpar;
//         const AVCodec *codec = avcodec_find_decoder(codecPar->codec_id);
//         if (!codec) {
//             qWarning() << "No suitable decoder found";
//             avformat_close_input(&fmtCtx);
//             return QImage();
//         }

//         AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
//         avcodec_parameters_to_context(codecCtx, codecPar);
//         avcodec_open2(codecCtx, codec, nullptr);

//         AVPacket *packet = av_packet_alloc();
//         AVFrame *frame = av_frame_alloc();
//         AVFrame *rgbFrame = av_frame_alloc();

//         SwsContext *swsCtx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
//                                             300, 200, AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr, nullptr, nullptr);
//         int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, 300, 200, 1);
//         uint8_t *buffer = (uint8_t *)av_malloc(numBytes);
//         av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, buffer,
//                              AV_PIX_FMT_RGB24, 300, 200, 1);

//         QImage image;
//         bool gotFrame = false;

//         while (av_read_frame(fmtCtx, packet) >= 0) {
//             if (packet->stream_index == videoStream) {
//                 if (avcodec_send_packet(codecCtx, packet) == 0) {
//                     if (avcodec_receive_frame(codecCtx, frame) == 0) {
//                         sws_scale(swsCtx, frame->data, frame->linesize, 0, codecCtx->height,
//                                   rgbFrame->data, rgbFrame->linesize);

//                         image = QImage(rgbFrame->data[0], 300, 200, rgbFrame->linesize[0],
//                                        QImage::Format_RGB888).copy();
//                         gotFrame = true;
//                         break;
//                     }
//                 }
//             }
//             av_packet_unref(packet);
//         }

//         av_packet_free(&packet);
//         av_frame_free(&frame);
//         av_frame_free(&rgbFrame);
//         avcodec_free_context(&codecCtx);
//         avformat_close_input(&fmtCtx);
//         sws_freeContext(swsCtx);
//         av_free(buffer);

//         return gotFrame ? image : QImage();
//     }
// };

// class VideoDropWidget : public QLabel {
//     Q_OBJECT
// public:
//     explicit VideoDropWidget(QWidget *parent = nullptr) : QLabel(parent) {
//         setAcceptDrops(true);
//         setAlignment(Qt::AlignCenter);
//         setText("Drag & Drop Video Here");
//         setStyleSheet("QLabel { border: 2px dashed gray; }");
//     }

// protected:
//     void dragEnterEvent(QDragEnterEvent *event) override {
//         if (event->mimeData()->hasUrls()) event->acceptProposedAction();
//     }

//     void dropEvent(QDropEvent *event) override {
//         if (!event->mimeData()->hasUrls()) return;
//         QString path = event->mimeData()->urls().first().toLocalFile();
//         QFileInfo info(path);
//         if (!info.exists()) return;

//         QString duration;
//         QImage thumb = loader.generateThumbnail(path, duration);
//         if (!thumb.isNull()) {
//             setPixmap(QPixmap::fromImage(thumb).scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
//             videoPath = path;
//             qDebug() << "Video dropped:" << path << "Duration:" << duration;
//         }
//     }

//     void mousePressEvent(QMouseEvent *event) override {
//         if (videoPath.isEmpty()) return;
//         if (event->button() == Qt::LeftButton) {
//             QMimeData *mime = new QMimeData;
//             mime->setText(videoPath);

//             QDrag *drag = new QDrag(this);
//             drag->setMimeData(mime);
//             drag->exec(Qt::CopyAction);
//         }
//     }

// private:
//     QString videoPath;
//     VideoThumbnailLoader loader;
// };

// class VideoReceiver : public QLabel {
//     Q_OBJECT
// public:
//     explicit VideoReceiver(QWidget *parent = nullptr) : QLabel(parent) {
//         setAcceptDrops(true);
//         setAlignment(Qt::AlignCenter);
//         setText("Drop Here");
//         setStyleSheet("QLabel { border: 2px dashed blue; }");
//     }

// protected:
//     void dragEnterEvent(QDragEnterEvent *event) override {
//         if (event->mimeData()->hasText()) event->acceptProposedAction();
//     }

//     void dropEvent(QDropEvent *event) override {
//         QString path = event->mimeData()->text();
//         setText("Received:\n" + path);
//         qDebug() << "Received video path:" << path;
//     }
// };

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("CutScene");
    window.resize(1000, 700);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(&window);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Add HeaderBar
    HeaderBarWidget *headerBar = new HeaderBarWidget(&window);
    mainLayout->addWidget(headerBar);

    // Create horizontal layout for sidebar and main content
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    // Add LeftTabBar
    LeftTabBarWidget *leftTabBar = new LeftTabBarWidget(&window);
    contentLayout->addWidget(leftTabBar);

    // Add main content area (video player + timeline placeholder)
    MainContentWidget *mainContentArea = new MainContentWidget(&window);
    contentLayout->addWidget(mainContentArea, 1);

    // Add Right Sidebar
    RightSidebarWidget *rightSidebar = new RightSidebarWidget(&window);
    contentLayout->addWidget(rightSidebar);
    mainLayout->addLayout(contentLayout);

    window.setAcceptDrops(true);
    auto *dropFilter = new AppDropFilter(rightSidebar, &window);
    app.installEventFilter(dropFilter);

    // Connect header bar signals
    QObject::connect(headerBar, &HeaderBarWidget::fileClicked, []() {
        qDebug() << "File menu action triggered";
    });
    
    QObject::connect(headerBar, &HeaderBarWidget::importClicked, []() {
        qDebug() << "Import action triggered";
    });
    
    QObject::connect(headerBar, &HeaderBarWidget::exportClicked, []() {
        qDebug() << "Export action triggered";
    });

    // Play selected media from right sidebar
    QObject::connect(rightSidebar, &RightSidebarWidget::mediaSelected, mainContentArea, &MainContentWidget::openMedia);
    QObject::connect(leftTabBar, &LeftTabBarWidget::vintageFilterChanged, mainContentArea, &MainContentWidget::setVintageEnabled);
    QObject::connect(leftTabBar, &LeftTabBarWidget::brightnessChanged, mainContentArea, &MainContentWidget::setBrightness);

    window.show();
    return app.exec();
}


//calisannnnn
// #include <QGuiApplication>
// #include <QApplication>
// #include <QQmlApplicationEngine>
// #include <QQmlContext>
// #include <QWidget>
// #include <QHBoxLayout>
// #include "DragLabel.h"
// #include "DropWidget.h"

// #include "VideoThumbnailLoader.h"
// #include "MediaItemModel.h"
// #include "MediaImageProvider.h"

// #include <QApplication>
// #include <QQuickWidget>
// #include <QHBoxLayout>
// #include "DragLabel.h"
// #include "DropWidget.h"

// int main(int argc, char *argv[])
// {
//     QApplication app(argc, argv);

//     QWidget *mainWin = new QWidget;
//     QHBoxLayout *layout = new QHBoxLayout(mainWin);

//     // QML yükleyecek QQuickWidget
//     QQuickWidget *quickWidget = new QQuickWidget;
//     // ---> context property ve image provider buradan ayarlanır
//     VideoThumbnailLoader videoLoader;
//     quickWidget->rootContext()->setContextProperty("videoThumbnailLoader", &videoLoader);

//     MediaItemModel model;
//     quickWidget->rootContext()->setContextProperty("mediaModel", &model);

//     quickWidget->engine()->addImageProvider("media", new MediaImageProvider(&model));
//     quickWidget->setSource(QUrl("qrc:/CutScene/qml/Main.qml"));
//     quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

//     // klasik widgetler
//     auto *dragLabel = new DragLabel("Drag Me");
//     auto *dropWidget = new DropWidget();

//     layout->addWidget(quickWidget, 1); // QML tarafı
//     layout->addWidget(dragLabel);
//     layout->addWidget(dropWidget);

//     mainWin->resize(800, 600);
//     mainWin->show();

//     return app.exec();
// }

//calisan

// int main(int argc, char *argv[])
// {
//     // QGuiApplication app(argc, argv);
//     QApplication app(argc, argv);

//     QQmlApplicationEngine engine;

//     VideoThumbnailLoader videoLoader;
//     engine.rootContext()->setContextProperty("videoThumbnailLoader", &videoLoader);

//     MediaItemModel model;
//     engine.rootContext()->setContextProperty("mediaModel", &model);

//     engine.addImageProvider("media", new MediaImageProvider(&model));

//     const QUrl url(QStringLiteral("qrc:/CutScene/qml/Main.qml"));
//     QObject::connect(
//         &engine,
//         &QQmlApplicationEngine::objectCreationFailed,
//         &app,
//         []() { QCoreApplication::exit(-1); },
//         Qt::QueuedConnection);
//     engine.load(url);

//     auto *qmlWindow = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
//     if (!qmlWindow) return -1;

//     // QWidget oluştur
//     QWidget *widget = new QWidget;
//     QHBoxLayout *layout = new QHBoxLayout(widget);
//     layout->addWidget(new DragLabel("Drag Me"));
//     layout->addWidget(new DropWidget());

//     // QWidget'i QML penceresi içine container olarak göm
//     QWidget *container = QWidget::createWindowContainer(qmlWindow);
//     container->setMinimumSize(800, 600);

//     // üst düzey pencerede hem QML hem widget göster
//     QWidget *mainWin = new QWidget;
//     QHBoxLayout *mainLayout = new QHBoxLayout(mainWin);
//     mainLayout->addWidget(container, 1);
//     mainLayout->addWidget(widget);

//     mainWin->show();

//     return app.exec();
// }

#include "main.moc"
