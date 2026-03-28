#include "uikit/form/ui_combo_box.h"

#include <QEvent>
#include <QStyle>

namespace uikit {

UiComboBox::UiComboBox(QWidget* parent)
    : QComboBox(parent) {
    setProperty("uiRole", "comboBox");
    updateComboCursor();
    refreshStyle();
}

bool UiComboBox::hasError() const {
    return error_;
}

void UiComboBox::setErrorState(bool error) {
    error_ = error;
    refreshStyle();
}

void UiComboBox::changeEvent(QEvent* event) {
    QComboBox::changeEvent(event);
    if (event->type() == QEvent::EnabledChange) {
        updateComboCursor();
    }
}

void UiComboBox::updateComboCursor() {
    setCursor(isEnabled() ? Qt::PointingHandCursor : Qt::ForbiddenCursor);
}

void UiComboBox::refreshStyle() {
    setProperty("uiError", error_);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

}  // namespace uikit
