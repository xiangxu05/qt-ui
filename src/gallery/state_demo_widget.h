#ifndef GALLERY_STATE_DEMO_WIDGET_H
#define GALLERY_STATE_DEMO_WIDGET_H

#include <QWidget>

/**
 * Gallery 页：交互状态演示（Tab 焦点链、Loading、Error 输入框）。
 */
class StateDemoWidget : public QWidget {
public:
    explicit StateDemoWidget(QWidget* parent = nullptr);
};

#endif
