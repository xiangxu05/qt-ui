#include "uikit/controls/ui_button.h"

#include <QStyle>

namespace uikit {

UiButton::UiButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent) {
    setCursor(Qt::PointingHandCursor);
    updateStyleState();
}

UiButton::Variant UiButton::variant() const {
    return variant_;
}

void UiButton::setVariant(Variant variant) {
    variant_ = variant;
    updateStyleState();
}

UiButton::Size UiButton::size() const {
    return size_;
}

void UiButton::setSize(Size size) {
    size_ = size;
    setFixedHeight(heightForSize(size_));
    updateStyleState();
}

bool UiButton::isLoading() const {
    return loading_;
}

void UiButton::setLoading(bool loading) {
    if (loading_ == loading) {
        return;
    }
    loading_ = loading;
    if (loading_) {
        cachedText_ = text();
        setText("...");
        setEnabled(false);
    } else {
        setText(cachedText_);
        setEnabled(true);
    }
    updateStyleState();
}

void UiButton::updateStyleState() {
    setProperty("uiRole", "button");
    setProperty("uiVariant", variantName(variant_));
    setProperty("uiSize", sizeName(size_));
    setProperty("uiLoading", loading_);
    style()->unpolish(this);
    style()->polish(this);
    update();
}

int UiButton::heightForSize(Size size) const {
    switch (size) {
        case Size::Small:
            return 30;
        case Size::Large:
            return 44;
        case Size::Medium:
        default:
            return 36;
    }
}

QString UiButton::variantName(Variant variant) const {
    switch (variant) {
        case Variant::Primary:
            return "primary";
        case Variant::Secondary:
            return "secondary";
        case Variant::Ghost:
            return "ghost";
        case Variant::Danger:
            return "danger";
        case Variant::Icon:
            return "icon";
    }
    return "primary";
}

QString UiButton::sizeName(Size size) const {
    switch (size) {
        case Size::Small:
            return "sm";
        case Size::Large:
            return "lg";
        case Size::Medium:
        default:
            return "md";
    }
}

}  // namespace uikit
