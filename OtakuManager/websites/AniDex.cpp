#include "OMA.h"
#include "Website.h"

bool Website::getEpisodes_AniDex_TT() {
	QString html = MyUtils::urlToQStringJS(OMA::Settings::getAniDexUrl());
	QString start = "<table class=\"table table-striped table-hover table-condensed\">";
	QString end = "</table>";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<tr>");
		for (int i = 2; i < list.size(); i++) {
			Episode episode;
			html = list[i];
			episode.url = "https://anidex.info/" + MyUtils::substring(html, "href=\"", "\"");
			episode.name = MyUtils::substring(html, "<span class=\"span-1440\">", "</span>");
			episode.downloadLink =
				"https://anidex.info/dl" + MyUtils::substring(html, "<a href=\"/dl", "\"");
			episode.magnetLink = "magnet:" + MyUtils::substring(html, "<a href=\"magnet:", "\"");
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
