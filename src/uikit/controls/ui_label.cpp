#include "uikit/controls/ui_label.h"

#include <QStyle>

namespace uikit {

UiLabel::UiLabel(QWidget* parent)
    : QLabel(parent) {
    refreshTextRoleStyle();
}

UiLabel::UiLabel(const QString& text, QWidget* parent)
    : QLabel(text, parent) {
    refreshTextRoleStyle();
}

UiLabel::TextRole UiLabel::textRole() const {
    return textRole_;
}

void UiLabel::setTextRole(TextRole role) {
    if (textRole_ == role) {
        return;
    }
    textRole_ = role;
    refreshTextRoleStyle();
}

void UiLabel::refreshTextRoleStyle() {
    setProperty("uiTextRole", textRoleToProperty(textRole_));
    if (style()) {
        style()->unpolish(this);
        style()->polish(this);
    }
    update();
}

QString UiLabel::textRoleToProperty(TextRole role) {
    switch (role) {
        case TextRole::H1:
            return QStringLiteral("h1");
        case TextRole::H2:
            return QStringLiteral("h2");
        case TextRole::H3:
            return QStringLiteral("h3");
        case TextRole::H4:
            return QStringLiteral("h4");
        case TextRole::H5:
            return QStringLiteral("h5");
        case TextRole::BodyLarge:
            return QStringLiteral("bodyLarge");
        case TextRole::Body:
            return QStringLiteral("body");
        case TextRole::BodySmall:
            return QStringLiteral("bodySmall");
        case TextRole::Caption:
            return QStringLiteral("caption");
        case TextRole::Small:
            return QStringLiteral("small");
    }
    return QStringLiteral("body");
}

}  // namespace uikit
