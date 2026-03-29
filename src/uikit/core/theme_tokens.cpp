#include "uikit/core/theme_tokens.h"

#include <QColor>
#include <QMap>

namespace uikit {
namespace {

void mergeDarkOverrides(ThemeTokens* out) {
    const QMap<QString, QString> k = {
        // Core surfaces and text
        {"@BG_BASE@", "#1a1a1a"},
        {"@BG_ELEVATED@", "#2d2d2d"},
        {"@TEXT_PRIMARY@", "#e8e8e8"},
        {"@TEXT_SECONDARY@", "#a8a8a8"},
        {"@TEXT_MUTED@", "#6e6e6e"},
        {"@TEXT_DISABLED@", "#555555"},
        {"@BORDER_BASE@", "#3d3d3d"},
        {"@BORDER_STRONG@", "#4d4d4d"},
        {"@BORDER_WEAK@", "#2a2a2a"},

        // Brand (slightly brighter on dark canvas)
        {"@PRIMARY@", "#6b9bd4"},
        {"@PRIMARY_HOVER@", "#7aabdf"},
        {"@PRIMARY_PRESSED@", "#558bca"},
        {"@PRIMARY_SOFT@", "#1e2a38"},
        {"@ON_PRIMARY@", "#ffffff"},
        {"@PRIMARY_FOCUS_RING@", "0 0 0 3px rgba(107, 155, 212, 0.28)"},
        {"@DANGER@", "#e85d5a"},
        {"@DANGER_FOCUS_RING@", "0 0 0 3px rgba(232, 93, 90, 0.22)"},

        {"@SUCCESS@", "#5cbf2a"},
        {"@SUCCESS_HOVER@", "#6fd13d"},
        {"@SUCCESS_PRESSED@", "#4aa820"},
        {"@WARNING@", "#ffc53d"},
        {"@WARNING_HOVER@", "#ffd666"},
        {"@WARNING_PRESSED@", "#d9a520"},
        {"@INFO@", "#4da3ff"},
        {"@INFO_HOVER@", "#69b3ff"},
        {"@INFO_PRESSED@", "#2f8ae6"},

        // Gray scale (dark UI progression)
        {"@GRAY_1@", "#0d0d0d"},
        {"@GRAY_2@", "#141414"},
        {"@GRAY_3@", "#1a1a1a"},
        {"@GRAY_4@", "#222222"},
        {"@GRAY_5@", "#3d3d3d"},
        {"@GRAY_6@", "#5a5a5a"},
        {"@GRAY_7@", "#8c8c8c"},
        {"@GRAY_8@", "#b0b0b0"},
        {"@GRAY_9@", "#d0d0d0"},
        {"@GRAY_10@", "#ececec"},

        {"@LINK@", "#7aabdf"},
        {"@LINK_HOVER@", "#9bc4f0"},
        {"@LINK_ACTIVE@", "#5a8bc8"},
        {"@BG_OVERLAY@", "rgba(0, 0, 0, 0.65)"},
        {"@BG_SPOTLIGHT@", "rgba(30, 35, 42, 0.95)"},

        {"@DISABLED_BG@", "#2a2f36"},
        {"@DISABLED_BORDER@", "#3a4555"},
        {"@SWITCH_TRACK_BG@", "#3a4048"},

        {"@GLASS_TITLE@", "#2a2a2a"},
        {"@GLASS_NAV@", "#252525"},
        {"@GLASS_BORDER@", "#3d3d3d"},
        {"@CONTENT_BG@", "#141414"},
        {"@NAV_HEADER_DIVIDER@", "#3d3d3d"},
        {"@NAV_SCROLL_THUMB@", "#5a6a7a"},
        {"@NAV_SCROLL_THUMB_HOVER@", "#6a7a8a"},

        {"@TITLE_BTN_TEXT@", "#e0e0e0"},
        {"@TITLE_BTN_HOVER@", "#2a3544"},
        {"@TITLE_BTN_PRESSED@", "#1f2835"},
        {"@TITLE_BTN_CHECKED@", "#243040"},
        {"@TITLE_CLOSE_HOVER@", "#e8504d"},
        {"@TITLE_CLOSE_PRESSED@", "#cf4744"},
        {"@TITLE_CLOSE_TEXT@", "#ffffff"},
        {"@NAV_ITEM_HOVER@", "#2a3544"},

        {"@STATE_SUCCESS_BG@", "#1a2f22"},
        {"@STATE_SUCCESS_BORDER@", "#2d5a40"},
        {"@STATE_WARNING_BG@", "#2f2a1a"},
        {"@STATE_WARNING_BORDER@", "#6a5a2a"},
        {"@STATE_ERROR_BG@", "#2f1a1a"},
        {"@STATE_ERROR_BORDER@", "#6a3a3a"},
        {"@STATE_INFO_BG@", "#1a2838"},
        {"@STATE_INFO_BORDER@", "#3a5a78"},
        {"@TOAST_BG@", "#0f1419"},
        {"@TOAST_SUCCESS_BG@", "#2d4a3a"},
        {"@TOAST_WARNING_BG@", "#5a4a2a"},
        {"@TOAST_ERROR_BG@", "#5a3a40"},

        // Stronger shadows on dark backgrounds
        {"@SHADOW_SM@", "0 1px 3px rgba(0, 0, 0, 0.45)"},
        {"@SHADOW_MD@", "0 4px 12px rgba(0, 0, 0, 0.5)"},
        {"@SHADOW_LG@", "0 10px 24px rgba(0, 0, 0, 0.55)"},
        {"@SHADOW_XL@", "0 20px 40px rgba(0, 0, 0, 0.6)"},
    };
    for (auto it = k.constBegin(); it != k.constEnd(); ++it) {
        out->vars[it.key()] = it.value();
    }
}

}  // namespace

namespace {

QString hexRgb(const QColor& c) {
    return c.name(QColor::HexRgb);
}

QString focusRingBoxShadow(const QColor& c, qreal alpha) {
    return QStringLiteral("0 0 0 3px rgba(%1,%2,%3,%4)")
        .arg(c.red())
        .arg(c.green())
        .arg(c.blue())
        .arg(alpha, 0, 'f', 2);
}

QColor blendColors(const QColor& a, const QColor& b, qreal t) {
    return QColor::fromRgbF(a.redF() + (b.redF() - a.redF()) * t,
                            a.greenF() + (b.greenF() - a.greenF()) * t,
                            a.blueF() + (b.blueF() - a.blueF()) * t,
                            a.alphaF() + (b.alphaF() - a.alphaF()) * t);
}

QColor primarySoftForBrand(const QColor& brand, bool baseDark) {
    if (baseDark) {
        return blendColors(QColor(0x2d, 0x2d, 0x2d), brand, 0.22);
    }
    return blendColors(QColor(255, 255, 255), brand, 0.14);
}

}  // namespace

ThemeTokens ThemeTokenFactory::lightBlueTheme() {
    ThemeTokens tokens;
    tokens.vars = {
        // Core surfaces and text
        {"@BG_BASE@", "#f4f4f4"},
        {"@BG_ELEVATED@", "#ffffff"},
        {"@TEXT_PRIMARY@", "#191919"},
        {"@TEXT_SECONDARY@", "#6b6b6b"},
        {"@TEXT_MUTED@", "#9a9a9a"},
        {"@TEXT_DISABLED@", "#bfbfbf"},
        {"@BORDER_BASE@", "#dddddd"},
        {"@BORDER_STRONG@", "#d0d0d0"},
        {"@BORDER_WEAK@", "#eeeeee"},

        // Brand and semantic emphasis
        {"@PRIMARY@", "#4a78a6"},
        {"@PRIMARY_HOVER@", "#5a88b5"},
        {"@PRIMARY_PRESSED@", "#3f6d9b"},
        {"@PRIMARY_SOFT@", "#e7f0fa"},
        {"@ON_PRIMARY@", "#ffffff"},
        {"@PRIMARY_FOCUS_RING@", "0 0 0 3px rgba(74, 120, 166, 0.20)"},
        {"@DANGER@", "#d9534f"},
        {"@DANGER_FOCUS_RING@", "0 0 0 3px rgba(217, 83, 79, 0.15)"},

        // Semantic functional colors (for controls / badges)
        {"@SUCCESS@", "#52c41a"},
        {"@SUCCESS_HOVER@", "#73d13d"},
        {"@SUCCESS_PRESSED@", "#389e0d"},
        {"@WARNING@", "#faad14"},
        {"@WARNING_HOVER@", "#ffc53d"},
        {"@WARNING_PRESSED@", "#d48806"},
        {"@INFO@", "#1890ff"},
        {"@INFO_HOVER@", "#40a9ff"},
        {"@INFO_PRESSED@", "#096dd9"},

        // Neutral gray scale (for surfaces / borders)
        {"@GRAY_1@", "#ffffff"},
        {"@GRAY_2@", "#fafafa"},
        {"@GRAY_3@", "#f5f5f5"},
        {"@GRAY_4@", "#f0f0f0"},
        {"@GRAY_5@", "#d9d9d9"},
        {"@GRAY_6@", "#bfbfbf"},
        {"@GRAY_7@", "#8c8c8c"},
        {"@GRAY_8@", "#595959"},
        {"@GRAY_9@", "#434343"},
        {"@GRAY_10@", "#262626"},

        // Link / overlay (semantic)
        {"@LINK@", "#4a78a6"},
        {"@LINK_HOVER@", "#5a88b5"},
        {"@LINK_ACTIVE@", "#3f6d9b"},
        {"@BG_OVERLAY@", "rgba(0, 0, 0, 0.45)"},
        {"@BG_SPOTLIGHT@", "rgba(255, 255, 255, 0.92)"},

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
        {"@STATE_INFO_BG@", "#e7f3fc"},
        {"@STATE_INFO_BORDER@", "#b8d4e8"},
        {"@TOAST_BG@", "#1f2a37"},
        {"@TOAST_SUCCESS_BG@", "#3f6f5c"},
        {"@TOAST_WARNING_BG@", "#8e7241"},
        {"@TOAST_ERROR_BG@", "#8e4d56"},

        // Typography (for QSS font-size / font-weight / line-height)
        {"@FONT_SIZE_H1@", "38px"},
        {"@FONT_SIZE_H2@", "30px"},
        {"@FONT_SIZE_H3@", "24px"},
        {"@FONT_SIZE_H4@", "20px"},
        {"@FONT_SIZE_H5@", "16px"},
        {"@FONT_SIZE_BODY_LG@", "16px"},
        {"@FONT_SIZE_BODY@", "14px"},
        {"@FONT_SIZE_BODY_SM@", "13px"},
        {"@FONT_SIZE_CAPTION@", "12px"},
        {"@FONT_SIZE_SMALL@", "11px"},
        {"@FONT_WEIGHT_REGULAR@", "400"},
        {"@FONT_WEIGHT_MEDIUM@", "500"},
        {"@FONT_WEIGHT_SEMIBOLD@", "600"},
        {"@FONT_WEIGHT_BOLD@", "700"},
        {"@LINE_HEIGHT_TIGHT@", "1.25"},
        {"@LINE_HEIGHT_NORMAL@", "1.5"},
        {"@LINE_HEIGHT_RELAXED@", "1.75"},

        // Shape tokens
        {"@RADIUS_XS@", "2px"},
        {"@RADIUS_SM@", "6px"},
        {"@RADIUS_MD@", "10px"},
        {"@RADIUS_LG@", "14px"},
        {"@RADIUS_XL@", "16px"},
        {"@RADIUS_2XL@", "20px"},
        {"@RADIUS_FULL@", "9999px"},

        // Spacing scale (margins / gaps / padding in QSS)
        {"@SPACING_XS@", "4px"},
        {"@SPACING_SM@", "8px"},
        {"@SPACING_MD@", "12px"},
        {"@SPACING_LG@", "16px"},
        {"@SPACING_XL@", "20px"},
        {"@SPACING_2XL@", "24px"},
        {"@PADDING_SM@", "8px"},
        {"@PADDING_MD@", "12px"},
        {"@PADDING_LG@", "16px"},
        {"@MARGIN_SM@", "8px"},
        {"@MARGIN_MD@", "12px"},
        {"@MARGIN_LG@", "16px"},

        // Border system (QSS: border-width / border-style)
        {"@BORDER_WIDTH@", "1px"},
        {"@BORDER_WIDTH_STRONG@", "2px"},
        {"@BORDER_STYLE_SOLID@", "solid"},
        {"@BORDER_STYLE_DASHED@", "dashed"},
        {"@BORDER_STYLE_DOTTED@", "dotted"},

        // Elevation (use in QSS `box-shadow` where the style supports it, e.g. Qt 6+)
        {"@SHADOW_SM@", "0 1px 2px rgba(0, 0, 0, 0.05)"},
        {"@SHADOW_MD@", "0 4px 6px rgba(0, 0, 0, 0.1)"},
        {"@SHADOW_LG@", "0 10px 15px rgba(0, 0, 0, 0.1)"},
        {"@SHADOW_XL@", "0 20px 25px rgba(0, 0, 0, 0.15)"},

        // Motion (QSS time values or reference; `QPropertyAnimation` uses ms as int in code)
        {"@DURATION_FAST@", "100ms"},
        {"@DURATION_NORMAL@", "200ms"},
        {"@DURATION_SLOW@", "300ms"},
        {"@DURATION_FAST_MS@", "100"},
        {"@DURATION_NORMAL_MS@", "200"},
        {"@DURATION_SLOW_MS@", "300"},

        {"@EASING_EASE_IN_OUT@", "cubic-bezier(0.4, 0, 0.2, 1)"},
        {"@EASING_EASE_OUT@", "cubic-bezier(0.0, 0, 0.2, 1)"},
        {"@EASING_EASE_IN@", "cubic-bezier(0.4, 0, 1, 1)"}
    };
    return tokens;
}

ThemeTokens ThemeTokenFactory::darkTheme() {
    ThemeTokens t = lightBlueTheme();
    mergeDarkOverrides(&t);
    return t;
}

ThemeTokens ThemeTokenFactory::createThemeWithBrand(const QColor& brandPrimary, bool baseDark) {
    ThemeTokens t = baseDark ? darkTheme() : lightBlueTheme();
    auto& v = t.vars;
    const QColor b = brandPrimary;
    v["@PRIMARY@"] = hexRgb(b);
    v["@PRIMARY_HOVER@"] = hexRgb(b.lighter(120));
    v["@PRIMARY_PRESSED@"] = hexRgb(b.darker(120));
    v["@PRIMARY_SOFT@"] = hexRgb(primarySoftForBrand(b, baseDark));
    v["@LINK@"] = hexRgb(b);
    v["@LINK_HOVER@"] = hexRgb(b.lighter(115));
    v["@LINK_ACTIVE@"] = hexRgb(b.darker(115));
    v["@INFO@"] = hexRgb(b.lighter(108));
    v["@INFO_HOVER@"] = hexRgb(b.lighter(125));
    v["@INFO_PRESSED@"] = hexRgb(b.darker(115));
    v["@STATE_INFO_BG@"] = hexRgb(primarySoftForBrand(b, baseDark));
    v["@STATE_INFO_BORDER@"] = hexRgb(b.darker(118));
    v["@PRIMARY_FOCUS_RING@"] = focusRingBoxShadow(b, baseDark ? 0.28 : 0.20);
    v["@DANGER_FOCUS_RING@"] = focusRingBoxShadow(QColor(v["@DANGER@"]), baseDark ? 0.22 : 0.15);
    return t;
}

QColor ThemeTokenFactory::brandPresetColor(BrandPreset preset) {
    switch (preset) {
        case BrandPreset::Blue:
            return QColor(QStringLiteral("#4a78a6"));
        case BrandPreset::Red:
            return QColor(QStringLiteral("#c94c4c"));
        case BrandPreset::Green:
            return QColor(QStringLiteral("#3d8b6e"));
        case BrandPreset::Purple:
            return QColor(QStringLiteral("#7d5a9a"));
        case BrandPreset::Orange:
            return QColor(QStringLiteral("#d97a2e"));
    }
    return QColor(QStringLiteral("#4a78a6"));
}

ThemeTokens ThemeTokenFactory::createThemeWithBrand(BrandPreset preset, bool baseDark) {
    return createThemeWithBrand(brandPresetColor(preset), baseDark);
}

}  // namespace uikit
