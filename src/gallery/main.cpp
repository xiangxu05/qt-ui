#include <QApplication>
#include <QColor>
#include <QLabel>
#include <QPainter>
#include <QResource>
#include <QStyle>
#include <QVBoxLayout>

#include "gallery/demo_button_widget.h"

#include "uikit/core/theme_tokens.h"
#include "uikit/core/theme_manager.h"
#include "uikit/shell/app_shell_window.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("qt-ui");

    // Explicitly initialize resources from static library target `uikit`.
    Q_INIT_RESOURCE(uikit);
    auto& themeManager = uikit::ThemeManager::instance();
    themeManager.loadThemeFromSettings();

    // 红色品牌色示例：深色基底 + BrandPreset::Red（也可传任意 QColor）
    // 运行：`qt-ui.exe --brand-red`
    const bool brandRedExample = app.arguments().contains(QStringLiteral("--brand-red"));
    if (brandRedExample) {
        themeManager.setCustomTokens(
            uikit::ThemeTokenFactory::createThemeWithBrand(uikit::BrandPreset::Red, false));
        themeManager.setTheme(uikit::ThemeManager::ThemeType::Custom, false);
    } else {
        themeManager.setTheme(uikit::ThemeManager::ThemeType::Light, false);
    }
    themeManager.applyTo(&app);

    uikit::UiAppShellWindow shell;
    shell.setMinimumWindowSize(800, 600);
    shell.resize(880, 620);

    auto parsePosition = [&](const QStringList& args) {
        for (const QString& arg : args) {
            if (arg == "--nav-top") {
                return uikit::UiNavigationPanel::Position::Top;
            }
            if (arg == "--nav-right") {
                return uikit::UiNavigationPanel::Position::Right;
            }
            if (arg == "--nav-bottom") {
                return uikit::UiNavigationPanel::Position::Bottom;
            }
            if (arg == "--nav-left") {
                return uikit::UiNavigationPanel::Position::Left;
            }
        }
        return uikit::UiNavigationPanel::Position::Left;
    };
    const auto navPosition = parsePosition(app.arguments());
    shell.setNavigationPosition(navPosition);

    auto createPage = [](const QString& title, const QString& desc) {
        // Keep page detached from top-level shell. AppShellWindow will adopt
        // pages into contentHost_ through setNavigationModel().
        auto* page = new QWidget(nullptr);
        page->setProperty("uiRole", "galleryContentPage");
        auto* layout = new QVBoxLayout(page);
        layout->setContentsMargins(24, 24, 24, 24);
        layout->setSpacing(8);

        auto* titleLabel = new QLabel(title, page);
        titleLabel->setProperty("uiRole", "galleryTitle");
        layout->addWidget(titleLabel);

        auto* descLabel = new QLabel(desc, page);
        descLabel->setProperty("uiRole", "galleryDesc");
        layout->addWidget(descLabel);
        layout->addStretch();
        return page;
    };

    const QColor logoPrimary = brandRedExample
        ? uikit::ThemeTokenFactory::brandPresetColor(uikit::BrandPreset::Red)
        : QColor(QStringLiteral("#4a78a6"));

    QPixmap logo(24, 24);
    logo.fill(Qt::transparent);
    {
        QPainter painter(&logo);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::NoPen);
        painter.setBrush(logoPrimary);
        painter.drawRoundedRect(0, 0, 24, 24, 6, 6);
        painter.setPen(Qt::white);
        painter.drawText(logo.rect(), Qt::AlignCenter, "Q");
    }

    uikit::UiNavigationPanel::Model navModel;
    navModel.header.logo = logo;
    navModel.header.title = "Qt UI Workbench";
    const auto iconFromStyle = [&](QStyle::StandardPixmap sp) { return app.style()->standardIcon(sp); };
    navModel.primaryItems = {
        {"state-demo",
         "状态演示",
         iconFromStyle(QStyle::SP_CommandLink),
         new DemoButtonWidget(nullptr)},
        // {"dashboard", "仪表盘", iconFromStyle(QStyle::SP_DesktopIcon), createPage("仪表盘", "主列表第 1 项（有图标）")},
        // {"message", "消息中心", iconFromStyle(QStyle::SP_MessageBoxInformation), createPage("消息中心", "主列表第 2 项（有图标）")},
        // {"contacts", "联系人", iconFromStyle(QStyle::SP_DirHomeIcon), createPage("联系人", "主列表第 3 项（有图标）")},
        // {"calendar", "日程", QIcon(), createPage("日程", "主列表第 4 项（无图标，Top/Bottom 显示文字）")},
        // {"doc", "文档", iconFromStyle(QStyle::SP_FileIcon), createPage("文档", "主列表第 5 项（有图标）")},
        // {"tasks", "任务", iconFromStyle(QStyle::SP_DialogApplyButton), createPage("任务", "主列表第 6 项（有图标）")},
        // {"repo", "代码仓", QIcon(), createPage("代码仓", "主列表第 7 项（无图标，Top/Bottom 显示文字）")},
        // {"deploy", "发布", iconFromStyle(QStyle::SP_BrowserReload), createPage("发布", "主列表第 8 项（有图标）")}
    };
    navModel.secondarySections = {
    //     {"次要列表 A",
    //      {{"analytics", "数据分析", iconFromStyle(QStyle::SP_DriveNetIcon), createPage("数据分析", "次要列表 A - 1")},
    //       {"monitor", "监控告警", iconFromStyle(QStyle::SP_BrowserStop), createPage("监控告警", "次要列表 A - 2")},
    //       {"audit", "审计日志", iconFromStyle(QStyle::SP_FileDialogDetailedView), createPage("审计日志", "次要列表 A - 3")}}},
    //     {"次要列表 B",
    //      {{"setting", "系统设置", iconFromStyle(QStyle::SP_FileDialogContentsView), createPage("系统设置", "次要列表 B - 1")},
    //       {"permission", "权限管理", QIcon(), createPage("权限管理", "次要列表 B - 2（无图标）")},
    //       {"integration", "集成中心", iconFromStyle(QStyle::SP_ComputerIcon), createPage("集成中心", "次要列表 B - 3")},
    //       {"about", "关于", iconFromStyle(QStyle::SP_MessageBoxQuestion), createPage("关于", "次要列表 B - 4")}}}
    };
    shell.setNavigationModel(navModel);
    shell.show();

    return app.exec();
}
