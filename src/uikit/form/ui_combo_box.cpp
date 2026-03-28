#include "uikit/form/ui_combo_box.h"

#include <QStyle>

namespace uikit {

UiComboBox::UiComboBox(QWidget* parent)
    : QComboBox(parent) {
    setProperty("uiRole", "comboBox");
    refreshStyle();
}

bool UiComboBox::hasError() const {
    return error_;
}

void UiComboBox::setErrorState(bool error) {
    error_ = error;
    refreshStyle();
}

void UiComboBox::refreshStyle() {
    setProperty("uiError", error_);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

}  // namespace uikit
