#include "uikit/core/theme_manager.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QRegularExpression>
#include <QSettings>

namespace uikit {

ThemeManager& ThemeManager::instance() {
    static ThemeManager manager;
    return manager;
}

void ThemeManager::applyTo(QApplication* app) {
    if (!app) {
        return;
    }
    lastApp_ = app;
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
    merged += loadResource(stylePrefix + QStringLiteral("form.qss"));
    merged += "\n";
    merged += loadResource(stylePrefix + QStringLiteral("feedback.qss"));
    merged += "\n";
    merged += loadResource(stylePrefix + QStringLiteral("shell.qss"));
    return applyTokens(merged, tokenSet);
}

ThemeTokens ThemeManager::tokens() const {
    switch (currentTheme_) {
        case ThemeType::Dark:
            return ThemeTokenFactory::darkTheme();
        case ThemeType::Custom:
            if (customTokensValid_) {
                return customTokens_;
            }
            return ThemeTokenFactory::lightBlueTheme();
        case ThemeType::Light:
        default:
            return ThemeTokenFactory::lightBlueTheme();
    }
}

void ThemeManager::setTheme(ThemeType type, bool persist) {
    if (type == ThemeType::Custom && !customTokensValid_) {
        qWarning() << "ThemeManager::setTheme(Custom) ignored: call setCustomTokens() first.";
        return;
    }
    if (currentTheme_ == type) {
        return;
    }
    currentTheme_ = type;
    if (persist) {
        saveThemePreference();
    }
    if (lastApp_) {
        applyTo(lastApp_);
    }
    emit themeChanged(currentTheme_);
}

void ThemeManager::setCustomTokens(const ThemeTokens& tokens) {
    customTokens_ = tokens;
    customTokensValid_ = true;
}

void ThemeManager::clearCustomTokens() {
    customTokens_.vars.clear();
    customTokensValid_ = false;
    if (currentTheme_ == ThemeType::Custom) {
        currentTheme_ = ThemeType::Light;
        saveThemePreference();
        if (lastApp_) {
            applyTo(lastApp_);
        }
        emit themeChanged(currentTheme_);
    }
}

void ThemeManager::loadThemeFromSettings() {
    QSettings settings;
    settings.beginGroup(QStringLiteral("uikit"));
    const int raw = settings.value(QStringLiteral("themeType"), static_cast<int>(ThemeType::Light)).toInt();
    settings.endGroup();
    if (raw < static_cast<int>(ThemeType::Light) || raw > static_cast<int>(ThemeType::Custom)) {
        currentTheme_ = ThemeType::Light;
        return;
    }
    const auto t = static_cast<ThemeType>(raw);
    if (t == ThemeType::Custom && !customTokensValid_) {
        currentTheme_ = ThemeType::Light;
        return;
    }
    currentTheme_ = t;
}

void ThemeManager::saveThemePreference() const {
    QSettings settings;
    settings.beginGroup(QStringLiteral("uikit"));
    settings.setValue(QStringLiteral("themeType"), static_cast<int>(currentTheme_));
    settings.endGroup();
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
    // Guardrail: keep partial style regressions observable.
    static const QRegularExpression kUnresolvedTokenPattern(QStringLiteral(R"(@[A-Z0-9_]+@)"));
    QRegularExpressionMatchIterator it = kUnresolvedTokenPattern.globalMatch(qss);
    if (it.hasNext()) {
        QStringList unresolved;
        while (it.hasNext()) {
            const QString token = it.next().captured(0);
            if (!unresolved.contains(token)) {
                unresolved.push_back(token);
            }
            if (unresolved.size() >= 8) {
                break;
            }
        }
        qWarning() << "ThemeManager unresolved tokens in stylesheet:" << unresolved;
    }
    return qss;
}

}  // namespace uikit
