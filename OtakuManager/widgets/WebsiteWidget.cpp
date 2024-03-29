#include <QtConcurrent\QtConcurrentRun>
#include "AnimeWidget.h"
#include "EpisodeWidget.h"
#include "OMA.h"
#include "WebsiteWidget.h"

WebsiteWidget::WebsiteWidget(Website* website, QTabWidget* parentTab, QWidget* followedWidget,
							 QTabWidget* followedTab, int tabIndex)
	: QWidget((QWidget*)parentTab) {
	ui.setupUi(this);
	if (OMA::Settings::getShowFollowedTab()) {
		ui.updateAllButton->hide();
	}

	this->website = website;
	this->parentTab = parentTab;
	this->followedWidget = followedWidget;
	this->followedTab = followedTab;
	this->tabIndex = tabIndex;
	ui.websiteLabel->setText("<h2><a href='" +
							 MyUtils::substringFromEnd(website->homepage, "", "index.php") + "'>" +
							 website->name + "</a></h2>");

	connect(this, SIGNAL(episodesUpdated(bool)), this, SLOT(applyUpdatedEpisodes(bool)));
}

void getEpisodes(WebsiteWidget* ww) {
	bool successful = ww->website->getEpisodes();
	emit ww->episodesUpdated(successful);
}
void WebsiteWidget::updateEpisodes() {
	OMA::addStatusMessage("Getting episodes from " + website->name + "...");

	parentTab->setTabText(tabIndex, website->name + "...");
	while (QLayoutItem* item = ui.episodesScrollAreaContent->layout()->takeAt(0)) {
		delete item->widget();
	}

	followedTab->setTabText(tabIndex - 1, website->name + "...");
	while (QLayoutItem* item = followedWidget->layout()->takeAt(0)) {
		delete item->widget();
	}
	episodes.clear();
	QCoreApplication::processEvents();
	QFuture<void> future = QtConcurrent::run(getEpisodes, this);
}
void WebsiteWidget::applyUpdatedEpisodes(bool successful) {
	if (successful) {
		int newCount = 0;
		int followedCount = 0;
		int followedNewCount = 0;
		if (website->episodes.size() == 0) {
			ui.episodesScrollAreaContent->layout()->addWidget(
				new QLabel("<h3 style='text-align: center;'>No episodes found.</h3>"));
		} else {
			for (int i = 0; i < website->episodes.size(); i++) {
				EpisodeWidget* episode = new EpisodeWidget(&(website->episodes[i]), website);
				ui.episodesScrollAreaContent->layout()->addWidget(episode);
				if (OMA::isFollowed(website->episodes[i].name, website->name)) {
					followedWidget->layout()->addWidget(
						new EpisodeWidget(&(website->episodes[i]), website));
					followedCount++;
					if (website->episodes[i].isNew) {
						followedNewCount++;
					}
				}
				if (website->episodes[i].isNew) {
					newCount++;
				}
				episodes.append(&website->episodes[i]);
			}
		}

		if (newCount > 0) {
			parentTab->setTabText(tabIndex, website->name + " (" +
												(!OMA::Settings::getShowFollowedTab()
													 ? QString::number(followedNewCount) + "/"
													 : "") +
												QString::number(newCount) + ")");
			HighlightTab(parentTab, tabIndex, true);
		} else {
			parentTab->setTabText(tabIndex, website->name);
			HighlightTab(parentTab, tabIndex, false);
		}

		if (followedNewCount > 0) {
			followedTab->setTabText(tabIndex - 1,
									website->name + " (" + QString::number(followedNewCount) + ")");
			HighlightTab(followedTab, tabIndex - 1, true);
		} else {
			followedTab->setTabText(tabIndex - 1, website->name);
			HighlightTab(parentTab, tabIndex, false);
		}

		if (followedCount == 0) {
			followedWidget->layout()->addWidget(
				new QLabel("<h3 style='text-align: center;'>No followed episodes found.</h3>"));
		}
	} else {
		parentTab->setTabText(tabIndex, website->name + " :(");
		followedTab->setTabText(tabIndex - 1, website->name + " :(");

		ui.episodesScrollAreaContent->layout()->addWidget(new QLabel(OMA::errorMex));
		followedWidget->layout()->addWidget(new QLabel(OMA::errorMex));
	}

	OMA::removeStatusMessage("Getting episodes from " + website->name + "...");
	QCoreApplication::processEvents();
}

void WebsiteWidget::HighlightTab(QTabWidget* parentTab, int tabIndex, bool value) {
	QFont font = parentTab->font();
	if (value) {
		font.setBold(true);
		parentTab->tabBar()->setTabTextColor(tabIndex, Qt::blue);
	} else {
		font.setBold(false);
		parentTab->tabBar()->setTabTextColor(tabIndex, Qt::black);
	}
}

void WebsiteWidget::on_updateButton_clicked() {
	updateEpisodes();
}

void WebsiteWidget::on_updateAllButton_clicked() {
	OMA::getMainWindow()->updateAllEpisodes();
}
