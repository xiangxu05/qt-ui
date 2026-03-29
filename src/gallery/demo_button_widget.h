#ifndef DEMO_BUTTON_WIDGET_H
#define DEMO_BUTTON_WIDGET_H

#include <QWidget>

/**
 * Gallery 页：交互状态演示（Tab 焦点链、Loading、Error 输入框）。
 */
class DemoButtonWidget : public QWidget {
public:
    explicit DemoButtonWidget(QWidget* parent = nullptr);
};

#endif
