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
	torrent,
	magnet,
	animeInfo,
	customLink,
	post,
	postIfNoDownload
};

/*
 * HOW TO ADD A WEBSITE
 * Insert here the new website name.
 * Note: add the language suffix and put the website in the correct language group. Groups are
 * sorted alphabeticcaly.
 */
enum Websites {
	ENGLISH,
	AnimeHeaven_EN,
	KissAnime_EN,
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
	QString website = "";
	QString episode = "";
	LastEpisode(QStringList list);
};

class FollowedAnime {
public:
	QString anime = "";
	bool regex = false;
	QString website = "*";
	QString customLink = "";
	FollowedAnime();
	FollowedAnime(QStringList list);
	FollowedAnime(QString anime, bool regex, QString website, QString customLink);
};

class Feed {
public:
	QString name = "";
	QString url = "";
	QString oldName = "";
	Feed();
	Feed(QStringList list);
	Feed(QString name, QString url, QString oldName = "");
};

namespace OMA {

const QString version = "3.4";

const QStringList linkTypes = {"Streaming",   "StreamingIfNoDownload",
							   "Streaming 2", "Streaming 2IfNoDownload",
							   "Download",	"Download 2",
							   "Torrent",	 "Magnet",
							   "Anime info",  "Custom link",
							   "Post",		  "PostIfNoDownload"};

const QStringList feedTypes = {"Atom", "RSS"};

/*
 * HOW TO ADD A WEBSITE
 * Insert here the new website name.
 * Note: put the website in the correct language group. Groups are sorted alphabeticcaly. The
 * website position must be the same in 'enum Websites' and in 'const QStringList websites'.
 */
const QStringList websites = {
	"- English -",		"Anime Heaven", "KissAnime",	"- Italian -", "AnimeForce",
	"RedAnimeDatabase", "WebAnimex",	"- Torrents -", "AniDex",	  "Nyaa"};

const QString errorMex = "<h3 style='text-align: center;'>Error during connection :(</h3><p "
						 "style='text-align: center;'>If problem persists, check your internet "
						 "connection or try another website.</p>";

const QString torrentDownloaded = "OMA::torrentDownloaded";

MainWindow* getMainWindow();
void setMainWindow(MainWindow* mainWindow);

/**
	Add a message in the status bar.
*/
void addStatusMessage(QString message);
/**
	Remove a message from the status bar.
*/
void removeStatusMessage(QString message);

/**
	Check if an episode is seen.
*/
bool isSeen(QString episode);

/**
	Check if an episode is followed.
*/
bool isFollowed(QString name, QString website = "");

/**
	Open an url in the default browser.
*/
bool upenUrl(QString url);

namespace Settings {

/**
	Save settings in file 'settings.ini'.
*/
void sync();

/**
	Fix changes between versions.
*/
void fix();

// [General]

/**
	Get the version of the software that created the file 'settings.ini'.
*/
QString getVersion();
/**
	Set the current software version.
*/
void setVersion();

/**
	Is check for updates on startup enable?
*/
bool getCheckForUpdates();
/**
	Set check for updates on startup ON (true) or OFF (false).
*/
void setCheckForUpdates(bool b);

/**
	Get the enabled websites.
*/
QString getWebsitesQString();
/**
	Get the enabled websites.
*/
QStringList getWebsites();
/**
	Set the enabled websites.
*/
void setWebsites(QStringList list);

/**
	Get the enabled buttons.
*/
QStringList getButtons();
/**
	Set the enabled buttons.
*/
void setButtons(QString hidden, bool hide = true);

/**
	Get Feed.
*/
QList<Feed> getFeeds();
/**
Get Feed.
*/
QStringList getFeedNames();
/**
	Add an Feed.
*/
void setFeeds(QString name, QString url, QString oldName = "");
/**
	Set Feed.
*/
void setFeeds(QList<Feed> list);

// [Followed]

/**
	Get the followed anime/keywords.
*/
QList<FollowedAnime> getFollowed();
/**
	Get the count of followed anime/keywords.
*/
// int getFollowedCount(QString website);
/**
	Return the FollowedAnime structure for an episode name and website. Leave website as an empty
	string to search in all websites. If the episode is not followed or it is not followed for the
	specified website, return FollowedAnime().
*/
FollowedAnime getFollowed(QString name, QString website = "");
/**
	Add a followed anime/keyword.
*/
void setFollowed(QString name, bool regex = false, QString website = "*", QString customLink = "",
				 bool followed = true);
/**
	Bulk add followed anime/keywords.
*/
void setFollowed(QList<FollowedAnime> list);

// [Other]

/**
	Get custom AniDex url.
*/
QString getAniDexUrl();
/**
	Set custom AniDex url.
*/
void setAniDexUrl(QString url);

/**
	Direct download torren files?
*/
bool getDownloadTorrent();
/**
	Set direct download torren files ON (true) of OFF (false).
*/
void setDownloadTorrent(bool value);

/**
	Get torren files download directory.
*/
QString getTorrentDir();
/**
	Set torren files download directory.
*/
void setTorrentDir(QString dir);

/**
	Open YouTube links with Ummy?
*/
bool getYoutubeToUmmy();
/**
	Set open YouTube links with Ummy ON (true) of OFF (false).
*/
void setYoutubeToUmmy(bool value);

// [Log]

/**
	Get the last parsed episodes for each website.
*/
QList<LastEpisode> getLastEps();
/**
	Get the last parsed episode for a website.
*/
QString getLastEp(QString website);
/**
	Set the last parsed episode for a website.
*/
void setLastEp(QString website, QString episode);

/**
	Get the seen episodes.
*/
QString getSeen();
/**
	Set if a episodes is seen.
*/
void setSeen(QString ep, bool seen = true);

// QList<int> getWindowValues();
// void setWindowValues(QList<int> values);

} // namespace Settings

} // namespace OMA
