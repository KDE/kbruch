/***************************************************************************
                          qt_para.cpp  -  Header File
                             -------------------
    begin                : Tue Jan 31 17:25:00 CET 2002
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

#ifndef QT_PARA_H
#define QT_PARA_H

#include "tool.h"
#include "qt_task.h"

/* this includes are needed for Qt support */
#include <qabstractlayout.h>
#include <kapplication.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <qfont.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qvbox.h>
#include <klocale.h>
#include <qtooltip.h>

#ifdef DEBUG
#include <iostream.h>
#endif

#define YES 1
#define NO  0

/* number of items stored in the denoComBox */
#define NR_DENO_VALUES 6

class qt_task;

class qt_para : public QDialog
{
	Q_OBJECT

	public:
		qt_para(QWidget * parent = 0, const char * name = 0,
			short padd_sub = YES, short pmul_div = NO);
		~qt_para(); /* destructor */
	signals:
		void closed();
	protected:
		virtual void closeEvent(QCloseEvent * ev_close);
	private:
		short add_sub;
		short mul_div;
		qt_task * task_window;

		void setdenoComBox(int index = 0);

		QLabel * denoLabel;
		QLabel * termLabel;
		QPushButton * backBtn;
		QPushButton * helpBtn;
		QPushButton * okBtn;
		QComboBox * denoComBox;
		QComboBox * termComBox;
		QVBoxLayout * layout1;
		QBoxLayout * termLabelHBox;
		QBoxLayout * termComHBox;
		QBoxLayout * denoLabelHBox;
		QBoxLayout * denoComHBox;
		QBoxLayout * buttonHBox;

		QWidget * pparent;
		const char * pname;
	private slots:
		void term_nr_changed(int index);
		void showQt_Task();
		void closeQt_Task();
};

#endif
