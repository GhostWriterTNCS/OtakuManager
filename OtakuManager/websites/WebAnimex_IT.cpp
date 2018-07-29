#include "OMA.h"
#include "Website.h"

void Website::initialize_WebAnimex_IT() {
	homepage = "http://webanimex.com/";
	seriesPage = "http://webanimex.com/episodi-in-streaming/anime-in-corso/";
	getEpisodesFunction = std::bind(&Website::getEpisodes_WebAnimex_IT, this);
	getSeriesFunction = std::bind(&Website::getSeries_WebAnimex_IT, this);
	goToEpisodeFunction = std::bind(&Website::goToEpisode_WebAnimex_IT, this, std::placeholders::_1,
									std::placeholders::_2);
}

bool Website::getEpisodes_WebAnimex_IT() {
	QString html = MyUtils::urlToQString(homepage);
	QString start = "<ul class=\"list list-episodes\">";
	QString end = "<!-- #content -->";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<h3>");
		for (int i = 1; i < list.size(); i++) {
			Episode episode;
			html = list[i];
			html = MyUtils::substring(html, "href=\"", "\"");
			episode.url = html;

			html = list[i];
			html = MyUtils::advancedReplace(html, "<", ">", "");
			episode.name = MyUtils::decodeHtml(html).trimmed();

			if (!episode.name.contains("Download"))
				episode.hasDownload = false;
			episode.name = MyUtils::substring(episode.name, "", " - [SubITA]");
			episodes.push_back(episode);
		}
		return true;
	}
	return false;
}

bool Website::getSeries_WebAnimex_IT() {
	QString html = MyUtils::urlToQString(seriesPage);
	QString start = "<div id=\"content\">";
	QString end = "<!-- #content -->";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<h2>");
		for (int i = 1; i < list.size(); i++) {
			Anime anime;
			QString s = list[i];
			s = MyUtils::substring(s, "href=\"", "\"");
			anime.url = s;

			s = list[i];
			s = MyUtils::substring(s, "title=\"", " in streaming");
			s = MyUtils::substring(s, "", "\"");
			anime.name = s.trimmed();
			series.push_back(anime);
		}
		return true;
	}
	return false;
}

QString Website::goToEpisode_WebAnimex_IT(Episode* episode, QString type) {
	QString s;
	if (type == OMA::linkTypes[LinkTypes::streaming]) {
		s = MyUtils::urlToQString(episode->url);
		s = MyUtils::substring(s, "class=\"box-link-streaming");
		s = MyUtils::substring(s, "href=\"");
		s = MyUtils::substring(s, "", "\"");
		s = MyUtils::substring(s, "red.html?af=");
	} else if (type == OMA::linkTypes[LinkTypes::download]) {
		s = MyUtils::urlToQString(episode->url);
		s = MyUtils::substring(s, "class=\"box-link-download");
		s = MyUtils::substring(s, "href=\"", "\"");
		s = MyUtils::substring(s, "red.html?af=");
		s = MyUtils::redirectedUrlQt(s);
		/*s = MyUtils::advancedReplace(
			s, "", ".php?file=", "https://anon.to/?http://animeforce.stream/?u=http://");*/
		s = MyUtils::advancedReplace(s, "", ".php?file=", "http://");
	} else if (type == OMA::linkTypes[LinkTypes::animeInfo]) {
		s = MyUtils::urlToQString(episode->url);
		if (s.contains("class=\"breadcrumb-list\"")) {
			s = MyUtils::substring(s, "class=\"breadcrumb-list\"");
			s = MyUtils::substring(s, "/ul");
			s = MyUtils::substring(s, "href=\"", "\"");
		}
	}
	if (s.endsWith("&")) {
		s.truncate(s.length() - 1);
	}
	return s;
}
