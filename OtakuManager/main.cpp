#include <QMessageBox>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QtConcurrent\QtConcurrentRun>
#include <QtWidgets/QApplication>
#include <windows.h>
#include "MainWindow.h"
#include "MyAwesomium.h"
#include "MyUtils.h"
#include "NewVersionWindow.h"
#include "OMA.h"

void checkForUpdates(NewVersionWindow* newVersionWindow) {
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

	QSharedMemory sharedMemory;
	sharedMemory.setKey("OtakuManagerKey");
	if (!sharedMemory.create(1)) {
		QMessageBox msgBox;
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setText("Otaku Manager is already running.");
		msgBox.setWindowIcon(QIcon(":/MainWindow/resources/icon.ico"));
		msgBox.exec();
		exit(1);
		return 1;
	}

	qRegisterMetaType<QMessageBox::Icon>();

	MainWindow w;
	w.show();
	QCoreApplication::processEvents();

	if (OMA::Settings::getConsole()) {
		if (GetConsoleWindow() == NULL) {
			if (AllocConsole()) {
				(void)freopen("CONIN$", "r", stdin);
				(void)freopen("CONOUT$", "w", stdout);
				(void)freopen("CONOUT$", "w", stderr);

				w.raise();
			}
		}
	}

	NewVersionWindow* newVersionWindow = new NewVersionWindow();
	QFuture<void> future = QtConcurrent::run(checkForUpdates, newVersionWindow);

	w.updateAllEpisodes();
	a.exec();

	MyAwesomium::ShutdownWebCore();
	return 0;
}
