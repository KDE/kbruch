/***************************************************************************
                          statisticsview.cpp  -  Header File
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

#ifndef STATISTICSVIEW_H
#define STATISTICSVIEW_H

class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

#include <qwidget.h>

/*!
  * StatisticsView takes care of the statistics of a test.
  * It saves the number of correct and wrong answers and
  * displays this data to the user.
  * \author Sebastian Stein
  * \author Eva Brucherseifer
  */

class StatisticsView : public QWidget
{
	Q_OBJECT

public:
	/** constructor */
	StatisticsView(QWidget * parent = 0, const char * name = 0);

	/** destructor */
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
	/** calculate percentages and update view */
	void calc();

	unsigned int m_count;
	unsigned int m_correct;

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
