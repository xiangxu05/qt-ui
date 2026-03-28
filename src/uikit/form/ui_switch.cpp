#include "uikit/form/ui_switch.h"

#include <QEvent>

namespace uikit {

UiSwitch::UiSwitch(QWidget* parent)
    : QCheckBox(parent) {
    setProperty("uiRole", "switch");
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(false);
    updateSwitchCursor();
}

void UiSwitch::changeEvent(QEvent* event) {
    QCheckBox::changeEvent(event);
    if (event->type() == QEvent::EnabledChange) {
        updateSwitchCursor();
    }
}

void UiSwitch::updateSwitchCursor() {
    setCursor(isEnabled() ? Qt::PointingHandCursor : Qt::ForbiddenCursor);
}

}  // namespace uikit
