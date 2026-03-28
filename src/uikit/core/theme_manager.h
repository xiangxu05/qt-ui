/**
 * @file theme_manager.h
 * @brief 全局主题管理：多主题、QSS 合并、Token 替换、可选持久化
 */
#ifndef UIKIT_THEME_MANAGER_H
#define UIKIT_THEME_MANAGER_H

#include <QObject>
#include <QString>

#include "uikit/core/theme_tokens.h"

class QApplication;

namespace uikit {

/**
 * @brief 单例主题管理器
 *
 * @details
 * - `resolvedStyleSheet()` 合并 `base` / `controls` / `form` / `feedback` / `shell` 等 QSS 并做 Token 替换
 * - `applyTo()` 将结果设置到 `QApplication::setStyleSheet`，并记录应用指针以便 `setTheme` 时刷新
 * - `ThemeType::Custom` 需先调用 `setCustomTokens()`，否则 `setTheme(Custom)` 会失败并保留当前主题
 */
class ThemeManager : public QObject {
    Q_OBJECT
public:
    enum class ThemeType {
        Light,
        Dark,
        Custom
    };
    Q_ENUM(ThemeType)

    /**
     * @brief 全局单例（遵循 Qt 惯例：小写 `instance()`）
     */
    static ThemeManager& instance();

    void applyTo(QApplication* app);

    QString resolvedStyleSheet() const;

    ThemeTokens tokens() const;

    /**
     * @brief 切换主题；若 `persist == true` 则写入 QSettings（`uikit/themeType`）。
     * 命令行临时预览可传 `persist = false`，避免覆盖用户已保存偏好。
     */
    void setTheme(ThemeType type, bool persist = true);

    ThemeType currentTheme() const { return currentTheme_; }

    /**
     * @brief 供 `ThemeType::Custom` 使用：注入完整 Token 映射（通常由 `lightBlueTheme()` 拷贝后修改）
     */
    void setCustomTokens(const ThemeTokens& tokens);

    void clearCustomTokens();

    bool hasCustomTokens() const { return customTokensValid_; }

    /**
     * @brief 从 QSettings 恢复 `currentTheme_`（不发射信号、不刷新界面；应在首次 `applyTo` 前调用）
     */
    void loadThemeFromSettings();

signals:
    void themeChanged(ThemeType newTheme);

private:
    ThemeManager() = default;
    void saveThemePreference() const;
    QString loadResource(const QString& path) const;
    QString applyTokens(QString qss, const ThemeTokens& tokenSet) const;

    ThemeType currentTheme_ = ThemeType::Light;
    ThemeTokens customTokens_;
    bool customTokensValid_ = false;
    QApplication* lastApp_ = nullptr;
};

}  // namespace uikit

#endif  // UIKIT_THEME_MANAGER_H
