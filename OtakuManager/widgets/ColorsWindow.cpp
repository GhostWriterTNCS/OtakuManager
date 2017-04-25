#include "ColorsWindow.h"

ColorsWindow::ColorsWindow(QWidget* parent) : QDialog(parent) {
	ui.setupUi(this);
}

void ColorsWindow::on_close_clicked() {
	close();
}