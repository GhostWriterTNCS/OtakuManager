#include <QDesktopServices>
#include <QUrl>
#include "NewVersionWindow.h"
#include "OMA.h"

NewVersionWindow::NewVersionWindow(QWidget* parent) : QDialog(parent) {
	ui.setupUi(this);
	connect(this, SIGNAL(versionChecked(QString)), this, SLOT(showWindow(QString)));
}

void NewVersionWindow::showWindow(QString newVersion) {
	if (!newVersion.isEmpty() && newVersion != OMA::version) {
		ui.label->setText("<h2>New version available: " + newVersion + "</h2>");
		show();
	}
}

void NewVersionWindow::on_goToGithub_clicked() {
	QDesktopServices::openUrl(
		QUrl("https://github.com/GhostWriterTNCS/OtakuManager/releases/latest"));
}

void NewVersionWindow::on_cancel_clicked() {
	close();
}
