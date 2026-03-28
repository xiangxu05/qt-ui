#ifndef UIKIT_DEMO_GALLERY_WIDGET_H
#define UIKIT_DEMO_GALLERY_WIDGET_H

#include <QWidget>

namespace uikit {
class UiAppShellWindow;
}

class DemoGalleryWidget : public QWidget {
    Q_OBJECT
public:
    explicit DemoGalleryWidget(uikit::UiAppShellWindow* shell, QWidget* parent = nullptr);

signals:
    void requestSetAltPage();
    void requestSwitchAltPage();

private:
    uikit::UiAppShellWindow* shell_ = nullptr;
};

#endif  // UIKIT_DEMO_GALLERY_WIDGET_H
