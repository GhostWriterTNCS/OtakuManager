#pragma once

#include <QPushButton>
#include "ui_SeenIcon.h"

class SeenIcon : public QPushButton {
	Q_OBJECT

public:
	QString name;

	SeenIcon(QString name, QWidget* parent = Q_NULLPTR);

private slots:
	void on_SeenIcon_clicked();

private:
	Ui::SeenIcon ui;
};
