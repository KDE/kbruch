/*
    SPDX-FileCopyrightText: 2001-2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TASKVIEW_H
#define TASKVIEW_H

#define _CHECK_TASK 0
#define _NEXT_TASK  1

#include "ExerciseBase.h"
#include "ResultWidget.h"
#include "Task.h"
#include "TaskWidget.h"

#include <QLabel>
#include <QWidget>


class QGridLayout;
class QPushButton;

class QLineEdit;

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
    explicit TaskView(QWidget * parent = nullptr,
                      bool padd_add = true, bool padd_div = false,
                      bool padd_mult = false, bool padd_sub = false,
                      unsigned int pnr_ratios = 2, unsigned int pmax_md = 10);

    /** destructor */
    ~TaskView();

    /** set new task parameters, which will be used for the next task to be
     * generated */
    void setTaskParameters(bool padd_add = true, bool padd_div = false,
                           bool padd_mult = false, bool padd_sub = false,
                           unsigned int pnr_ratios = 2, unsigned int pmax_md = 2);

    /** force the creation of a new task */
    void forceNewTask() Q_DECL_OVERRIDE;

    void setReducedForm(bool value);
    void setQuestionMixed(bool value);
    void setAnswerMixed(bool value);

Q_SIGNALS:
    /** class emits this signal, if the task was solved correctly by the user */
    void signalTaskSolvedCorrect();

    /** class emits this signal, if the task was skipped by the user */
    void signalTaskSkipped();

    /** class emits this signal, if the task was solved not correctly by the user
     * */
    void signalTaskSolvedWrong();

private:
    bool m_questionMixed;
    bool m_answerMixed;
    bool m_solutionMixed;
    bool m_reducedForm;
    bool m_addAdd;
    bool m_addDiv;
    bool m_addMult;
    bool m_addSub;
    unsigned int nr_ratios;
    unsigned int curr_nr_ratios;
    unsigned int max_md;
    short m_currentState;

    ResultWidget* m_resultWidget;

    QPushButton* m_checkButton;
    QPushButton* m_skipButton;

    TaskWidget* m_taskWidget;
    QLineEdit* numer_edit;
    QFrame* edit_line;
    QLineEdit* deno_edit;
    QLineEdit* integer_edit;

    QGridLayout* checkLayout;
    QGridLayout* taskLayout;
    QGridLayout* baseGrid;

    QWidget * checkWidget;
    QWidget * taskWidget;

    Task current_task;
    Ratio solution;
    Ratio entered_result;
    QFont defaultFont;

    void showResult();
    void nextTask();

    void slotCheckButtonClicked();
    void slotSkipButtonClicked();
    void numeratorReturnPressed();
    void integerReturnPressed();
    void denominatorReturnPressed();

protected:
    void showEvent(QShowEvent * event) Q_DECL_OVERRIDE;
};

#endif
