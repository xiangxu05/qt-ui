#ifndef UIKIT_UI_LINE_EDIT_H
#define UIKIT_UI_LINE_EDIT_H

#include <QLineEdit>

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

private:
    void refreshStyle();

    bool error_ = false;
    QString errorMessage_;
};

}  // namespace uikit

#endif  // UIKIT_UI_LINE_EDIT_H
