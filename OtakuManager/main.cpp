#include <QtConcurrent\QtConcurrentRun>
#include <QtWidgets/QApplication>
#include "MainWindow.h"
#include "MyUtils.h"
#include "NewVersionWindow.h"
#include "OMA.h"

void checkForUpadte(NewVersionWindow* newVersionWindow) {
	if (OMA::Settings::getCheckForUpdates()) {
		QString s = MyUtils::urlToQString(
			"https://github.com/GhostWriterTNCS/OtakuManager/releases/latest");
		s = MyUtils::substring(s, "<h1 class=\"release-title\">", "</h1>");
		s = MyUtils::substring(s, ">", "<").trimmed();
		if (s == "Not Found") {
			s = "";
		}
		emit newVersionWindow->versionChecked(s);
	}
}

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	NewVersionWindow* newVersionWindow = new NewVersionWindow();
	QCoreApplication::processEvents();

	QFuture<void> future = QtConcurrent::run(checkForUpadte, newVersionWindow);

	w.updateAllEpisodes();
	w.ui.statusBar->clearMessage();

	return a.exec();
}
