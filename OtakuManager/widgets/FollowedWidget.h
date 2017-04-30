#pragma once

#include <QWidget>
#include "OMA.h"
#include "ui_FollowedWidget.h"

class FollowedWidget : public QWidget {
	Q_OBJECT

public:
	FollowedAnime* followed;
	Ui::FollowedWidget ui;

	FollowedWidget(FollowedAnime* followed, QWidget* parent = Q_NULLPTR);
	// FollowedWidget(QStringList list, QWidget* parent = Q_NULLPTR);
};
