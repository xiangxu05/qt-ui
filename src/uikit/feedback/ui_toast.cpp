#include "uikit/feedback/ui_toast.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QStyle>
#include <QTimer>

namespace uikit {

UiToast::UiToast(QWidget* parent)
    : QWidget(parent) {
    setProperty("uiRole", "toast");
    setProperty("uiType", "info");
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground, true);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 8, 12, 8);
    textLabel_ = new QLabel(this);
    layout->addWidget(textLabel_);
}

void UiToast::setType(Type type) {
    setProperty("uiType", typeName(type));
    style()->unpolish(this);
    style()->polish(this);
    update();
}

void UiToast::setMessage(const QString& message) {
    textLabel_->setText(message);
    adjustSize();
}

void UiToast::showFor(int ms) {
    show();
    QTimer::singleShot(ms, this, [this]() {
        close();
        deleteLater();
    });
}

void UiToast::showToast(QWidget* parent, const QString& message, Type type, int ms) {
    auto* toast = new UiToast(parent);
    toast->setType(type);
    toast->setMessage(message);
    if (parent) {
        const QPoint center = parent->mapToGlobal(parent->rect().center());
        toast->move(center.x() - toast->width() / 2, center.y() + parent->height() / 3);
    }
    toast->showFor(ms);
}

QString UiToast::typeName(Type type) const {
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
