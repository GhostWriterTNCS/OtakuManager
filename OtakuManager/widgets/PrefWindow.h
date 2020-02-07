#pragma once

#include <QDialog>
#include "OMA.h"
#include "ui_PrefWindow.h"

class PrefWindow : public QDialog {
	Q_OBJECT

public:
	PrefWindow(QWidget* parent = Q_NULLPTR);

private slots:
	void save();
	void on_removeWebsite_clicked();
	void on_addWebsite_clicked();

	void on_addFollowed_clicked();

	void on_addFeed_clicked();
	void on_removeFeed_clicked();

	void on_browseTorrentsDir_clicked();

private:
	Ui::PrefWindow ui;
	void setupTableRow(int row, FollowedAnime* anime = nullptr);
};
