#ifndef MAINCONTENTWIDGET_H
#define MAINCONTENTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>

class MainContentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainContentWidget(QWidget *parent = nullptr);

public slots:
    void openMedia(const QString &filePath);

private slots:
    void togglePlay();
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void setPosition(int value);
    void toggleMute();
    void toggleFullscreen();

private:
    static QString formatTime(qint64 ms);

    QVBoxLayout *m_rootLayout;
    QWidget *m_videoArea;
    QVBoxLayout *m_videoLayout;
    QVideoWidget *m_videoWidget;

    QHBoxLayout *m_controlsLayout;
    QPushButton *m_playPauseButton;
    QLabel *m_timeLabel;
    QSlider *m_seekSlider;
    QPushButton *m_volumeButton;
    QPushButton *m_fullscreenButton;

    QWidget *m_timelinePlaceholder;

    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    qint64 m_durationMs;
};

#endif // MAINCONTENTWIDGET_H


