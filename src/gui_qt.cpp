/***************************************************************************
                          gui_qt.cpp  -  Qt GUI for KBruch
                             -------------------
    begin                : Tue Jan 29 12:00:00 CET 2002
    copyright            : (C) 2001 - 2002 by Sebastian Stein
    email                : bruch@hpfsc.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "gui_qt.h"


/* ------ public member functions ------ */

/* constructor */
gui_qt::gui_qt(QWidget * parent, const char * name): QVBox(parent, name),
			pparent(parent), pname(name)
{
#ifdef DEBUG
	cout << "constructor gui_qt()" << endl;
#endif

	/* add/sub button */
	addsubBtn = new QPushButton(i18n("&addition and subtraction"), this);
	addsubBtn->setFont(QFont("helvetica", 10));
	QObject::connect(addsubBtn, SIGNAL(clicked()), this,
												SLOT(showQt_Para_addsub()));
	
	/* mul/div button */
	muldivBtn = new QPushButton(i18n("&multiplication and division"), this);
	muldivBtn->setFont(QFont("helvetica", 10));
	QObject::connect(muldivBtn, SIGNAL(clicked()), this,
												SLOT(showQt_Para_muldiv()));

	/* add/sub/mul/div button */
	allBtn = new QPushButton(i18n("m&ixed"), this);
	allBtn->setFont(QFont("helvetica", 10));
	allBtn->resize(100, 50);
	QObject::connect(allBtn, SIGNAL(clicked()), this, SLOT(showQt_Para_all()));

	/* help button */
	helpBtn = new QPushButton(i18n("&help"), this);
	helpBtn->setFont(QFont("helvetica", 10));

	/* exit button */
	quitBtn = new QPushButton(i18n("E&xit"), this);
	QObject::connect(quitBtn, SIGNAL(clicked()), qApp, SLOT(quit()));

	/* show KBruch - version in the titel of the main widget */
	QString caption = i18n("KBruch - ");
	caption += VERSION;
	setCaption(caption);

	/* fix the size of the main widget with +10% over the minimum */
	setFixedSize(QSize(minimumSize() * 0.1f));

	addsubBtn->setFocus();
}

/* destructor */
gui_qt::~gui_qt()
{
#ifdef DEBUG
	cout << "destructor gui_qt()" << endl;
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}


/* ------ private slots ------ */

/* shows the main menu again and deletes the parameter window */
void gui_qt::closeQt_Para()
{
	show(); /* show the main menu */
	delete para_window; /* delete the parameter window */

	return;
}

/* shows the parameter window */
void gui_qt::showQt_Para_addsub()
{
	hide(); /* hides the main menu */

	/* we build a new parameter window */
	para_window = new qt_para(pparent, pname, YES, NO);

	/* we connect a slot to the close signal of the window, so we get to know
	 * when the other window was closed */
	QObject::connect(para_window, SIGNAL(closed()), this, SLOT(closeQt_Para()));
	para_window->show(); /* show the new window */

	return;
}

/* shows the parameter window */
void gui_qt::showQt_Para_muldiv()
{
	hide(); /* hides the main menu */

	/* we build a new parameter window */
	para_window = new qt_para(0, 0, NO, YES);

	/* we connect a slot to the close signal of the window, so we get to know
	 * when the other window was closed */
	QObject::connect(para_window, SIGNAL(closed()), this, SLOT(closeQt_Para()));
	para_window->show(); /* show the new window */

	return;
}

/* shows the parameter window */
void gui_qt::showQt_Para_all()
{
	hide(); /* hides the main menu */

	/* we build a new parameter window */
	para_window = new qt_para(0, 0, YES, YES);

	/* we connect a slot to the close signal of the window, so we get to know
	 * when the other window was closed */
	QObject::connect(para_window, SIGNAL(closed()), this, SLOT(closeQt_Para()));
	para_window->show(); /* show the new window */

	return;
}
