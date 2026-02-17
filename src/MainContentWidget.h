#ifndef MAINCONTENTWIDGET_H
#define MAINCONTENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QImage>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QVideoSink>
#include <QtMultimedia/QVideoFrame>

class MainContentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainContentWidget(QWidget *parent = nullptr);

public slots:
    void openMedia(const QString &filePath);
    void setVintageEnabled(bool enabled);
    void setBrightness(int value);

private slots:
    void togglePlay();
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void setPosition(int value);
    void toggleMute();
    void toggleFullscreen();
    void onVideoFrameChanged(const QVideoFrame &frame);

private:
    static QString formatTime(qint64 ms);
    void renderFrame(const QImage &frame);
    static QImage applyVintage(const QImage &source);
    static QImage applyBrightness(const QImage &source, int value);
    void resizeEvent(QResizeEvent *event) override;

    QVBoxLayout *m_rootLayout;
    QWidget *m_videoArea;
    QVBoxLayout *m_videoLayout;
    QLabel *m_videoLabel;

    QHBoxLayout *m_controlsLayout;
    QPushButton *m_playPauseButton;
    QLabel *m_timeLabel;
    QSlider *m_seekSlider;
    QPushButton *m_volumeButton;
    QPushButton *m_fullscreenButton;

    QWidget *m_timelinePlaceholder;

    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    QVideoSink *m_videoSink;
    QImage m_currentFrame;
    bool m_vintageEnabled;
    int m_brightness;
    qint64 m_durationMs;
};

#endif // MAINCONTENTWIDGET_H


