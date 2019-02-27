#pragma once

#include <QMessageBox>
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

signals:
	void showMessageBoxSignal(QMessageBox::Icon type, QString title, QString mex);

private slots:
	void on_updateButton_clicked();
	void on_updateAllButton_clicked();
	void showMessageBox(QMessageBox::Icon type, QString title, QString mex);
};
