#ifndef UIKIT_UI_SWITCH_H
#define UIKIT_UI_SWITCH_H

#include <QCheckBox>

namespace uikit {

class UiSwitch : public QCheckBox {
    Q_OBJECT
public:
    explicit UiSwitch(QWidget* parent = nullptr);
};

}  // namespace uikit

#endif  // UIKIT_UI_SWITCH_H
