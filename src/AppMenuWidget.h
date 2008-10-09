/***************************************************************************
                          appmenuwidget.h  -  The main Qt/KDE window
                             -------------------
    begin                : Tue Sep 16 00:00:00 CET 2008
    copyright            : (C) 2008 by LTIA
    email                : danilo.balzaque@ltia.fc.unesp.br
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef APPMENUWIDGET_H
#define APPMENUWIDGET_H

#ifdef DEBUG
#include <kdebug.h>
#endif

#include <kxmlguiwindow.h>
#include <kapplication.h>

#include <QHBoxLayout>

#include "mainqtwidget.h"
#include "FractionRingWidget.h"

class QHBoxLayout;
class QPushButton;
class MainQtWidget;
class FractionRingWidget;
class KAction;

/** Constructs the main window and presents the user interface.
 *  The window give the user the hability to choose what application
 *  he want to open first.
 *  \author Danilo Balzaque
 **/
class AppMenuWidget : public KXmlGuiWindow
{
	Q_OBJECT

public:
	/** constructor */
	AppMenuWidget();

	/** destructor */
	~AppMenuWidget();

private:
	QWidget * baseWidget;
	QWidget * interfaceWidget;

	QHBoxLayout * layout1;
	QGridLayout * gridLayout;

	QPushButton * m_Freestyle;
	QLabel * labelFreestyle;
	QPushButton * m_Learning;
	QLabel * labelLearning;
	QLabel * labelInfo;

	KAction * m_NewTaskAction;
	KAction * m_BackAction;
	KAction * m_HintAction;

	MainQtWidget * kbruchApp;
	FractionRingWidget * fractionRing;

	void setupActions();

private slots:
	void slotFreestyleClicked();
	void slotLearningClicked();
	/**
	* calls the settings dialog
	*/
	void slotPrefs();
	/**
	* makes sure, all parts of the UI update to new settings
	*/
	void slotApplySettings();
};

#endif // AppMenuWidget
