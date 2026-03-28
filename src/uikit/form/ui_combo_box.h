#ifndef UIKIT_UI_COMBO_BOX_H
#define UIKIT_UI_COMBO_BOX_H

#include <QComboBox>

class QEvent;

namespace uikit {

class UiComboBox : public QComboBox {
    Q_OBJECT
    Q_PROPERTY(bool error READ hasError WRITE setErrorState)
public:
    explicit UiComboBox(QWidget* parent = nullptr);

    bool hasError() const;
    void setErrorState(bool error);

protected:
    void changeEvent(QEvent* event) override;

private:
    void refreshStyle();
    void updateComboCursor();

    bool error_ = false;
};

}  // namespace uikit

#endif  // UIKIT_UI_COMBO_BOX_H
