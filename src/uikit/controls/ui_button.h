#ifndef UIKIT_UI_BUTTON_H
#define UIKIT_UI_BUTTON_H

#include <QPushButton>

namespace uikit {

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
    void setVariant(Variant variant);

    Size size() const;
    void setSize(Size size);

    bool isLoading() const;
    void setLoading(bool loading);

private:
    void updateStyleState();
    int heightForSize(Size size) const;
    QString variantName(Variant variant) const;
    QString sizeName(Size size) const;

    Variant variant_ = Variant::Primary;
    Size size_ = Size::Medium;
    bool loading_ = false;
    QString cachedText_;
};

}  // namespace uikit

#endif  // UIKIT_UI_BUTTON_H
