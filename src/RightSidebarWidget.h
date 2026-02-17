#ifndef RIGHTSIDEBARWIDGET_H
#define RIGHTSIDEBARWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabBar>
#include <QListWidget>
#include <QFileDialog>
#include <QLabel>
#include <QVector>
#include <QString>

struct MediaEntry {
    QString filePath;
    QString displayName;
    QString durationText; // e.g., 0:15
    enum class Type { Video, Audio, Unknown } type;
};

class RightSidebarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RightSidebarWidget(QWidget *parent = nullptr);
    void importMediaFile(const QString &path);

signals:
    void mediaImported(const MediaEntry &entry);
    void mediaSelected(const QString &filePath);

private slots:
    void onAddClicked();
    void onTabChanged(int index);

private:
    void setupUI();
    void addMediaToModel(const QString &path);
    void rebuildList();
    bool isVideo(const QString &path) const;
    bool isAudio(const QString &path) const;
    QWidget *createListRowWidget(const MediaEntry &entry) const;

    QVBoxLayout *m_mainLayout;
    QTabBar *m_tabBar;
    QPushButton *m_addButton;
    QListWidget *m_listWidget;

    QVector<MediaEntry> m_allEntries;
};

#endif // RIGHTSIDEBARWIDGET_H


