/**
 * @file ui_label.h
 * @brief 文本层级标签：将 `uiTextRole` 与 `base.qss` 中的排版 Token 对齐
 */
#ifndef UIKIT_UI_LABEL_H
#define UIKIT_UI_LABEL_H

#include <QLabel>

namespace uikit {

/**
 * @class UiLabel
 * @brief 继承 `QLabel`，通过 `TextRole` 设置动态属性 `uiTextRole`，样式由 `base.qss` 定义
 *
 * @example
 * @code
 * auto* title = new UiLabel(tr("标题"), this);
 * title->setTextRole(UiLabel::TextRole::H2);
 * @endcode
 */
class UiLabel : public QLabel {
    Q_OBJECT
    Q_PROPERTY(TextRole textRole READ textRole WRITE setTextRole)
public:
    enum class TextRole {
        H1,
        H2,
        H3,
        H4,
        H5,
        BodyLarge,
        Body,
        BodySmall,
        Caption,
        Small
    };
    Q_ENUM(TextRole)

    explicit UiLabel(QWidget* parent = nullptr);
    explicit UiLabel(const QString& text, QWidget* parent = nullptr);

    TextRole textRole() const;
    /**
     * @brief 对应 QSS 选择器 `QLabel[uiTextRole="h1"]` 等
     */
    void setTextRole(TextRole role);

private:
    void refreshTextRoleStyle();
    static QString textRoleToProperty(TextRole role);

    TextRole textRole_ = TextRole::Body;
};

}  // namespace uikit

#endif  // UIKIT_UI_LABEL_H
