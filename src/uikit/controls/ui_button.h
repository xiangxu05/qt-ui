/**
 * @file ui_button.h
 * @brief 统一样式的按钮控件（变体、尺寸、加载态由 QSS + 动态属性驱动）
 *
 * @details
 * 通过 `setProperty("uiVariant" / "uiSize" / "uiLoading")` 与 `controls.qss` 配合。
 * 高度由 `Size` 映射为固定像素，常量见 `ui_button.cpp` 中 `kDefaultButtonHeight*`。
 */
#ifndef UIKIT_UI_BUTTON_H
#define UIKIT_UI_BUTTON_H

#include <QPushButton>

class QTimer;
class QEvent;

namespace uikit {

/**
 * @class UiButton
 * @brief 继承 `QPushButton`，提供 Primary / Secondary / Ghost / Danger / Icon 等变体
 *
 * @example
 * @code
 * auto* btn = new UiButton(tr("确定"), this);
 * btn->setVariant(UiButton::Variant::Primary);
 * btn->setSize(UiButton::Size::Large);
 * @endcode
 */
class UiButton : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(Variant variant READ variant WRITE setVariant)
    Q_PROPERTY(Size size READ size WRITE setSize)
    Q_PROPERTY(bool loading READ isLoading WRITE setLoading)
public:
    enum class Variant {
        Primary,
        Secondary,
        Ghost,
        Danger,
        Icon
    };
    Q_ENUM(Variant)

    enum class Size {
        Small,
        Medium,
        Large
    };
    Q_ENUM(Size)

    explicit UiButton(const QString& text = QString(), QWidget* parent = nullptr);

    Variant variant() const;
    /**
     * @brief 设置视觉变体（对应 QSS 中 `uiVariant`）
     */
    void setVariant(Variant variant);

    Size size() const;
    /**
     * @brief 设置尺寸档，会更新固定高度（Small/Medium/Large）
     */
    void setSize(Size size);

    bool isLoading() const;
    /**
     * @brief 加载态：禁用按钮并显示占位文案，恢复后还原原文本
     */
    void setLoading(bool loading);

protected:
    void changeEvent(QEvent* event) override;
    bool event(QEvent* event) override;

private slots:
    void onLoadingTick();

private:
    void updateStyleState();
    void updateButtonCursor();
    int heightForSize(Size size) const;
    QString variantName(Variant variant) const;
    QString sizeName(Size size) const;

    Variant variant_ = Variant::Primary;
    Size size_ = Size::Medium;
    bool loading_ = false;
    QString cachedText_;
    QTimer* loadingTimer_ = nullptr;
    int loadingFrame_ = 0;
};

}  // namespace uikit

#endif  // UIKIT_UI_BUTTON_H
