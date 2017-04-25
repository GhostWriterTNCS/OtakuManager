#pragma once

#include <QDialog>
#include "ui_ColorsWindow.h"

class ColorsWindow : public QDialog {
	Q_OBJECT

public:
	ColorsWindow(QWidget* parent = Q_NULLPTR);

private slots:
	void on_close_clicked();

private:
	Ui::ColorsWindow ui;
};
