#include <QDesktopServices>
#include <QUrl>
#include <QtConcurrent\QtConcurrentRun>
#include "OMA.h"
#include "SeenIcon.h"
#include "UrlButton.h"

UrlButton::UrlButton(QString name, EpisodeWidget* parent) : QPushButton(parent) {
	ui.setupUi(this);
	this->type = name;
	this->episodeWidget = parent;
	this->setText(name);

	connect(this, SIGNAL(episodeDone(bool)), this, SLOT(applyEpisode(bool)));
}

void getEpisodeUrl(UrlButton* button) {
	bool succesful = false;
	if (button->type == OMA::linkTypes[LinkTypes::customLink]) {
		QString link = OMA::Settings::getFollowed(button->episodeWidget->episode->name).customLink;
		if (!link.startsWith("http://") && !link.startsWith("https://") &&
			!link.startsWith("file:///")) {
			link = "file:///" + link;
		}
		succesful = OMA::upenUrl(link);
	} else {
		succesful = button->episodeWidget->website->goToEpisode(button->episodeWidget->episode,
																button->type);
	}
	emit button->episodeDone(succesful);
}

void UrlButton::applyEpisode(bool succesful) {
	OMA::removeStatusMessage("Getting " + type + " of " + episodeWidget->episode->name + "...");
	if (succesful && type != OMA::linkTypes[LinkTypes::animeInfo]) {
		while (QLayoutItem* item = episodeWidget->ui.iconLayout->itemAt(0)) {
			delete item->widget();
		}
		episodeWidget->ui.iconLayout->addWidget(new SeenIcon(episodeWidget->episode->name));
		OMA::Settings::setSeen(episodeWidget->episode->name);
	}
}

void UrlButton::on_UrlButton_clicked() {
	OMA::addStatusMessage("Getting " + type + " of " + episodeWidget->episode->name + "...");
	QFuture<void> future = QtConcurrent::run(getEpisodeUrl, this);
}
