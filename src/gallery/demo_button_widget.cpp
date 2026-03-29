#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>

#include "gallery/demo_button_widget.h"

#include "uikit/controls/ui_button.h"
#include "uikit/controls/ui_label.h"
#include "uikit/types/common_types.h"

DemoButtonWidget::DemoButtonWidget(QWidget* parent)
    : QWidget(parent) {
    setProperty("uiRole", "galleryContentPage");
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(false);
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    root->setSpacing(16);

    // 标题
    auto* title = new uikit::UiLabel(QStringLiteral("按钮演示"), this);
    title->setProperty("uiRole", "galleryTitle");
    root->addWidget(title);

    // 第一部分
    auto* part1 = new QVBoxLayout();
    part1->setSpacing(16);
    root->addLayout(part1);

    // 第一部分标题
    auto* subTitle1 = new uikit::UiLabel(QStringLiteral("按钮样式"), this);
    subTitle1->setProperty("uiRole", uikit::toString(uikit::UiTextTypes::H2));
    part1->addWidget(subTitle1);

    // 描述
    auto* intro = new uikit::UiLabel(
        QStringLiteral("这部分演示不同类型的按钮，包括 Primary、Secondary、 Ghost 等。"),
        this);
    intro->setProperty("uiRole", uikit::toString(uikit::UiTextTypes::GalleryDesc));
    intro->setWordWrap(true);
    part1->addWidget(intro);

    // 以下新建各个样式的按钮
    auto* part1BtnRow = new QHBoxLayout();
    part1BtnRow->setSpacing(12);
    part1->addLayout(part1BtnRow);

    auto* btnPrimary = new uikit::UiButton(uikit::toString(uikit::UiVariant::Primary), this);
    btnPrimary->setVariant(uikit::UiButton::Variant::Primary);

    auto* btnSecondary = new uikit::UiButton(uikit::toString(uikit::UiVariant::Secondary), this);
    btnSecondary->setVariant(uikit::UiButton::Variant::Secondary);

    auto* btnGhost = new uikit::UiButton(uikit::toString(uikit::UiVariant::Ghost), this);
    btnGhost->setVariant(uikit::UiButton::Variant::Ghost);

    auto* btnDanger = new uikit::UiButton(uikit::toString(uikit::UiVariant::Danger), this);
    btnDanger->setVariant(uikit::UiButton::Variant::Danger);

    auto* btnIcon = new uikit::UiButton(uikit::toString(uikit::UiVariant::Icon), this);
    btnIcon->setVariant(uikit::UiButton::Variant::Icon);

    part1BtnRow->addWidget(btnPrimary);
    part1BtnRow->addWidget(btnSecondary);
    part1BtnRow->addWidget(btnGhost);
    part1BtnRow->addWidget(btnDanger);
    part1BtnRow->addWidget(btnIcon);
    part1BtnRow->addStretch();

    // 第二部分
    auto* part2 = new QVBoxLayout();
    part2->setSpacing(16);
    root->addLayout(part2);

    // 第二部分标题
    auto* subTitle2 = new uikit::UiLabel(QStringLiteral("加载态"), this);
    subTitle2->setProperty("uiRole", uikit::toString(uikit::UiTextTypes::H2));
    part2->addWidget(subTitle2);

    // 描述
    auto* intro2 = new uikit::UiLabel(
        QStringLiteral("这部分演示不同类型的按钮的加载态。"),
        this);
    intro2->setProperty("uiRole", uikit::toString(uikit::UiTextTypes::GalleryDesc));
    intro2->setWordWrap(true);
    part2->addWidget(intro2);

    // 以下新建各个样式的按钮
    auto* part2BtnRow = new QHBoxLayout();
    part2BtnRow->setSpacing(12);
    part2->addLayout(part2BtnRow);

    auto* btnPrimary2 = new uikit::UiButton(uikit::toString(uikit::UiVariant::Primary), this);
    btnPrimary2->setVariant(uikit::UiButton::Variant::Primary);

    auto* btnSecondary2 = new uikit::UiButton(uikit::toString(uikit::UiVariant::Secondary), this);
    btnSecondary2->setVariant(uikit::UiButton::Variant::Secondary);

    auto* btnGhost2 = new uikit::UiButton(uikit::toString(uikit::UiVariant::Ghost), this);
    btnGhost2->setVariant(uikit::UiButton::Variant::Ghost);

    auto* btnDanger2 = new uikit::UiButton(uikit::toString(uikit::UiVariant::Danger), this);
    btnDanger2->setVariant(uikit::UiButton::Variant::Danger);

    auto* btnIcon2 = new uikit::UiButton(uikit::toString(uikit::UiVariant::Icon), this);
    btnIcon2->setVariant(uikit::UiButton::Variant::Icon);

    part2BtnRow->addWidget(btnPrimary2);
    part2BtnRow->addWidget(btnSecondary2);
    part2BtnRow->addWidget(btnGhost2);
    part2BtnRow->addWidget(btnDanger2);
    part2BtnRow->addWidget(btnIcon2);
    part2BtnRow->addStretch();
    
    // 为第二部分的按钮添加加载态
    auto addLoadingEffect = [](uikit::UiButton* button) {
        connect(button, &uikit::UiButton::clicked, button, [button]() {
            if (button->isLoading()) {
                return;
            }
            button->setLoading(true);
            QTimer::singleShot(2000, button, [button]() {
                button->setLoading(false);
            });
        });
    };
    
    addLoadingEffect(btnPrimary2);
    addLoadingEffect(btnSecondary2);
    addLoadingEffect(btnGhost2);
    addLoadingEffect(btnDanger2);
    addLoadingEffect(btnIcon2);

    root->addStretch();

    // 修复后的 Tab 键顺序
    setTabOrder(btnPrimary, btnSecondary);
    setTabOrder(btnSecondary, btnGhost);
    setTabOrder(btnGhost, btnDanger);
    setTabOrder(btnDanger, btnIcon);
    setTabOrder(btnIcon, btnPrimary2);
    setTabOrder(btnPrimary2, btnSecondary2);
    setTabOrder(btnSecondary2, btnGhost2);
    setTabOrder(btnGhost2, btnDanger2);
    setTabOrder(btnDanger2, btnIcon2);
}
