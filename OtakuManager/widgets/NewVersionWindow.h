#pragma once

#include <QDialog>
#include "ui_UpdateWindow.h"

class NewVersionWindow : public QDialog {
	Q_OBJECT

public:
	NewVersionWindow(QWidget* parent = Q_NULLPTR);

signals:
	void versionChecked(QString newVersion);

private slots:
	void showWindow(QString newVersion);
	void on_goToGithub_clicked();
	void on_cancel_clicked();

private:
	Ui::NewVersionWindow ui;
};
