#include "RightSidebarWidget.h"
#include <QIcon>
#include <QFileInfo>
#include <QTime>

RightSidebarWidget::RightSidebarWidget(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_tabBar(nullptr)
    , m_addButton(nullptr)
    , m_listWidget(nullptr)
{
    setupUI();
}

void RightSidebarWidget::setupUI()
{
    setStyleSheet("RightSidebarWidget { background-color: #1e1e1e; }");
    setMinimumWidth(280);

    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(12, 12, 12, 12);
    m_mainLayout->setSpacing(8);

    // Header row with right-aligned plus button
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addStretch();
    m_addButton = new QPushButton("+");
    m_addButton->setFixedSize(28, 28);
    m_addButton->setStyleSheet(
        "QPushButton { color: #ffffff; background-color: #2b2b2b; border: 1px solid #3f3f3f; border-radius: 6px; }"
        "QPushButton:hover { background-color: #3b3b3b; border: 1px solid #ffffff; }"
    );
    headerLayout->addWidget(m_addButton);
    m_mainLayout->addLayout(headerLayout);

    // Horizontal tab bar (All, Video, Audio)
    m_tabBar = new QTabBar(this);
    m_tabBar->addTab("All");
    m_tabBar->addTab("Video");
    m_tabBar->addTab("Audio");
    m_tabBar->setDrawBase(false);
    m_tabBar->setExpanding(true);
    m_tabBar->setElideMode(Qt::ElideRight);
    m_tabBar->setStyleSheet(
        "QTabBar::tab { color: #ffffff; background: #2b2b2b; padding: 8px 12px; border: 1px solid #3f3f3f; }"
        "QTabBar::tab:hover { background: #3b3b3b; }"
        "QTabBar::tab:selected { background: #1e1e1e; border-bottom: 2px solid #0078d4; }"
    );
    m_mainLayout->addWidget(m_tabBar);

    // List widget
    m_listWidget = new QListWidget(this);
    m_listWidget->setStyleSheet(
        "QListWidget { background-color: #1e1e1e; border: 1px solid #3f3f3f; }"
        "QListWidget::item { padding: 6px; }"
        "QListWidget::item:selected { background: #2b2b2b; }"
    );
    m_mainLayout->addWidget(m_listWidget, 1);

    connect(m_addButton, &QPushButton::clicked, this, &RightSidebarWidget::onAddClicked);
    connect(m_tabBar, &QTabBar::currentChanged, this, &RightSidebarWidget::onTabChanged);
}

bool RightSidebarWidget::isVideo(const QString &path) const
{
    const QString ext = QFileInfo(path).suffix().toLower();
    static const QStringList videoExts = {"mp4", "mov", "mkv", "avi", "wmv", "webm"};
    return videoExts.contains(ext);
}

bool RightSidebarWidget::isAudio(const QString &path) const
{
    const QString ext = QFileInfo(path).suffix().toLower();
    static const QStringList audioExts = {"mp3", "wav", "aac", "flac", "ogg", "m4a"};
    return audioExts.contains(ext);
}

void RightSidebarWidget::onAddClicked()
{
    const QString path = QFileDialog::getOpenFileName(this, "Import Media");
    if (path.isEmpty()) return;
    addMediaToModel(path);
}

void RightSidebarWidget::addMediaToModel(const QString &path)
{
    MediaEntry entry;
    entry.filePath = path;
    entry.displayName = QFileInfo(path).fileName();
    entry.type = isVideo(path) ? MediaEntry::Type::Video : (isAudio(path) ? MediaEntry::Type::Audio : MediaEntry::Type::Unknown);
    // Dummy duration for now (00:15)
    entry.durationText = "0:15";

    m_allEntries.push_back(entry);
    emit mediaImported(entry);
    rebuildList();
}

QWidget *RightSidebarWidget::createListRowWidget(const MediaEntry &entry) const
{
    QWidget *row = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(row);
    layout->setContentsMargins(6, 6, 6, 6);
    layout->setSpacing(8);

    // Square gray tile with icon
    QWidget *tile = new QWidget(row);
    tile->setFixedSize(48, 48);
    tile->setStyleSheet("QWidget { background-color: #333333; border-radius: 6px; }");
    QVBoxLayout *tileLayout = new QVBoxLayout(tile);
    tileLayout->setContentsMargins(4, 4, 4, 4);
    tileLayout->setSpacing(0);

    QLabel *iconLabel = new QLabel(tile);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 18px; }");
    iconLabel->setText(entry.type == MediaEntry::Type::Video ? "🎥" : (entry.type == MediaEntry::Type::Audio ? "🎵" : "📄"));
    tileLayout->addWidget(iconLabel);

    // Right side: name on top, duration under it
    QVBoxLayout *textCol = new QVBoxLayout();
    textCol->setContentsMargins(0, 0, 0, 0);
    textCol->setSpacing(2);

    QLabel *nameLabel = new QLabel(entry.displayName, row);
    nameLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 13px; }");

    QLabel *durBelow = new QLabel(entry.durationText, row);
    durBelow->setStyleSheet("QLabel { color: #bbbbbb; font-size: 11px; }");

    textCol->addWidget(nameLabel);
    textCol->addWidget(durBelow);

    layout->addWidget(tile);
    layout->addLayout(textCol, 1);

    return row;
}

void RightSidebarWidget::rebuildList()
{
    m_listWidget->clear();
    const int tab = m_tabBar->currentIndex();
    for (const auto &entry : m_allEntries) {
        if (tab == 1 && entry.type != MediaEntry::Type::Video) continue; // Video tab
        if (tab == 2 && entry.type != MediaEntry::Type::Audio) continue; // Audio tab

        QListWidgetItem *item = new QListWidgetItem(m_listWidget);
        item->setSizeHint(QSize(0, 60));
        m_listWidget->addItem(item);
        m_listWidget->setItemWidget(item, createListRowWidget(entry));
    }
}

void RightSidebarWidget::onTabChanged(int)
{
    rebuildList();
}


