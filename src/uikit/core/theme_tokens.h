/**
 * @file theme_tokens.h
 * @brief 主题变量集合与浅色主题工厂（Design Tokens 的数据层）
 *
 * @details
 * `ThemeTokens` 持有 `QMap<QString, QString>`，键为 QSS 占位符（如 `@PRIMARY@`），
 * 值为替换后的 CSS 值。`ThemeTokenFactory::lightBlueTheme()` / `darkTheme()` 提供浅色与深色基底。
 *
 * 键名覆盖：主色与文本、功能色、灰阶、链接与遮罩、字号与字重、圆角与间距、
 * 边框与阴影、Focus Ring（`@PRIMARY_FOCUS_RING@` / `@DANGER_FOCUS_RING@`）、动效时长与缓动字符串等，
 * 完整列表见 `theme_tokens.cpp` 中 `tokens.vars`。
 */
#ifndef UIKIT_THEME_TOKENS_H
#define UIKIT_THEME_TOKENS_H

#include <QColor>
#include <QMap>
#include <QString>

namespace uikit {

/**
 * @brief 内置品牌主色预设（与 `createThemeWithBrand(BrandPreset, ...)` 配合）
 */
enum class BrandPreset {
    Blue,
    Red,
    Green,
    Purple,
    Orange
};

/**
 * @brief 一组可注入 QSS 的主题变量
 */
struct ThemeTokens {
    QMap<QString, QString> vars;
};

/**
 * @brief 构建预置主题 Token
 */
class ThemeTokenFactory {
public:
    static ThemeTokens lightBlueTheme();
    /** @brief 深色主题：在浅色完整键集上覆盖背景/文本/边框与 Shell 语义色 */
    static ThemeTokens darkTheme();

    /**
     * @brief 在浅色或深色基底上替换品牌主色，并生成 Hover/Pressed/Soft 与链接、Info 强调色
     */
    static ThemeTokens createThemeWithBrand(const QColor& brandPrimary, bool baseDark = false);

    /** @brief 使用预设品牌色（蓝/红/绿/紫/橙） */
    static ThemeTokens createThemeWithBrand(BrandPreset preset, bool baseDark = false);

    static QColor brandPresetColor(BrandPreset preset);
};

}  // namespace uikit

#endif  // UIKIT_THEME_TOKENS_H
