#include <functional>
#include "AboutWindow.h"
#include "ColorsWindow.h"
#include "EpisodeWidget.h"
#include "MainWindow.h"
#include "OMA.h"
#include "PrefWindow.h"
#include "WebsiteWidget.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	ui.setupUi(this);
	OMA::setMainWindow(this);
	OMA::Settings::fix();

	setWindowTitle("Otaku Manager " + OMA::version);

	connect(ui.actionPreferences, &QAction::triggered, this, &MainWindow::openPreferences);
	connect(ui.actionQuit, &QAction::triggered, this, &QApplication::quit);
	connect(ui.actionColorsInfo, &QAction::triggered, this, &MainWindow::openColorsInfo);
	connect(ui.actionAbout, &QAction::triggered, this, &MainWindow::openAbout);

	ui.mainTabWidget->removeTab(1);
	ui.followedTabWidget->removeTab(0);
	ui.followedTabWidget->removeTab(0);

	QStringList list = OMA::Settings::getWebsites();
	for (int i = 0; i < list.size(); i++) {
		Website* website = new Website(list[i]);
		QWidget* scrollAreaContent = new QWidget();
		WebsiteWidget* websiteWidget = new WebsiteWidget(
			website, ui.mainTabWidget, scrollAreaContent, ui.followedTabWidget, i + 1);
		websites.append(websiteWidget);
		ui.mainTabWidget->addTab(websiteWidget, website->name);
		QWidget* followed = new QWidget();
		ui.followedTabWidget->addTab(followed, website->name);
		followed->setLayout(new QVBoxLayout());
		followed->layout()->setMargin(0);
		QScrollArea* scrollArea = new QScrollArea();
		followed->layout()->addWidget(scrollArea);
		scrollArea->setWidgetResizable(true);
		new QGridLayout(scrollAreaContent);
		scrollArea->setWidget(scrollAreaContent);
		followedScrollAreas.append(scrollAreaContent);
	}
}

MainWindow::~MainWindow() {
	OMA::Settings::setVersion();
	OMA::Settings::sync();
}

void MainWindow::updateAllEpisodes() {
	for (int i = 0; i < websites.size(); i++) {
		websites[i]->updateEpisodes();
	}
}

/*void MainWindow::initSettings() {
	OMA::Settings::setVersion();
	OMA::Settings::setCheckForUpdates(OMA::Settings::getCheckForUpdates());
	OMA::Settings::setWebsites(OMA::Settings::getWebsites());
	QStringList list = OMA::Settings::getButtons();
	QString buttons;
	for (int i = 0; i < list.size(); i++) {
		buttons = list[i] + "|";
	}
	if (buttons.endsWith("|")) {
		buttons = buttons.mid(0, buttons.length() - 1);
	}
	OMA::Settings::setButtons(buttons);
	OMA::Settings::setFollowed(OMA::Settings::getFollowed());
}*/

void MainWindow::openPreferences() {
	PrefWindow* w = new PrefWindow();
	w->show();
}

void MainWindow::openColorsInfo() {
	ColorsWindow* w = new ColorsWindow();
	w->show();
}

void MainWindow::openAbout() {
	AboutWindow* w = new AboutWindow();
	w->show();
}

void MainWindow::on_updateButton_clicked() {
	int index = ui.followedTabWidget->currentIndex();
	websites[index]->updateEpisodes();
}

void MainWindow::on_updateAllButton_clicked() {
	updateAllEpisodes();
}
