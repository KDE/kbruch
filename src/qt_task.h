/***************************************************************************
                          qt_task.cpp  -  Header File
                             -------------------
    begin                : Tue Feb 08 13:42:00 CET 2002
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

#ifndef QT_TASK_H
#define QT_TASK_H

#include "ratio.h"
#include "qt_stat.h"
#include "task.h"
#include "tool.h"

/* this includes are needed for Qt support */
#include <qabstractlayout.h>
#include <kapplication.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <qfont.h>
#include <qgrid.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <kmessagebox.h>
#include <qpushbutton.h>
#include <qvbox.h>
#include <klocale.h>
#include <qtooltip.h>

#ifdef DEBUG
#include <iostream.h>
#endif

#define YES 1
#define NO  0

class qt_task : public QDialog
{
	Q_OBJECT

	public:
		qt_task(QWidget * parent = 0, const char * name = 0,
			short padd_sub = YES, short pmul_div = NO,
				short pnr_ratios = 2, unsigned int pmax_md = 10);
		~qt_task(); /* destructor */
		void set_position();
	public slots:
		void slotShowBook();
	signals:
		void closed();
	protected:
		virtual void closeEvent(QCloseEvent * ev_close);
	private:
		short add_sub;
		short mul_div;
		short nr_ratios;
		unsigned int max_md;

		QPushButton * backBtn;
		QPushButton * helpBtn;
		QPushButton * okBtn;

		QVBoxLayout * layout1;

		QBoxLayout * buttonHBox;
		QGridLayout * taskGrid;
		QFrame * ratio_line;
		QFrame * edit_line;
		QFrame * res1_line;
		QFrame * res2_line;
		QLabel * eq1_label;
		QLabel * eq2_label;
		QLabel * eq3_label;
		QLabel * res_num1_label;
		QLabel * res_num2_label;
		QLabel * res_deno1_label;
		QLabel * res_deno2_label;
		QLabel * res_common_label;
		QLabel ** numer_lab_vek;
		QLabel ** deno_lab_vek;
		QLabel ** op_lab_vek;
		QLabel * result_label;
		QLineEdit * numer_edit;
		QLineEdit * deno_edit;
		QSpacerItem * spacer_item;
		
		task current_task;
		ratio result;
		ratio entered_result;
		short isWaitResult;

		static qt_stat * stats;
	private slots:
		void okBtnClicked();
};

#endif
