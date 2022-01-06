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

	WebsiteWidget(Website* website, QTabWidget* parentTab, QWidget* followedWidget,
				  QTabWidget* followedTab, int tabIndex);
	void updateEpisodes();

signals:
	void episodesUpdated(bool successful);

private slots:
	void on_updateButton_clicked();
	void on_updateAllButton_clicked();
	void applyUpdatedEpisodes(bool successful);

private:
	Ui::WebsiteWidget ui;
	void HighlightTab(QTabWidget* parentTab, int tabIndex, bool value);
};
