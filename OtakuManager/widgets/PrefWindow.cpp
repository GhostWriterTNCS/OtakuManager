#include <iostream>
#include <QComboBox>
#include <QItemDelegate>
#include <QTreeWidgetItem>
#include "PrefWindow.h"

PrefWindow::PrefWindow(QWidget* parent) : QDialog(parent) {
	ui.setupUi(this);
	ui.updateCheckBox->setChecked(OMA::Settings::getCheckForUpdates());

	ui.streaming->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::streaming]));
	ui.streaming2->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::streaming2]));
	ui.streamingIfNoDownload->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::streamingIfNoDownload]));
	ui.streaming2IfNoDownload->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::streaming2IfNoDownload]));
	ui.download->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::download]));
	ui.download2->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::download2]));
	ui.animeInfo->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::animeInfo]));
	ui.post->setChecked(OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::post]));
	ui.postIfNoDownload->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::postIfNoDownload]));

	QList<FollowedAnime> list = OMA::Settings::getFollowed();
	for (int i = 0; i < list.size(); i++) {
		FollowedWidget* widget = new FollowedWidget(&(list[i]));
		QListWidgetItem* item = new QListWidgetItem();
		item->setSizeHint(QSize(item->sizeHint().width(), widget->sizeHint().height()));
		ui.followedListWidget->addItem(item);
		ui.followedListWidget->setItemWidget(item, widget);
	}

	/*for (int i = 0; i < OMA::websites.size(); i++) {
		ui.availableWebsites->addItem(OMA::websites[i]);
	}*/
	QStringList activeWebsites = OMA::Settings::getWebsites();
	for (int i = 0; i < activeWebsites.size(); i++) {
		ui.activeWebsites->addItem(activeWebsites[i]);
	}

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &PrefWindow::save);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);
}

void PrefWindow::save() {
	OMA::Settings::setCheckForUpdates(ui.updateCheckBox->isChecked());

	QStringList websitesList;
	for (int i = 0; i < ui.activeWebsites->count(); ++i) {
		websitesList.append(ui.activeWebsites->item(i)->text());
	}
	OMA::Settings::setWebsites(websitesList);

	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::streaming], ui.streaming->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::streaming2], ui.streaming2->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::streamingIfNoDownload],
							  ui.streamingIfNoDownload->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::streaming2IfNoDownload],
							  ui.streaming2IfNoDownload->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::download], ui.download->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::download2], ui.download2->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::animeInfo], ui.animeInfo->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::post], ui.post->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::postIfNoDownload],
							  ui.postIfNoDownload->isChecked());

	QList<FollowedAnime> followedList;
	for (int i = 0; i < ui.followedListWidget->count(); i++) {
		FollowedWidget* followed =
			(FollowedWidget*)(ui.followedListWidget->itemWidget(ui.followedListWidget->item(i)));
		followedList.append(FollowedAnime({followed->ui.animeTitle->text(),
										   followed->ui.websiteComboBox->currentText(),
										   followed->ui.customLinkLineEdit->text()}));
	}
	OMA::Settings::setFollowed(followedList);

	close();
}

void PrefWindow::on_removeWebsite_clicked() {
	if (ui.activeWebsites->selectedItems().size() > 0) {
		delete ui.activeWebsites->selectedItems()[0];
	}
}
void PrefWindow::on_addWebsite_clicked() {
	if (ui.availableWebsites->selectedItems().size() > 0) {
		QTreeWidgetItem* item = ui.availableWebsites->selectedItems()[0];
		if (item->childCount() == 0) {
			bool found = false;
			for (int i = 0; i < ui.activeWebsites->count(); ++i) {
				if (ui.activeWebsites->item(i)->text() == item->text(0)) {
					found = true;
					break;
				}
			}
			if (!found) {
				ui.activeWebsites->addItem(item->text(0));
			}
		}
	}
}

void PrefWindow::on_addFollowed_clicked() {
	FollowedWidget* widget = new FollowedWidget(new FollowedAnime({"", "*", ""}));
	QListWidgetItem* item = new QListWidgetItem();
	item->setSizeHint(QSize(item->sizeHint().width(), widget->sizeHint().height()));
	ui.followedListWidget->addItem(item);
	ui.followedListWidget->setItemWidget(item, widget);
}

void PrefWindow::on_remove_clicked() {
	QList<QListWidgetItem*> list = ui.followedListWidget->selectedItems();
	if (list.size() > 0) {
		delete list[0];
	}
}
