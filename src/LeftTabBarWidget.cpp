#include "LeftTabBarWidget.h"
#include <QDebug>

LeftTabBarWidget::LeftTabBarWidget(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_tabLayout(nullptr)
    , m_contentStack(nullptr)
    , m_effectsButton(nullptr)
    , m_filtersButton(nullptr)
    , m_textButton(nullptr)
    , m_effectsContent(nullptr)
    , m_filtersContent(nullptr)
    , m_textContent(nullptr)
    , m_currentTab(0)
{
    setupUI();
}

void LeftTabBarWidget::setupUI()
{
    // Set background color and fixed width
    setStyleSheet("LeftTabBarWidget { background-color: #1e1e1e; }");
    setFixedWidth(250);
    
    // Create main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    
    setupTabs();
    setupContent();
}

void LeftTabBarWidget::setupTabs()
{
    // Create tab layout
    m_tabLayout = new QHBoxLayout();
    m_tabLayout->setContentsMargins(0, 0, 0, 0);
    m_tabLayout->setSpacing(0);
    
    // Create tab buttons
    m_effectsButton = new QPushButton("Effects", this);
    m_filtersButton = new QPushButton("Filters", this);
    m_textButton = new QPushButton("Text", this);
    
    // Style tab buttons
    QString tabButtonStyle = 
        "QPushButton { "
        "    color: #ffffff; "
        "    background-color: #2b2b2b; "
        "    border: none; "
        "    border-bottom: 2px solid transparent; "
        "    padding: 12px 16px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "} "
        "QPushButton:hover { "
        "    background-color: #3b3b3b; "
        "} "
        "QPushButton:pressed { "
        "    background-color: #404040; "
        "}";
    
    QString activeTabStyle = 
        "QPushButton { "
        "    color: #ffffff; "
        "    background-color: #1e1e1e; "
        "    border: none; "
        "    border-bottom: 2px solid #0078d4; "
        "    padding: 12px 16px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "}";
    
    m_effectsButton->setStyleSheet(activeTabStyle);
    m_filtersButton->setStyleSheet(tabButtonStyle);
    m_textButton->setStyleSheet(tabButtonStyle);
    
    // Connect signals
    connect(m_effectsButton, &QPushButton::clicked, this, &LeftTabBarWidget::onTabClicked);
    connect(m_filtersButton, &QPushButton::clicked, this, &LeftTabBarWidget::onTabClicked);
    connect(m_textButton, &QPushButton::clicked, this, &LeftTabBarWidget::onTabClicked);
    
    // Add buttons to layout
    m_tabLayout->addWidget(m_effectsButton);
    m_tabLayout->addWidget(m_filtersButton);
    m_tabLayout->addWidget(m_textButton);
    
    m_mainLayout->addLayout(m_tabLayout);
}

void LeftTabBarWidget::setupContent()
{
    // Create stacked widget for content
    m_contentStack = new QStackedWidget(this);
    
    // Effects content
    m_effectsContent = new QWidget();
    m_effectsContent->setStyleSheet("QWidget { background-color: #1e1e1e; }");
    QVBoxLayout *effectsLayout = new QVBoxLayout(m_effectsContent);
    effectsLayout->setContentsMargins(16, 16, 16, 16);
    
    QLabel *effectsLabel = new QLabel("Effects", m_effectsContent);
    effectsLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 16px; font-weight: bold; margin-bottom: 16px; }");
    effectsLayout->addWidget(effectsLabel);
    
    QLabel *effectsDesc = new QLabel("Video effects and transitions", m_effectsContent);
    effectsDesc->setStyleSheet("QLabel { color: #cccccc; font-size: 12px; }");
    effectsLayout->addWidget(effectsDesc);
    
    effectsLayout->addStretch();
    m_contentStack->addWidget(m_effectsContent);
    
    // Filters content
    m_filtersContent = new QWidget();
    m_filtersContent->setStyleSheet("QWidget { background-color: #1e1e1e; }");
    QVBoxLayout *filtersLayout = new QVBoxLayout(m_filtersContent);
    filtersLayout->setContentsMargins(16, 16, 16, 16);
    
    QLabel *filtersLabel = new QLabel("Filters", m_filtersContent);
    filtersLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 16px; font-weight: bold; margin-bottom: 16px; }");
    filtersLayout->addWidget(filtersLabel);
    
    QLabel *filtersDesc = new QLabel("Color correction and filters", m_filtersContent);
    filtersDesc->setStyleSheet("QLabel { color: #cccccc; font-size: 12px; }");
    filtersLayout->addWidget(filtersDesc);
    
    filtersLayout->addStretch();
    m_contentStack->addWidget(m_filtersContent);
    
    // Text content
    m_textContent = new QWidget();
    m_textContent->setStyleSheet("QWidget { background-color: #1e1e1e; }");
    QVBoxLayout *textLayout = new QVBoxLayout(m_textContent);
    textLayout->setContentsMargins(16, 16, 16, 16);
    
    QLabel *textLabel = new QLabel("Text", m_textContent);
    textLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 16px; font-weight: bold; margin-bottom: 16px; }");
    textLayout->addWidget(textLabel);
    
    QLabel *textDesc = new QLabel("Text overlays and titles", m_textContent);
    textDesc->setStyleSheet("QLabel { color: #cccccc; font-size: 12px; }");
    textLayout->addWidget(textDesc);
    
    textLayout->addStretch();
    m_contentStack->addWidget(m_textContent);
    
    m_mainLayout->addWidget(m_contentStack);
}

void LeftTabBarWidget::onTabClicked()
{
    QPushButton *sender = qobject_cast<QPushButton*>(QObject::sender());
    if (!sender) return;
    
    int index = -1;
    if (sender == m_effectsButton) {
        index = 0;
    } else if (sender == m_filtersButton) {
        index = 1;
    } else if (sender == m_textButton) {
        index = 2;
    }
    
    if (index != -1) {
        switchTab(index);
    }
}

void LeftTabBarWidget::switchTab(int index)
{
    if (index == m_currentTab) return;
    
    m_currentTab = index;
    
    // Update button styles
    QString tabButtonStyle = 
        "QPushButton { "
        "    color: #ffffff; "
        "    background-color: #2b2b2b; "
        "    border: none; "
        "    border-bottom: 2px solid transparent; "
        "    padding: 12px 16px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "} "
        "QPushButton:hover { "
        "    background-color: #3b3b3b; "
        "} "
        "QPushButton:pressed { "
        "    background-color: #404040; "
        "}";
    
    QString activeTabStyle = 
        "QPushButton { "
        "    color: #ffffff; "
        "    background-color: #1e1e1e; "
        "    border: none; "
        "    border-bottom: 2px solid #0078d4; "
        "    padding: 12px 16px; "
        "    font-size: 14px; "
        "    font-weight: bold; "
        "}";
    
    m_effectsButton->setStyleSheet(index == 0 ? activeTabStyle : tabButtonStyle);
    m_filtersButton->setStyleSheet(index == 1 ? activeTabStyle : tabButtonStyle);
    m_textButton->setStyleSheet(index == 2 ? activeTabStyle : tabButtonStyle);
    
    // Switch content
    m_contentStack->setCurrentIndex(index);
    
    emit tabChanged(index);
    
    qDebug() << "Switched to tab:" << index;
}
