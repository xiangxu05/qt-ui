#include "uikit/core/theme_manager.h"

#include <QApplication>
#include <QDebug>
#include <QFile>

namespace uikit {

ThemeManager& ThemeManager::instance() {
    static ThemeManager manager;
    return manager;
}

void ThemeManager::applyTo(QApplication* app) {
    if (!app) {
        return;
    }
    app->setStyleSheet(resolvedStyleSheet());
}

QString ThemeManager::resolvedStyleSheet() const {
    const ThemeTokens tokenSet = tokens();
    const QString stylePrefix = QStringLiteral(":/uikit/styles/");
    QString merged;
    merged += loadResource(stylePrefix + QStringLiteral("base.qss"));
    merged += "\n";
    merged += loadResource(stylePrefix + QStringLiteral("controls.qss"));
    merged += "\n";
    merged += loadResource(stylePrefix + QStringLiteral("feedback.qss"));
    merged += "\n";
    merged += loadResource(stylePrefix + QStringLiteral("shell.qss"));
    return applyTokens(merged, tokenSet);
}

ThemeTokens ThemeManager::tokens() const {
    return ThemeTokenFactory::lightBlueTheme();
}

QString ThemeManager::loadResource(const QString& path) const {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "ThemeManager failed to open resource:" << path;
        return QString();
    }
    return QString::fromUtf8(file.readAll());
}

QString ThemeManager::applyTokens(QString qss, const ThemeTokens& tokenSet) const {
    for (auto it = tokenSet.vars.constBegin(); it != tokenSet.vars.constEnd(); ++it) {
        qss.replace(it.key(), it.value());
    }
    return qss;
}

}  // namespace uikit
