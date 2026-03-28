#ifndef UIKIT_UI_COMBO_BOX_H
#define UIKIT_UI_COMBO_BOX_H

#include <QComboBox>

namespace uikit {

class UiComboBox : public QComboBox {
    Q_OBJECT
    Q_PROPERTY(bool error READ hasError WRITE setErrorState)
public:
    explicit UiComboBox(QWidget* parent = nullptr);

    bool hasError() const;
    void setErrorState(bool error);

private:
    void refreshStyle();

    bool error_ = false;
};

}  // namespace uikit

#endif  // UIKIT_UI_COMBO_BOX_H
