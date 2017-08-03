# How to add a website

These are the files to edit in order to add a new website.

## OMA.h

Add the website name in *enum Websites* and in *const QStringList websites*.

Websites are grouped by language and sorted alphabetically.

## Website.h

Add these private methods:
* void initialize_*WebsiteName*();
* bool getEpisodes_*WebsiteName*();
* bool getSeries_*WebsiteName*();
* QString goToEpisode_*WebsiteName*(Episode* episode, QString type);

## Website.cpp

Add initialize_*WebsiteName*(); in the if block.

## Add *WebsiteName*.cpp in websites folder

Include:
* #include "OMA.h"
* #include "Website.h"

Implement:
* void initialize_*WebsiteName*();  
	Initialize Website with the required parameters:
	* QString homepage.
	* QString episodesPage -> only if it's different from homepage.
	* QString seriesPage -> only if website has it.
	* bool hasStreaming -> default *true*.
	* bool hasDownload -> default *true*.
	* bool hasDoubleButtons -> default *false*.
	* getEpisodesFunction = std::bind(&Website::getEpisodes_*WebsiteName*, this).
	* getSeriesFunction = std::bind(&Website::getEpisodes_*WebsiteName*, this).
	* goToEpisodeFunction = std::bind(&Website::getEpisodes_*WebsiteName*, this).
* bool Website::getEpisodes_*WebsiteName*().
* bool Website::getSeries_*WebsiteName*() -> just *return false;* if website doesn't have series.
* QString Website::goToEpisode_*WebsiteName*(Episode* episode, QString type).

Take a look to other websites files.
