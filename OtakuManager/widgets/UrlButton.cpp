#include <iostream>
#include "OMA.h"
#include "SeenIcon.h"
#include "UrlButton.h"

UrlButton::UrlButton(QString name, EpisodeWidget* parent) : QPushButton(parent) {
	ui.setupUi(this);
	this->type = name;
	this->episodeWidget = parent;
	this->setText(name);
}

void UrlButton::on_UrlButton_clicked() {
	bool b = false;
	OMA::getMainWindow()->ui.statusBar->showMessage("Getting episode link...");
	if (type == OMA::linkTypes[LinkTypes::customLink]) {
		b = OMA::upenUrl(OMA::Settings::getFollowed(episodeWidget->episode->name).customLink);
	} else {
		b = episodeWidget->website->goToEpisode(episodeWidget->episode, type);
	}
	OMA::getMainWindow()->ui.statusBar->clearMessage();
	if (b) {
		while (QLayoutItem* item = episodeWidget->ui.iconLayout->itemAt(0)) {
			delete item->widget();
		}
		episodeWidget->ui.iconLayout->addWidget(new SeenIcon(episodeWidget->episode->name));
		OMA::Settings::setSeen(episodeWidget->episode->name);
	}
}
