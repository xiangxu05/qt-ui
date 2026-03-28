/**
 * @file common_types.h
 * @brief 跨组件共享的枚举与轻量值类型（与具体 QWidget 解耦，便于主题/状态统一）
 *
 * @details
 * - `UiState`：通用交互状态（与 QSS `hover`/`disabled` 等配合时作语义扩展）
 * - `UiSize` / `UiVariant`：与 `UiButton` 等控件的档位对齐，便于将来抽公共 API
 * - `UiMargins`：简单四边距结构，避免直接依赖 `QMargins` 时在头文件引入过多依赖
 */
#ifndef UIKIT_TYPES_COMMON_TYPES_H
#define UIKIT_TYPES_COMMON_TYPES_H

namespace uikit {

enum class UiState {
    Default,
    Hover,
    Focus,
    Active,
    Disabled,
    Loading,
    Error
};

enum class UiSize {
    Small,
    Medium,
    Large
};

enum class UiVariant {
    Primary,
    Secondary,
    Ghost,
    Danger,
    Icon
};

struct UiMargins {
    int left = 0;
    int top = 0;
    int right = 0;
    int bottom = 0;

    UiMargins() = default;
    UiMargins(int all) : left(all), top(all), right(all), bottom(all) {}
    UiMargins(int h, int v) : left(h), top(v), right(h), bottom(v) {}
    UiMargins(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}
};

}  // namespace uikit

#endif  // UIKIT_TYPES_COMMON_TYPES_H
