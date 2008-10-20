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

#include <qwidget.h>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QPushButton;

class KLineEdit;

/*! Constructs a QWidget, which shows the task to the user.
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
	explicit TaskView(QWidget * parent = 0,
	         bool padd_add = true, bool padd_div = false,
	         bool padd_mult = false, bool padd_sub = false,
	         unsigned int pnr_ratios = 2, unsigned int pmax_md = 10);

	/** destructor */
	~TaskView();

	/** set new task parameters, which will be used for the next task to be
	 * generated */
	void setTaskParameters( bool padd_add = true, bool padd_div = false, 
				bool padd_mult = false, bool padd_sub = false, 
				unsigned int pnr_ratios = 2, unsigned int pmax_md = 2);

	/** force the creation of a new task */
	void forceNewTask();

	void forceReduce(bool force);

public slots:
	void update();

signals:
	/** class emits this signal, if the task was solved correctly by the user */
	void signalTaskSolvedCorrect();

	/** class emits this signal, if the task was skipped by the user */
	void signalTaskSkipped();

	/** class emits this signal, if the task was solved not correctly by the user
	 * */
	void signalTaskSolvedWrong();

private:
	bool m_forceReduce;
	bool m_addSub;
	bool m_addAdd;
	bool m_addMult;
	bool m_addDiv;
	unsigned int nr_ratios;
	unsigned int curr_nr_ratios;
	unsigned int max_md;
	short m_currentState;

	ResultWidget* m_resultWidget;

	QPushButton* m_checkButton;
	QPushButton* m_skipButton;

	TaskWidget* m_taskWidget;
	KLineEdit* numer_edit;
	QFrame* edit_line;
	KLineEdit* deno_edit;

	QGridLayout* checkLayout;
	QGridLayout* taskLayout;
	QGridLayout* baseGrid;

	QWidget * checkWidget;
	QWidget * taskWidget;

	task current_task;
	ratio result;
	ratio entered_result;
	QFont defaultFont;

	void showResult();
	void nextTask();

private slots:
	void slotCheckButtonClicked();
	void slotSkipButtonClicked();
	void numeratorReturnPressed(const QString &);
	void denominatorReturnPressed(const QString &);

protected:
	void showEvent ( QShowEvent * event );
};

#endif
