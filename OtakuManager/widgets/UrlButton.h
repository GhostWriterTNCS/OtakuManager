#pragma once

#include <QPushButton>
#include "EpisodeWidget.h"
#include "ui_UrlButton.h"

class UrlButton : public QPushButton {
	Q_OBJECT

public:
	QString type;
	EpisodeWidget* episodeWidget;

	UrlButton(QString name, EpisodeWidget* parent = Q_NULLPTR);

signals:
	void episodeDone(bool succesful);

private slots:
	void on_UrlButton_clicked();
	void applyEpisode(bool succesful);

private:
	Ui::UrlButton ui;
};
