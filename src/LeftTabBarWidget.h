#ifndef LEFTTABBARWIDGET_H
#define LEFTTABBARWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QLabel>
#include <QGridLayout>

class LeftTabBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeftTabBarWidget(QWidget *parent = nullptr);

signals:
    void tabChanged(int index);

private slots:
    void onTabClicked();

private:
    void setupUI();
    void setupTabs();
    void setupContent();
    void switchTab(int index);

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_tabLayout;
    QStackedWidget *m_contentStack;
    
    // Tab buttons
    QPushButton *m_effectsButton;
    QPushButton *m_filtersButton;
    QPushButton *m_textButton;
    
    // Content widgets
    QWidget *m_effectsContent;
    QWidget *m_filtersContent;
    QWidget *m_textContent;
    
    int m_currentTab;
};

#endif // LEFTTABBARWIDGET_H
