#pragma once
#include <functional>
#include <QString>
#include <QStringList>
#include <QtWidgets/QMainWindow>
#include "MyUtils.h"

class Episode {
public:
	QString url;
	QString name;
	QString streamingLink;
	QString downloadLink;
	QString magnetLink;
	bool isNew = false;
	bool hasDownload = true;
}; // class Episode

class Anime {
public:
	QString url;
	QString name;
}; // class Anime

class Website {
public:
	QString name;
	QString homepage;
	QString episodesPage;
	bool hasStreaming = true;
	bool hasDownload = true;
	bool hasTorrent = false;
	bool hasMagnet = false;
	bool hasInfo = true;
	bool isFeed = false;
	std::function<bool()> getEpisodesFunction;
	std::function<QString(Episode*, QString)> goToEpisodeFunction;

	QList<Episode> episodes;

	Website(QString website);

	/**
		Get the episodes.
	*/
	bool getEpisodes();

	/**
		Perform an action according to the chosen type (streaming, download, anime info...)
	*/
	bool goToEpisode(Episode* episode, QString type);

private:
	// ENGLISH
	void initialize_AnimeHeaven_EN();
	bool getEpisodes_AnimeHeaven_EN();
	QString goToEpisode_AnimeHeaven_EN(Episode* episode, QString type);

	void initialize_KissAnime_EN();
	bool getEpisodes_KissAnime_EN();
	QString goToEpisode_KissAnime_EN(Episode* episode, QString type);

	// ITALIAN
	void initialize_RedAnimeDatabase_IT();
	bool getEpisodes_RedAnimeDatabase_IT();
	QString goToEpisode_RedAnimeDatabase_IT(Episode* episode, QString type);

	void initialize_WebAnimex_IT();
	bool getEpisodes_WebAnimex_IT();
	QString goToEpisode_WebAnimex_IT(Episode* episode, QString type);

	// TORRENT
	void initialize_Nyaa_TT();
	bool getEpisodes_Nyaa_TT();
	QString goToEpisode_Nyaa_TT(Episode* episode, QString type);

	// FEED
	void initialize_Feed();
	bool getEpisodes_Feed();
	QString goToEpisode_Feed(Episode* episode, QString type);

}; // class Website
