#include <QMessageBox>
#include "OMA.h"
#include "Website.h"

Website::Website(QString website) {
	name = website;
	if (website == OMA::websites[Websites::AnimeHeaven_EN]) {
		homepage = "http://animehaven.to/";
		episodesPage = "http://animehaven.to/anime-list?show_latest_anime=true";
		seriesPage = "http://animehaven.to/anime-list?show_latest_anime=true&show=series";
		hasDownload = false;
		getEpisodesFunction = std::bind(&Website::getEpisodes_AnimeHeaven_EN, this);
		getSeriesFunction = std::bind(&Website::getSeries_AnimeHeaven_EN, this);
		goToEpisodeFunction = std::bind(&Website::goToEpisode_AnimeHeaven_EN, this,
										std::placeholders::_1, std::placeholders::_2);
	} else if (website == OMA::websites[Websites::AnimeForce_IT]) {
		homepage = "http://www.animeforce.org/";
		seriesPage = "http://www.animeforce.org/lista-anime-in-corso/";
		getEpisodesFunction = std::bind(&Website::getEpisodes_AnimeForce_IT, this);
		getSeriesFunction = std::bind(&Website::getSeries_AnimeForce_IT, this);
		goToEpisodeFunction = std::bind(&Website::goToEpisode_AnimeForce_IT, this,
										std::placeholders::_1, std::placeholders::_2);
	} else if (website == OMA::websites[Websites::RedAnimeDatabase_IT]) {
		homepage = "http://redanimedatabase.forumcommunity.net/index.php";
		seriesPage = "http://redanimedatabase.forumcommunity.net/?f=8821471";
		hasDoubleButtons = true;
		getEpisodesFunction = std::bind(&Website::getEpisodes_RedAnimeDatabase_IT, this);
		getSeriesFunction = std::bind(&Website::getSeries_RedAnimeDatabase_IT, this);
		goToEpisodeFunction = std::bind(&Website::goToEpisode_RedAnimeDatabase_IT, this,
										std::placeholders::_1, std::placeholders::_2);
	} else if (website == OMA::websites[Websites::WebAnimex_IT]) {
		homepage = "http://webanimex.com/";
		seriesPage = "http://webanimex.com/episodi-in-streaming/anime-in-corso/";
		getEpisodesFunction = std::bind(&Website::getEpisodes_WebAnimex_IT, this);
		getSeriesFunction = std::bind(&Website::getSeries_WebAnimex_IT, this);
		goToEpisodeFunction = std::bind(&Website::goToEpisode_WebAnimex_IT, this,
										std::placeholders::_1, std::placeholders::_2);
	} else if (website == OMA::websites[Websites::AniDex_TT]) {
		homepage = "https://anidex.info/";
		// episodesPage = "https://anidex.info/rss/cat/1";
		hasStreaming = false;
		hasMagnet = true;
		getEpisodesFunction = std::bind(&Website::getEpisodes_AniDex_TT, this);
		getSeriesFunction = std::bind(&Website::getSeries_AniDex_TT, this);
		goToEpisodeFunction = std::bind(&Website::goToEpisode_AniDex_TT, this,
										std::placeholders::_1, std::placeholders::_2);
	} else if (website == OMA::websites[Websites::Nyaa_TT]) {
		homepage = "https://www.nyaa.se/";
		episodesPage = "https://www.nyaa.se/?cats=1_0";
		hasStreaming = false;
		getEpisodesFunction = std::bind(&Website::getEpisodes_Nyaa_TT, this);
		getSeriesFunction = std::bind(&Website::getSeries_Nyaa_TT, this);
		goToEpisodeFunction = std::bind(&Website::goToEpisode_Nyaa_TT, this, std::placeholders::_1,
										std::placeholders::_2);
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
	if (type == OMA::linkTypes[LinkTypes::download] && OMA::Settings::getDownloadTorrent()) {
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
