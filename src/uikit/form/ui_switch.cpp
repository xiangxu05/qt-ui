#include "uikit/form/ui_switch.h"

namespace uikit {

UiSwitch::UiSwitch(QWidget* parent)
    : QCheckBox(parent) {
    setProperty("uiRole", "switch");
    setCursor(Qt::PointingHandCursor);
}

}  // namespace uikit
