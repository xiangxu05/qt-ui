#include "uikit/shell/app_shell_window.h"

#include <QApplication>
#include <QBoxLayout>
#include <QEasingCurve>
#include <QEvent>
#include <QFrame>
#include <QMouseEvent>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QSet>
#include <QSizePolicy>

#include "uikit/shell/title_bar.h"

namespace uikit {
namespace {
constexpr int kMinWindowWidth = 800;
constexpr int kMinWindowHeight = 600;
constexpr int kResizeBorder = 6;
}

AppShellWindow::AppShellWindow(QWidget* parent)
    : QMainWindow(parent) {
    // 设置窗口窗口基本属性
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    // Keep top-level window opaque to avoid click-through.
    setAttribute(Qt::WA_TranslucentBackground, false);
    setMinimumWindowSize(kMinWindowWidth, kMinWindowHeight);  // 设置窗口最小尺寸
    setMouseTracking(true); // 设置鼠标跟踪
    qApp->installEventFilter(this); // 安装事件过滤器
    installEventFilter(this);

    auto* root = new QWidget(this);
    root->setObjectName("AppShellRoot"); // 设置窗口对象名称
    root->setProperty("uiRole", "appShellRoot");
    root->setMouseTracking(true);
    root->setAttribute(Qt::WA_TranslucentBackground, false);
    root->setAttribute(Qt::WA_StyledBackground, true);
    root->setAutoFillBackground(true);
    setCentralWidget(root);

    // 创建外层布局
    outerLayout_ = new QVBoxLayout(root);
    // Keep a thin hit-test ring for frameless resize.
    outerLayout_->setContentsMargins(0, 0, 0, 0);
    outerLayout_->setSpacing(0);

    titleBar_ = new TitleBar(root);
    titleBar_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleBar_->setMouseTracking(true);
    navPanel_ = new NavigationPanel(root);
    navPanel_->setMouseTracking(true);
    contentHost_ = new QFrame(root);
    contentHost_->setProperty("uiRole", "contentHost"); // 设置内容宿主属性
    contentHost_->setFrameShape(QFrame::NoFrame); // 设置内容宿主框架形状
    contentHost_->setMouseTracking(true);
    contentHost_->setAttribute(Qt::WA_StyledBackground, true);
    contentHost_->setAutoFillBackground(true);
    contentHost_->installEventFilter(this);
    contentHost_->setContentsMargins(16, 16, 16, 16);

    outerLayout_->addWidget(titleBar_);
    bodyLayout_ = new QHBoxLayout();
    bodyLayout_->setContentsMargins(0, 0, 0, 0);
    bodyLayout_->setSpacing(0);
    outerLayout_->addLayout(bodyLayout_);

    connect(titleBar_, &TitleBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(titleBar_, &TitleBar::maximizeRestoreRequested, this, [this]() {
        isMaximized() ? showNormal() : showMaximized();
        titleBar_->setMaximizedState(isMaximized());
    });
    connect(titleBar_, &TitleBar::closeRequested, this, &QWidget::close);
    connect(titleBar_, &TitleBar::titleDoubleClicked, this, [this]() {
        isMaximized() ? showNormal() : showMaximized();
        titleBar_->setMaximizedState(isMaximized());
    });
    connect(navPanel_, &NavigationPanel::itemActivatedWithWidget, this, [this](int index, const QString& key, QWidget* page) {
        emit navigationItemActivated(index, key);
        emit navigationItemWidgetActivated(index, key, page);
        if (page) {
            switchContentWidget(page, 180);
        }
    });

    setNavigationPosition(NavigationPanel::Position::Left);
    titleBar_->setMaximizedState(isMaximized());
    titleBar_->raise();
}

void AppShellWindow::setMinimumWindowSize(int width, int height) {
    setMinimumSize(width, height);
}

void AppShellWindow::setNavigationModel(const NavigationPanel::Model& model) {
    QSet<QWidget*> adoptedPages;
    const auto adoptPage = [this, &adoptedPages](QWidget* page) {
        if (!page || adoptedPages.contains(page)) {
            return;
        }
        adoptedPages.insert(page);
        // Normalize ownership to avoid accidental top-level artifacts when
        // callers create pages with shell/top-level parent.
        if (page->parentWidget() != contentHost_) {
            page->setParent(contentHost_);
        }
        page->setGeometry(contentTargetRect());
        page->hide();
    };

    for (const auto& item : model.primaryItems) {
        adoptPage(item.page);
    }
    for (const auto& section : model.secondarySections) {
        for (const auto& item : section.items) {
            adoptPage(item.page);
        }
    }

    navPanel_->setModel(model);
}

void AppShellWindow::changeEvent(QEvent* event) {
    QMainWindow::changeEvent(event);
    if (event && event->type() == QEvent::WindowStateChange && titleBar_) {
        titleBar_->setMaximizedState(isMaximized());
    }
}

// 设置导航栏位置
void AppShellWindow::setNavigationPosition(NavigationPanel::Position position) {
    navPosition_ = position;
    navPanel_->setPosition(position);
    rebuildBodyLayout();
}

// 设置导航栏折叠
void AppShellWindow::setNavigationCollapsed(bool collapsed) {
    navPanel_->setCollapsed(collapsed);
}

// 设置内容宿主
void AppShellWindow::setContentWidget(QWidget* widget) {
    if (!widget) {
        return;
    }
    // 如果内容相同，则直接返回
    if (contentWidget_ == widget) {
        return;
    }
    switchingContent_ = false;
    if (contentWidget_) {
        contentWidget_->hide();
    }
    contentWidget_ = widget;
    contentWidget_->setParent(contentHost_);
    contentWidget_->setGeometry(contentTargetRect());
    contentWidget_->show();
}

void AppShellWindow::switchContentWidget(QWidget* widget, int durationMs) {
    if (!widget) {
        return;
    }
    if (!contentWidget_ || durationMs <= 0) {
        setContentWidget(widget);
        return;
    }
    // 内容相同或正在切换时忽略重复触发，避免动画重入。
    if (contentWidget_ == widget || switchingContent_) {
        return;
    }

    QWidget* previous = contentWidget_;
    contentWidget_ = widget;
    switchingContent_ = true;

    const QRect target = contentTargetRect();
    const QPoint startPos = switchStartPosForDirection(target);

    widget->setParent(contentHost_);
    widget->setGeometry(QRect(startPos, target.size()));
    widget->show();
    widget->raise(); // 将内容提升到最上层

    auto* group = new QParallelAnimationGroup(widget);

    auto* slide = new QPropertyAnimation(widget, "pos", widget);
    slide->setDuration(durationMs);
    slide->setStartValue(startPos);
    slide->setEndValue(target.topLeft());
    slide->setEasingCurve(QEasingCurve::OutQuart); // 设置缓动曲线：开始快，结束慢，更自然。
    group->addAnimation(slide);

    connect(group, &QParallelAnimationGroup::finished, this, [this, previous, widget, target]() {
        if (previous) {
            previous->hide();
            previous->setGraphicsEffect(nullptr);
        }
        widget->setGeometry(target);
        widget->setGraphicsEffect(nullptr);
        switchingContent_ = false;
    });
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

// 事件过滤器
bool AppShellWindow::eventFilter(QObject* watched, QEvent* event) {
    // 内容区自适应
    if (watched == contentHost_ && event->type() == QEvent::Resize) {
        if (contentWidget_ && !switchingContent_) {
            contentWidget_->setGeometry(contentTargetRect());
        }
    }

    // 判断是否是可调整大小的区域，如果是，则处理调整大小事件
    if (!isResizeEventTarget(watched) || isMaximized()) {
        return QMainWindow::eventFilter(watched, event);
    }

    auto* watchedWidget = qobject_cast<QWidget*>(watched);
    switch (event->type()) {
        case QEvent::MouseMove: {
            auto* me = static_cast<QMouseEvent*>(event);
            if (windowDragging_ && me->buttons().testFlag(Qt::LeftButton) && !isMaximized()) {
                move(me->globalPosition().toPoint() - windowDragAnchor_);
                return true;
            }
            if (resizeEdge_ != ResizeEdge::None && me->buttons().testFlag(Qt::LeftButton)) {
                applyResize(me->globalPosition().toPoint());
                return true;
            }
            const QPoint localPos = mapFromGlobal(me->globalPosition().toPoint());
            const ResizeEdge edge = hitTestResizeEdge(localPos);
            const Qt::CursorShape shape = cursorForEdge(edge);
            setCursor(shape);
            if (watchedWidget) {
                watchedWidget->setCursor(shape);
            }
            break;
        }
        case QEvent::MouseButtonPress: {
            auto* me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton) {
                if (isTopDragRegion(watched, me->globalPosition().toPoint()) && !isMaximized()) {
                    windowDragging_ = true;
                    windowDragAnchor_ = me->globalPosition().toPoint() - frameGeometry().topLeft();
                    return true;
                }
                const QPoint localPos = mapFromGlobal(me->globalPosition().toPoint());
                resizeEdge_ = hitTestResizeEdge(localPos);
                if (resizeEdge_ != ResizeEdge::None) {
                    initialGeometry_ = geometry();
                    initialMousePos_ = me->globalPosition().toPoint();
                    return true;
                }
            }
            break;
        }
        case QEvent::MouseButtonRelease:
            windowDragging_ = false;
            resizeEdge_ = ResizeEdge::None;
            if (watchedWidget) {
                watchedWidget->unsetCursor();
            }
            unsetCursor();
            break;
        case QEvent::Leave:
            if (resizeEdge_ == ResizeEdge::None) {
                if (watchedWidget) {
                    watchedWidget->unsetCursor();
                }
                unsetCursor();
            }
            break;
        default:
            break;
    }

    return QMainWindow::eventFilter(watched, event);
}

bool AppShellWindow::isResizeEventTarget(QObject* watched) const {
    if (watched == this) {
        return true;
    }
    auto* widget = qobject_cast<QWidget*>(watched);
    return widget && (widget == centralWidget() || isAncestorOf(widget));
}

QRect AppShellWindow::contentTargetRect() const {
    if (!contentHost_) {
        return QRect();
    }
    return contentHost_->contentsRect();
}

QPoint AppShellWindow::switchStartPosForDirection(const QRect& target) const {
    switch (navPosition_) {
        case NavigationPanel::Position::Left:
            return QPoint(target.left() - target.width(), target.top());
        case NavigationPanel::Position::Top:
            return QPoint(target.left(), target.top() - target.height());
        case NavigationPanel::Position::Right:
            return QPoint(target.left() + target.width(), target.top());
        case NavigationPanel::Position::Bottom:
            return QPoint(target.left(), target.top() + target.height());
    }
    return target.topLeft();
}

void AppShellWindow::rebuildBodyLayout() {
    while (QLayoutItem* item = bodyLayout_->takeAt(0)) {
        item->widget();
        delete item;
    }

    const bool horizontal = (navPosition_ == NavigationPanel::Position::Left ||
                             navPosition_ == NavigationPanel::Position::Right);

    if (horizontal) {
        if (navPosition_ == NavigationPanel::Position::Left) {
            bodyLayout_->addWidget(navPanel_);
            bodyLayout_->addWidget(contentHost_, 1);
        } else {
            bodyLayout_->addWidget(contentHost_, 1);
            bodyLayout_->addWidget(navPanel_);
        }
    } else {
        auto* vertical = new QVBoxLayout();
        vertical->setContentsMargins(0, 0, 0, 0);
        vertical->setSpacing(0);
        if (navPosition_ == NavigationPanel::Position::Top) {
            vertical->addWidget(navPanel_);
            vertical->addWidget(contentHost_, 1);
        } else {
            vertical->addWidget(contentHost_, 1);
            vertical->addWidget(navPanel_);
        }
        bodyLayout_->addLayout(vertical, 1);
    }

    if (titleBar_) {
        titleBar_->raise();
    }
}

AppShellWindow::ResizeEdge AppShellWindow::hitTestResizeEdge(const QPoint& pos) const {
    int flags = 0;
    if (pos.x() <= kResizeBorder) {
        flags |= static_cast<int>(ResizeEdge::Left);
    } else if (pos.x() >= width() - kResizeBorder) {
        flags |= static_cast<int>(ResizeEdge::Right);
    }
    if (pos.y() <= kResizeBorder) {
        flags |= static_cast<int>(ResizeEdge::Top);
    } else if (pos.y() >= height() - kResizeBorder) {
        flags |= static_cast<int>(ResizeEdge::Bottom);
    }
    return static_cast<ResizeEdge>(flags);
}

void AppShellWindow::applyResize(const QPoint& globalPos) {
    QRect next = initialGeometry_;
    const QPoint delta = globalPos - initialMousePos_;
    const int edge = static_cast<int>(resizeEdge_);

    if (edge & static_cast<int>(ResizeEdge::Left)) {
        next.setLeft(next.left() + delta.x());
    }
    if (edge & static_cast<int>(ResizeEdge::Right)) {
        next.setRight(next.right() + delta.x());
    }
    if (edge & static_cast<int>(ResizeEdge::Top)) {
        next.setTop(next.top() + delta.y());
    }
    if (edge & static_cast<int>(ResizeEdge::Bottom)) {
        next.setBottom(next.bottom() + delta.y());
    }

    if (next.width() >= minimumWidth() && next.height() >= minimumHeight()) {
        setGeometry(next);
    }
}

Qt::CursorShape AppShellWindow::cursorForEdge(ResizeEdge edge) const {
    const int value = static_cast<int>(edge);
    if (value == (static_cast<int>(ResizeEdge::Left) | static_cast<int>(ResizeEdge::Top)) ||
        value == (static_cast<int>(ResizeEdge::Right) | static_cast<int>(ResizeEdge::Bottom))) {
        return Qt::SizeFDiagCursor;
    }
    if (value == (static_cast<int>(ResizeEdge::Right) | static_cast<int>(ResizeEdge::Top)) ||
        value == (static_cast<int>(ResizeEdge::Left) | static_cast<int>(ResizeEdge::Bottom))) {
        return Qt::SizeBDiagCursor;
    }
    if (value & (static_cast<int>(ResizeEdge::Left) | static_cast<int>(ResizeEdge::Right))) {
        return Qt::SizeHorCursor;
    }
    if (value & (static_cast<int>(ResizeEdge::Top) | static_cast<int>(ResizeEdge::Bottom))) {
        return Qt::SizeVerCursor;
    }
    return Qt::ArrowCursor;
}

bool AppShellWindow::isTopDragRegion(QObject* watched, const QPoint& globalPos) const {
    if (!titleBar_) {
        return false;
    }
    auto* watchedWidget = qobject_cast<QWidget*>(watched);
    const QPoint localPos = mapFromGlobal(globalPos);
    if (!watchedWidget) {
        return false;
    }

    // 1) Normal title bar region.
    if (localPos.y() <= titleBar_->height()) {
        // Exclude direct interaction with title bar action buttons.
        if (watchedWidget->property("uiSurface").toString() == "titleBar") {
            return false;
        }
        return true;
    }

    // 2) Keep navigation top blank area draggable when large top margin is used.
    if (!navPanel_) {
        return false;
    }
    const QRect navRect = navPanel_->geometry();
    const int navTopBlankHeight = 50;  // aligned with navigation layout top margin preference
    if (!navRect.contains(localPos) || localPos.y() > navRect.top() + navTopBlankHeight) {
        return false;
    }

    // Do not hijack drag on interactive navigation widgets.
    const QString role = watchedWidget->property("uiRole").toString();
    if (role == "navigationItem" || role == "navigationSectionTitle" || role == "navigationLogo" ||
        role == "navigationTitle" || role == "navigationCollapseButton") {
        return false;
    }
    return true;
}

}  // namespace uikit
