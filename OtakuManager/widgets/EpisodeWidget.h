#pragma once

#include <QWidget>
#include "Website.h"
#include "ui_EpisodeWidget.h"

class EpisodeWidget : public QWidget {
	Q_OBJECT

public:
	Episode* episode;
	Website* website;
	Ui::EpisodeWidget ui;

	EpisodeWidget(Episode* episode, Website* website, QWidget* parent = Q_NULLPTR);
};
