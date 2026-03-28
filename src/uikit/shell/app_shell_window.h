#ifndef UIKIT_APP_SHELL_WINDOW_H
#define UIKIT_APP_SHELL_WINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QString>

#include "uikit/shell/navigation_panel.h"

class QBoxLayout;
class QFrame;

namespace uikit {

class UiTitleBar;

class UiAppShellWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit UiAppShellWindow(QWidget* parent = nullptr);
    void setMinimumWindowSize(int width, int height);
    void setNavigationModel(const UiNavigationPanel::Model& model);

    void setNavigationPosition(UiNavigationPanel::Position position);
    void setNavigationCollapsed(bool collapsed);
    void setContentWidget(QWidget* widget);
    void switchContentWidget(QWidget* widget, int durationMs = 220);

signals:
    void navigationItemActivated(int index, const QString& key);
    void navigationItemWidgetActivated(int index, const QString& key, QWidget* page);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    enum class ResizeEdge {
        None = 0,
        Left = 1,
        Top = 2,
        Right = 4,
        Bottom = 8
    };

    void rebuildBodyLayout();
    bool isResizeEventTarget(QObject* watched) const;
    QRect contentTargetRect() const;
    QPoint switchStartPosForDirection(const QRect& target) const;
    ResizeEdge hitTestResizeEdge(const QPoint& pos) const;
    void applyResize(const QPoint& globalPos);
    Qt::CursorShape cursorForEdge(ResizeEdge edge) const;
    bool isTopDragRegion(QObject* watched, const QPoint& globalPos) const;

    UiTitleBar* titleBar_ = nullptr;
    UiNavigationPanel* navPanel_ = nullptr;
    QFrame* contentHost_ = nullptr;
    QWidget* contentWidget_ = nullptr;
    QBoxLayout* outerLayout_ = nullptr;
    QBoxLayout* bodyLayout_ = nullptr;

    UiNavigationPanel::Position navPosition_ = UiNavigationPanel::Position::Left;
    ResizeEdge resizeEdge_ = ResizeEdge::None;
    bool windowDragging_ = false;
    QPoint windowDragAnchor_;
    QRect initialGeometry_;
    QPoint initialMousePos_;
    bool switchingContent_ = false;
};

}  // namespace uikit

#endif  // UIKIT_APP_SHELL_WINDOW_H
