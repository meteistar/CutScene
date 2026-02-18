#include "../src/HeaderBarWidget.h"

#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QList>
#include <gtest/gtest.h>

namespace {
QPushButton *findButtonByText(HeaderBarWidget &widget, const QString &text)
{
    const QList<QPushButton *> buttons = widget.findChildren<QPushButton *>();
    for (QPushButton *button : buttons) {
        if (button && button->text() == text) {
            return button;
        }
    }
    return nullptr;
}
}

TEST(HeaderBarWidgetTest, ConstructorSetsExpectedTitleAndHeight)
{
    HeaderBarWidget widget;

    EXPECT_EQ(widget.minimumHeight(), 50);
    EXPECT_EQ(widget.maximumHeight(), 50);

    const QList<QLabel *> labels = widget.findChildren<QLabel *>();
    bool foundTitle = false;
    for (QLabel *label : labels) {
        if (label && label->text() == "CutScene") {
            foundTitle = true;
            break;
        }
    }
    EXPECT_TRUE(foundTitle);
}

TEST(HeaderBarWidgetTest, FileButtonClickEmitsFileClickedSignal)
{
    HeaderBarWidget widget;
    int emissionCount = 0;
    QObject::connect(&widget, &HeaderBarWidget::fileClicked, [&emissionCount]() {
        ++emissionCount;
    });

    QPushButton *fileButton = findButtonByText(widget, "File");
    ASSERT_NE(fileButton, nullptr);

    fileButton->click();
    EXPECT_EQ(emissionCount, 1);
}
