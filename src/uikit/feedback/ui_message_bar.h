#ifndef UIKIT_UI_MESSAGE_BAR_H
#define UIKIT_UI_MESSAGE_BAR_H

#include <QWidget>

class QLabel;
class QToolButton;

namespace uikit {

class UiMessageBar : public QWidget {
    Q_OBJECT
public:
    enum class Type {
        Info,
        Success,
        Warning,
        Error
    };
    Q_ENUM(Type)

    explicit UiMessageBar(QWidget* parent = nullptr);

    void setType(Type type);
    void setMessage(const QString& message);

private:
    QString typeName(Type type) const;

    QLabel* iconLabel_ = nullptr;
    QLabel* textLabel_ = nullptr;
    QToolButton* closeButton_ = nullptr;
};

}  // namespace uikit

#endif  // UIKIT_UI_MESSAGE_BAR_H
