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
	ui.websiteLabel->setText("<h2><a href='" + MyUtils::substring(website->homepage, "/index") +
							 "'>" + website->name + "</a></h2>");
	if (website->seriesPage.isEmpty()) {
		ui.seriesLabel->setText("Series not available for this website.");
	}

	connect(this, SIGNAL(episodesUpdated()), this, SLOT(applyUpdatedEpisodes()));
	connect(this, SIGNAL(seriesUpdated()), this, SLOT(applyUpdatedSeries()));
}

void getEpisodes(WebsiteWidget* ww) {
	ww->website->getEpisodes();
	emit ww->episodesUpdated();
}
void WebsiteWidget::updateEpisodes() {
	OMA::getMainWindow()->ui.statusBar->showMessage("Getting episodes from " + website->name +
													"...");
	while (QLayoutItem* item = ui.episodesScrollAreaContent->layout()->takeAt(0)) {
		delete item->widget();
	}
	while (QLayoutItem* item = followedWidget->layout()->takeAt(0)) {
		delete item->widget();
	}
	episodes.clear();
	QCoreApplication::processEvents();
	QFuture<void> future = QtConcurrent::run(getEpisodes, this);
}
void WebsiteWidget::applyUpdatedEpisodes() {
	int newCount = 0;
	int followedNewCount = 0;
	for (int i = 0; i < website->episodes.size(); i++) {
		EpisodeWidget* episode = new EpisodeWidget(&(website->episodes[i]), website);
		ui.episodesScrollAreaContent->layout()->addWidget(episode);
		if (OMA::isFollowed(website->episodes[i].name, website->name)) {
			followedWidget->layout()->addWidget(
				new EpisodeWidget(&(website->episodes[i]), website));
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

	if (OMA::Settings::getFollowedCount(website->name) == 0) {
		followedWidget->layout()->addWidget(
			new QLabel("<h3 style='text-align: center;'>No followed episodes.</h3>"));
	}
	OMA::getMainWindow()->ui.statusBar->clearMessage();
	QCoreApplication::processEvents();
}

void getSeries(WebsiteWidget* ww) {
	ww->website->getSeries();
	emit ww->seriesUpdated();
}
void WebsiteWidget::updateSeries() {
	if (!website->seriesPage.isEmpty()) {
		OMA::getMainWindow()->ui.statusBar->showMessage("Getting series from " + website->name +
														"...");
		while (QLayoutItem* item = ui.seriesScrollAreaContent->layout()->takeAt(0)) {
			delete item->widget();
		}
		series.clear();
		QCoreApplication::processEvents();
		QFuture<void> future = QtConcurrent::run(getSeries, this);
	}
}
void WebsiteWidget::applyUpdatedSeries() {
	for (int i = 0; i < website->series.size(); i++) {
		AnimeWidget* anime = new AnimeWidget(&(website->series[i]), website);
		ui.seriesScrollAreaContent->layout()->addWidget(anime);
		series.append(&website->series[i]);
	}
	OMA::getMainWindow()->ui.statusBar->clearMessage();
	QCoreApplication::processEvents();
}

void WebsiteWidget::on_updateButton_clicked() {
	if (ui.tabWidget->currentIndex() == 0) {
		updateEpisodes();
	} else {
		updateSeries();
	}
}
