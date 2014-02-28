/*
    KBruch - exercise to convert mixed numbers in ratios and vice versa
    Copyright 2011 - Sebastian Stein <seb.kde@hpfsc.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef EXERCISEMIXEDNUMBERS_H
#define EXERCISEMIXEDNUMBERS_H

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "ExerciseBase.h"
#include "ResultWidget.h"
#include "Task.h"
#include "TaskWidget.h"


class KLineEdit;
class QFrame;
class QGridLayout;
class QPushButton;
class QWidget;


/**
 * exercise to convert mixed numbers in ordinary ratios and vice versa
 * \author Sebastian Stein
 */
class ExerciseMixedNumbers: public ExerciseBase
{
    Q_OBJECT

public:
    /** constructor */
    explicit ExerciseMixedNumbers(QWidget* parent = 0);

    /** destructor */
    virtual ~ExerciseMixedNumbers();

    /** resets the current state, creates a new task and counts the last task
     * as wrong, if it wasn't solved (in _NEXT_TASK state) yet mainly used
     * after changing the task parameters */
    void forceNewTask();

signals:
    /** signal emitted if task solved correctly */
    void signalExerciseSolvedCorrect();

    /** signal emitted if task skipped */
    void signalExerciseSkipped();

    /** signal emitted if task solved not correctly */
    void signalExerciseSolvedWrong();

private:
    /** holds current state of this exercise */
    short m_currentState;

    /** true if task is a mixed number and user must enter ratio */
    bool m_isMixedTask;

    /** task currently to be solved */
    Task m_task;

    // components to enter result
    KLineEdit * m_integerEdit;
    KLineEdit * m_numerEdit;
    QFrame * m_editLine;
    KLineEdit * m_denoEdit;

    // check and skip buttons
    QPushButton * m_checkButton;
    QPushButton * m_skipButton;

    // base grid layout
    QGridLayout * m_baseGrid;
    QWidget * m_checkWidget;
    QWidget * m_tmpTaskWidget;

    // layout to show task and input fields
    QGridLayout * m_taskLayout;
    TaskWidget * m_taskWidget;

    // layout to show solution
    QGridLayout * m_checkLayout;
    ResultWidget * m_resultWidget;

    /** creates a new task */
    void createTask();

    /** shows next task; hides previous solution */
    void nextTask();

    /** checks result and shows solution */
    void showResult();

protected:
    /** sets focus to input fields */
    void showEvent(QShowEvent * event);

private slots:
    /** handle check button click */
    void slotCheckButtonClicked();

    /** handle skip button click */
    void slotSkipButtonClicked();

    /** focus set to numerator edit if return pressed in integer edit */
    void integerReturnPressed(const QString &);

    /** focus set to denominator edit if return pressed in numerator edit */
    void numerReturnPressed(const QString &);

    /** invokes check button if return pressed in denominator edit */
    void denoReturnPressed(const QString &);
};

#endif // EXERCISEMIXEDNUMBERS_H
