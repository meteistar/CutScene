#include "../src/DropWidget.h"

#include <QMimeData>
#include <gtest/gtest.h>

class TestableDropWidget : public DropWidget {
public:
    using DropWidget::DropWidget;
    using DropWidget::dragEnterEvent;
    using DropWidget::dropEvent;
};

TEST(DropWidgetTest, ConstructorSetsExpectedDefaults)
{
    TestableDropWidget widget;

    EXPECT_EQ(widget.text().toStdString(), "Drop Here");
    EXPECT_TRUE(widget.acceptDrops());
    EXPECT_EQ(widget.alignment(), Qt::AlignCenter);
}

TEST(DropWidgetTest, DragEnterAcceptsTextMimeData)
{
    TestableDropWidget widget;
    QMimeData mimeData;
    mimeData.setText("clip.mp4");

    QDragEnterEvent event(QPoint(5, 5), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);

    widget.dragEnterEvent(&event);

    EXPECT_TRUE(event.isAccepted());
}

TEST(DropWidgetTest, DropEventUpdatesDisplayedText)
{
    TestableDropWidget widget;
    QMimeData mimeData;
    mimeData.setText("new media");

    QDropEvent event(QPointF(10, 10), Qt::CopyAction, &mimeData, Qt::LeftButton, Qt::NoModifier);

    widget.dropEvent(&event);

    EXPECT_EQ(widget.text().toStdString(), "new media");
}
