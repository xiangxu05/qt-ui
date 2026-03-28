#ifndef UIKIT_NAVIGATION_PANEL_H
#define UIKIT_NAVIGATION_PANEL_H

#include <QIcon>
#include <QList>
#include <QPixmap>
#include <QString>
#include <QWidget>

class QBoxLayout;
class QButtonGroup;
class QEnterEvent;
class QEvent;
class QResizeEvent;
class QScrollArea;
class QScrollBar;
class QPropertyAnimation;
class QToolButton;

namespace uikit {

class NavigationPanel : public QWidget {
    Q_OBJECT
public:
    struct HeaderModel {
        QPixmap logo;
        QString title;
    };

    struct ItemModel {
        QString key;
        QString text;
        QIcon icon;
        QWidget* page = nullptr;
    };

    struct SectionModel {
        QString title;
        QList<ItemModel> items;
    };

    struct Model {
        HeaderModel header;
        QList<ItemModel> primaryItems;
        QList<SectionModel> secondarySections;
    };

    enum class Position {
        Left,
        Top,
        Right,
        Bottom
    };
    Q_ENUM(Position)

    explicit NavigationPanel(QWidget* parent = nullptr);
    void setModel(const Model& model);
    const Model& model() const;
    void activateItem(const QString& key);

    void setPosition(Position position);
    Position position() const;

    void setCollapsed(bool collapsed);
    bool isCollapsed() const;

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

signals:
    void itemActivated(int index, const QString& key);
    void itemActivatedWithWidget(int index, const QString& key, QWidget* page);

private:
    bool isHorizontal() const;
    QWidget* buildHeaderWidget();
    void ensureCollapseToggle();
    void updateCollapseToggleGeometry();
    QWidget* buildListHostWidget();
    void configureRootLayout();
    void configureScrollContainer();
    void applyPanelSizeForPosition();
    void refreshItemPresentation();
    void refreshCollapseToggle();
    QToolButton* createItemButton(const ItemModel& item, int index);
    void appendSection(QBoxLayout* layout, const QString& title, const QList<ItemModel>& items, int* indexCounter);
    void rebuildLayout();
    void clearLayout();
    void activateFirstItemIfAny();
    void setOverlayScrollBarVisible(bool visible);
    void animateScrollTo(int targetValue);
    void updateOverlayScrollBarGeometry();

    Model model_;
    Position position_ = Position::Left;
    bool collapsed_ = false;
    QBoxLayout* layout_ = nullptr;
    QButtonGroup* buttonGroup_ = nullptr;
    QScrollArea* listScrollArea_ = nullptr;
    QScrollBar* sourceScrollBar_ = nullptr;
    QScrollBar* overlayScrollBar_ = nullptr;
    QPropertyAnimation* scrollAnimation_ = nullptr;
    QToolButton* collapseToggle_ = nullptr;
    QList<QToolButton*> items_;
};

}  // namespace uikit

#endif  // UIKIT_NAVIGATION_PANEL_H
