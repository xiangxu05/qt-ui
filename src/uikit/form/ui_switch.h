/**
 * @file ui_switch.h
 * @brief 开关控件（基于 `QCheckBox`，样式由 QSS 的 `uiRole`/属性选择器控制）
 *
 * @details
 * **状态说明**：与 `QCheckBox` 一致——未勾选 / 勾选 / 三态（若启用）；禁用态由 `setEnabled(false)` 控制。
 * 视觉上的 on/off 对应 `checked` 属性。
 */
#ifndef UIKIT_UI_SWITCH_H
#define UIKIT_UI_SWITCH_H

#include <QCheckBox>

class QEvent;

namespace uikit {

class UiSwitch : public QCheckBox {
    Q_OBJECT
public:
    explicit UiSwitch(QWidget* parent = nullptr);

protected:
    void changeEvent(QEvent* event) override;

private:
    void updateSwitchCursor();
};

}  // namespace uikit

#endif  // UIKIT_UI_SWITCH_H
