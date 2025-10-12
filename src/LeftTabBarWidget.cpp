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
    effectsLayout->setSpacing(12);
    
    QLabel *effectsLabel = new QLabel("Effects", m_effectsContent);
    effectsLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 16px; font-weight: bold; margin-bottom: 8px; }");
    effectsLayout->addWidget(effectsLabel);
    
    // Brightness
    QLabel *brightnessLabel = new QLabel("Brightness", m_effectsContent);
    brightnessLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 12px; }");
    brightnessLabel->setAlignment(Qt::AlignLeft);
    effectsLayout->addWidget(brightnessLabel);
    
    QSlider *brightnessSlider = new QSlider(Qt::Horizontal, m_effectsContent);
    brightnessSlider->setRange(0, 100);
    brightnessSlider->setValue(50);
    brightnessSlider->setStyleSheet(
        "QSlider::groove:horizontal { "
        "    border: 1px solid #555555; "
        "    height: 6px; "
        "    background: #2b2b2b; "
        "    border-radius: 3px; "
        "} "
        "QSlider::handle:horizontal { "
        "    background: #0078d4; "
        "    border: 1px solid #0078d4; "
        "    width: 16px; "
        "    margin: -5px 0; "
        "    border-radius: 8px; "
        "} "
        "QSlider::handle:horizontal:hover { "
        "    background: #106ebe; "
        "} "
    );
    effectsLayout->addWidget(brightnessSlider);
    
    // Contrast
    QLabel *contrastLabel = new QLabel("Contrast", m_effectsContent);
    contrastLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 12px; }");
    contrastLabel->setAlignment(Qt::AlignLeft);
    effectsLayout->addWidget(contrastLabel);
    
    QSlider *contrastSlider = new QSlider(Qt::Horizontal, m_effectsContent);
    contrastSlider->setRange(0, 100);
    contrastSlider->setValue(50);
    contrastSlider->setStyleSheet(
        "QSlider::groove:horizontal { "
        "    border: 1px solid #555555; "
        "    height: 6px; "
        "    background: #2b2b2b; "
        "    border-radius: 3px; "
        "} "
        "QSlider::handle:horizontal { "
        "    background: #0078d4; "
        "    border: 1px solid #0078d4; "
        "    width: 16px; "
        "    margin: -5px 0; "
        "    border-radius: 8px; "
        "} "
        "QSlider::handle:horizontal:hover { "
        "    background: #106ebe; "
        "} "
    );
    effectsLayout->addWidget(contrastSlider);
    
    // Saturation
    QLabel *saturationLabel = new QLabel("Saturation", m_effectsContent);
    saturationLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 12px; }");
    saturationLabel->setAlignment(Qt::AlignLeft);
    effectsLayout->addWidget(saturationLabel);
    
    QSlider *saturationSlider = new QSlider(Qt::Horizontal, m_effectsContent);
    saturationSlider->setRange(0, 100);
    saturationSlider->setValue(50);
    saturationSlider->setStyleSheet(
        "QSlider::groove:horizontal { "
        "    border: 1px solid #555555; "
        "    height: 6px; "
        "    background: #2b2b2b; "
        "    border-radius: 3px; "
        "} "
        "QSlider::handle:horizontal { "
        "    background: #0078d4; "
        "    border: 1px solid #0078d4; "
        "    width: 16px; "
        "    margin: -5px 0; "
        "    border-radius: 8px; "
        "} "
        "QSlider::handle:horizontal:hover { "
        "    background: #106ebe; "
        "} "
    );
    effectsLayout->addWidget(saturationSlider);
    
    // Exposure
    QLabel *exposureLabel = new QLabel("Exposure", m_effectsContent);
    exposureLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 12px; }");
    exposureLabel->setAlignment(Qt::AlignLeft);
    effectsLayout->addWidget(exposureLabel);
    
    QSlider *exposureSlider = new QSlider(Qt::Horizontal, m_effectsContent);
    exposureSlider->setRange(0, 100);
    exposureSlider->setValue(50);
    exposureSlider->setStyleSheet(
        "QSlider::groove:horizontal { "
        "    border: 1px solid #555555; "
        "    height: 6px; "
        "    background: #2b2b2b; "
        "    border-radius: 3px; "
        "} "
        "QSlider::handle:horizontal { "
        "    background: #0078d4; "
        "    border: 1px solid #0078d4; "
        "    width: 16px; "
        "    margin: -5px 0; "
        "    border-radius: 8px; "
        "} "
        "QSlider::handle:horizontal:hover { "
        "    background: #106ebe; "
        "} "
    );
    effectsLayout->addWidget(exposureSlider);
    
    // Blur
    QLabel *blurLabel = new QLabel("Blur", m_effectsContent);
    blurLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 12px; }");
    blurLabel->setAlignment(Qt::AlignLeft);
    effectsLayout->addWidget(blurLabel);
    
    QSlider *blurSlider = new QSlider(Qt::Horizontal, m_effectsContent);
    blurSlider->setRange(0, 100);
    blurSlider->setValue(0);
    blurSlider->setStyleSheet(
        "QSlider::groove:horizontal { "
        "    border: 1px solid #555555; "
        "    height: 6px; "
        "    background: #2b2b2b; "
        "    border-radius: 3px; "
        "} "
        "QSlider::handle:horizontal { "
        "    background: #0078d4; "
        "    border: 1px solid #0078d4; "
        "    width: 16px; "
        "    margin: -5px 0; "
        "    border-radius: 8px; "
        "} "
        "QSlider::handle:horizontal:hover { "
        "    background: #106ebe; "
        "} "
    );
    effectsLayout->addWidget(blurSlider);
    
    // Vignette
    QLabel *vignetteLabel = new QLabel("Vignette", m_effectsContent);
    vignetteLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 12px; }");
    vignetteLabel->setAlignment(Qt::AlignLeft);
    effectsLayout->addWidget(vignetteLabel);
    
    QSlider *vignetteSlider = new QSlider(Qt::Horizontal, m_effectsContent);
    vignetteSlider->setRange(0, 100);
    vignetteSlider->setValue(0);
    vignetteSlider->setStyleSheet(
        "QSlider::groove:horizontal { "
        "    border: 1px solid #555555; "
        "    height: 6px; "
        "    background: #2b2b2b; "
        "    border-radius: 3px; "
        "} "
        "QSlider::handle:horizontal { "
        "    background: #0078d4; "
        "    border: 1px solid #0078d4; "
        "    width: 16px; "
        "    margin: -5px 0; "
        "    border-radius: 8px; "
        "} "
        "QSlider::handle:horizontal:hover { "
        "    background: #106ebe; "
        "} "
    );
    effectsLayout->addWidget(vignetteSlider);
    
    effectsLayout->addStretch();
    m_contentStack->addWidget(m_effectsContent);
    
    // Filters content
    m_filtersContent = new QWidget();
    m_filtersContent->setStyleSheet("QWidget { background-color: #1e1e1e; }");
    QVBoxLayout *filtersLayout = new QVBoxLayout(m_filtersContent);
    filtersLayout->setContentsMargins(16, 16, 16, 16);
    filtersLayout->setSpacing(16);
    
    QLabel *filtersLabel = new QLabel("Filters", m_filtersContent);
    filtersLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 16px; font-weight: bold; margin-bottom: 8px; }");
    filtersLayout->addWidget(filtersLabel);
    
    // Create grid layout for filter buttons
    QGridLayout *filtersGrid = new QGridLayout();
    filtersGrid->setSpacing(8);
    
    // Filter button names
    QStringList filterNames = {"Original", "Vintage", "B&W", "Sepia", "Cool", "Warm", "Vivid", "Dramatic"};
    
    // Create filter buttons
    for (int i = 0; i < 8; ++i) {
        QPushButton *filterButton = new QPushButton(filterNames[i], m_filtersContent);
        filterButton->setFixedSize(100, 60);
        filterButton->setStyleSheet(
            "QPushButton { "
            "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "        stop:0 #8b5cf6, stop:1 #7c3aed); "
            "    color: #ffffff; "
            "    border: none; "
            "    border-radius: 8px; "
            "    font-size: 12px; "
            "    font-weight: bold; "
            "} "
            "QPushButton:hover { "
            "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "        stop:0 #a78bfa, stop:1 #8b5cf6); "
            "    border: 2px solid #ffffff; "
            "} "
            "QPushButton:pressed { "
            "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "        stop:0 #7c3aed, stop:1 #6d28d9); "
            "} "
        );
        
        // Add to grid (2 columns, 4 rows)
        int row = i / 2;
        int col = i % 2;
        filtersGrid->addWidget(filterButton, row, col);
        
        // Connect signal
        connect(filterButton, &QPushButton::clicked, [filterNames, i]() {
            qDebug() << "Filter clicked:" << filterNames[i];
        });
    }
    
    filtersLayout->addLayout(filtersGrid);
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
