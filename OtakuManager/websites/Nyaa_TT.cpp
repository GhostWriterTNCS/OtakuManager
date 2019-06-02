#include "OMA.h"
#include "Website.h"

void Website::initialize_Nyaa_TT() {
	homepage = "https://nyaa.si/";
	episodesPage = "https://nyaa.si/?q";
	hasStreaming = false;
	hasDownload = false;
	hasTorrent = true;
	hasMagnet = true;
	getEpisodesFunction = std::bind(&Website::getEpisodes_Nyaa_TT, this);
	getSeriesFunction = std::bind(&Website::getSeries_Nyaa_TT, this);
	goToEpisodeFunction = std::bind(&Website::goToEpisode_Nyaa_TT, this, std::placeholders::_1,
									std::placeholders::_2);
}

bool Website::getEpisodes_Nyaa_TT() {
	QString html = MyUtils::urlToQString(episodesPage);
	QString start = "<tbody>";
	QString end = "</tbody>";
	if (html.count(end) == 1) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<tr class=\"");
		for (int i = 1; i < list.size(); i++) {
			Episode episode;
			html = list[i];
			html = MyUtils::substring(html, "<td colspan=\"2\">");
			if (html.contains("class=\"comments\"")) {
				html = MyUtils::substring(html, "</a>");
			}
			episode.url =
				homepage + MyUtils::substring(html, "href=\"/", "\"").replace("&#38;", "&");

			episode.name = MyUtils::substring(html, "title=\"", "\"");

			html = MyUtils::substring(html, "<td class=\"text-center\"");
			episode.downloadLink =
				homepage + MyUtils::substring(html, "href=\"/", "\"").replace("&#38;", "&");
			episode.magnetLink =
				"magnet" + MyUtils::substring(html, "href=\"magnet", "\"").replace("&#38;", "&");
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
