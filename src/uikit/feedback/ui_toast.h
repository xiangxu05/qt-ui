/**
 * @file ui_toast.h
 * @brief 轻量提示条：支持类型色与自动消失
 *
 * @details
 * **使用场景**：操作结果反馈、非阻塞提示；需要模态确认时请用 `UiDialog`。
 * 便捷入口：`UiToast::showToast(parent, message, type, durationMs)`。
 */
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
    /** @brief 显示若干毫秒后自动隐藏 */
    void showFor(int ms);

    /**
     * @brief 静态便捷方法：在 parent 上创建并显示一条 Toast
     */
    static void showToast(QWidget* parent, const QString& message, Type type = Type::Info, int ms = 1600);

private:
    QString typeName(Type type) const;

    QLabel* textLabel_ = nullptr;
};

}  // namespace uikit

#endif  // UIKIT_UI_TOAST_H
