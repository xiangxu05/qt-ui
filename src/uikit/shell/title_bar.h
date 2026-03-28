#ifndef UIKIT_TITLE_BAR_H
#define UIKIT_TITLE_BAR_H

#include <QWidget>

namespace uikit {

class UiButton;

class UiTitleBar : public QWidget {
    Q_OBJECT
public:
    explicit UiTitleBar(QWidget* parent = nullptr);
    void setMaximizedState(bool maximized);
signals:
    void minimizeRequested();
    void maximizeRestoreRequested();
    void closeRequested();
    void titleDoubleClicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    UiButton* minButton_ = nullptr;
    UiButton* maxButton_ = nullptr;
    UiButton* closeButton_ = nullptr;
    QPoint dragAnchor_;
    bool dragging_ = false;
};

}  // namespace uikit

#endif  // UIKIT_TITLE_BAR_H
