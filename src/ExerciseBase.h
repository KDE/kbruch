/*
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXERCISEBASE_H
#define EXERCISEBASE_H

#define _CHECK_TASK 0
#define _NEXT_TASK  1

#include <QWidget>

/*! Constructs a QWidget.
 *
 * It is the base class for showing the different exercises.
 *
 *  \author Sebastian Stein
 * */
class ExerciseBase : public QWidget
{
    Q_OBJECT
public:
    /** constructor */
    explicit ExerciseBase(QWidget * parent = nullptr);

    /** destructor */
    ~ExerciseBase();

    /** force the creation of a new task */
    virtual void forceNewTask() = 0;
};

#endif
