#include "OMA.h"
#include "Website.h"

bool Website::getEpisodes_AnimeHeaven_EN() {
	QString html = MyUtils::urlToQString(episodesPage);
	QString start = "<div id=\"latest_updates_section\" class=\"full-width-artikle-block "
					"infinite-scroll all-anime-list-section\">";
	QString end = "<li class=\"last_justify_fix\">";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<!-- #post- -->");
		for (int i = 0; i < list.size() - 1; i++) {
			Episode episode;
			html = list[i];
			if (html.contains("<span class=\"new_movie\">New movie</span>")) {
				episode.url = MyUtils::substring(html, "href=\"", "\"");
				episode.name = MyUtils::substring(html, "title=\"", "\"") + " Movie";
			} else if (html.contains("<span class=\"new_series\">New series</span>")) {
				episode.url = MyUtils::substring(html, "href=\"", "\"");
				episode.name = MyUtils::substring(html, "title=\"", "\"") + " Episode 01";
			} else if(html.contains("<span class=\"new_episode\">New episode</span>")) {
				html = MyUtils::substring(html, "<span>Latest:</span>");
				episode.url = MyUtils::substring(html, "href=\"", "\"");

				html = list[i];
				html = MyUtils::substring(html, "<h2 class=\"entry-title\">", "</h2>");
				html = MyUtils::substring(html, ">", "<").trimmed();
				episode.name =
					html.trimmed() + " " + MyUtils::substring(list[i], episode.url + "\">", "</a>");
			} else {
				episode.url = MyUtils::substring(html, "href=\"", "\"");
				episode.name = MyUtils::substring(html, "title=\"", "\"");
			}
			episodes.push_back(episode);
		}
		return true;
	}
	return false;
}

bool Website::getSeries_AnimeHeaven_EN() {
	QString html = MyUtils::urlToQString(seriesPage);
	QString start = "<div id=\"latest_updates_section\" class=\"full-width-artikle-block "
					"infinite-scroll all-anime-list-section\">";
	QString end = "<li class=\"last_justify_fix\">";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<!-- #post- -->");
		for (int i = 0; i < list.size() - 1; i++) {
			Anime anime;
			html = list[i];
			anime.url = MyUtils::substring(html, "href=\"", "\"");
			anime.name = MyUtils::substring(html, "title=\"", "\"");
			series.push_back(anime);
		}
		return true;
	}
	return false;
}

QString Website::goToEpisode_AnimeHeaven_EN(Episode* episode, QString type) {
	QString s;
	if (type == OMA::linkTypes[LinkTypes::streaming]) {
		s = episode->url;
	} else if (type == OMA::linkTypes[LinkTypes::download]) {
		s = MyUtils::urlToQString(episode->url);
		s = MyUtils::substring(s, "class=\"box-link-download");
		s = MyUtils::substring(s, "href=\"", "\"");
		s = MyUtils::substring(s, "red.html?af=");
		s = MyUtils::redirectedUrlQt(s);
		s = MyUtils::advancedReplace(s, "",
									 ".php?file=", "http://go.animeforce.org/1706398/http://");
	} else if (type == OMA::linkTypes[LinkTypes::animeInfo]) {
		s = MyUtils::urlToQString(episode->url);
		if (s.contains("class=\"breadcrumbs\"")) {
			s = MyUtils::substring(s, "class=\"breadcrumbs\"");
			s = MyUtils::substring(s, "&raquo;");
			s = MyUtils::substring(s, "&raquo;");
			s = MyUtils::substring(s, "href=\"", "\"");
		}
	}
	return s;
}
