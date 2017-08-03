#include <QMessageBox>
#include "OMA.h"
#include "Website.h"

Website::Website(QString website) {
	name = website;
	/*
	 * HOW TO ADD A WEBSITE
	 * Insert here the new website initializer.
	 * Note: group languages and sort alphabetically.
	 */
	if (website == OMA::websites[Websites::AnimeHeaven_EN]) {
		initialize_AnimeHeaven_EN();
	} else if (website == OMA::websites[Websites::KissAnime_EN]) {
		initialize_KissAnime_EN();
	} else if (website == OMA::websites[Websites::AnimeForce_IT]) {
		initialize_AnimeForce_IT();
	} else if (website == OMA::websites[Websites::RedAnimeDatabase_IT]) {
		initialize_RedAnimeDatabase_IT();
	} else if (website == OMA::websites[Websites::WebAnimex_IT]) {
		initialize_WebAnimex_IT();
	} else if (website == OMA::websites[Websites::AniDex_TT]) {
		initialize_AniDex_TT();
	} else if (website == OMA::websites[Websites::Nyaa_TT]) {
		initialize_Nyaa_TT();
	} else {
		QMessageBox::warning(OMA::getMainWindow(), "Error", "Website not valid: " + website);
	}
}

bool Website::getEpisodes() {
	episodes.clear();
	bool succesful = getEpisodesFunction();
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
	}
	return succesful;
}

bool Website::getSeries() {
	series.clear();
	if (!seriesPage.isEmpty()) {
		return getSeriesFunction();
	}
	return false;
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
		url = goToEpisodeFunction(episode, type);
	}

	if (url.startsWith("https://www.youtube.com/watch?v=") && OMA::Settings::getYoutubeToUmmy()) {
		url = url.replace("https://", "ummy://");
	}

	return OMA::upenUrl(url);
}
