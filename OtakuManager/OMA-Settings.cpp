#include <QStandardPaths>
#include "MyUtils.h"
#include "OMA.h"

namespace OMA {
namespace Settings {

QSettings* settings = new QSettings("settings.ini", QSettings::IniFormat);

void sync() {
	settings->sync();
}

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
	return settings->value("websites", "Anime Heaven").toString();
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
							   "|" + linkTypes[LinkTypes::animeInfo])
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

// Followed
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
	QList<FollowedAnime> list = Settings::getFollowed();
	int count = 0;
	for (int i = 0; i < list.size(); i++) {
		if (website.isEmpty() || list[i].website == "*" || website == list[i].website) {
			count++;
		}
	}
	return count;
}
FollowedAnime getFollowed(QString name) {
	QList<FollowedAnime> followed = getFollowed();
	for (int i = 0; i < followed.size(); i++) {
		if (name.contains(followed[i].anime)) {
			return followed[i];
		}
	}
	return FollowedAnime("", false, "", "");
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

bool getDownloadTorrent() {
	return settings->value("downloadTorrent", "false").toBool();
}
void setDownloadTorrent(bool value) {
	settings->setValue("downloadTorrent", value);
}

QString getTorrentDir() {
	return settings
		->value("torrendDir", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation))
		.toString();
}
void setTorrentDir(QString dir) {
	settings->setValue("torrendDir", dir);
}

bool getYoutubeToUmmy() {
	return settings->value("youtubeToUmmy", "false").toBool();
}
void setYoutubeToUmmy(bool value) {
	settings->setValue("youtubeToUmmy", value);
}

// Other
QList<LastEpisode> getLastEps() {
	QList<LastEpisode> lastEps;
	QStringList list = settings->value("Other/lastEps", "").toString().split("\n");
	for (int i = 0; i < list.size(); i++) {
		if (!list[i].isEmpty()) {
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
	settings->setValue("Other/lastEps", s);
}

QString getSeen() {
	return settings->value("Other/seen", "").toString();
}
void setSeen(QString ep, bool seen) {
	ep = MyUtils::simplifyEp(ep);
	if (!isSeen(ep) && seen) {
		if (!getSeen().isEmpty()) {
			ep = "|" + ep;
		}
		settings->setValue("Other/seen", getSeen() + ep);
	} else if (isSeen(ep) && !seen) {
		QString s = "|" + getSeen();
		s = s.replace("|" + ep, "");
		if (s.startsWith("|")) {
			s = s.mid(1, s.size() - 1);
		}
		settings->setValue("Other/seen", s);
	}
}

QList<int> getWindowValues() {
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
}

} // namespace Settings
} // namespace OMA