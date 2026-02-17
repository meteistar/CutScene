#include "MainContentWidget.h"

#include <QPixmap>
#include <QResizeEvent>
#include <QUrl>

MainContentWidget::MainContentWidget(QWidget *parent)
    : QWidget(parent)
    , m_rootLayout(new QVBoxLayout(this))
    , m_videoArea(new QWidget(this))
    , m_videoLayout(new QVBoxLayout(m_videoArea))
    , m_videoLabel(new QLabel(m_videoArea))
    , m_controlsLayout(new QHBoxLayout())
    , m_playPauseButton(new QPushButton(QStringLiteral("\u25B6")))
    , m_timeLabel(new QLabel("00:00 / 00:00"))
    , m_seekSlider(new QSlider(Qt::Horizontal))
    , m_volumeButton(new QPushButton(QStringLiteral("\U0001F50A")))
    , m_fullscreenButton(new QPushButton(QStringLiteral("\u26F6")))
    , m_timelinePlaceholder(new QWidget(this))
    , m_player(new QMediaPlayer(this))
    , m_audioOutput(new QAudioOutput(this))
    , m_videoSink(new QVideoSink(this))
    , m_vintageEnabled(false)
    , m_brightness(50)
    , m_durationMs(0)
{
    setStyleSheet("MainContentWidget { background-color: #1e1e1e; }");
    m_rootLayout->setContentsMargins(0, 0, 0, 0);
    m_rootLayout->setSpacing(0);

    m_videoArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_videoLayout->setContentsMargins(16, 16, 16, 8);
    m_videoLayout->setSpacing(8);

    m_videoLabel->setAlignment(Qt::AlignCenter);
    m_videoLabel->setStyleSheet("QLabel { background-color: black; }");
    m_videoLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_videoLayout->addWidget(m_videoLabel, 1);

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

    m_rootLayout->addWidget(m_videoArea, 2);

    m_timelinePlaceholder->setStyleSheet("QWidget { background-color: #151515; border-top: 1px solid #2a2a2a; }");
    m_rootLayout->addWidget(m_timelinePlaceholder, 1);

    m_player->setVideoOutput(m_videoSink);
    m_player->setAudioOutput(m_audioOutput);
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainContentWidget::updatePosition);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainContentWidget::updateDuration);
    connect(m_videoSink, &QVideoSink::videoFrameChanged, this, &MainContentWidget::onVideoFrameChanged);
}

void MainContentWidget::openMedia(const QString &filePath)
{
    m_player->setSource(QUrl::fromLocalFile(filePath));
    m_player->play();
    m_playPauseButton->setText(QStringLiteral("\u23F8"));
}

void MainContentWidget::setVintageEnabled(bool enabled)
{
    m_vintageEnabled = enabled;
    if (!m_currentFrame.isNull()) {
        renderFrame(m_currentFrame);
    }
}


void MainContentWidget::setBrightness(int value)
{
    m_brightness = qBound(0, value, 100);
    if (!m_currentFrame.isNull()) {
        renderFrame(m_currentFrame);
    }
}

void MainContentWidget::togglePlay()
{
    if (m_player->playbackState() == QMediaPlayer::PlayingState) {
        m_player->pause();
        m_playPauseButton->setText(QStringLiteral("\u25B6"));
    } else {
        m_player->play();
        m_playPauseButton->setText(QStringLiteral("\u23F8"));
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

void MainContentWidget::toggleMute()
{
    m_audioOutput->setMuted(!m_audioOutput->isMuted());
    m_volumeButton->setText(m_audioOutput->isMuted() ? QStringLiteral("\U0001F507") : QStringLiteral("\U0001F50A"));
}

void MainContentWidget::toggleFullscreen()
{
    QWidget *topLevel = window();
    if (!topLevel) {
        return;
    }

    if (topLevel->isFullScreen()) {
        topLevel->showNormal();
    } else {
        topLevel->showFullScreen();
    }
}

void MainContentWidget::onVideoFrameChanged(const QVideoFrame &frame)
{
    const QImage image = frame.toImage();
    if (image.isNull()) {
        return;
    }

    m_currentFrame = image;
    renderFrame(m_currentFrame);
}

void MainContentWidget::renderFrame(const QImage &frame)
{
    QImage output = m_vintageEnabled ? applyVintage(frame) : frame;
    output = applyBrightness(output, m_brightness);
    const QPixmap pixmap = QPixmap::fromImage(output).scaled(
        m_videoLabel->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );
    m_videoLabel->setPixmap(pixmap);
}

QImage MainContentWidget::applyVintage(const QImage &source)
{
    QImage img = source.convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
        for (int x = 0; x < img.width(); ++x) {
            const int r = qRed(line[x]);
            const int g = qGreen(line[x]);
            const int b = qBlue(line[x]);

            const int sepiaR = qBound(0, static_cast<int>(0.393 * r + 0.769 * g + 0.189 * b), 255);
            const int sepiaG = qBound(0, static_cast<int>(0.349 * r + 0.686 * g + 0.168 * b), 255);
            const int sepiaB = qBound(0, static_cast<int>(0.272 * r + 0.534 * g + 0.131 * b), 255);

            const int fadedR = qBound(0, static_cast<int>(sepiaR * 0.95 + 10), 255);
            const int fadedG = qBound(0, static_cast<int>(sepiaG * 0.9 + 5), 255);
            const int fadedB = qBound(0, static_cast<int>(sepiaB * 0.75), 255);

            line[x] = qRgba(fadedR, fadedG, fadedB, qAlpha(line[x]));
        }
    }
    return img;
}


QImage MainContentWidget::applyBrightness(const QImage &source, int value)
{
    if (value == 50) {
        return source;
    }

    QImage img = source.convertToFormat(QImage::Format_ARGB32);
    const int delta = static_cast<int>((value - 50) * 2.55);

    for (int y = 0; y < img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
        for (int x = 0; x < img.width(); ++x) {
            const int r = qBound(0, qRed(line[x]) + delta, 255);
            const int g = qBound(0, qGreen(line[x]) + delta, 255);
            const int b = qBound(0, qBlue(line[x]) + delta, 255);
            line[x] = qRgba(r, g, b, qAlpha(line[x]));
        }
    }

    return img;
}

void MainContentWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (!m_currentFrame.isNull()) {
        renderFrame(m_currentFrame);
    }
}

QString MainContentWidget::formatTime(qint64 ms)
{
    qint64 secs = ms / 1000;
    qint64 mins = secs / 60;
    secs %= 60;
    qint64 hours = mins / 60;
    mins %= 60;
    if (hours > 0) {
        return QString::asprintf("%lld:%02lld:%02lld", hours, mins, secs);
    }
    return QString::asprintf("%02lld:%02lld", mins, secs);
}
