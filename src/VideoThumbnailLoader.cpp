// #include "VideoThumbnailLoader.h"
// #include <QDebug>
// #include <QTime>
// #include <QDir>

// extern "C" {
// #include <libavformat/avformat.h>
// #include <libavcodec/avcodec.h>
// #include <libswscale/swscale.h>
// #include <libavutil/imgutils.h>
// }

// VideoThumbnailLoader::VideoThumbnailLoader(QObject *parent)
//     : QObject(parent)
// {}

// void VideoThumbnailLoader::loadFile(const QString &filePath) {
//     AVFormatContext *fmtCtx = nullptr;
//     if (avformat_open_input(&fmtCtx, filePath.toStdString().c_str(), nullptr, nullptr) < 0) {
//         qWarning() << "Could not open file:" << filePath;
//         return;
//     }

//     if (avformat_find_stream_info(fmtCtx, nullptr) < 0) {
//         qWarning() << "Could not find stream info";
//         avformat_close_input(&fmtCtx);
//         return;
//     }

//     // Süreyi hesapla
//     qint64 durationMs = (fmtCtx->duration / AV_TIME_BASE) * 1000;
//     QString durationStr = QTime(0,0).addMSecs(durationMs).toString("mm:ss");

//     // Video stream bul
//     int videoStream = av_find_best_stream(fmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
//     if (videoStream < 0) {
//         qWarning() << "No video stream found";
//         avformat_close_input(&fmtCtx);
//         return;
//     }

//     AVCodecParameters *codecPar = fmtCtx->streams[videoStream]->codecpar;
//     const AVCodec *codec = avcodec_find_decoder(codecPar->codec_id);
//     if (!codec) {
//         qWarning() << "No suitable decoder found";
//         avformat_close_input(&fmtCtx);
//         return;
//     }

//     AVCodecContext *codecCtx = avcodec_alloc_context3(codec);
//     avcodec_parameters_to_context(codecCtx, codecPar);
//     avcodec_open2(codecCtx, codec, nullptr);

//     AVPacket *packet = av_packet_alloc();
//     AVFrame *frame = av_frame_alloc();
//     AVFrame *rgbFrame = av_frame_alloc();

//     SwsContext *swsCtx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
//                                         300, 200, AV_PIX_FMT_RGB24, SWS_BILINEAR, nullptr, nullptr, nullptr);

//     int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, 300, 200, 1);
//     uint8_t *buffer = (uint8_t *)av_malloc(numBytes);
//     av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, buffer,
//                          AV_PIX_FMT_RGB24, 300, 200, 1);

//     QImage image;
//     bool gotFrame = false;

//     while (av_read_frame(fmtCtx, packet) >= 0) {
//         if (packet->stream_index == videoStream) {
//             if (avcodec_send_packet(codecCtx, packet) == 0) {
//                 if (avcodec_receive_frame(codecCtx, frame) == 0) {
//                     sws_scale(swsCtx, frame->data, frame->linesize, 0, codecCtx->height,
//                               rgbFrame->data, rgbFrame->linesize);

//                     image = QImage(rgbFrame->data[0], 300, 200, rgbFrame->linesize[0],
//                                    QImage::Format_RGB888).copy();
//                     gotFrame = true;
//                     break;
//                 }
//             }
//         }
//         av_packet_unref(packet);
//     }

//     // Temizlik
//     av_packet_free(&packet);
//     av_frame_free(&frame);
//     av_frame_free(&rgbFrame);
//     avcodec_free_context(&codecCtx);
//     avformat_close_input(&fmtCtx);
//     sws_freeContext(swsCtx);
//     av_free(buffer);

//     if (gotFrame) {
//         QString tmpPath = QDir::temp().filePath("thumb.png");
//         QFile::remove(tmpPath);
//         image.save(tmpPath);
//         emit thumbnailReady(tmpPath, durationStr);
//     } else {
//         qWarning() << "No frame could be decoded.";
//     }
// }

// QImage VideoThumbnailLoader::generateTimelineImage(const QString &filePath) {
//     // 1. video aç
//     // 2. toplam süreyi al
//     // 3. her saniye seek -> frame decode
//     // 4. QImage listesi oluştur
//     // 5. Yan yana birleştir
//     // 6. return birleşmiş QImage
//     return QImage();
// }
