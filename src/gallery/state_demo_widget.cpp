#include "gallery/state_demo_widget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

#include "uikit/controls/ui_button.h"
#include "uikit/form/ui_line_edit.h"
#include "uikit/form/ui_switch.h"

StateDemoWidget::StateDemoWidget(QWidget* parent)
    : QWidget(parent) {
    setProperty("uiRole", "galleryContentPage");
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(false);
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    root->setSpacing(16);

    auto* title = new QLabel(QStringLiteral("状态演示"), this);
    title->setProperty("uiRole", "galleryTitle");
    root->addWidget(title);

    auto* intro = new QLabel(
        QStringLiteral("使用 Tab / Shift+Tab 在控件间切换焦点，观察 Focus Ring；加载按钮约 2 秒后自动结束。"),
        this);
    intro->setProperty("uiRole", "galleryDesc");
    intro->setWordWrap(true);
    root->addWidget(intro);

    auto* focusCaption = new QLabel(QStringLiteral("焦点链"), this);
    focusCaption->setProperty("uiRole", "galleryDesc");
    root->addWidget(focusCaption);

    auto* focusRow = new QHBoxLayout();
    focusRow->setSpacing(12);

    auto* btnPrimary = new uikit::UiButton(QStringLiteral("Primary"), this);
    btnPrimary->setVariant(uikit::UiButton::Variant::Primary);

    auto* btnSecondary = new uikit::UiButton(QStringLiteral("Secondary"), this);
    btnSecondary->setVariant(uikit::UiButton::Variant::Secondary);

    auto* btnGhost = new uikit::UiButton(QStringLiteral("Ghost"), this);
    btnGhost->setVariant(uikit::UiButton::Variant::Ghost);

    auto* lineFocus = new uikit::UiLineEdit(this);
    lineFocus->setPlaceholderText(QStringLiteral("可聚焦输入"));

    auto* sw = new uikit::UiSwitch(this);
    sw->setText(QStringLiteral("开关"));

    focusRow->addWidget(btnPrimary);
    focusRow->addWidget(btnSecondary);
    focusRow->addWidget(btnGhost);
    focusRow->addWidget(lineFocus, 1);
    focusRow->addWidget(sw);
    root->addLayout(focusRow);

    auto* loadingCaption = new QLabel(QStringLiteral("加载态"), this);
    loadingCaption->setProperty("uiRole", "galleryDesc");
    root->addWidget(loadingCaption);

    auto* loadingRow = new QHBoxLayout();
    loadingRow->setSpacing(12);
    auto* loadingBtn = new uikit::UiButton(QStringLiteral("点我加载 2 秒"), this);
    loadingBtn->setVariant(uikit::UiButton::Variant::Primary);
    loadingRow->addWidget(loadingBtn);
    loadingRow->addStretch();
    root->addLayout(loadingRow);

    connect(loadingBtn, &uikit::UiButton::clicked, this, [loadingBtn]() {
        if (loadingBtn->isLoading()) {
            return;
        }
        loadingBtn->setLoading(true);
        QTimer::singleShot(2000, loadingBtn, [loadingBtn]() { loadingBtn->setLoading(false); });
    });

    auto* errCaption = new QLabel(QStringLiteral("错误输入"), this);
    errCaption->setProperty("uiRole", "galleryDesc");
    root->addWidget(errCaption);

    auto* errEdit = new uikit::UiLineEdit(this);
    errEdit->setPlaceholderText(QStringLiteral("邮箱"));
    errEdit->setErrorState(true);
    errEdit->setErrorMessage(QStringLiteral("格式不正确，请填写有效邮箱。"));
    root->addWidget(errEdit);

    auto* errRow = new QHBoxLayout();
    errRow->setSpacing(12);
    auto* toggleErr = new uikit::UiButton(QStringLiteral("切换错误态"), this);
    toggleErr->setVariant(uikit::UiButton::Variant::Secondary);
    errRow->addWidget(toggleErr);
    errRow->addStretch();
    root->addLayout(errRow);

    connect(toggleErr, &uikit::UiButton::clicked, this, [errEdit]() {
        errEdit->setErrorState(!errEdit->hasError());
        if (errEdit->hasError()) {
            errEdit->setErrorMessage(QStringLiteral("格式不正确，请填写有效邮箱。"));
        } else {
            errEdit->setErrorMessage(QString());
        }
    });

    root->addStretch();

    setTabOrder(btnPrimary, btnSecondary);
    setTabOrder(btnSecondary, btnGhost);
    setTabOrder(btnGhost, lineFocus);
    setTabOrder(lineFocus, sw);
    setTabOrder(sw, loadingBtn);
    setTabOrder(loadingBtn, errEdit);
    setTabOrder(errEdit, toggleErr);
}
