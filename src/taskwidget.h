/***************************************************************************
                          taskwidget.h  -  paint a task
                             -------------------
    begin                : 2004/05/30
    copyright            : (C) 2004 by Sebastian Stein
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

#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "fractionbasewidget.h"
#include "task.h"

#include <qwidget.h>

/** important for add_sub and mul_div */
#define YES 1
#define NO  0

/** important for op_vector */
#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3

/*! class to paint task with fractions
 *
 *  \author Sebastian Stein */
class TaskWidget : public FractionBaseWidget
{
	Q_OBJECT

public:
	/** constructor */
	TaskWidget(QWidget * parent, const char * name, const task para_task);

	/** destructor */
	~TaskWidget();

	/** set the task to be displayed */
	void setTask(const task para_task);

private:
	
	/** the task to be displayed */
	task m_task;

	/** overrideing the paint event of FractionBaseWidget */
	void paintEvent(QPaintEvent*);
};

#endif
