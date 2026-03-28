#include "uikit/feedback/ui_message_bar.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QToolButton>

namespace uikit {

UiMessageBar::UiMessageBar(QWidget* parent)
    : QWidget(parent) {
    setProperty("uiRole", "messageBar");
    setProperty("uiType", "info");

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 8, 10, 8);
    layout->setSpacing(8);

    iconLabel_ = new QLabel("i", this);
    iconLabel_->setFixedWidth(12);
    textLabel_ = new QLabel(this);
    textLabel_->setText("Message");
    textLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    closeButton_ = new QToolButton(this);
    closeButton_->setText("x");
    connect(closeButton_, &QToolButton::clicked, this, &UiMessageBar::hide);

    layout->addWidget(iconLabel_);
    layout->addWidget(textLabel_);
    layout->addWidget(closeButton_);
}

void UiMessageBar::setType(Type type) {
    setProperty("uiType", typeName(type));
    switch (type) {
        case Type::Info:
            iconLabel_->setText("i");
            break;
        case Type::Success:
            iconLabel_->setText("v");
            break;
        case Type::Warning:
            iconLabel_->setText("!");
            break;
        case Type::Error:
            iconLabel_->setText("x");
            break;
    }
    style()->unpolish(this);
    style()->polish(this);
    update();
}

void UiMessageBar::setMessage(const QString& message) {
    textLabel_->setText(message);
}

QString UiMessageBar::typeName(Type type) const {
    switch (type) {
        case Type::Info:
            return "info";
        case Type::Success:
            return "success";
        case Type::Warning:
            return "warning";
        case Type::Error:
            return "error";
    }
    return "info";
}

}  // namespace uikit
