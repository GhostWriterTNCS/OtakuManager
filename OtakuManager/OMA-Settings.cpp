#include <QStandardPaths>
#include "MyUtils.h"
#include "OMA.h"

namespace OMA {
namespace Settings {

QSettings* settings;

void sync() {
	settings->sync();
}

void fix() {
	settings = new QSettings(QCoreApplication::applicationDirPath() + "\\settings.ini",
							 QSettings::IniFormat);

	QString v = getVersion();
	if (!v.isEmpty()) {
		if (MyUtils::substring(v, "", ".").toInt() <= 3 &&
			MyUtils::substring(v, ".", ".").toInt() <= 3) {
			settings->setValue("Other/AniDexUrl",
							   settings->value("AniDexUrl", "https://anidex.info/").toString());
			settings->setValue("Other/downloadTorrent",
							   settings->value("downloadTorrent", "false").toString());
			settings->setValue("Other/torrendDir",
							   settings
								   ->value("torrendDir", QStandardPaths::writableLocation(
															 QStandardPaths::DownloadLocation))
								   .toString());
			settings->setValue("Other/youtubeToUmmy",
							   settings->value("youtubeToUmmy", "false").toString());
			settings->setValue("Log/lastEps", settings->value("Other/lastEps", "").toString());
			settings->setValue("Log/seen", settings->value("Other/seen", "").toString());

			settings->remove("AniDexUrl");
			settings->remove("downloadTorrent");
			settings->remove("torrendDir");
			settings->remove("youtubeToUmmy");
			settings->remove("Other/lastEps");
			settings->remove("Other/seen");
		}
	}
}

// [General]

QString getVersion() {
	return settings->value("version", "").toString();
}
void setVersion() {
	settings->setValue("version", OMA::version);
}

bool getCheckForUpdates() {
	return settings->value("autoUpdate", "true").toBool();
}
void setCheckForUpdates(bool value) {
	settings->setValue("autoUpdate", value);
}

QString getWebsitesQString() {
	return settings->value("websites", "Nyaa").toString();
}
QStringList getWebsites() {
	return getWebsitesQString().split("|");
}
void setWebsites(QString s) {
	settings->setValue("websites", s);
}
void setWebsites(QStringList list) {
	QString s = "";
	for (int i = 0; i < list.size(); i++) {
		s += list[i] + "|";
	}
	if (s.endsWith("|")) {
		s = s.mid(0, s.length() - 1);
	}
	setWebsites(s);
}

QStringList getButtons() {
	return settings
		->value("buttons", linkTypes[LinkTypes::streaming] + "|" + linkTypes[LinkTypes::download] +
							   "|" + linkTypes[LinkTypes::torrent] + "|" +
							   linkTypes[LinkTypes::animeInfo])
		.toString()
		.split("|");
}
void setButtons(QString button, bool show) {
	QString s = "";
	QStringList list = getButtons();
	if (show) {
		for (int i = 0; i < list.size(); i++) {
			s += list[i] + "|";
		}
		if (!("|" + s).contains("|" + button + "|")) {
			s += button;
		}
	} else {
		for (int i = 0; i < list.size(); i++) {
			if (list[i] != button) {
				s += list[i] + "|";
			}
		}
	}
	if (s.endsWith("|")) {
		s = s.mid(0, s.length() - 1);
	}
	settings->setValue("buttons", s);
}

QString getFeedString() {
	return settings->value("feed", "").toString();
}
QList<Feed> getFeeds() {
	QStringList list = getFeedString().split("\n");
	QList<Feed> feed;
	for (int i = 0; i < list.size(); i++) {
		if (list[i].split("|").size() == 2) {
			feed.append(list[i].split("|"));
		}
	}
	return feed;
}
QStringList getFeedNames() {
	QList<Feed> list = getFeeds();
	QStringList names = QStringList();
	for (int i = 0; i < list.size(); i++) {
		names.append(list[i].name);
	}
	return names;
}
void setFeeds(QString name, QString url, QString oldName) {
	QList<Feed> list = getFeeds();
	if (!url.isEmpty()) {
		list.append(Feed(name, url, oldName));
	}
	setFeeds(list);
}
void setFeeds(QList<Feed> list) {
	QString s;
	QStringList websites = getWebsites();
	bool websitesdEdited = false;
	QList<FollowedAnime> followed = getFollowed();
	bool followedEdited = false;
	for (int i = 0; i < list.size(); i++) {
		s += list[i].name + "|" + list[i].url + "\n";
		if (!list[i].oldName.isEmpty()) {
			for (int w = 0; w < websites.size(); w++) {
				if (websites[w] == list[i].oldName) {
					websites[w] = list[i].name;
					websitesdEdited = true;
				}
			}
			for (int f = 0; f < followed.size(); f++) {
				if (followed[f].website == list[i].oldName) {
					followed[f].website = list[i].name;
					followedEdited = true;
				}
			}
		}
	}
	if (websitesdEdited) {
		setWebsites(websites);
	}
	if (followedEdited) {
		setFollowed(followed);
	}
	if (s.endsWith("\n")) {
		s = s.mid(0, s.length() - 1);
	}
	settings->setValue("feed", s);
}

// [Followed]

QString getFollowedString() {
	return settings
		->value("Followed/followed", "[^0-9]+01[^0-9]+|true|*|\n[^a-z]+sp[^a-z]+|true|*|\n[^a-z]+"
									 "special[^a-z]+|true|*|\n[^a-z]+ova[^a-z]+|true|*|\n[^a-z]+"
									 "oav[^a-z]+|true|*|\n[^a-z]+movie[^a-z]+|true|*|")
		.toString();
}
QList<FollowedAnime> getFollowed() {
	QStringList list = getFollowedString().split("\n");
	QList<FollowedAnime> followed;
	for (int i = 0; i < list.size(); i++) {
		if (!list[i].isEmpty()) {
			if (list[i].split("|").size() == 4) {
				followed.append(list[i].split("|"));
			} else {
				QStringList temp = list[i].split("|");
				followed.append(FollowedAnime(temp[0], false, temp[1], temp[2]));
			}
		}
	}
	return followed;
}
int getFollowedCount(QString website) {
	QList<FollowedAnime> list = getFollowed();
	int count = 0;
	for (int i = 0; i < list.size(); i++) {
		if (website.isEmpty() || list[i].website == "*" || website == list[i].website) {
			count++;
		}
	}
	return count;
}
FollowedAnime getFollowed(QString name, QString website) {
	name = " " + name + " ";
	QList<FollowedAnime> list = getFollowed();
	for (int i = 0; i < list.size(); i++) {
		if (list[i].regex) {
			if (name.contains(QRegExp(list[i].anime, Qt::CaseInsensitive))) {
				if (website.isEmpty() || list[i].website == "*" || website == list[i].website) {
					return list[i];
				}
			}
		} else {
			if (MyUtils::simplify(name).contains(list[i].anime, Qt::CaseInsensitive)) {
				if (website.isEmpty() || list[i].website == "*" || website == list[i].website) {
					return list[i];
				}
			}
		}
	}
	return FollowedAnime();
}
void setFollowed(QString name, bool regex, QString website, QString customLink, bool followed) {
	if (!regex) {
		name = MyUtils::simplify(name);
	}
	QList<FollowedAnime> list = getFollowed();
	if (!followed) {
		for (int i = 0; i < list.size(); i++) {
			if (name.contains(list[i].anime)) {
				list.removeAt(i);
				break;
			}
		}
	} else {
		if (!name.isEmpty()) {
			list.append(FollowedAnime(name, regex, website, customLink));
		}
	}
	setFollowed(list);
}
void setFollowed(QList<FollowedAnime> followed) {
	QString s;
	for (int i = 0; i < followed.size(); i++) {
		QString anime, regex;
		if (followed[i].regex) {
			anime = followed[i].anime;
			regex = "true";
		} else {
			anime = MyUtils::simplifyOneLine(followed[i].anime);
			regex = "false";
		}
		s += anime + "|" + regex + "|" + followed[i].website + "|" + followed[i].customLink + "\n";
	}
	if (s.endsWith("\n")) {
		s = s.mid(0, s.length() - 1);
	}
	settings->setValue("Followed/followed", s);
}

// [Other]

QString getAniDexUrl() {
	return settings->value("Other/AniDexUrl", "https://anidex.info/").toString();
}
void setAniDexUrl(QString url) {
	settings->setValue("Other/AniDexUrl", url);
}

bool getDownloadTorrent() {
	return settings->value("Other/downloadTorrent", "false").toBool();
}
void setDownloadTorrent(bool value) {
	settings->setValue("Other/downloadTorrent", value);
}

QString getTorrentDir() {
	return settings
		->value("Other/torrendDir",
				QStandardPaths::writableLocation(QStandardPaths::DownloadLocation))
		.toString();
}
void setTorrentDir(QString dir) {
	settings->setValue("Other/torrendDir", dir);
}

bool getYoutubeToUmmy() {
	return settings->value("Other/youtubeToUmmy", "false").toBool();
}
void setYoutubeToUmmy(bool value) {
	settings->setValue("Other/youtubeToUmmy", value);
}

// [Log]

QList<LastEpisode> getLastEps() {
	QList<LastEpisode> lastEps;
	QStringList list = settings->value("Log/lastEps", "").toString().split("\n");
	for (int i = 0; i < list.size(); i++) {
		if (list[i].contains("|")) {
			lastEps.push_back(list[i].split("|"));
		}
	}
	return lastEps;
}
QString getLastEp(QString website) {
	QList<LastEpisode> lastEps = getLastEps();
	for (int i = 0; i < lastEps.size(); i++) {
		if (lastEps[i].website == website)
			return lastEps[i].episode;
	}
	return "";
}
void setLastEp(QString website, QString episode) {
	bool added = false;
	QList<LastEpisode> lastEps = getLastEps();
	for (int i = 0; i < lastEps.size(); i++) {
		if (lastEps[i].website == website) {
			lastEps[i].episode = episode;
			added = true;
			break;
		}
	}
	QString s = "";
	for (int i = 0; i < lastEps.size(); i++) {
		s += lastEps[i].website + "|" + lastEps[i].episode + "\n";
	}
	if (!added) {
		s += website + "|" + episode + "\n";
	}
	if (s.endsWith("\n")) {
		s = s.mid(0, s.length() - 1);
	}
	settings->setValue("Log/lastEps", s);
}

QString getSeen() {
	return settings->value("Log/seen", "").toString();
}
void setSeen(QString ep, bool seen) {
	ep = MyUtils::simplifyEp(ep);
	if (!isSeen(ep) && seen) {
		if (!getSeen().isEmpty()) {
			ep = "|" + ep;
		}
		settings->setValue("Log/seen", getSeen() + ep);
	} else if (isSeen(ep) && !seen) {
		QString s = "|" + getSeen();
		s = s.replace("|" + ep, "");
		if (s.startsWith("|")) {
			s = s.mid(1, s.size() - 1);
		}
		settings->setValue("Log/seen", s);
	}
}

/*QList<int> getWindowValues() {
	QList<int> values = {770, 500, -9999, -9999, 0};
#ifndef DEBUG
	QStringList list = settings->value("windowValues", "").toString().split("|");
	for (int i = 0; i < 5; i++) {
		values[i] = list[i].toInt();
	}
#endif
	return values;
}
void setWindowValues(QList<int> values) {
	QString s = "";
	for (int i = 0; i < values.size(); i++) {
		s += values[i] + "|";
	}
	s = s.mid(0, s.length() - 1);
	settings->setValue("windowValues", s);
}*/

} // namespace Settings
} // namespace OMA
