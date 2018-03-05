#pragma once

#include <QWidget>
#include "OMA.h"
#include "ui_FeedWidget.h"

class FeedWidget : public QWidget {
	Q_OBJECT

public:
	Feed* feed;
	QString oldName = "";
	Ui::FeedWidget ui;

	FeedWidget(Feed* feed, QWidget* parent = Q_NULLPTR);
};
