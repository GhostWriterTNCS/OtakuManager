#include <QtConcurrent\QtConcurrentRun>
#include "AnimeWidget.h"
#include "EpisodeWidget.h"
#include "OMA.h"
#include "WebsiteWidget.h"

WebsiteWidget::WebsiteWidget(Website* website, QTabWidget* parentTab, QWidget* followedWidget,
							 QTabWidget* followedTab, int tabIndex)
	: QWidget((QWidget*)parentTab) {
	ui.setupUi(this);
	this->website = website;
	this->parentTab = parentTab;
	this->followedWidget = followedWidget;
	this->followedTab = followedTab;
	this->tabIndex = tabIndex;
	QString url = website->homepage;
	ui.websiteLabel->setText("<h2><a href='" +
							 MyUtils::substringFromEnd(website->homepage, "", "index.php") + "'>" +
							 website->name + "</a></h2>");
	if (website->seriesPage.isEmpty()) {
		ui.seriesLabel->setText("Series not available for this website.");
	}

	connect(this, SIGNAL(episodesUpdated(bool)), this, SLOT(applyUpdatedEpisodes(bool)));
	connect(this, SIGNAL(seriesUpdated(bool)), this, SLOT(applyUpdatedSeries(bool)));
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

		if (newCount > 0) {
			parentTab->setTabText(tabIndex, website->name + " (" + QString::number(newCount) + ")");
		} else {
			parentTab->setTabText(tabIndex, website->name);
		}
		if (followedNewCount > 0) {
			followedTab->setTabText(tabIndex - 1,
									website->name + " (" + QString::number(followedNewCount) + ")");
		} else {
			followedTab->setTabText(tabIndex - 1, website->name);
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

void getSeries(WebsiteWidget* ww) {
	bool successful = ww->website->getSeries();
	emit ww->seriesUpdated(successful);
}
void WebsiteWidget::updateSeries() {
	if (!website->seriesPage.isEmpty()) {
		OMA::addStatusMessage("Getting series from " + website->name + "...");
		while (QLayoutItem* item = ui.seriesScrollAreaContent->layout()->takeAt(0)) {
			delete item->widget();
		}
		series.clear();
		QCoreApplication::processEvents();
		QFuture<void> future = QtConcurrent::run(getSeries, this);
	}
}
void WebsiteWidget::applyUpdatedSeries(bool successful) {
	if (successful) {
		for (int i = 0; i < website->series.size(); i++) {
			AnimeWidget* anime = new AnimeWidget(&(website->series[i]), website);
			ui.seriesScrollAreaContent->layout()->addWidget(anime);
			series.append(&website->series[i]);
		}
	} else {
		ui.seriesScrollAreaContent->layout()->addWidget(new QLabel(OMA::errorMex));
	}
	OMA::removeStatusMessage("Getting series from " + website->name + "...");
	QCoreApplication::processEvents();
}

void WebsiteWidget::on_updateButton_clicked() {
	if (ui.tabWidget->currentIndex() == 0) {
		updateEpisodes();
	} else {
		updateSeries();
	}
}
