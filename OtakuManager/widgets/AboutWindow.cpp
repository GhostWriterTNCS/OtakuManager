#include "AboutWindow.h"
#include "OMA.h"

AboutWindow::AboutWindow(QWidget* parent) : QDialog(parent) {
	ui.setupUi(this);
	ui.label->setText(ui.label->text().replace("$version$", OMA::version));
}

void AboutWindow::on_close_clicked() {
	close();
}