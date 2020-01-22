#include <QStandardItemModel>
#include "FollowedWidget.h"

FollowedWidget::FollowedWidget(FollowedAnime* followed, QWidget* parent) : QWidget(parent) {
	ui.setupUi(this);
	ui.websiteComboBox->insertItem(0, "*");
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui.websiteComboBox->model());
	QHash<QString, QStringList> websites = OMA::websites();
	int i = 0;
	QList<QString> groups = websites.keys();
	groups.sort();
	foreach (QString group, groups) {
		ui.websiteComboBox->addItem(group);
		QStandardItem* item = model->item(i + 1);
		item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
		i++;
		foreach (QString w, websites[group]) {
			ui.websiteComboBox->addItem(w);
			QStandardItem* item = model->item(i + 1);
			item->setFlags(item->flags() | Qt::ItemIsEnabled);
			i++;
		}
	}

	this->followed = followed;
	ui.animeTitle->setText(followed->anime);
	ui.regex->setChecked(followed->regex);
	ui.websiteComboBox->setCurrentText(followed->website);
	ui.customLinkLineEdit->setText(followed->customLink);
}
