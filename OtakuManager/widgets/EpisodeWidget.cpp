#include <QSignalMapper>
#include "EpisodeWidget.h"
#include "NewIcon.h"
#include "OMA.h"
#include "SeenIcon.h"
#include "UrlButton.h"

EpisodeWidget::EpisodeWidget(Episode* episode, Website* website, QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);
	this->episode = episode;
	this->website = website;
	ui.episodeTitle->setText(episode->name);
	// ui.episodeTitle->setToolTip(episode->url);
	if (OMA::isSeen(episode->name)) {
		ui.iconLayout->addWidget(new SeenIcon(episode->name));
	} else if (episode->isNew) {
		ui.iconLayout->addWidget(new NewIcon());
	}

	QString name = " " + episode->name + " ";
	if (name.contains(QRegExp("[^a-z]+movie[^a-z]+", Qt::CaseInsensitive)) ||
		name.contains(QRegExp("[^a-z]+film[^a-z]+", Qt::CaseInsensitive))) {
		setStyleSheet("#groupBox { background: #CC66FF; }");
	} else if (name.contains(QRegExp("[^a-z]+ova[^a-z]+", Qt::CaseInsensitive)) ||
			   name.contains(QRegExp("[^a-z]+oav[^a-z]+", Qt::CaseInsensitive)) ||
			   name.contains(QRegExp("[^a-z]+sp[^a-z]+", Qt::CaseInsensitive)) ||
			   name.contains(QRegExp("[^a-z]+special[^a-z]+", Qt::CaseInsensitive))) {
		setStyleSheet("#groupBox { background: deepskyblue; }");
	} else if (name.endsWith("end", Qt::CaseInsensitive)) {
		setStyleSheet("#groupBox { background: tomato; }");
	} else if (name.contains(QRegExp("[^0-9]+01[^0-9]+"))) {
		setStyleSheet("#groupBox { background: lime; }");
	}

	int count = 0;
	for (int i = 0; i < OMA::linkTypes.size(); i++) {
		QString button = OMA::linkTypes[i];
		if (OMA::Settings::getButtons().contains(button) ||
			button == OMA::linkTypes[LinkTypes::customLink] &&
				!OMA::Settings::getFollowed(episode->name).customLink.isEmpty()) {
			if (!button.contains("2") || website->hasDoubleButtons) {
				bool show = false;
				if (button == OMA::linkTypes[LinkTypes::animeInfo]) {
					if (!website->seriesPage.isEmpty()) {
						show = true;
					} else {
						show = false;
					}
				} else if (button == OMA::linkTypes[LinkTypes::magnet] && website->hasMagnet) {
					show = true;
				} else if (button.contains("Download")) {
					if (button.contains("IfNoDownload")) {
						if (!episode->hasDownload || !website->hasDownload) {
							show = true;
						}
					} else {
						if (episode->hasDownload && website->hasDownload) {
							show = true;
						}
					}
				} else if (!button.contains("Streaming") || website->hasStreaming) {
					show = true;
				}
				if (show) {
					UrlButton* urlButton = new UrlButton(button.replace("IfNoDownload", ""), this);
					ui.gridLayout->addWidget(urlButton, count / 2, count % 2, 1, 1);
					count++;
				}
			}
		}
	}
}
