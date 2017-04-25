#pragma once

#include <QWidget>
#include "Website.h"
#include "ui_WebsiteWidget.h"

class WebsiteWidget : public QWidget {
	Q_OBJECT

public:
	Website* website;
	QTabWidget* parentTab;
	QWidget* followedWidget;
	QTabWidget* followedTab;
	int tabIndex;
	QList<Episode*> episodes;
	QList<Anime*> series;

	WebsiteWidget(Website* website, QTabWidget* parentTab, QWidget* followedWidget,
				  QTabWidget* followedTab, int tabIndex);
	void updateEpisodes();
	void updateSeries();

signals:
	void episodesUpdated();
	void seriesUpdated();

private slots:
	void on_updateButton_clicked();
	void applyUpdatedEpisodes();
	void applyUpdatedSeries();

private:
	Ui::WebsiteWidget ui;
};
