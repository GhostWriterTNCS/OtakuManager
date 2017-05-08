#include <iostream>
#include "OMA.h"
#include "Website.h"

bool Website::getEpisodes_AniDex_TT() {
	QString html = MyUtils::urlToQString(episodesPage);
	QString start = "<channel>";
	QString end = "</channel>";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<item>");
		for (int i = 1; i < list.size(); i++) {
			Episode episode;
			html = list[i];
			episode.url = MyUtils::substring(html, "<link>", "</link>");
			episode.name = MyUtils::substring(html, "<title>", "</title>");
			episode.downloadLink = MyUtils::substring(html, "<a href=\"", "\"");
			episode.magnetLink =
				"magnet:" + MyUtils::substring(html, "<a href=\"magnet:", "\"").replace(" ", "");
			episodes.push_back(episode);
		}
		return true;
	}
	return false;
}

bool Website::getSeries_AniDex_TT() {
	return false;
}

QString Website::goToEpisode_AniDex_TT(Episode* episode, QString type) {
	if (type == OMA::linkTypes[LinkTypes::download]) {
		return episode->downloadLink;
	}
	return "";
}
