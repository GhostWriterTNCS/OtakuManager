#pragma once

#include <QMessageBox>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QtWidgets/QMainWindow>
#include "WebsiteWidget.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	QList<WebsiteWidget*> websites;
	QList<QWidget*> followedScrollAreas;
	Ui::MainWindowClass ui;

	MainWindow(QWidget* parent = Q_NULLPTR);
	~MainWindow();
	void updateAllEpisodes();
	// void initSettings();

	void openPreferences();
	void openColorsInfo();
	void openAbout();

signals:
	void showMessageBoxSignal(QMessageBox::Icon type, QString title, QString mex);

private slots:
	void on_updateButton_clicked();
	void on_updateAllButton_clicked();
	void showMessageBox(QMessageBox::Icon type, QString title, QString mex);
};

class CustomTabStyle : public QProxyStyle {
public:
	QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size,
						   const QWidget* widget) const {
		QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
		if (type == QStyle::CT_TabBarTab) {
			s.transpose();
		}
		return s;
	}

	void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
					 const QWidget* widget) const {
		if (element == CE_TabBarTabLabel) {
			if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
				QStyleOptionTab opt(*tab);
				opt.shape = QTabBar::RoundedNorth;
				QProxyStyle::drawControl(element, &opt, painter, widget);
				return;
			}
		}
		QProxyStyle::drawControl(element, option, painter, widget);
	}
};
