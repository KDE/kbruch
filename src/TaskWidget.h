/***************************************************************************
                          TaskWidget.h  -  paint a task
                             -------------------
    begin                : 2004/05/30
    copyright            : (C) 2004 by Sebastian Stein
    email                : seb.kde@hpfsc.de

    copyright            : (C) 2008 by Tadeu Araujo, tadeu.araujo@ltia.fc.unesp.br
    copyright            : (C) 2008 by Danilo Balzaque, danilo.balzaque@ltia.fc.unesp.br
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

#include "FractionBaseWidget.h"
#include "Task.h"

#include <QWidget>

class QPaintEvent;

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
public:
    /** constructor */
    explicit TaskWidget(QWidget * parent = 0, const Task para_task = Task());

    /** destructor */
    ~TaskWidget();

    /** set the task to be displayed */
    void setTask(const Task para_task);

    void setQuestionMixed(bool value);

private:

    /** the task to be displayed */
    Task m_task;

    bool m_questionMixed;

    /** overrideing the paint event of FractionBaseWidget */
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
};

#endif
