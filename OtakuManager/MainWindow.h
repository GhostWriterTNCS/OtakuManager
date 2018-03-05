#pragma once

#include <QtWidgets/QMainWindow>
#include "WebsiteWidget.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	QList<WebsiteWidget*> websites;
	QList<QWidget*> followedScrollAreas;
	Ui::MainWindowClass ui;

	MainWindow(QWidget* parent = Q_NULLPTR);
	~MainWindow();
	void updateAllEpisodes();
	// void initSettings();

	void openPreferences();
	void openColorsInfo();
	void openAbout();

private slots:
	void on_updateButton_clicked();
	void on_updateAllButton_clicked();
};
