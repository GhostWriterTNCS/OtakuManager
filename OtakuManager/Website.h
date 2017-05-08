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
	QString seriesPage;
	bool hasStreaming = true;
	bool hasDownload = true;
	bool hasDoubleButtons = false;
	bool hasMagnet = false;
	std::function<bool()> getEpisodesFunction;
	std::function<bool()> getSeriesFunction;
	std::function<QString(Episode*, QString)> goToEpisodeFunction;

	QList<Episode> episodes;
	QList<Anime> series;

	Website(QString website);
	bool getEpisodes();
	bool getSeries();
	bool goToEpisode(Episode* episode, QString type);

private:
	bool getEpisodes_AnimeHeaven_EN();
	bool getSeries_AnimeHeaven_EN();
	QString goToEpisode_AnimeHeaven_EN(Episode* episode, QString type);

	bool getEpisodes_AnimeForce_IT();
	bool getSeries_AnimeForce_IT();
	QString goToEpisode_AnimeForce_IT(Episode* episode, QString type);

	bool getEpisodes_RedAnimeDatabase_IT();
	bool getSeries_RedAnimeDatabase_IT();
	QString goToEpisode_RedAnimeDatabase_IT(Episode* episode, QString type);

	bool getEpisodes_WebAnimex_IT();
	bool getSeries_WebAnimex_IT();
	QString goToEpisode_WebAnimex_IT(Episode* episode, QString type);

	bool getEpisodes_AniDex_TT();
	bool getSeries_AniDex_TT();
	QString goToEpisode_AniDex_TT(Episode* episode, QString type);

	bool getEpisodes_Nyaa_TT();
	bool getSeries_Nyaa_TT();
	QString goToEpisode_Nyaa_TT(Episode* episode, QString type);
}; // class Website
