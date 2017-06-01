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
		bool show = false;
		if (OMA::Settings::getButtons().contains(button) || i == LinkTypes::customLink) {
			switch (i) {
				case LinkTypes::streaming:
					show = website->hasStreaming;
					break;
				case LinkTypes::streamingIfNoDownload:
					show =
						(!episode->hasDownload || !website->hasDownload) && website->hasStreaming;
					break;
				case LinkTypes::streaming2:
					show = website->hasStreaming && website->hasDoubleButtons;
					break;
				case LinkTypes::streaming2IfNoDownload:
					show = (!episode->hasDownload || !website->hasDownload) &&
						website->hasStreaming && website->hasDoubleButtons;
					break;
				case LinkTypes::download:
					show = episode->hasDownload && website->hasDownload;
					break;
				case LinkTypes::download2:
					show =
						episode->hasDownload && website->hasDownload && website->hasDoubleButtons;
					break;
				case LinkTypes::torrent:
					show = website->hasTorrent;
					break;
				case LinkTypes::magnet:
					show = website->hasMagnet;
					break;
				case LinkTypes::animeInfo:
					show = !website->seriesPage.isEmpty();
					break;
				case LinkTypes::customLink:
					show = !OMA::Settings::getFollowed(episode->name).customLink.isEmpty();
					break;
				case LinkTypes::postIfNoDownload:
					show = !episode->hasDownload || !website->hasDownload;
					break;
				default:
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
