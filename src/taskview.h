/***************************************************************************
                          taskview.cpp  -  Header File
                             -------------------
    begin                : Tue Feb 08 13:42:00 CET 2002
    copyright            : (C) 2001 - 2003 by Sebastian Stein
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

#ifndef TASKVIEW_H
#define TASKVIEW_H

#define _CHECK_TASK	0
#define _NEXT_TASK	1

#include "task.h"

#include <qwidget.h>

class QLabel;
class QFrame;
class QGridLayout;
class QHBoxLayout;
class QLineEdit;
class QSpacerItem;
class QPushButton;
class QVBoxLayout;

/*! Constructs a QWidget, which shows the task to the user.
 *  The class also provides input fields, so that the user can enter the result.
 *  It also controls the interaction, so that the entered result gets checked
 *  and a new task can be generated.
 *  \author Sebastian Stein
 * */
class TaskView : public QWidget
{
	Q_OBJECT

public:
	/** constructor */
	TaskView(QWidget * parent = 0, const char * name = 0,
	         bool padd_sub = true, bool pmul_div = false,
	         unsigned int pnr_ratios = 2, unsigned int pmax_md = 10);

	/** destructor */
	~TaskView();

	/** set new task parameters, which will be used for the next task to be
	 * generated */
	void setTaskParameters(bool padd_sub, bool pmul_div, unsigned int pnr_ratios, unsigned int pmax_md);

	/** force the creation of a new task */
	void forceNewTask();

signals:
	/** class emits this signal, if the task was solved correctly by the user */
	void signalTaskSolvedCorrect();

	/** class emits this signal, if the task was solved not correctly by the user
	 * */
	void signalTaskSolvedWrong();

private:
	bool add_sub;
	bool mul_div;
	unsigned int nr_ratios;
	unsigned int curr_nr_ratios;
	unsigned int max_md;
	short m_currentState;

	QGridLayout * baseGrid;
	QWidget * baseWidget;
	QVBoxLayout * realLayout;
	QHBoxLayout * taskHBoxLayout;
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
	QPushButton * m_checkButton;

	task current_task;
	ratio result;
	ratio entered_result;

	void showResult();
	void nextTask();
	void newLayout();

private slots:
	void slotShowBook();

	void slotCheckButtonClicked();
};

#endif
