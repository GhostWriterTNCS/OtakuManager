#include "FeedWidget.h"

FeedWidget::FeedWidget(Feed* feed, QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);
	this->feed = feed;
	oldName = feed->name;
	ui.name->setText(feed->name);
	ui.url->setText(feed->url);
}
