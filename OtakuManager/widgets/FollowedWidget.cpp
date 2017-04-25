#include "FollowedWidget.h"

FollowedWidget::FollowedWidget(FollowedAnime* followed, QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);
	delete ui.separator;
	this->followed = followed;
	ui.animeTitle->setText(followed->anime);
	ui.websiteComboBox->insertItem(0, "*");
	ui.websiteComboBox->insertItems(1, OMA::websites);
	ui.websiteComboBox->setCurrentText(followed->website);
	ui.customLinkLineEdit->setText(followed->customLink);
}

FollowedWidget::FollowedWidget(QStringList list, QWidget* parent) : QWidget(parent) {
	FollowedWidget(new FollowedAnime(list), parent);
}
