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

	connect(this, SIGNAL(showMessageBoxSignal(QMessageBox::Icon, QString, QString)), this,
			SLOT(showMessageBox(QMessageBox::Icon, QString, QString)));

	ui.mainTabWidget->removeTab(1);
	if (ui.mainTabWidget->tabPosition() == QTabWidget::TabPosition::West ||
		ui.mainTabWidget->tabPosition() == QTabWidget::TabPosition::East) {
		ui.mainTabWidget->tabBar()->setStyle(new CustomTabStyle);
	}
	if (!OMA::Settings::getShowFollowedTab()) {
		ui.mainTabWidget->removeTab(0);
	}

	ui.followedTabWidget->removeTab(0);
	ui.followedTabWidget->removeTab(0);
	if (ui.followedTabWidget->tabPosition() == QTabWidget::TabPosition::West ||
		ui.followedTabWidget->tabPosition() == QTabWidget::TabPosition::East) {
		ui.followedTabWidget->tabBar()->setStyle(new CustomTabStyle);
	}

	QStringList list = OMA::Settings::getWebsites();
	for (int i = 0; i < list.size(); i++) {
		Website* website = new Website(list[i]);
		QWidget* scrollAreaContent = new QWidget();
		WebsiteWidget* websiteWidget =
			new WebsiteWidget(website, ui.mainTabWidget, scrollAreaContent, ui.followedTabWidget,
							  i + (OMA::Settings::getShowFollowedTab() ? 1 : 0));
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

void MainWindow::showMessageBox(QMessageBox::Icon type, QString title, QString mex) {
	switch (type) {
		case QMessageBox::Icon::Warning:
			QMessageBox::warning(this, title, mex);
			break;
		default:
			QMessageBox::information(this, title, mex);
			break;
	}
}
