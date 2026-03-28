/**
 * @file ui_line_edit.h
 * @brief 带错误态与错误文案的单行输入框
 *
 * @details
 * `setErrorState(true)` 会设置动态属性供 QSS 使用；`setErrorMessage` 用于业务保存提示文案（可选）。
 *
 * @example
 * @code
 * auto* edit = new UiLineEdit(this);
 * edit->setPlaceholderText(tr("邮箱"));
 * edit->setErrorState(true);
 * edit->setErrorMessage(tr("格式不正确"));
 * @endcode
 */
#ifndef UIKIT_UI_LINE_EDIT_H
#define UIKIT_UI_LINE_EDIT_H

#include <QLineEdit>

class QEvent;

namespace uikit {

class UiLineEdit : public QLineEdit {
    Q_OBJECT
    Q_PROPERTY(bool error READ hasError WRITE setErrorState)
public:
    explicit UiLineEdit(QWidget* parent = nullptr);

    bool hasError() const;
    void setErrorState(bool error);
    void setErrorMessage(const QString& message);
    QString errorMessage() const;

protected:
    void changeEvent(QEvent* event) override;

private:
    void refreshStyle();
    void updateEditCursor();

    bool error_ = false;
    QString errorMessage_;
};

}  // namespace uikit

#endif  // UIKIT_UI_LINE_EDIT_H
