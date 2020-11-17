#include <QComboBox>
#include <QFileDialog>
#include <QItemDelegate>
#include <QScrollBar>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include "FeedWidget.h"
#include "FollowedWidget.h"
#include "PrefWindow.h"

PrefWindow::PrefWindow(QWidget* parent) : QDialog(parent) {
	ui.setupUi(this);

	// General settings
	ui.updateCheckBox->setChecked(OMA::Settings::getCheckForUpdates());
	ui.followedTabCheckBox->setChecked(OMA::Settings::getShowFollowedTab());
	ui.tabsOnSideCheckBox->setChecked(OMA::Settings::getTabsOnSide());

	ui.streaming->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::streaming]));
	ui.streamingIfNoDownload->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::streamingIfNoDownload]));
	ui.download->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::download]));
	ui.torrent->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::torrent]));
	ui.magnet->setChecked(OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::magnet]));
	ui.animeInfo->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::animeInfo]));
	ui.post->setChecked(OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::post]));
	ui.postIfNoDownload->setChecked(
		OMA::Settings::getButtons().contains(OMA::linkTypes[LinkTypes::postIfNoDownload]));

	QHash<QString, QStringList> websites = OMA::websites(true);
	QList<QString> groups = websites.keys();
	groups.sort();
	foreach (QString group, groups) {
		QTreeWidgetItem* topItem = new QTreeWidgetItem();
		topItem->setText(0, group);
		foreach (QString w, websites[group]) {
			QTreeWidgetItem* item = new QTreeWidgetItem();
			item->setText(0, w);
			topItem->addChild(item);
		}
		ui.availableWebsites->addTopLevelItem(topItem);
	}

	// Followed anime
	QList<FollowedAnime> followedList = OMA::Settings::getFollowed();
	ui.followedTable->setHorizontalHeaderLabels({"Anime", "Regex", "Website", "Custom link"});
	ui.followedTable->setRowCount(followedList.size());
	ui.followedTable->setColumnWidth(0, 230);
	ui.followedTable->setColumnWidth(1, 50);
	ui.followedTable->setColumnWidth(2, 140);
	for (int i = 0; i < followedList.size(); i++) {
		setupTableRow(i, &followedList[i]);
	}

	QStringList activeWebsites = OMA::Settings::getWebsites();
	for (int i = 0; i < activeWebsites.size(); i++) {
		ui.activeWebsites->addItem(activeWebsites[i]);
	}

	// Feeds
	QList<Feed> feedList = OMA::Settings::getFeeds();
	for (int i = 0; i < feedList.size(); i++) {
		FeedWidget* widget = new FeedWidget(&(feedList[i]));
		QListWidgetItem* item = new QListWidgetItem();
		item->setSizeHint(QSize(item->sizeHint().width(), widget->sizeHint().height()));
		ui.feedListWidget->addItem(item);
		ui.feedListWidget->setItemWidget(item, widget);
	}
	ui.feedListWidget->verticalScrollBar()->setSingleStep(10);

	// Other
	// ui.AniDexUrl->setText(OMA::Settings::getAniDexUrl());
	// ui.downloadTorrents->setChecked(OMA::Settings::getDownloadTorrent());
	ui.torrentsDir->setText(OMA::Settings::getTorrentDir());
	ui.youtubeToUmmy->setChecked(OMA::Settings::getYoutubeToUmmy());
	// ui.youtubeCustomLink->setText(OMA::Settings::getYoutubeCustomLink());
	ui.console->setChecked(OMA::Settings::getConsole());
	ui.curlVerbose->setChecked(OMA::Settings::getCurlVerbose());
	ui.curlUserAgent->setText(OMA::Settings::getCurlUserAgent());

	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &PrefWindow::save);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);
}

void PrefWindow::setupTableRow(int row, FollowedAnime* anime) {
	if (anime != nullptr) {
		ui.followedTable->setItem(row, 0, new QTableWidgetItem(anime->anime));
	} else {
		ui.followedTable->setItem(row, 0, new QTableWidgetItem(""));
	}

	QTableWidgetItem* pItem = new QTableWidgetItem();
	if (anime != nullptr) {
		pItem->setCheckState(anime->regex ? Qt::Checked : Qt::Unchecked);
	} else {
		pItem->setCheckState(Qt::Unchecked);
	}
	ui.followedTable->setItem(row, 1, pItem);

	QComboBox* combo;
	combo = new QComboBox;
	combo->addItem("*");
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(combo->model());
	QHash<QString, QStringList> websites = OMA::websites();
	QList<QString> groups = websites.keys();
	groups.sort();
	int j = 0;
	foreach (QString group, groups) {
		j++;
		combo->addItem(group);
		QStandardItem* item = model->item(j);
		item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
		foreach (QString w, websites[group]) {
			j++;
			combo->addItem(w);
		}
	}
	if (anime != nullptr) {
		combo->setCurrentText(anime->website);
	}
	ui.followedTable->setCellWidget(row, 2, combo);

	if (anime != nullptr) {
		ui.followedTable->setItem(row, 3, new QTableWidgetItem(anime->customLink));
	} else {
		ui.followedTable->setItem(row, 3, new QTableWidgetItem(""));
	}
}

void PrefWindow::save() {
	// General settings
	OMA::Settings::setCheckForUpdates(ui.updateCheckBox->isChecked());
	OMA::Settings::setShowFollowedTab(ui.followedTabCheckBox->isChecked());
	OMA::Settings::setTabsOnSide(ui.tabsOnSideCheckBox->isChecked());

	QStringList websitesList;
	for (int i = 0; i < ui.activeWebsites->count(); ++i) {
		websitesList.append(ui.activeWebsites->item(i)->text());
	}
	OMA::Settings::setWebsites(websitesList);

	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::streaming], ui.streaming->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::streamingIfNoDownload],
							  ui.streamingIfNoDownload->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::download], ui.download->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::torrent], ui.torrent->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::magnet], ui.magnet->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::animeInfo], ui.animeInfo->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::post], ui.post->isChecked());
	OMA::Settings::setButtons(OMA::linkTypes[LinkTypes::postIfNoDownload],
							  ui.postIfNoDownload->isChecked());

	// Followed anime
	QList<FollowedAnime> followedList;
	for (int i = 0; i < ui.followedTable->rowCount(); i++) {
		if (!ui.followedTable->item(i, 0)->text().isEmpty()) {
			FollowedAnime anime;
			anime.anime = ui.followedTable->item(i, 0)->text();
			anime.regex = ui.followedTable->item(i, 1)->checkState() == Qt::Checked ? true : false;
			anime.website = ((QComboBox*)ui.followedTable->cellWidget(i, 2))->currentText();
			anime.customLink = ui.followedTable->item(i, 3)->text();
			followedList.append(anime);
		}
	}
	OMA::Settings::setFollowed(followedList);

	// Feeds
	QList<Feed> feedList;
	for (int i = 0; i < ui.feedListWidget->count(); i++) {
		FeedWidget* feed = (FeedWidget*)(ui.feedListWidget->itemWidget(ui.feedListWidget->item(i)));
		if (!feed->ui.url->text().isEmpty()) {
			feedList.append(Feed(feed->ui.name->text(), feed->ui.url->text(),
								 feed->ui.homepage->text(), feed->oldName));
		}
	}
	OMA::Settings::setFeeds(feedList);

	// Other
	// OMA::Settings::setAniDexUrl(ui.AniDexUrl->displayText().trimmed());
	// OMA::Settings::setDownloadTorrent(ui.downloadTorrents->isChecked());
	OMA::Settings::setTorrentDir(ui.torrentsDir->displayText().trimmed());
	OMA::Settings::setYoutubeToUmmy(ui.youtubeToUmmy->isChecked());
	// OMA::Settings::setYoutubeCustomLink(ui.youtubeCustomLink->displayText().trimmed());
	OMA::Settings::setConsole(ui.console->isChecked());
	OMA::Settings::setCurlVerbose(ui.curlVerbose->isChecked());
	OMA::Settings::setCurlUserAgent(ui.curlUserAgent->displayText().trimmed());

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
	ui.followedTable->setRowCount(ui.followedTable->rowCount() + 1);
	setupTableRow(ui.followedTable->rowCount() - 1);
	ui.followedTable->scrollToBottom();
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
