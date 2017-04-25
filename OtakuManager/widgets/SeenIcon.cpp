#include "OMA.h"
#include "SeenIcon.h"

SeenIcon::SeenIcon(QString name, QWidget* parent) : QPushButton(parent) {
	ui.setupUi(this);
	this->name = name;
}

void SeenIcon::on_SeenIcon_clicked() {
	OMA::Settings::setSeen(name, false);
	delete this;
}