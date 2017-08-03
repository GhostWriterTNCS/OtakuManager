#include "OMA.h"
#include "Website.h"

void Website::initialize_Nyaa_TT() {
	homepage = "https://www.nyaa.se/";
	episodesPage = "https://nyaa.se/?q=&f=0&c=1_0";
	hasStreaming = false;
	hasDownload = false;
	hasTorrent = true;
	getEpisodesFunction = std::bind(&Website::getEpisodes_Nyaa_TT, this);
	getSeriesFunction = std::bind(&Website::getSeries_Nyaa_TT, this);
	goToEpisodeFunction = std::bind(&Website::goToEpisode_Nyaa_TT, this, std::placeholders::_1,
		std::placeholders::_2);
}

bool Website::getEpisodes_Nyaa_TT() {
	QString html = MyUtils::urlToQString(homepage);
	QString start = "<th class=\"tlistthfive\">";
	QString end = "</table><div class=\"leftpages\">";
	if (html.count(end) == 2) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<tr class=\"tlistrow");
		for (int i = 1; i < list.size(); i++) {
			Episode episode;
			html = list[i];
			html = MyUtils::substring(html, "<td class=\"tlistname\">");
			episode.url = MyUtils::substring(html, "href=\"", "\"").replace("&#38;", "&");

			episode.name = MyUtils::substring(html, ">", "<");

			html = MyUtils::substring(html, "<td class=\"tlistdownload\">");
			episode.downloadLink = MyUtils::substring(html, "href=\"", "\"").replace("&#38;", "&");
			episodes.push_back(episode);
		}
		return true;
	}
	return false;
}

bool Website::getSeries_Nyaa_TT() {
	return false;
}

QString Website::goToEpisode_Nyaa_TT(Episode* episode, QString type) {
	if (type == OMA::linkTypes[LinkTypes::torrent]) {
		return episode->downloadLink;
	}
	return "";
}
