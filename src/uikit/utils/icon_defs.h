/**
 * @file icon_defs.h
 * @brief 文本/Emoji 形式的轻量图标常量（UTF-8 字节序列，供 `QString::fromUtf8` 使用）
 *
 * @details
 * 每项为 `const char[]`，用法：`QString::fromUtf8(uikit::UiIcons::Home)`。
 * 源文件须以 UTF-8 保存。若后续改为 SVG/QIcon，可迁移到 `icons/` 资源并保留同名别名。
 */
#ifndef UIKIT_UTILS_ICON_DEFS_H
#define UIKIT_UTILS_ICON_DEFS_H

namespace uikit {

namespace UiIcons {

inline constexpr const char Home[] = "🏠";
inline constexpr const char Menu[] = "☰";
inline constexpr const char Close[] = "✕";
inline constexpr const char Back[] = "←";
inline constexpr const char Forward[] = "→";

inline constexpr const char Add[] = "+";
inline constexpr const char Remove[] = "−";
inline constexpr const char Edit[] = "✎";
inline constexpr const char Delete[] = "🗑";
inline constexpr const char Search[] = "🔍";

inline constexpr const char Success[] = "✓";
inline constexpr const char Warning[] = "⚠";
inline constexpr const char Error[] = "✕";
inline constexpr const char Info[] = "ℹ";
inline constexpr const char Loading[] = "⟳";

inline constexpr const char Play[] = "▶";
inline constexpr const char Pause[] = "⏸";
inline constexpr const char Stop[] = "⏹";
inline constexpr const char Volume[] = "🔊";
inline constexpr const char Mute[] = "🔇";

}  // namespace UiIcons

}  // namespace uikit

#endif  // UIKIT_UTILS_ICON_DEFS_H
