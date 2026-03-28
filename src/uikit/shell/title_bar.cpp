#include "uikit/shell/title_bar.h"

#include "uikit/controls/ui_button.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QWidget>

namespace uikit {
namespace {
const QString kMinGlyph = QString::fromUtf8("−");
const QString kMaxRestoreGlyph = QString::fromUtf8("□");
const QString kMaximizedGlyph = QString::fromUtf8("❐");
const QString kCloseGlyph = QString::fromUtf8("✕");
}

TitleBar::TitleBar(QWidget* parent)
    : QWidget(parent) {
    setProperty("uiRole", "titleBar");
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(false);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 0, 0, 0);
    layout->setSpacing(0);

    auto createActionButton = [this](const QString& text, const QString& action, const QString& objectName) {
        auto* button = new UiButton(text, this);
        button->setObjectName(objectName);
        button->setProperty("uiSurface", "titleBar");
        button->setProperty("uiAction", action);
        button->setVariant(UiButton::Variant::Icon);
        return button;
    };

    minButton_ = createActionButton(kMinGlyph, "minimize", "TitleBarMinButton");
    maxButton_ = createActionButton(kMaxRestoreGlyph, "maximize", "TitleBarMaxButton");
    maxButton_->setCheckable(true);
    closeButton_ = createActionButton(kCloseGlyph, "close", "TitleBarCloseButton");

    connect(minButton_, &QPushButton::clicked, this, &TitleBar::minimizeRequested);
    connect(maxButton_, &QPushButton::clicked, this, &TitleBar::maximizeRestoreRequested);
    connect(closeButton_, &QPushButton::clicked, this, &TitleBar::closeRequested);

    auto* actionGroup = new QWidget(this);
    actionGroup->setProperty("uiRole", "titleBarActions");
    auto* actionLayout = new QHBoxLayout(actionGroup);
    actionLayout->setContentsMargins(0, 0, 0, 0);
    actionLayout->setSpacing(0);
    actionLayout->addWidget(minButton_);
    actionLayout->addWidget(maxButton_);
    actionLayout->addWidget(closeButton_);

    layout->addStretch();
    layout->addWidget(actionGroup, 0, Qt::AlignRight);
}

void TitleBar::setMaximizedState(bool maximized) {
    if (!maxButton_) {
        return;
    }
    maxButton_->setChecked(maximized);
    maxButton_->setText(maximized ? kMaximizedGlyph : kMaxRestoreGlyph);
}

void TitleBar::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        dragging_ = true;
        if (window()) {
            dragAnchor_ = event->globalPosition().toPoint() - window()->frameGeometry().topLeft();
        } else {
            dragAnchor_ = event->globalPosition().toPoint();
        }
        event->accept();
        return;
    }
    QWidget::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent* event) {
    if (dragging_ && event->buttons().testFlag(Qt::LeftButton) && window() && !window()->isMaximized()) {
        window()->move(event->globalPosition().toPoint() - dragAnchor_);
        event->accept();
        return;
    }
    QWidget::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        dragging_ = false;
    }
    QWidget::mouseReleaseEvent(event);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent* event) {
    emit titleDoubleClicked();
    QWidget::mouseDoubleClickEvent(event);
}

}  // namespace uikit
