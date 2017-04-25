#pragma once

#include <QWidget>
#include "Website.h"
#include "ui_AnimeWidget.h"

class AnimeWidget : public QWidget {
	Q_OBJECT

public:
	Anime* anime;
	Website* website;
	QString followedName;

	AnimeWidget(Anime* anime, Website* website, QWidget* parent = Q_NULLPTR);

private slots:
	void on_followCheckBox_clicked(bool checked);
	void on_saveButton_clicked();

private:
	Ui::AnimeWidget ui;
};
