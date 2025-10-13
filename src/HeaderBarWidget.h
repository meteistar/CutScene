#ifndef HEADERBARWIDGET_H
#define HEADERBARWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>

class HeaderBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeaderBarWidget(QWidget *parent = nullptr);

signals:
    void fileClicked();
    void editClicked();
    void viewClicked();
    void helpClicked();
    void undoClicked();
    void redoClicked();
    void settingsClicked();
    void importClicked();
    void exportClicked();

private slots:
    void onFileClicked();
    void onEditClicked();
    void onViewClicked();
    void onHelpClicked();
    void onUndoClicked();
    void onRedoClicked();
    void onSettingsClicked();
    void onImportClicked();
    void onExportClicked();

private:
    void setupUI();
    void setupMenuButtons();
    void setupActionButtons();

    QHBoxLayout *m_layout;
    
    // Left side - Title and menu buttons
    QLabel *m_titleLabel;
    QPushButton *m_fileButton;
    QPushButton *m_editButton;
    QPushButton *m_viewButton;
    QPushButton *m_helpButton;
    
    // Spacer
    QSpacerItem *m_spacer;
    
    // Right side - Action buttons
    QPushButton *m_undoButton;
    QPushButton *m_redoButton;
    QPushButton *m_settingsButton;
    QPushButton *m_importButton;
    QPushButton *m_exportButton;
};

#endif // HEADERBARWIDGET_H
