#include "OMA.h"
#include "Website.h"

void Website::initialize_AniDex_TT() {
	homepage = "https://anidex.info/";
	// episodesPage = "https://anidex.info/rss/cat/1";
	hasStreaming = false;
	hasDownload = false;
	hasTorrent = true;
	hasMagnet = true;
	getEpisodesFunction = std::bind(&Website::getEpisodes_AniDex_TT, this);
	goToEpisodeFunction = std::bind(&Website::goToEpisode_AniDex_TT, this, std::placeholders::_1,
									std::placeholders::_2);
}

bool Website::getEpisodes_AniDex_TT() {
	QString url = homepage;
	if (!OMA::Settings::getAniDexUrl().isEmpty()) {
		url = OMA::Settings::getAniDexUrl();
	}
	QString html = MyUtils::urlToQStringJS(url);
	QString start = "<table class=\"table table-striped table-hover table-condensed\">";
	QString end = "</table>";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<tr>");
		for (int i = 2; i < list.size(); i++) {
			Episode episode;
			html = list[i];
			html = MyUtils::substring(html, "<a class=\"torrent\"");
			episode.url = "https://anidex.info/" + MyUtils::substring(html, "href=\"", "\"");
			episode.name = MyUtils::substring(html, "title=\"", "\"");
			episode.downloadLink =
				"https://anidex.info/dl" + MyUtils::substring(html, "<a href=\"/dl", "\"");
			episode.magnetLink = "magnet:" + MyUtils::substring(html, "<a href=\"magnet:", "\"");
			episodes.push_back(episode);
		}
		return true;
	}
	return false;
}

QString Website::goToEpisode_AniDex_TT(Episode* episode, QString type) {
	if (type == OMA::linkTypes[LinkTypes::torrent]) {
		return episode->downloadLink;
	}
	return "";
}
