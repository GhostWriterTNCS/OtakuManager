#include <iostream>
#include <QFileInfo>
#include <QMessageBox>
#include "MyLua.h"
#include "OMA.h"
#include "Website.h"

Website::Website(QString website) {
	name = website;
	QFileInfo check_file("lua/" + website + ".lua");
	if (check_file.exists() && check_file.isFile()) {
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
	} else if (website == "AniDex") {
		initialize_AniDex_TT();
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
	if (!succesful) {
		succesful = getEpisodesFunction();
	}
	if (episodes.size() > 0) {
		QString lastEp = OMA::Settings::getLastEp(name);
		for (int i = 0; i < episodes.size(); i++) {
			if (episodes[i].name != lastEp) {
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
		if (url.isEmpty()) {
			url = goToEpisodeFunction(episode, type);
		}
	}

	if (url.startsWith("https://www.youtube.com/watch?v=") && OMA::Settings::getYoutubeToUmmy()) {
		url = url.replace("https://", "ummy://");
	}

	return OMA::upenUrl(url);
}
