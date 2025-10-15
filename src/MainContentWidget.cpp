#include "MainContentWidget.h"
#include <QStyle>

MainContentWidget::MainContentWidget(QWidget *parent)
    : QWidget(parent)
    , m_rootLayout(new QVBoxLayout(this))
    , m_videoArea(new QWidget(this))
    , m_videoLayout(new QVBoxLayout(m_videoArea))
    , m_videoWidget(new QVideoWidget(m_videoArea))
    , m_controlsLayout(new QHBoxLayout())
    , m_playPauseButton(new QPushButton("▶"))
    , m_timeLabel(new QLabel("00:00 / 00:00"))
    , m_seekSlider(new QSlider(Qt::Horizontal))
    , m_volumeButton(new QPushButton("🔊"))
    , m_fullscreenButton(new QPushButton("⛶"))
    , m_timelinePlaceholder(new QWidget(this))
    , m_player(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_durationMs(0)
{
    setStyleSheet("MainContentWidget { background-color: #1e1e1e; }");
    m_rootLayout->setContentsMargins(0, 0, 0, 0);
    m_rootLayout->setSpacing(0);

    // Split vertically: top 2 parts (video + controls), bottom 1 part (timeline placeholder)
    m_videoArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_videoLayout->setContentsMargins(16, 16, 16, 8);
    m_videoLayout->setSpacing(8);

    // Video widget
    m_videoWidget->setStyleSheet("QVideoWidget { background-color: black; }");
    m_videoLayout->addWidget(m_videoWidget, 1);

    // Controls
    m_controlsLayout->setContentsMargins(0, 0, 0, 0);
    m_controlsLayout->setSpacing(8);

    m_playPauseButton->setFixedWidth(32);
    m_playPauseButton->setStyleSheet("QPushButton { color: #ffffff; background-color: #2b2b2b; border: 1px solid #3f3f3f; border-radius: 4px; }");
    connect(m_playPauseButton, &QPushButton::clicked, this, &MainContentWidget::togglePlay);

    m_seekSlider->setRange(0, 0);
    connect(m_seekSlider, &QSlider::sliderMoved, this, &MainContentWidget::setPosition);

    m_volumeButton->setFixedWidth(32);
    m_volumeButton->setStyleSheet("QPushButton { color: #ffffff; background-color: #2b2b2b; border: 1px solid #3f3f3f; border-radius: 4px; }");
    connect(m_volumeButton, &QPushButton::clicked, this, &MainContentWidget::toggleMute);

    m_fullscreenButton->setFixedWidth(32);
    m_fullscreenButton->setStyleSheet("QPushButton { color: #ffffff; background-color: #2b2b2b; border: 1px solid #3f3f3f; border-radius: 4px; }");
    connect(m_fullscreenButton, &QPushButton::clicked, this, &MainContentWidget::toggleFullscreen);

    m_timeLabel->setStyleSheet("QLabel { color: #ffffff; }");

    m_controlsLayout->addWidget(m_playPauseButton);
    m_controlsLayout->addWidget(m_timeLabel);
    m_controlsLayout->addWidget(m_seekSlider, 1);
    m_controlsLayout->addWidget(m_volumeButton);
    m_controlsLayout->addWidget(m_fullscreenButton);

    QWidget *controlsContainer = new QWidget(m_videoArea);
    controlsContainer->setLayout(m_controlsLayout);
    m_videoLayout->addWidget(controlsContainer);

    m_rootLayout->addWidget(m_videoArea, 2); // top 2 parts

    // Timeline placeholder (bottom 1 part)
    m_timelinePlaceholder->setStyleSheet("QWidget { background-color: #151515; border-top: 1px solid #2a2a2a; }");
    m_rootLayout->addWidget(m_timelinePlaceholder, 1);

    // Media player wiring
    m_player->setVideoOutput(m_videoWidget);
    m_player->setAudioOutput(m_audioOutput);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainContentWidget::updatePosition);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainContentWidget::updateDuration);
}

void MainContentWidget::openMedia(const QString &filePath)
{
    m_player->setSource(QUrl::fromLocalFile(filePath));
    m_player->play();
    m_playPauseButton->setText("⏸");
}

void MainContentWidget::togglePlay()
{
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        m_player->pause();
        m_playPauseButton->setText("▶");
    } else {
        m_player->play();
        m_playPauseButton->setText("⏸");
    }
}

void MainContentWidget::toggleMute()
{
    m_audioOutput->setMuted(!m_audioOutput->isMuted());
    m_volumeButton->setText(m_audioOutput->isMuted() ? "🔇" : "🔊");
}

void MainContentWidget::toggleFullscreen()
{
    if (m_videoWidget->isFullScreen()) {
        m_videoWidget->setFullScreen(false);
    } else {
        m_videoWidget->setFullScreen(true);
    }
}

void MainContentWidget::updatePosition(qint64 position)
{
    if (!m_seekSlider->isSliderDown()) {
        m_seekSlider->setValue(static_cast<int>(position));
    }
    m_timeLabel->setText(QString("%1 / %2").arg(formatTime(position), formatTime(m_durationMs)));
}

void MainContentWidget::updateDuration(qint64 duration)
{
    m_durationMs = duration;
    m_seekSlider->setRange(0, static_cast<int>(duration));
}

void MainContentWidget::setPosition(int value)
{
    m_player->setPosition(value);
}

QString MainContentWidget::formatTime(qint64 ms)
{
    qint64 secs = ms / 1000;
    qint64 mins = secs / 60; secs %= 60;
    qint64 hours = mins / 60; mins %= 60;
    if (hours > 0) return QString::asprintf("%lld:%02lld:%02lld", hours, mins, secs);
    return QString::asprintf("%02lld:%02lld", mins, secs);
}


