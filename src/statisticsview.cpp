/***************************************************************************
                          statisticsview.cpp  -  the statistic window
                             -------------------
    begin                : Tue Mar 08 17:20:00 CET 2002
    copyright            : (C) 2001 - 2004 by Sebastian Stein, Eva Brucherseifer
    email                : seb.kde@hpfsc.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "statisticsview.h"
#include "statisticsview.moc"

/* the includes are needed for Qt support */
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include <klocale.h>
#include <kdebug.h>

#include "settingsclass.h"

/* constructor */
StatisticsView::StatisticsView(QWidget * parent, const char * name):
		QWidget(parent, name), m_count(0), m_correct(0)
{
#ifdef DEBUG
	kdDebug() << "constructor StatisticsView()" << endl;
#endif

	// load statistics from config file
	m_count = SettingsClass::count();
	m_correct = SettingsClass::correct();

	QPalette pal;
	QColorGroup cg;

	/* set the caption of the window */
	//	setCaption(i18n("Statistics"));

	/* add a layout as a base */
	layout1 = new QVBoxLayout(this);
	layout1->setSpacing(6);
	layout1->setMargin(6);

	/* now add a v-spacer */
	QSpacerItem * v_spacer = new QSpacerItem(1, 1);
	layout1->addItem(v_spacer);

	/* create a grid to show the labels */
	labelGrid = new QGridLayout(layout1, 3, 2);

	/* add 6 labels to the grid */
	info1Label = new QLabel(this);
	info1Label->setText(i18n("Tasks so far:"));
	labelGrid->addWidget(info1Label, 1, 0);

	result1Label = new QLabel(this);
	labelGrid->addWidget(result1Label, 1, 1);
	QToolTip::add(result1Label,
	              i18n("This is the current total number of solved tasks."));

	info2Label = new QLabel(this);
	info2Label->setText(i18n("Correct:"));
	labelGrid->addWidget(info2Label, 2, 0);

	result2Label = new QLabel(this);

	/* set green text color for this label */
	pal = result2Label->palette();
	cg = pal.active();
	cg.setColor(QColorGroup::Foreground, QColor(6, 179, 0));
	pal.setActive(cg);
	cg = pal.inactive();
	cg.setColor(QColorGroup::Foreground, QColor(6, 179, 0));
	pal.setInactive(cg);
	result2Label->setPalette(pal);

	labelGrid->addWidget(result2Label, 2, 1);
	QToolTip::add(result2Label,
	              i18n("This is the current total number of correctly solved tasks."));

	info3Label = new QLabel(this);
	info3Label->setText(i18n("Incorrect:"));
	labelGrid->addWidget(info3Label, 3, 0);

	result3Label = new QLabel(this);

	/* set red text color for this label */
	pal = result3Label->palette();
	cg = pal.active();
	cg.setColor(QColorGroup::Foreground, QColor(red));
	pal.setActive(cg);
	cg = pal.inactive();
	cg.setColor(QColorGroup::Foreground, QColor(red));
	pal.setInactive(cg);
	result3Label->setPalette(pal);

	labelGrid->addWidget(result3Label, 3, 1);
	QToolTip::add(result3Label,
	              i18n("This is the current total number of unsolved tasks."));

	/* now add a v-spacer */
	v_spacer = new QSpacerItem(1, 1);
	layout1->addItem(v_spacer);

	/* the Reset button */
	buttonLayout = new QHBoxLayout(layout1);
	resetBtn = new QPushButton(i18n("&Reset"), this);
	QObject::connect(resetBtn, SIGNAL(clicked()), this, SLOT(resetStatistics()));
	buttonLayout->addWidget(resetBtn);
	QToolTip::add(resetBtn, i18n("Press the button to reset the statistics."));
	QSpacerItem* spacer = new QSpacerItem(0,0);
	buttonLayout->addItem(spacer);

	/* calculate the statistics */
	(void) calc();

	// add tooltip and qwhatsthis help to the widget
	QToolTip::add(this, i18n("This part of the window shows the statistics."));
	QWhatsThis::add(this, i18n("This part of the window shows the statistics.  Each exercise you do is counted. You can reset the statistics by clicking on the button below. Also, if you do not want to see the statistics, use the vertical bar on the left to reduce the size of this window part."));
}

/* destructor */
StatisticsView::~StatisticsView()
{
#ifdef DEBUG
	kdDebug() << "destructor StatisticsView()" << endl;
#endif
	// save statistics for next run
	SettingsClass::setCount(m_count);
	SettingsClass::setCorrect(m_correct);
	SettingsClass::writeConfig();

	/* no need to delete any child widgets, Qt does it by itself */
}

/* called, if a task solved correctly */
void StatisticsView::addCorrect()
{
	++m_count;
	++m_correct;
	(void) calc(); /* repaint the statistics */
}

/* called, if a task was solved wrong */
void StatisticsView::addWrong()
{
	++m_count;
	(void) calc(); /* repaint the statistics */
}


/* ------ private member functions ------ */

/* recalculates the statistics and changes the corresponding labels */
void StatisticsView::calc()
{
	QString new_text;
	QString number;

	new_text = QString("<b>%1</b>").arg(m_count);
	result1Label->setText(new_text);

	/* we have to be careful with division by 0 */
	if (m_count == 0)
	{
		result2Label->setText("- (- %)");
		result3Label->setText("- (- %)");
	} else {
		/* set the correct label */
		new_text = QString("%1 (%2 %)").arg(m_correct).arg(int(double(m_correct) / m_count * 100));
		result2Label->setText(new_text);

		/* set the incorrect label */
		new_text = QString("%1 (%2 %)").arg(m_count - m_correct).arg(int(double(m_count - m_correct) / m_count * 100));
		result3Label->setText(new_text);
	}
}

/* ------ private slots ------ */

/* called by the reset button */
void StatisticsView::resetStatistics()
{
	m_count = 0;
	m_correct = 0;
	(void) calc();
}
