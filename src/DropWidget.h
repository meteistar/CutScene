#pragma once
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

class DropWidget : public QLabel {
    Q_OBJECT
public:
    DropWidget(QWidget *parent = nullptr) : QLabel("Drop Here", parent) {
        setAcceptDrops(true);
        setAlignment(Qt::AlignCenter);
        setStyleSheet("background: lightgreen; border: 2px dashed black;");
    }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasText()) {
            event->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent *event) override {
        setText(event->mimeData()->text());
    }
};
