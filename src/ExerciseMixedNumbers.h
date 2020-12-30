/*
    KBruch - exercise to convert mixed numbers in ratios and vice versa
    SPDX-FileCopyrightText: 2011 - Sebastian Stein <seb.kde@hpfsc.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXERCISEMIXEDNUMBERS_H
#define EXERCISEMIXEDNUMBERS_H

#include "ExerciseBase.h"
#include "ResultWidget.h"
#include "Task.h"
#include "TaskWidget.h"


class QLineEdit;
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
    explicit ExerciseMixedNumbers(QWidget* parent = nullptr);

    /** destructor */
    virtual ~ExerciseMixedNumbers();

    /** resets the current state, creates a new task and counts the last task
     * as wrong, if it wasn't solved (in _NEXT_TASK state) yet mainly used
     * after changing the task parameters */
    void forceNewTask() Q_DECL_OVERRIDE;

Q_SIGNALS:
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
    QLineEdit * m_integerEdit;
    QLineEdit * m_numerEdit;
    QFrame * m_editLine;
    QLineEdit * m_denoEdit;

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

    /** handle check button click */
    void slotCheckButtonClicked();

    /** handle skip button click */
    void slotSkipButtonClicked();

    /** focus set to numerator edit if return pressed in integer edit */
    void integerReturnPressed();

    /** focus set to denominator edit if return pressed in numerator edit */
    void numerReturnPressed();

    /** invokes check button if return pressed in denominator edit */
    void denoReturnPressed();

    protected:
    /** sets focus to input fields */
    void showEvent(QShowEvent * event) Q_DECL_OVERRIDE;
};

#endif // EXERCISEMIXEDNUMBERS_H
