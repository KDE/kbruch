/***************************************************************************
                          statisticsview.cpp  -  Header File
                             -------------------
    begin                : Tue Mar 08 17:20:00 CET 2002
    copyright            : (C) 2001 - 2002 by Sebastian Stein, Eva Brucherseifer
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

#ifndef STATISTICSVIEW_H
#define STATISTICSVIEW_H

/* the includes are needed for Qt support */
#include <kapplication.h>
#include <qwidget.h>
#include <qfont.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qvbox.h>
#include <klocale.h>
#include <qtooltip.h>

#ifdef DEBUG
#include <iostream>
#endif

/**
  * StatisticsView takes care of the statistics of a test.
  * It saves the number of correct and wrong questions and
  * views its data to the user
  */
class StatisticsView : public QWidget
{
	Q_OBJECT

public:
	StatisticsView(QWidget * parent = 0, const char * name = 0);
	~StatisticsView();

public slots:
	/** increment number of correct answers */
	void addCorrect();

	/** increment number of wrong answers */
	void addWrong();

	/** set statistics to zero.
         *  Triggered by internal button or when a new test is started
         */
	void resetStatistics();
private:
	//! calculate percentages and update view
	void calc();

	unsigned int count;
	unsigned int correct;

	QPushButton * resetBtn;
	QHBoxLayout * buttonLayout;
	QVBoxLayout * layout1;
	QGridLayout * labelGrid;
	QLabel * result1Label;
	QLabel * result2Label;
	QLabel * result3Label;
	QLabel * info1Label;
	QLabel * info2Label;
	QLabel * info3Label;
};

#endif
