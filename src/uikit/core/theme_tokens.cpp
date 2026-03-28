#include "uikit/core/theme_tokens.h"

namespace uikit {

ThemeTokens ThemeTokenFactory::lightBlueTheme() {
    ThemeTokens tokens;
    tokens.vars = {
        // Core surfaces and text
        {"@BG_BASE@", "#f4f4f4"},
        {"@BG_ELEVATED@", "#ffffff"},
        {"@TEXT_PRIMARY@", "#191919"},
        {"@TEXT_SECONDARY@", "#6b6b6b"},
        {"@TEXT_MUTED@", "#9a9a9a"},
        {"@BORDER_BASE@", "#dddddd"},
        {"@BORDER_STRONG@", "#d0d0d0"},

        // Brand and semantic emphasis
        {"@PRIMARY@", "#4a78a6"},
        {"@PRIMARY_HOVER@", "#5a88b5"},
        {"@PRIMARY_PRESSED@", "#3f6d9b"},
        {"@PRIMARY_SOFT@", "#e7f0fa"},
        {"@ON_PRIMARY@", "#ffffff"},
        {"@DANGER@", "#d9534f"},

        // Control state helpers
        {"@DISABLED_BG@", "#e8edf3"},
        {"@DISABLED_BORDER@", "#dde4ee"},
        {"@SWITCH_TRACK_BG@", "#d8dee8"},

        // Shell layout (title/navigation/content)
        {"@GLASS_TITLE@", "#ededed"},
        {"@GLASS_NAV@", "#ededed"},
        {"@GLASS_BORDER@", "#d9d9d9"},
        {"@CONTENT_BG@", "#f7f7f7"},
        {"@NAV_HEADER_DIVIDER@", "#d7dde5"},
        {"@NAV_SCROLL_TRACK@", "transparent"},
        {"@NAV_SCROLL_THUMB@", "#c2cfdf"},
        {"@NAV_SCROLL_THUMB_HOVER@", "#a9bcd3"},

        // Title bar behavior and metrics
        {"@TITLE_BAR_HEIGHT@", "40px"},
        {"@TITLE_BTN_TEXT@", "#303030"},
        {"@TITLE_BTN_HOVER@", "#e7f0fa"},
        {"@TITLE_BTN_PRESSED@", "#d8e6f4"},
        {"@TITLE_BTN_CHECKED@", "#dfeaf8"},
        {"@TITLE_CLOSE_HOVER@", "#e8504d"},
        {"@TITLE_CLOSE_PRESSED@", "#cf4744"},
        {"@TITLE_CLOSE_TEXT@", "#ffffff"},
        {"@NAV_ITEM_HOVER@", "#edf3fa"},

        // Feedback surfaces (message bar / toast)
        {"@STATE_SUCCESS_BG@", "#edf8f2"},
        {"@STATE_SUCCESS_BORDER@", "#c8e7d8"},
        {"@STATE_WARNING_BG@", "#fcf8ef"},
        {"@STATE_WARNING_BORDER@", "#ecd8b4"},
        {"@STATE_ERROR_BG@", "#fbf0f2"},
        {"@STATE_ERROR_BORDER@", "#e9c5ca"},
        {"@TOAST_BG@", "#1f2a37"},
        {"@TOAST_SUCCESS_BG@", "#3f6f5c"},
        {"@TOAST_WARNING_BG@", "#8e7241"},
        {"@TOAST_ERROR_BG@", "#8e4d56"},

        // Shape tokens
        {"@RADIUS_SM@", "6px"},
        {"@RADIUS_MD@", "10px"},
        {"@RADIUS_LG@", "14px"}
    };
    return tokens;
}

}  // namespace uikit
