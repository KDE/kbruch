/***************************************************************************
                          gui_qt.cpp  -  Header File
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

#ifndef GUI_QT_H
#define GUI_QT_H

#include "kbruch.h"
#include "qt_para.h"

/* this includes are needed for Qt support */
#include <kapp.h>
#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qfont.h>
#include <qvbox.h>
#include <klocale.h>

#ifdef DEBUG
#include <iostream.h>
#endif

#define YES 1
#define NO  0

class qt_para;

class gui_qt : public QVBox
{
	Q_OBJECT

	public:
		gui_qt(QWidget * parent = 0, const char * name = 0); /* constructor */
		~gui_qt(); /* destructor */
	private slots:
		void showQt_Para_addsub();
		void showQt_Para_muldiv();
		void showQt_Para_all();
		void closeQt_Para();
	private:
		QPushButton * addsubBtn;
		QPushButton * muldivBtn;
		QPushButton * allBtn;
		QPushButton * helpBtn;
		QPushButton * quitBtn;
		qt_para * para_window;

		QWidget * pparent;
		const char * pname;
};

#endif
