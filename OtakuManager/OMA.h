#pragma once
#include <QSettings>
#include "MainWindow.h"

enum LinkTypes {
	streaming,
	streamingIfNoDownload,
	streaming2,
	streaming2IfNoDownload,
	download,
	download2,
	magnet,
	animeInfo,
	customLink,
	post,
	postIfNoDownload
};

enum Websites {
	ENGLISH,
	AnimeHeaven_EN,
	ITALIAN,
	AnimeForce_IT,
	RedAnimeDatabase_IT,
	WebAnimex_IT,
	TORRENT,
	AniDex_TT,
	Nyaa_TT
};

class LastEpisode {
public:
	QString website;
	QString episode;
	LastEpisode(QStringList list);
};

class FollowedAnime {
public:
	QString anime;
	bool regex;
	QString website;
	QString customLink;
	FollowedAnime(QStringList list);
	FollowedAnime(QString anime, bool regex, QString website, QString customLink);
};

namespace OMA {

const QString version = "3.2";

const QStringList linkTypes = {"Streaming",		  "StreamingIfNoDownload",
							   "Streaming 2",	 "Streaming 2IfNoDownload",
							   "Download",		  "Download 2",
							   "Magnet",		  "Anime info",
							   "Custom link",	 "Post",
							   "PostIfNoDownload"};

const QStringList websites = {"- English -", "Anime Heaven",	 "- Italian -",
							  "AnimeForce",  "RedAnimeDatabase", "WebAnimex",
							  "- Torrent -", "AniDex",			 "Nyaa"};

const QString errorMex = "<h3 style='text-align: center;'>Error during connection :(</h3><p "
						 "style='text-align: center;'>If problem persists, check your internet "
						 "connection or try another website.</p>";

const QString torrentDownloaded = "OMA::torrentDownloaded";

MainWindow* getMainWindow();
void setMainWindow(MainWindow* mainWindow);

bool isSeen(QString episode);
bool isFollowed(QString name, QString website = "");

bool upenUrl(QString url);

namespace Settings {

void sync();

QString getVersion();
void setVersion();

bool getCheckForUpdates();
void setCheckForUpdates(bool b);

QString getWebsitesQString();
QStringList getWebsites();
void setWebsites(QStringList list);

QStringList getButtons();
void setButtons(QString hidden, bool hide = true);

// FollowedAndLinks
QList<FollowedAnime> getFollowed();
int getFollowedCount(QString website);
FollowedAnime getFollowed(QString name);
void setFollowed(QString name, bool regex = false, QString website = "*", QString customLink = "",
				 bool followed = true);
void setFollowed(QList<FollowedAnime> list);

bool getDownloadTorrent();
void setDownloadTorrent(bool value);

QString getTorrentDir();
void setTorrentDir(QString dir);

bool getYoutubeToUmmy();
void setYoutubeToUmmy(bool value);

// Other
QList<LastEpisode> getLastEps();
QString getLastEp(QString website);
void setLastEp(QString website, QString episode);

QString getSeen();
void setSeen(QString ep, bool seen = true);

QList<int> getWindowValues();
void setWindowValues(QList<int> values);

} // namespace Settings

} // namespace OMA
