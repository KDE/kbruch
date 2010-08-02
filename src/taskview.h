/***************************************************************************
                          taskview.cpp  -  Header File
                             -------------------
    begin                : Tue Feb 08 13:42:00 CET 2002
    copyright            : (C) 2001 - 2004 by Sebastian Stein
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

#include "exercisebase.h"
#include "resultwidget.h"
#include "task.h"
#include "taskwidget.h"

#ifdef DEBUG
#include <kdebug.h>
#endif

#include <tqwidget.h>


class TQVBoxLayout;
class TQHBoxLayout;
class TQGridLayout;
class TQSpacerItem;
class TQLabel;
class TQPushButton;
class TQLineEdit;
class TQFrame;

/*! Constructs a TQWidget, which shows the task to the user.
 *  The class also provides input fields, so that the user can enter the result.
 *  It also controls the interaction, so that the entered result gets checked
 *  and a new task can be generated.
 *  \author Sebastian Stein
 * */
class TaskView : public ExerciseBase
{
	Q_OBJECT

public:
	/** constructor */
	TaskView(TQWidget * parent = 0, const char * name = 0,
	         bool padd_sub = true, bool pmul_div = false,
	         unsigned int pnr_ratios = 2, unsigned int pmax_md = 10);

	/** destructor */
	~TaskView();

	/** set new task parameters, which will be used for the next task to be
	 * generated */
	void setTaskParameters(bool padd_sub, bool pmul_div, unsigned int pnr_ratios, unsigned int pmax_md);

	/** force the creation of a new task */
	void forceNewTask();

public slots:
	void update();

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

	ResultWidget* m_resultWidget;
	TQPushButton* m_checkButton;
	TQLabel* result_label;
	TaskWidget* m_taskWidget;
	TQLineEdit* numer_edit;
	TQFrame* edit_line;
	TQLineEdit* deno_edit;

	TQGridLayout* baseGrid;
	TQWidget* baseWidget;
	TQVBoxLayout* realLayout;

	task current_task;
	ratio result;
	ratio entered_result;

	void showResult();
	void nextTask();

private slots:
	void slotCheckButtonClicked();
};

#endif
