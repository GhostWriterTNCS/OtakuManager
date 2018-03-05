#include <QStandardItemModel>
#include "FollowedWidget.h"

FollowedWidget::FollowedWidget(FollowedAnime* followed, QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);
	ui.websiteComboBox->insertItem(0, "*");
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.websiteComboBox->model());
	QStringList websites = OMA::websites;
	websites.append("- Feed -");
	QStringList feedNames = OMA::Settings::getFeedNames();
	websites.append(feedNames);
	for (int i = 0; i < websites.size(); i++) {
		ui.websiteComboBox->addItem(websites[i]);
		QStandardItem* item = model->item(i + 1);
		item->setFlags(websites[i].startsWith("-") ? item->flags() & ~Qt::ItemIsEnabled
												   : item->flags() | Qt::ItemIsEnabled);
	}

	this->followed = followed;
	ui.animeTitle->setText(followed->anime);
	ui.regex->setChecked(followed->regex);
	ui.websiteComboBox->setCurrentText(followed->website);
	ui.customLinkLineEdit->setText(followed->customLink);
}
