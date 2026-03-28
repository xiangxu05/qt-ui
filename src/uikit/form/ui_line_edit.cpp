#include "uikit/form/ui_line_edit.h"

#include <QStyle>

namespace uikit {

UiLineEdit::UiLineEdit(QWidget* parent)
    : QLineEdit(parent) {
    setProperty("uiRole", "lineEdit");
    refreshStyle();
}

bool UiLineEdit::hasError() const {
    return error_;
}

void UiLineEdit::setErrorState(bool error) {
    error_ = error;
    refreshStyle();
}

void UiLineEdit::setErrorMessage(const QString& message) {
    errorMessage_ = message;
    setToolTip(errorMessage_);
}

QString UiLineEdit::errorMessage() const {
    return errorMessage_;
}

void UiLineEdit::refreshStyle() {
    setProperty("uiError", error_);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

}  // namespace uikit
