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

#include <QString>

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

inline QString toString(UiState state) {
    switch (state) {
        case UiState::Default:
            return QStringLiteral("default");
        case UiState::Hover:
            return QStringLiteral("hover");
        case UiState::Focus:
            return QStringLiteral("focus");
        case UiState::Active:
            return QStringLiteral("active");
        case UiState::Disabled:
            return QStringLiteral("disabled");
        case UiState::Loading:
            return QStringLiteral("loading");
        case UiState::Error:
            return QStringLiteral("error");
        default:
            return QStringLiteral("default");
    }
}

enum class UiSize {
    Small,
    Medium,
    Large
};

inline QString toString(UiSize size) {
    switch (size) {
        case UiSize::Small:
            return QStringLiteral("sm");
        case UiSize::Medium:
            return QStringLiteral("md");
        case UiSize::Large:
            return QStringLiteral("lg");
        default:
            return QStringLiteral("md");
    }
}

enum class UiVariant {
    Primary,
    Secondary,
    Ghost,
    Danger,
    Icon
};

inline QString toString(UiVariant variant) {
    switch (variant) {
        case UiVariant::Primary:
            return QStringLiteral("primary");
        case UiVariant::Secondary:
            return QStringLiteral("secondary");
        case UiVariant::Ghost:
            return QStringLiteral("ghost");
        case UiVariant::Danger:
            return QStringLiteral("danger");
        case UiVariant::Icon:
            return QStringLiteral("icon");
        default:
            return QStringLiteral("primary");
    }
}

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

enum class UiTextTypes {
    DialogTitle,
    DialogMessage,
    GalleryTitle,
    GalleryDesc,
    H1,
    H2,
    H3,
    H4,
    H5,
    BodyLarge,
    Body,
    BodySmall,
    Caption,
    Small
};

inline QString toString(UiTextTypes textType) {
    switch (textType) {
        case UiTextTypes::DialogTitle:
            return QStringLiteral("dialogTitle");
        case UiTextTypes::DialogMessage:
            return QStringLiteral("dialogMessage");
        case UiTextTypes::GalleryTitle:
            return QStringLiteral("galleryTitle");
        case UiTextTypes::GalleryDesc:
            return QStringLiteral("galleryDesc");
        case UiTextTypes::H1:
            return QStringLiteral("h1");
        case UiTextTypes::H2:
            return QStringLiteral("h2");
        case UiTextTypes::H3:
            return QStringLiteral("h3");
        case UiTextTypes::H4:
            return QStringLiteral("h4");
        case UiTextTypes::H5:
            return QStringLiteral("h5");
        case UiTextTypes::BodyLarge:
            return QStringLiteral("bodyLarge");
        case UiTextTypes::Body:
            return QStringLiteral("body");
        case UiTextTypes::BodySmall:
            return QStringLiteral("bodySmall");
        case UiTextTypes::Caption:
            return QStringLiteral("caption");
        case UiTextTypes::Small:
            return QStringLiteral("small");
        default:
            return QStringLiteral("body");
    }
};

}  // namespace uikit

#endif  // UIKIT_TYPES_COMMON_TYPES_H
