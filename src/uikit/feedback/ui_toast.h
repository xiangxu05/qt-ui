#ifndef UIKIT_UI_TOAST_H
#define UIKIT_UI_TOAST_H

#include <QWidget>

class QLabel;

namespace uikit {

class UiToast : public QWidget {
    Q_OBJECT
public:
    enum class Type {
        Info,
        Success,
        Warning,
        Error
    };
    Q_ENUM(Type)

    explicit UiToast(QWidget* parent = nullptr);

    void setType(Type type);
    void setMessage(const QString& message);
    void showFor(int ms);

    static void showToast(QWidget* parent, const QString& message, Type type = Type::Info, int ms = 1600);

private:
    QString typeName(Type type) const;

    QLabel* textLabel_ = nullptr;
};

}  // namespace uikit

#endif  // UIKIT_UI_TOAST_H
