#include "OMA.h"
#include "Website.h"

bool Website::getEpisodes_KissAnime_EN() {
	QString html = MyUtils::urlToQStringJS(homepage);
	QString start = "<div class=\"barTitle full\">Latest anime update</div>";
	QString end = "<div class=\"full more_in_tab more_in_rightBox\">";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<div class=\"tit\">");
		for (int i = 1; i < list.size(); i++) {
			Episode episode;
			html = list[i];
			html = MyUtils::substring(html, "</h4>", "</a>");
			episode.url = MyUtils::substring(html, "href=\"", "\"");

			html = MyUtils::substring(html, "title=\"", "\"");
			episode.name = html.trimmed();

			episodes.push_back(episode);
		}
		return true;
	}
	return false;
}

bool Website::getSeries_KissAnime_EN() {
	QString html = MyUtils::urlToQStringJS(seriesPage);
	QString start = "<div class=\"listing full\">";
	QString end = "<div class=\"pagination pagination-left\">";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<a class=\"item_movies_link\"");
		for (int i = 1; i < list.size(); i++) {
			Anime anime;
			QString s = list[i];
			anime.url = MyUtils::substring(s, "href=\"", "\"");

			anime.name = MyUtils::substring(s, ">", "<").trimmed();
			series.push_back(anime);
		}
		return true;
	}
	return false;
}

QString Website::goToEpisode_KissAnime_EN(Episode* episode, QString type) {
	QString s = episode->url;
	if (type == OMA::linkTypes[LinkTypes::animeInfo]) {
		s = MyUtils::substringFromEnd(episode->url, "", "/");
	} else if (type == OMA::linkTypes[LinkTypes::download]) {
		s = MyUtils::urlToQStringJS(episode->url);
		if (s.contains("<div class=\"clear2 h20\"></div>")) {
			s = MyUtils::substring(s, "id=\"divDownload\"", "</div>");
			s = MyUtils::substring(s, "href=\"", "\"").replace("&amp;", "&");
		} else {
			s = "";
		}
	}
	return s;
}
