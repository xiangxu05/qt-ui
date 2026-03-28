#include "uikit/feedback/ui_dialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace uikit {

UiDialog::UiDialog(QWidget* parent)
    : QDialog(parent) {
    setProperty("uiRole", "dialog");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setMinimumWidth(360);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 18, 20, 14);
    layout->setSpacing(12);

    titleLabel_ = new QLabel(this);
    titleLabel_->setProperty("uiRole", "dialogTitle");
    messageLabel_ = new QLabel(this);
    messageLabel_->setProperty("uiRole", "dialogMessage");
    messageLabel_->setWordWrap(true);

    buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox_, &QDialogButtonBox::accepted, this, &UiDialog::accept);
    connect(buttonBox_, &QDialogButtonBox::rejected, this, &UiDialog::reject);

    layout->addWidget(titleLabel_);
    layout->addWidget(messageLabel_);
    layout->addWidget(buttonBox_);
}

void UiDialog::setTitle(const QString& title) {
    titleLabel_->setText(title);
}

void UiDialog::setMessage(const QString& message) {
    messageLabel_->setText(message);
}

bool UiDialog::confirm(QWidget* parent, const QString& title, const QString& message) {
    UiDialog dialog(parent);
    dialog.setTitle(title);
    dialog.setMessage(message);
    return dialog.exec() == QDialog::Accepted;
}

void UiDialog::info(QWidget* parent, const QString& title, const QString& message) {
    UiDialog dialog(parent);
    dialog.buttonBox_->setStandardButtons(QDialogButtonBox::Ok);
    dialog.setTitle(title);
    dialog.setMessage(message);
    dialog.exec();
}

}  // namespace uikit
