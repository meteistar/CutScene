#pragma once
#include <QLabel>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

class DragLabel : public QLabel {
    Q_OBJECT
public:
    DragLabel(const QString &text, QWidget *parent = nullptr) : QLabel(text, parent) {
        setAlignment(Qt::AlignCenter);
        setStyleSheet("background: lightblue; border: 1px solid black;");
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            auto *mimeData = new QMimeData;
            mimeData->setText(text());

            auto *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
        }
        QLabel::mousePressEvent(event);
    }
};
