#include <iostream>
#include <QFileInfo>
#include <QMessageBox>
#include "MyLua.h"
#include "OMA.h"
#include "Website.h"

Website::Website(QString website) {
	name = website;
	QFileInfo check_file("lua/" + website + ".lua");
	if (check_file.exists() && check_file.isFile() &&
		MyLua::getString(name, "homepage").size() > 0) {
		homepage = MyLua::getString(name, "homepage");
		hasStreaming = MyLua::getBool(name, "hasStreaming");
		hasDownload = MyLua::getBool(name, "hasDownload");
		hasTorrent = MyLua::getBool(name, "hasTorrent");
		hasMagnet = MyLua::getBool(name, "hasMagnet");
		return;
	}

	if (website == "Anime Heaven") {
		initialize_AnimeHeaven_EN();
	} else if (website == "KissAnime") {
		initialize_KissAnime_EN();
	} else if (website == "RedAnimeDatabase") {
		initialize_RedAnimeDatabase_IT();
	} else if (website == "WebAnimex") {
		initialize_WebAnimex_IT();
	} else if (website == "Nyaa") {
		initialize_Nyaa_TT();
	} else if (OMA::Settings::getFeedNames().contains(website)) {
		initialize_Feed();
	} else {
		QMessageBox::warning(OMA::getMainWindow(), "Error", "Website not valid: " + website);
	}
}

bool Website::getEpisodes() {
	episodes.clear();
	bool succesful = MyLua::getEpisodes(name, &episodes);
	if (!succesful && getEpisodesFunction != nullptr) {
		succesful = getEpisodesFunction();
	}
	if (episodes.size() > 0) {
		QString lastEp = OMA::Settings::getLastEp(name);
		for (int i = 0; i < episodes.size(); i++) {
			if (episodes[i].name.remove("|") != lastEp) {
				episodes[i].isNew = true;
			} else {
				break;
			}
		}
		OMA::Settings::setLastEp(name, episodes[0].name);
	} else {
		std::cout << "No episodes for " << name.toStdString().c_str() << std::endl;
	}
	return succesful;
}

bool Website::goToEpisode(Episode* episode, QString type) {
	if (type == OMA::linkTypes[LinkTypes::torrent] && OMA::Settings::getDownloadTorrent()) {
		QString dir = OMA::Settings::getTorrentDir();
		if (!dir.isEmpty()) {
			dir = dir + "\\";
			dir = dir.replace("/", "\\");
		}
		MyUtils::urlToFile(episode->downloadLink, dir + episode->name + ".torrent");
		return true;
	}

	QString url = episode->url;
	if (type == OMA::linkTypes[LinkTypes::magnet]) {
		url = episode->magnetLink;
	} else if (type != OMA::linkTypes[LinkTypes::post] &&
			   type != OMA::linkTypes[LinkTypes::customLink]) {
		url = MyLua::goToEpisode(name, episode, type);
		if (url.isEmpty() && goToEpisodeFunction != nullptr) {
			url = goToEpisodeFunction(episode, type);
		}
	}

	if (url.startsWith("https://www.youtube.com/watch?v=")) {
		if (OMA::Settings::getYoutubeToUmmy()) {
			url = url.replace("https://", "ummy://");
		} else if (!OMA::Settings::getYoutubeCustomLink().isEmpty()) {
			url = OMA::Settings::getYoutubeCustomLink().replace("*", url);
		}
	}

	return OMA::upenUrl(url);
}
