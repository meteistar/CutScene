#include "HeaderBarWidget.h"
#include <QApplication>
#include <QStyle>

HeaderBarWidget::HeaderBarWidget(QWidget *parent)
    : QWidget(parent)
    , m_layout(nullptr)
    , m_titleLabel(nullptr)
    , m_fileButton(nullptr)
    , m_editButton(nullptr)
    , m_viewButton(nullptr)
    , m_helpButton(nullptr)
    , m_spacer(nullptr)
    , m_undoButton(nullptr)
    , m_redoButton(nullptr)
    , m_settingsButton(nullptr)
    , m_importButton(nullptr)
    , m_exportButton(nullptr)
{
    setupUI();
}

void HeaderBarWidget::setupUI()
{
    // Set background color
    setStyleSheet("HeaderBarWidget { background-color: #2b2b2b; }");
    setFixedHeight(50);

    // Create main layout
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(16, 12, 16, 12);
    m_layout->setSpacing(8);

    setupMenuButtons();

    // Add spacer to push action buttons to the right
    m_spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_layout->addItem(m_spacer);

    setupActionButtons();
}

void HeaderBarWidget::setupMenuButtons()
{
    // Title label
    m_titleLabel = new QLabel("CutScene", this);
    m_titleLabel->setStyleSheet("QLabel { color: #ffffff; font-size: 16px; font-weight: bold; margin-top: 9px; min-height: 26px;}");
    m_layout->addWidget(m_titleLabel);

    // Menu buttons
    m_fileButton = new QPushButton("File", this);
    m_editButton = new QPushButton("Edit", this);
    m_viewButton = new QPushButton("View", this);
    m_helpButton = new QPushButton("Help", this);

    // Style menu buttons
    QString menuButtonStyle =
        "QPushButton { "
        "    color: #ffffff; "
        "    background-color: transparent; "
        "    border: none; "
        "    padding: 10px 6px; "
        "    font-size: 14px; "
        "    min-height: 26px; "
        "} "
        "QPushButton:hover { "
        "    background-color: #3b3b3b; "
        "    border-radius: 4px; "
        "}";

    m_fileButton->setStyleSheet(menuButtonStyle);
    m_editButton->setStyleSheet(menuButtonStyle);
    m_viewButton->setStyleSheet(menuButtonStyle);
    m_helpButton->setStyleSheet(menuButtonStyle);

    // Set alignment for menu buttons
    m_fileButton->setProperty("alignment", Qt::AlignVCenter);
    m_editButton->setProperty("alignment", Qt::AlignVCenter);
    m_viewButton->setProperty("alignment", Qt::AlignVCenter);
    m_helpButton->setProperty("alignment", Qt::AlignVCenter);

    // Connect signals
    connect(m_fileButton, &QPushButton::clicked, this, &HeaderBarWidget::onFileClicked);
    connect(m_editButton, &QPushButton::clicked, this, &HeaderBarWidget::onEditClicked);
    connect(m_viewButton, &QPushButton::clicked, this, &HeaderBarWidget::onViewClicked);
    connect(m_helpButton, &QPushButton::clicked, this, &HeaderBarWidget::onHelpClicked);

    // Add to layout
    m_layout->addWidget(m_fileButton);
    m_layout->addWidget(m_editButton);
    m_layout->addWidget(m_viewButton);
    m_layout->addWidget(m_helpButton);
}

void HeaderBarWidget::setupActionButtons()
{
    // Action buttons
    m_undoButton = new QPushButton("↶", this);
    m_redoButton = new QPushButton("↷", this);
    m_settingsButton = new QPushButton("⚙", this);
    m_importButton = new QPushButton("📥 Import", this);
    m_exportButton = new QPushButton("📤 Export", this);

    // Style action buttons
    QString actionButtonStyle =
        "QPushButton { "
        "    color: #ffffff; "
        "    background-color: transparent; "
        "    border: 1px solid #555555; "
        "    border-radius: 4px; "
        "    font-size: 14px; "
        "    min-width: 36px; "
        "    min-height: 20px; "
        "    padding: 8px 12px; "
        "} "
        "QPushButton:hover { "
        "    background-color: #3b3b3b; "
        "}";

    m_undoButton->setStyleSheet(actionButtonStyle);
    m_redoButton->setStyleSheet(actionButtonStyle);
    m_settingsButton->setStyleSheet(actionButtonStyle);
    m_importButton->setStyleSheet(actionButtonStyle);
    m_exportButton->setStyleSheet(actionButtonStyle);

    // Connect signals
    connect(m_undoButton, &QPushButton::clicked, this, &HeaderBarWidget::onUndoClicked);
    connect(m_redoButton, &QPushButton::clicked, this, &HeaderBarWidget::onRedoClicked);
    connect(m_settingsButton, &QPushButton::clicked, this, &HeaderBarWidget::onSettingsClicked);
    connect(m_importButton, &QPushButton::clicked, this, &HeaderBarWidget::onImportClicked);
    connect(m_exportButton, &QPushButton::clicked, this, &HeaderBarWidget::onExportClicked);

    // Add to layout
    m_layout->addWidget(m_undoButton);
    m_layout->addWidget(m_redoButton);
    m_layout->addWidget(m_settingsButton);
    m_layout->addWidget(m_importButton);
    m_layout->addWidget(m_exportButton);
}

void HeaderBarWidget::onFileClicked()
{
    qDebug() << "File menu clicked";
    emit fileClicked();
}

void HeaderBarWidget::onEditClicked()
{
    qDebug() << "Edit menu clicked";
    emit editClicked();
}

void HeaderBarWidget::onViewClicked()
{
    qDebug() << "View menu clicked";
    emit viewClicked();
}

void HeaderBarWidget::onHelpClicked()
{
    qDebug() << "Help menu clicked";
    emit helpClicked();
}

void HeaderBarWidget::onUndoClicked()
{
    qDebug() << "Undo clicked";
    emit undoClicked();
}

void HeaderBarWidget::onRedoClicked()
{
    qDebug() << "Redo clicked";
    emit redoClicked();
}

void HeaderBarWidget::onSettingsClicked()
{
    qDebug() << "Settings clicked";
    emit settingsClicked();
}

void HeaderBarWidget::onImportClicked()
{
    qDebug() << "Import clicked";
    emit importClicked();
}

void HeaderBarWidget::onExportClicked()
{
    qDebug() << "Export clicked";
    emit exportClicked();
}
