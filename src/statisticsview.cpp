  	//setPalette(QPalette( QColor(255,255,255)));
  	//setAutoFillBackground(true);

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

//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <klocale.h>
#include <kdebug.h>

#include "settingsclass.h"
#include "statisticsbarwidget.h"

/* constructor */
StatisticsView::StatisticsView(QWidget * parent):
		QFrame(parent), m_count(0), m_correct(0)
{
#ifdef DEBUG
	kDebug() << "constructor StatisticsView()";
#endif
	// load statistics from config file
	m_count = SettingsClass::count();
	m_correct = SettingsClass::correct();
	m_skipped = SettingsClass::skipped();

	defaultFont = SettingsClass::getDefaultFont();
	defaultFont.setBold( TRUE );
	defaultFont.setPointSize(28);

	QPalette pal;

	/* create a grid to show the labels */
	labelGrid = new QGridLayout();
	setLayout(labelGrid);

	labelGrid->setColumnStretch(0,1);
	labelGrid->setColumnStretch(5,1);

	labelGrid->setColumnMinimumWidth(4,220);

	result1Label = new QLabel(this);
	labelGrid->addWidget(result1Label, 1, 1, 2, 1);
	result1Label->setFont(defaultFont);
	result1Label->setAlignment(Qt::AlignCenter);
	result1Label->setToolTip(
	              i18n("This is the current total number of solved tasks."));

	defaultFont.setBold( FALSE );
	defaultFont.setPointSize(10);

	info1Label = new QLabel(this);
	info1Label->setText(i18n("Questions:"));
	info1Label->setFont(defaultFont);
	info1Label->setAlignment(Qt::AlignCenter);
	labelGrid->addWidget(info1Label, 0, 1);
  	
	info2Label = new QLabel(this);
	info2Label->setText(i18nc("@info:status the number of correct answers", "Correct:"));
	info2Label->setFont(defaultFont);
	info2Label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	labelGrid->addWidget(info2Label, 0, 2);	

	info4Label = new QLabel(this);
	info4Label->setText(i18nc("@info:status the number of incorrect answers", "Incorrect:"));
	info4Label->setFont(defaultFont);
	info4Label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	labelGrid->addWidget(info4Label, 1, 2);

	info3Label = new QLabel(this);
	info3Label->setText(i18nc("@info:status the number of skipped answers", "Skipped:"));
	info3Label->setFont(defaultFont);
	info3Label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	labelGrid->addWidget(info3Label, 2, 2);

	defaultFont.setBold( TRUE );

	result2Label = new QLabel(this);
	labelGrid->addWidget(result2Label, 0, 3);
	result2Label->setFont(defaultFont);
	result2Label->setToolTip(
	              i18n("This is the current total number of correctly solved tasks."));
	
	result4Label = new QLabel(this);
	labelGrid->addWidget(result4Label, 1, 3);
	result4Label->setFont(defaultFont);
	result4Label->setToolTip(
	              i18n("This is the current total number of unsolved tasks."));

	result3Label = new QLabel(this);
	labelGrid->addWidget(result3Label, 2, 3);
	result3Label->setFont(defaultFont);
	result3Label->setToolTip(
	              i18n("This is the current total number of skipped tasks."));

	// add tooltip and qwhatsthis help to the widget
	setToolTip(i18n("This part of the window shows the statistics."));
	setWhatsThis( i18n("This part of the window shows the statistics.  Each exercise you do is counted. You can reset the statistics by clicking on the button below. Also, if you do not want to see the statistics, use the vertical bar on the left to reduce the size of this window part."));

	QBoxLayout * cLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);

	statisticsBar = new StatisticsBarWidget(this);
	labelGrid->addWidget(statisticsBar, 0, 4, 3, 1);
	labelGrid->addLayout(cLayout,0,4,Qt::AlignCenter);

	/* calculate the statistics */
	(void) calc();
}

/* destructor */
StatisticsView::~StatisticsView()
{
#ifdef DEBUG
	kDebug() << "destructor StatisticsView()";
#endif
	// save statistics for next run
	SettingsClass::setCount(m_count);
	SettingsClass::setCorrect(m_correct);
	SettingsClass::setSkipped(m_skipped);
	SettingsClass::self()->writeConfig();

	/* no need to delete any child widgets, Qt does it by itself */
}

/* called, if a task solved correctly */
void StatisticsView::addCorrect()
{
	++m_count;
	++m_correct;
	(void) calc(); /* repaint the statistics */
}

/* called, if a task is skipped */
void StatisticsView::addSkipped()
{
	++m_count;
	++m_skipped;
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
	statisticsBar->updateBar(m_correct, m_skipped, m_count);

	QString new_text;
	QString number;

	new_text = QString("<b>%1</b>").arg(m_count);
	result1Label->setText(new_text);

	/* we have to be careful with division by 0 */
	if (m_count == 0)
	{
		result2Label->setText("0 (0 %)");
		result3Label->setText("0 (0 %)");
		result4Label->setText("0 (0 %)");
	} else {
		/* set the correct label */
		new_text = QString("%1").arg(m_correct);
		result2Label->setText(new_text);

		/* set the correct label */
		new_text = QString("%1").arg(m_skipped);
		result3Label->setText(new_text);

		/* set the incorrect label */
		new_text = QString("%1").arg(m_count - m_correct - m_skipped);
		result4Label->setText(new_text);
	}
}

/* ------ private slots ------ */

/* called by the reset button */
void StatisticsView::resetStatistics()
{
	m_count = 0;
	m_correct = 0;
	m_skipped = 0;
	(void) calc();
}
