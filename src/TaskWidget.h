/*
    TaskWidget.h  -  paint a task
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    Q_OBJECT
public:
    /** constructor */
    explicit TaskWidget(QWidget * parent = nullptr, const Task &para_task = Task());

    /** destructor */
    ~TaskWidget();

    /** set the task to be displayed */
    void setTask(const Task &para_task);

    void setQuestionMixed(bool value);

private:

    /** the task to be displayed */
    Task m_task;

    bool m_questionMixed;

    /** overriding the paint event of FractionBaseWidget */
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
};

#endif
