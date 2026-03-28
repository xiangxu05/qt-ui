#include "gallery/demo_gallery_widget.h"

#include <QLabel>
#include <QVBoxLayout>

DemoGalleryWidget::DemoGalleryWidget(uikit::UiAppShellWindow* shell, QWidget* parent)
    : QWidget(parent), shell_(shell) {
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    root->setSpacing(8);

    auto* title = new QLabel("Style Load Check", this);
    title->setObjectName("StyleProbeTitle");
    title->setProperty("uiRole", "galleryTitle");
    root->addWidget(title);

    auto* desc = new QLabel("Only this title is rendered to isolate style loading.", this);
    desc->setProperty("uiRole", "galleryDesc");
    root->addWidget(desc);

    root->addStretch();
}
