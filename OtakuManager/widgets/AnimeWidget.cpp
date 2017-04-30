#include "AnimeWidget.h"
#include "OMA.h"

AnimeWidget::AnimeWidget(Anime* anime, Website* website, QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);
	this->anime = anime;
	this->website = website;
	ui.animeTitle->setText("<a href='" + anime->url + "'>" + anime->name + "</a>");
	if (OMA::isFollowed(anime->name)) {
		ui.followCheckBox->setChecked(true);
		QList<FollowedAnime> list = OMA::Settings::getFollowed();
		for (int i = 0; i < list.size(); i++) {
			if (list[i].regex) {
				if (anime->name.contains(QRegExp(list[i].anime, Qt::CaseInsensitive))) {
					this->followedName = list[i].anime;
					ui.customLinkLineEdit->setText(list[i].customLink);
				}
			} else {
				if (anime->name.contains(list[i].anime, Qt::CaseInsensitive)) {
					this->followedName = list[i].anime;
					ui.customLinkLineEdit->setText(list[i].customLink);
				}
			}
		}
	}
}

void AnimeWidget::on_followCheckBox_clicked(bool checked) {
	if (!followedName.isEmpty() && !checked) {
		OMA::Settings::setFollowed(followedName, false, website->name,
								   ui.customLinkLineEdit->text(), checked);
	} else {
		OMA::Settings::setFollowed(anime->name, false, website->name, ui.customLinkLineEdit->text(),
								   checked);
	}
}

void AnimeWidget::on_saveButton_clicked() {
	OMA::Settings::setFollowed(followedName, false, website->name, ui.customLinkLineEdit->text(),
							   ui.followCheckBox->isChecked());
}
