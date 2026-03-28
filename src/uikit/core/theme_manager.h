#ifndef UIKIT_THEME_MANAGER_H
#define UIKIT_THEME_MANAGER_H

#include <QObject>
#include <QString>

#include "uikit/core/theme_tokens.h"

class QApplication;

namespace uikit {

class ThemeManager : public QObject {
    Q_OBJECT
public:
    static ThemeManager& instance();

    void applyTo(QApplication* app);
    QString resolvedStyleSheet() const;
    ThemeTokens tokens() const;

private:
    ThemeManager() = default;
    QString loadResource(const QString& path) const;
    QString applyTokens(QString qss, const ThemeTokens& tokenSet) const;
};

}  // namespace uikit

#endif  // UIKIT_THEME_MANAGER_H
