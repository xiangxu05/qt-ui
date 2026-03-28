#include "uikit/shell/navigation_panel.h"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QEasingCurve>
#include <QEnterEvent>
#include <QEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QScrollBar>
#include <QSignalBlocker>
#include <QSizePolicy>
#include <QStyle>
#include <QToolButton>
#include <QWheelEvent>
#include <QVBoxLayout>

namespace uikit {
namespace {
constexpr int kVerticalNavWidth = 220;
constexpr int kVerticalCollapsedWidth = 60;
constexpr int kVerticalItemHeight = 36;
constexpr int kHorizontalNavHeight = 40;
constexpr int kVerticalHeaderTop = 50;
constexpr int kVerticalOuterMarginX = 30;
constexpr int kVerticalOuterMarginBottom = 70;
}

UiNavigationPanel::UiNavigationPanel(QWidget* parent)
    : QWidget(parent) {
    setProperty("uiRole", "navigation");
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);
    buttonGroup_ = new QButtonGroup(this);
    buttonGroup_->setExclusive(true);
    rebuildLayout();
}

void UiNavigationPanel::setModel(const Model& model) {
    model_ = model;
    rebuildLayout();
    activateFirstItemIfAny();
}

const UiNavigationPanel::Model& UiNavigationPanel::model() const {
    return model_;
}

void UiNavigationPanel::activateItem(const QString& key) {
    for (auto* item : items_) {
        if (item && item->property("navKey").toString() == key) {
            item->click();
            return;
        }
    }
}

void UiNavigationPanel::setPosition(Position position) {
    if (position_ == position) {
        return;
    }
    QString activeKey;
    for (auto* item : items_) {
        if (item && item->isChecked()) {
            activeKey = item->property("navKey").toString();
            break;
        }
    }
    position_ = position;
    if (isHorizontal()) {
        collapsed_ = false;
    }
    rebuildLayout();
    if (!activeKey.isEmpty()) {
        activateItem(activeKey);
    } else {
        activateFirstItemIfAny();
    }
}

UiNavigationPanel::Position UiNavigationPanel::position() const {
    return position_;
}

void UiNavigationPanel::setCollapsed(bool collapsed) {
    if (isHorizontal()) {
        collapsed_ = false;
        return;
    }
    if (collapsed_ == collapsed) {
        return;
    }
    QString activeKey;
    for (auto* item : items_) {
        if (item && item->isChecked()) {
            activeKey = item->property("navKey").toString();
            break;
        }
    }
    collapsed_ = collapsed;
    rebuildLayout();
    if (!activeKey.isEmpty()) {
        activateItem(activeKey);
    } else {
        activateFirstItemIfAny();
    }
}

bool UiNavigationPanel::isCollapsed() const {
    return collapsed_;
}

bool UiNavigationPanel::isHorizontal() const {
    return position_ == Position::Top || position_ == Position::Bottom;
}

void UiNavigationPanel::enterEvent(QEnterEvent* event) {
    setOverlayScrollBarVisible(true);
    QWidget::enterEvent(event);
}

void UiNavigationPanel::leaveEvent(QEvent* event) {
    setOverlayScrollBarVisible(false);
    QWidget::leaveEvent(event);
}

void UiNavigationPanel::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateCollapseToggleGeometry();
}

bool UiNavigationPanel::eventFilter(QObject* watched, QEvent* event) {
    if (listScrollArea_ && watched == listScrollArea_->viewport() && event &&
        (event->type() == QEvent::Wheel || event->type() == QEvent::Resize)) {
        if (event->type() == QEvent::Wheel) {
            auto* wheelEvent = static_cast<QWheelEvent*>(event);
            const QPoint delta = !wheelEvent->pixelDelta().isNull() ? wheelEvent->pixelDelta()
                                                                     : wheelEvent->angleDelta() / 2;
            const bool horizontal = isHorizontal();
            const int axisDelta = horizontal ? (delta.x() != 0 ? delta.x() : delta.y()) : delta.y();
            if (sourceScrollBar_ && axisDelta != 0) {
                const int target = qBound(sourceScrollBar_->minimum(),
                                          sourceScrollBar_->value() - axisDelta,
                                          sourceScrollBar_->maximum());
                animateScrollTo(target);
            }
            return true;
        }
        updateOverlayScrollBarGeometry();
    }
    return QWidget::eventFilter(watched, event);
}

void UiNavigationPanel::setOverlayScrollBarVisible(bool visible) {
    if (!overlayScrollBar_ || !sourceScrollBar_) {
        return;
    }
    if (!visible || sourceScrollBar_->maximum() <= sourceScrollBar_->minimum()) {
        overlayScrollBar_->hide();
        return;
    }
    overlayScrollBar_->show();
    overlayScrollBar_->raise();
}

void UiNavigationPanel::animateScrollTo(int targetValue) {
    if (!sourceScrollBar_) {
        return;
    }
    if (scrollAnimation_ && scrollAnimation_->targetObject() != sourceScrollBar_) {
        scrollAnimation_->deleteLater();
        scrollAnimation_ = nullptr;
    }
    if (!scrollAnimation_) {
        scrollAnimation_ = new QPropertyAnimation(sourceScrollBar_, "value", this);
        scrollAnimation_->setEasingCurve(QEasingCurve::OutCubic);
    }
    if (scrollAnimation_->state() == QAbstractAnimation::Running) {
        scrollAnimation_->stop();
    }
    scrollAnimation_->setDuration(140);
    scrollAnimation_->setStartValue(sourceScrollBar_->value());
    scrollAnimation_->setEndValue(targetValue);
    scrollAnimation_->start();
    setOverlayScrollBarVisible(true);
}

void UiNavigationPanel::updateOverlayScrollBarGeometry() {
    if (!listScrollArea_ || !overlayScrollBar_ || !listScrollArea_->viewport()) {
        return;
    }
    const QRect viewportRect = listScrollArea_->viewport()->rect();
    const int barWidth = 4;
    const int margin = 2;
    overlayScrollBar_->setGeometry(viewportRect.width() - barWidth - margin,
                                   margin,
                                   barWidth,
                                   qMax(0, viewportRect.height() - margin * 2));
}

QWidget* UiNavigationPanel::buildHeaderWidget() {
    if (model_.header.logo.isNull() && model_.header.title.isEmpty()) {
        return nullptr;
    }
    const bool compactHeader = !isHorizontal() && collapsed_;
    auto* header = new QWidget(this);
    header->setProperty("uiRole", "navigationHeader");
    header->setAttribute(Qt::WA_StyledBackground, true);
    auto* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(isHorizontal() ? QMargins(2, 0, 2, 0) : QMargins(6, 6, 6, 8));
    headerLayout->setSpacing(8);

    if (!model_.header.logo.isNull()) {
        auto* logo = new QLabel(header);
        logo->setProperty("uiRole", "navigationLogo");
        logo->setPixmap(model_.header.logo);
        logo->setFixedSize(24, 24);
        logo->setScaledContents(true);
        headerLayout->addWidget(logo, 0, Qt::AlignVCenter);
    }

    if (!compactHeader) {
        auto* textHost = new QWidget(header);
        textHost->setProperty("uiRole", "navigationTextHost");
        textHost->setAttribute(Qt::WA_StyledBackground, true);
        auto* textLayout = new QVBoxLayout(textHost);
        textLayout->setContentsMargins(0, 0, 0, 0);
        textLayout->setSpacing(0);
        if (!model_.header.title.isEmpty()) {
            auto* title = new QLabel(model_.header.title, textHost);
            title->setProperty("uiRole", "navigationTitle");
            textLayout->addWidget(title);
        }
        headerLayout->addWidget(textHost, 1);
    }
    return header;
}

void UiNavigationPanel::ensureCollapseToggle() {
    if (isHorizontal()) {
        if (collapseToggle_) {
            collapseToggle_->hide();
        }
        return;
    }
    if (!collapseToggle_) {
        collapseToggle_ = new QToolButton(this);
        collapseToggle_->setProperty("uiRole", "navigationCollapseButton");
        collapseToggle_->setCheckable(false);
        collapseToggle_->setAutoRaise(true);
        collapseToggle_->setCursor(Qt::PointingHandCursor);
        collapseToggle_->setFixedSize(20, 20);
        connect(collapseToggle_, &QToolButton::clicked, this, [this]() {
            setCollapsed(!collapsed_);
        });
    }
    collapseToggle_->show();
    refreshCollapseToggle();
    updateCollapseToggleGeometry();
}

void UiNavigationPanel::updateCollapseToggleGeometry() {
    if (!collapseToggle_ || isHorizontal()) {
        return;
    }
    const int pad = 8;
    const QSize size = collapseToggle_->size();
    const int x = (position_ == Position::Right) ? pad : qMax(pad, width() - size.width() - pad);
    collapseToggle_->move(x, pad);
    collapseToggle_->raise();
}

void UiNavigationPanel::refreshCollapseToggle() {
    if (!collapseToggle_) {
        return;
    }
    QStyle::StandardPixmap pixmap = QStyle::SP_ArrowLeft;
    if (position_ == Position::Right) {
        pixmap = collapsed_ ? QStyle::SP_ArrowLeft : QStyle::SP_ArrowRight;
    } else {
        pixmap = collapsed_ ? QStyle::SP_ArrowRight : QStyle::SP_ArrowLeft;
    }
    collapseToggle_->setIcon(style()->standardIcon(pixmap));
    collapseToggle_->setToolTip(collapsed_ ? tr("展开导航栏") : tr("收起导航栏"));
}

void UiNavigationPanel::refreshItemPresentation() {
    const bool horizontal = isHorizontal();
    for (auto* item : items_) {
        if (!item) {
            continue;
        }
        const bool hasIcon = item->property("navHasIcon").toBool();
        const QString navText = item->property("navText").toString();
        item->setProperty("navCollapsed", !horizontal && collapsed_);
        Qt::ToolButtonStyle toolStyle = Qt::ToolButtonTextBesideIcon;
        if (horizontal) {
            item->setText(navText);
            toolStyle = hasIcon ? Qt::ToolButtonIconOnly : Qt::ToolButtonTextOnly;
            item->setToolTip(navText);
        } else if (collapsed_) {
            item->setText(hasIcon ? navText : navText.left(1));
            toolStyle = hasIcon ? Qt::ToolButtonIconOnly : Qt::ToolButtonTextOnly;
            item->setToolTip(navText);
        } else {
            item->setText(navText);
            toolStyle = Qt::ToolButtonTextBesideIcon;
            item->setToolTip(QString());
        }
        item->setToolButtonStyle(toolStyle);
        item->setMinimumWidth(0);
        item->setMaximumWidth(QWIDGETSIZE_MAX);
        item->style()->unpolish(item);
        item->style()->polish(item);
        item->updateGeometry();
        item->update();
    }
}

QToolButton* UiNavigationPanel::createItemButton(const ItemModel& item, int index) {
    auto* button = new QToolButton(this);
    button->setText(item.text);
    button->setIcon(item.icon);
    button->setCheckable(true);
    const bool hasIcon = !item.icon.isNull();
    button->setProperty("navHasIcon", hasIcon);
    button->setProperty("navText", item.text);
    button->setProperty("navKey", item.key);
    button->setProperty("navIndex", index);
    button->setProperty("uiRole", "navigationItem");
    if (isHorizontal()) {
        button->setProperty("navOrientation", "horizontal");
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        button->setMinimumHeight(kHorizontalNavHeight);
        button->setMaximumHeight(kHorizontalNavHeight);
        button->setContentsMargins(0, 0, 0, 0);
    } else {
        button->setProperty("navOrientation", "vertical");
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        button->setMinimumHeight(kVerticalItemHeight);
        button->setMaximumHeight(kVerticalItemHeight);
    }
    button->setIconSize(QSize(16, 16));
    buttonGroup_->addButton(button);
    connect(button, &QToolButton::clicked, this, [this, index, item]() {
        emit itemActivated(index, item.key);
        emit itemActivatedWithWidget(index, item.key, item.page);
    });
    items_.append(button);
    refreshItemPresentation();
    return button;
}

void UiNavigationPanel::appendSection(QBoxLayout* layout,
                                    const QString& title,
                                    const QList<ItemModel>& items,
                                    int* indexCounter) {
    if (items.isEmpty()) {
        return;
    }
    if (!title.isEmpty()) {
        auto* sectionLabel = new QLabel(title, this);
        sectionLabel->setProperty("uiRole", "navigationSectionTitle");
        layout->addWidget(sectionLabel);
    }
    for (const auto& item : items) {
        layout->addWidget(createItemButton(item, (*indexCounter)++));
    }
}

QWidget* UiNavigationPanel::buildListHostWidget() {
    auto* listHost = new QWidget(listScrollArea_);
    listHost->setProperty("uiRole", "navigationListHost");
    listHost->setAttribute(Qt::WA_StyledBackground, true);

    int indexCounter = 0;
    if (isHorizontal()) {
        auto* listLayout = new QHBoxLayout(listHost);
        listLayout->setContentsMargins(2, 0, 2, 0);
        listLayout->setSpacing(6);
        appendSection(listLayout, "", model_.primaryItems, &indexCounter);
        for (const auto& section : model_.secondarySections) {
            if (!section.items.isEmpty() && indexCounter > 0) {
                auto* separator = new QFrame(listHost);
                separator->setProperty("uiRole", "navigationSectionSeparator");
                separator->setFrameShape(QFrame::VLine);
                separator->setFrameShadow(QFrame::Plain);
                separator->setFixedWidth(1);
                separator->setFixedHeight(18);
                listLayout->addWidget(separator, 0, Qt::AlignVCenter);
            }
            appendSection(listLayout, "", section.items, &indexCounter);
        }
        listLayout->addStretch();
    } else {
        auto* listLayout = new QVBoxLayout(listHost);
        listLayout->setContentsMargins(2, 8, 2, 8);
        listLayout->setSpacing(6);
        appendSection(listLayout, "", model_.primaryItems, &indexCounter);
        for (const auto& section : model_.secondarySections) {
            appendSection(listLayout, section.title, section.items, &indexCounter);
        }
        listLayout->addStretch();
    }
    return listHost;
}

void UiNavigationPanel::configureRootLayout() {
    const bool horizontal = isHorizontal();
    if (!layout_) {
        layout_ = horizontal ? static_cast<QBoxLayout*>(new QHBoxLayout(this))
                             : static_cast<QBoxLayout*>(new QVBoxLayout(this));
    } else {
        auto* box = qobject_cast<QBoxLayout*>(layout_);
        box->setDirection(horizontal ? QBoxLayout::LeftToRight : QBoxLayout::TopToBottom);
    }

    if (horizontal) {
        layout_->setContentsMargins(20, 0, 20, 0);
        layout_->setSpacing(8);
    } else {
        const int sideMargin = collapsed_ ? 6 : kVerticalOuterMarginX;
        layout_->setContentsMargins(sideMargin, kVerticalHeaderTop, sideMargin, kVerticalOuterMarginBottom);
        layout_->setSpacing(8);
    }
}

void UiNavigationPanel::configureScrollContainer() {
    const bool horizontal = isHorizontal();
    listScrollArea_->setHorizontalScrollBarPolicy(horizontal ? Qt::ScrollBarAlwaysOff : Qt::ScrollBarAlwaysOff);
    listScrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sourceScrollBar_ = horizontal ? listScrollArea_->horizontalScrollBar()
                                  : listScrollArea_->verticalScrollBar();
    sourceScrollBar_->setProperty("uiRole", "navigationSourceScrollBar");
    sourceScrollBar_->setFixedSize(0, 0);
    sourceScrollBar_->hide();
    if (scrollAnimation_ && scrollAnimation_->targetObject() != sourceScrollBar_) {
        scrollAnimation_->deleteLater();
        scrollAnimation_ = nullptr;
    }
}

void UiNavigationPanel::applyPanelSizeForPosition() {
    const bool horizontal = isHorizontal();
    if (horizontal) {
        setFixedHeight(kHorizontalNavHeight);
        setMinimumWidth(260);
        setMaximumWidth(QWIDGETSIZE_MAX);
    } else {
        setFixedWidth(collapsed_ ? kVerticalCollapsedWidth : kVerticalNavWidth);
        setMinimumHeight(240);
        setMaximumHeight(QWIDGETSIZE_MAX);
    }
}

void UiNavigationPanel::clearLayout() {
    if (!layout_) {
        return;
    }
    while (QLayoutItem* item = layout_->takeAt(0)) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

void UiNavigationPanel::activateFirstItemIfAny() {
    if (items_.isEmpty()) {
        return;
    }
    if (!items_.first()->isChecked()) {
        items_.first()->click();
    }
}

void UiNavigationPanel::rebuildLayout() {
    if (layout_) {
        clearLayout();
    }
    configureRootLayout();

    setProperty("uiPosition",
                position_ == Position::Top
                    ? "top"
                    : (position_ == Position::Bottom ? "bottom"
                                                      : (position_ == Position::Right ? "right" : "left")));
    style()->unpolish(this);
    style()->polish(this);

    for (auto* button : items_) {
        buttonGroup_->removeButton(button);
    }
    items_.clear();
    listScrollArea_ = nullptr;
    sourceScrollBar_ = nullptr;
    overlayScrollBar_ = nullptr;
    ensureCollapseToggle();

    if (auto* header = buildHeaderWidget()) {
        if (isHorizontal()) {
            layout_->addWidget(header, 0, Qt::AlignVCenter);
        } else {
            layout_->addWidget(header);
        }
    }

    listScrollArea_ = new QScrollArea(this);
    listScrollArea_->setProperty("uiRole", "navigationScrollArea");
    listScrollArea_->setFrameShape(QFrame::NoFrame);
    listScrollArea_->setWidgetResizable(true);
    listScrollArea_->setAttribute(Qt::WA_StyledBackground, true);
    listScrollArea_->viewport()->setProperty("uiRole", "navigationScrollViewport");
    listScrollArea_->viewport()->setAttribute(Qt::WA_StyledBackground, true);
    listScrollArea_->viewport()->installEventFilter(this);
    configureScrollContainer();

    auto* listHost = buildListHostWidget();
    listScrollArea_->setWidget(listHost);
    layout_->addWidget(listScrollArea_, 1, Qt::AlignVCenter);

    if (!isHorizontal()) {
        overlayScrollBar_ = new QScrollBar(Qt::Vertical, listScrollArea_->viewport());
        overlayScrollBar_->setProperty("uiRole", "navigationOverlayScrollBar");
        overlayScrollBar_->hide();

        connect(sourceScrollBar_, &QScrollBar::rangeChanged, this, [this](int min, int max) {
            if (!overlayScrollBar_) {
                return;
            }
            overlayScrollBar_->setRange(min, max);
            overlayScrollBar_->setPageStep(sourceScrollBar_->pageStep());
            updateOverlayScrollBarGeometry();
            if (max <= min) {
                overlayScrollBar_->hide();
            }
        });
        connect(sourceScrollBar_, &QScrollBar::valueChanged, this, [this](int value) {
            if (!overlayScrollBar_) {
                return;
            }
            QSignalBlocker blocker(overlayScrollBar_);
            overlayScrollBar_->setValue(value);
            if (underMouse()) {
                setOverlayScrollBarVisible(true);
            }
        });
        connect(overlayScrollBar_, &QScrollBar::valueChanged, sourceScrollBar_, &QScrollBar::setValue);
        connect(overlayScrollBar_, &QScrollBar::sliderMoved, this, [this](int) {
            setOverlayScrollBarVisible(true);
        });
        updateOverlayScrollBarGeometry();
        setOverlayScrollBarVisible(underMouse());
    }

    applyPanelSizeForPosition();
    refreshItemPresentation();
    refreshCollapseToggle();
    updateCollapseToggleGeometry();
}

}  // namespace uikit
