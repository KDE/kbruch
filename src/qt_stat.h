/***************************************************************************
                          qt_stat.cpp  -  Header File
                             -------------------
    begin                : Tue Mar 08 17:20:00 CET 2002
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

#ifndef QT_STAT_H
#define QT_STAT_H

/* the includes are needed for Qt support */
#include <kapplication.h>
#include <qdialog.h>
#include <qfont.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qvbox.h>
#include <klocale.h>
#include <qtooltip.h>

#ifdef DEBUG
#include <iostream.h>
#endif

class qt_stat : public QDialog
{
	Q_OBJECT

	public:
		qt_stat(QWidget * parent = 0, const char * name = 0);
		~qt_stat();
		void addCorrect();
		void addWrong();
	private:
		void calc();

		unsigned int count;
		unsigned int correct;

		QPushButton * resetBtn;
		QVBoxLayout * layout1;
		QGridLayout * labelGrid;
		QLabel * result1Label;
		QLabel * result2Label;
		QLabel * result3Label;
		QLabel * info1Label;
		QLabel * info2Label;
		QLabel * info3Label;
	private slots:
		void reset_stat();
};

#endif
