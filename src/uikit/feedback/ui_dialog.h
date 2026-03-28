#ifndef UIKIT_UI_DIALOG_H
#define UIKIT_UI_DIALOG_H

#include <QDialog>

class QLabel;
class QDialogButtonBox;

namespace uikit {

class UiDialog : public QDialog {
    Q_OBJECT
public:
    explicit UiDialog(QWidget* parent = nullptr);

    void setTitle(const QString& title);
    void setMessage(const QString& message);

    static bool confirm(QWidget* parent, const QString& title, const QString& message);
    static void info(QWidget* parent, const QString& title, const QString& message);

private:
    QLabel* titleLabel_ = nullptr;
    QLabel* messageLabel_ = nullptr;
    QDialogButtonBox* buttonBox_ = nullptr;
};

}  // namespace uikit

#endif  // UIKIT_UI_DIALOG_H
