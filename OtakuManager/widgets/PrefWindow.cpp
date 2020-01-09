#include <QComboBox>
#include <QFileDialog>
#include <QItemDelegate>
#include <QScrollBar>
#include <QTreeWidgetItem>
#include "FeedWidget.h"
#include "FollowedWidget.h"
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
	ui.torrent->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::torrent]));
	ui.magnet->setChecked(OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::magnet]));
	ui.animeInfo->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::animeInfo]));
	ui.post->setChecked(OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::post]));
	ui.postIfNoDownload->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::postIfNoDownload]));

	for (int i = 0; i < OMA::websites.size(); i) {
		QTreeWidgetItem* topItem = new QTreeWidgetItem();
		topItem->setText(0, OMA::websites[i].mid(2, OMA::websites[i].length() - 4));
		i++;
		while (i < OMA::websites.size() && !OMA::websites[i].startsWith("-")) {
			QTreeWidgetItem* item = new QTreeWidgetItem();
			item->setText(0, OMA::websites[i]);
			topItem->addChild(item);
			i++;
		}
		ui.availableWebsites->addTopLevelItem(topItem);
	}
	QTreeWidgetItem* topItem = new QTreeWidgetItem();
	topItem->setText(0, "Feeds");
	QStringList feedNames = OMA::Settings::getFeedNames();
	for (int i = 0; i < feedNames.size(); i++) {
		QTreeWidgetItem* item = new QTreeWidgetItem();
		item->setText(0, feedNames[i]);
		topItem->addChild(item);
	}
	ui.availableWebsites->addTopLevelItem(topItem);

	QList<FollowedAnime> followedList = OMA::Settings::getFollowed();
	for (int i = 0; i < followedList.size(); i++) {
		FollowedWidget* widget = new FollowedWidget(&(followedList[i]));
		QListWidgetItem* item = new QListWidgetItem();
		item->setSizeHint(QSize(item->sizeHint().width(), widget->sizeHint().height()));
		ui.followedListWidget->addItem(item);
		ui.followedListWidget->setItemWidget(item, widget);
	}
	ui.followedListWidget->verticalScrollBar()->setSingleStep(10);

	QStringList activeWebsites = OMA::Settings::getWebsites();
	for (int i = 0; i < activeWebsites.size(); i++) {
		ui.activeWebsites->addItem(activeWebsites[i]);
	}

	QList<Feed> feedList = OMA::Settings::getFeeds();
	for (int i = 0; i < feedList.size(); i++) {
		FeedWidget* widget = new FeedWidget(&(feedList[i]));
		QListWidgetItem* item = new QListWidgetItem();
		item->setSizeHint(QSize(item->sizeHint().width(), widget->sizeHint().height()));
		ui.feedListWidget->addItem(item);
		ui.feedListWidget->setItemWidget(item, widget);
	}
	ui.feedListWidget->verticalScrollBar()->setSingleStep(10);

	ui.AniDexUrl->setText(OMA::Settings::getAniDexUrl());
	ui.downloadTorrents->setChecked(OMA::Settings::getDownloadTorrent());
	ui.torrentsDir->setText(OMA::Settings::getTorrentDir());
	ui.youtubeToUmmy->setChecked(OMA::Settings::getYoutubeToUmmy());
	ui.console->setChecked(OMA::Settings::getConsole());
	ui.curlVerbose->setChecked(OMA::Settings::getCurlVerbose());

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
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::torrent], ui.torrent->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::magnet], ui.magnet->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::animeInfo], ui.animeInfo->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::post], ui.post->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::postIfNoDownload],
							  ui.postIfNoDownload->isChecked());

	QList<FollowedAnime> followedList;
	for (int i = 0; i < ui.followedListWidget->count(); i++) {
		FollowedWidget* followed =
			(FollowedWidget*)(ui.followedListWidget->itemWidget(ui.followedListWidget->item(i)));
		if (!followed->ui.animeTitle->text().isEmpty()) {
			followedList.append(FollowedAnime(followed->ui.animeTitle->text(),
											  followed->ui.regex->isChecked(),
											  followed->ui.websiteComboBox->currentText(),
											  followed->ui.customLinkLineEdit->text()));
		}
	}
	OMA::Settings::setFollowed(followedList);

	QList<Feed> feedList;
	for (int i = 0; i < ui.feedListWidget->count(); i++) {
		FeedWidget* feed = (FeedWidget*)(ui.feedListWidget->itemWidget(ui.feedListWidget->item(i)));
		if (!feed->ui.url->text().isEmpty()) {
			feedList.append(Feed(feed->ui.name->text(), feed->ui.url->text(), feed->oldName));
		}
	}
	OMA::Settings::setFeeds(feedList);

	OMA::Settings::setAniDexUrl(ui.AniDexUrl->displayText().trimmed());
	OMA::Settings::setDownloadTorrent(ui.downloadTorrents->isChecked());
	OMA::Settings::setTorrentDir(ui.torrentsDir->displayText().trimmed());
	OMA::Settings::setYoutubeToUmmy(ui.youtubeToUmmy->isChecked());
	OMA::Settings::setConsole(ui.console->isChecked());
	OMA::Settings::setCurlVerbose(ui.curlVerbose->isChecked());

	close();
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
void PrefWindow::on_removeWebsite_clicked() {
	if (ui.activeWebsites->selectedItems().size() > 0) {
		delete ui.activeWebsites->selectedItems()[0];
	}
}

void PrefWindow::on_addFollowed_clicked() {
	FollowedWidget* widget = new FollowedWidget(new FollowedAnime());
	QListWidgetItem* item = new QListWidgetItem();
	item->setSizeHint(QSize(item->sizeHint().width(), widget->sizeHint().height()));
	ui.followedListWidget->addItem(item);
	ui.followedListWidget->setItemWidget(item, widget);
	ui.followedListWidget->scrollToBottom();
	widget->ui.animeTitle->setFocus();
}
void PrefWindow::on_removeFollowed_clicked() {
	QList<QListWidgetItem*> list = ui.followedListWidget->selectedItems();
	if (list.size() > 0) {
		delete list[0];
	}
}

void PrefWindow::on_addFeed_clicked() {
	FeedWidget* widget = new FeedWidget(new Feed("", "", ""));
	QListWidgetItem* item = new QListWidgetItem();
	item->setSizeHint(QSize(item->sizeHint().width(), widget->sizeHint().height()));
	ui.feedListWidget->addItem(item);
	ui.feedListWidget->setItemWidget(item, widget);
	ui.feedListWidget->scrollToBottom();
	widget->ui.name->setFocus();
}

void PrefWindow::on_removeFeed_clicked() {
	QList<QListWidgetItem*> list = ui.feedListWidget->selectedItems();
	if (list.size() > 0) {
		delete list[0];
	}
}

void PrefWindow::on_browseTorrentsDir_clicked() {
	QString folder = QFileDialog::getExistingDirectory(
		this, tr("Open Directory"), ui.torrentsDir->displayText(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (!folder.isEmpty())
		ui.torrentsDir->setText(folder);
}
