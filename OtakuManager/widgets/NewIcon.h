#pragma once

#include <QPushButton>
#include "ui_NewIcon.h"

class NewIcon : public QPushButton {
	Q_OBJECT

public:
	NewIcon(QWidget* parent = Q_NULLPTR);
	~NewIcon();

private:
	Ui::NewIcon ui;
};
