#ifndef UIKIT_THEME_TOKENS_H
#define UIKIT_THEME_TOKENS_H

#include <QMap>
#include <QString>

namespace uikit {

struct ThemeTokens {
    QMap<QString, QString> vars;
};

class ThemeTokenFactory {
public:
    static ThemeTokens lightBlueTheme();
};

}  // namespace uikit

#endif  // UIKIT_THEME_TOKENS_H
