#include "OMA.h"
#include "Website.h"

void Website::initialize_RedAnimeDatabase_IT() {
	homepage = "http://redanimedatabase.forumcommunity.net/index.php";
	getEpisodesFunction = std::bind(&Website::getEpisodes_RedAnimeDatabase_IT, this);
	goToEpisodeFunction = std::bind(&Website::goToEpisode_RedAnimeDatabase_IT, this,
									std::placeholders::_1, std::placeholders::_2);
}

bool Website::getEpisodes_RedAnimeDatabase_IT() {
	QString html = MyUtils::urlToQString(homepage);
	QString start = "<div style=\"overflow-y: scroll;";
	QString end = "<div style=\" width: 420px;";
	if (html.contains(end)) {
		html = MyUtils::substring(html, start, end);
		QStringList list = html.split("<span style=\"font-size: 7.5pt\">");
		for (int i = 1; i < list.size(); i++) {
			if (!list[i].contains("<span style=\"color:#0084c1\">")) {
				Episode episode;
				html = list[i];
				html = MyUtils::substring(html, "href=\"", "</span></a>");

				QString splitter = "\"><span style=\"color:blue\">";
				episode.url = MyUtils::substring(html, "", splitter);
				episode.name = MyUtils::substring(html, splitter).replace(" Sub ITA", "");
				episodes.push_back(episode);
			}
		}
		return true;
	}
	return false;
}

QString Website::goToEpisode_RedAnimeDatabase_IT(Episode* episode, QString type) {
	QString url = episode->url;
	if (type != OMA::linkTypes[LinkTypes::animeInfo]) {
		if (name.lastIndexOf("-END") == name.length() - 4)
			name = name.mid(0, name.length() - 4);
		if (name.lastIndexOf(" ITA") == name.length() - 4)
			name = name.mid(0, name.length() - 4);
		if (name.lastIndexOf(" BD") == name.length() - 3)
			name = name.mid(0, name.length() - 3);
		QString id = MyUtils::substringFromEnd(name, " ");
		QString tipo_ep = "Episodio ";
		if (name.contains(" OAV "))
			tipo_ep = "OAV ";
		else if (name.contains(" Movie "))
			tipo_ep = "Movie ";
		else if (name.contains(" Sp "))
			tipo_ep = "Special ";
		if (!id.contains("-")) {
			QString s = MyUtils::urlToQString(url);
			s = MyUtils::substring(s, "<fieldset>");
			s = MyUtils::substring(s, "", "</fieldset>");
			QStringList episode = s.split("<br>");
			s = "";
			for (int i = 0; i < episode.size(); i++) {
				if ((episode[i].contains(tipo_ep + id) ||
					 episode[i].contains(tipo_ep + "0" + id)) &&
					episode[i].contains("href")) {
					s = episode[i];
					break;
				}
			}
			if (!s.isEmpty()) {
				if (s.contains(tipo_ep))
					url = MyUtils::substring(s, "", tipo_ep);
				if (type == OMA::linkTypes[LinkTypes::streaming]) {
					if (s.contains("Streaming"))
						url = MyUtils::substring(s, "", ">Streaming<");
				}
				url = MyUtils::substringFromEnd(url, "href=\"");
				url = MyUtils::substring(url, "", "\"");
			}
		}
	}
	return url;
}
