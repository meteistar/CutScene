#pragma once
#include <QQuickItem>
#include <QWidget>
#include "DragLabel.h"

class DragLabelWrapper : public QQuickItem {
    Q_OBJECT
public:
    DragLabelWrapper(QQuickItem *parent = nullptr) : QQuickItem(parent) {
        setFlag(ItemHasContents, false);

        DragLabel *label = new DragLabel("Drag Me");
        label->setAttribute(Qt::WA_NativeWindow);
        QWidget *container = QWidget::createWindowContainer(label->windowHandle());
        container->setMinimumSize(200, 100);
        container->show();
    }
};
