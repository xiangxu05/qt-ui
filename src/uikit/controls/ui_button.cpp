#include "uikit/controls/ui_button.h"

#include <QEvent>
#include <QStyle>
#include <QTimer>

namespace uikit {
namespace {
constexpr int kDefaultButtonHeightSmall = 30;
constexpr int kDefaultButtonHeightMedium = 36;
constexpr int kDefaultButtonHeightLarge = 44;
const QString kSpinnerFrames = QStringLiteral("⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏");
}  // namespace

UiButton::UiButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent) {
    loadingTimer_ = new QTimer(this);
    loadingTimer_->setInterval(80);
    connect(loadingTimer_, &QTimer::timeout, this, &UiButton::onLoadingTick);
    updateButtonCursor();
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
        // 锁定当前宽度，避免加载文案变成单字符后按钮塌缩。
        const int lockedWidth = width() > 0 ? width() : sizeHint().width();
        setMinimumWidth(lockedWidth);
        setMaximumWidth(lockedWidth);
        cachedText_ = text();
        loadingFrame_ = 0;
        setText(kSpinnerFrames.mid(0, 1));
        loadingTimer_->start();
    } else {
        loadingTimer_->stop();
        setText(cachedText_);
        // 取消宽度锁定，恢复自适应。
        setMinimumWidth(0);
        setMaximumWidth(QWIDGETSIZE_MAX);
    }

    updateButtonCursor();
    updateStyleState();
}

void UiButton::changeEvent(QEvent* event) {
    QPushButton::changeEvent(event);
    if (event->type() == QEvent::EnabledChange) {
        updateButtonCursor();
    }
}

bool UiButton::event(QEvent* event) {
    if (loading_) {
        switch (event->type()) {
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonRelease:
            case QEvent::MouseButtonDblClick:
            case QEvent::KeyPress:
            case QEvent::KeyRelease:
                // Loading 时吞掉交互事件，避免重复触发 clicked。
                return true;
            default:
                break;
        }
    }
    return QPushButton::event(event);
}

void UiButton::onLoadingTick() {
    if (!loading_) {
        return;
    }
    loadingFrame_ = (loadingFrame_ + 1) % kSpinnerFrames.size();
    setText(kSpinnerFrames.mid(loadingFrame_, 1));
}

void UiButton::updateButtonCursor() {
    if (loading_) {
        setCursor(Qt::WaitCursor);
        return;
    }
    if (!isEnabled()) {
        setCursor(Qt::ForbiddenCursor);
        return;
    }
    setCursor(Qt::PointingHandCursor);
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
            return kDefaultButtonHeightSmall;
        case Size::Large:
            return kDefaultButtonHeightLarge;
        case Size::Medium:
        default:
            return kDefaultButtonHeightMedium;
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
