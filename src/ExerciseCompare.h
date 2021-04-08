/*
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXERCISECOMPARE_H
#define EXERCISECOMPARE_H

#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>


#include "ExerciseBase.h"
#include "Ratio.h"
#include "RatioWidget.h"
#include "ResultWidget.h"

class QPushButton;

/*! Constructs a QWidget, which shows the task to the user.
 *  The class also provides input fields, so that the user can enter the result.
 *  It also controls the interaction, so that the entered result gets checked
 *  and a new task can be generated.
 *  \author Sebastian Stein
 * */
class ExerciseCompare : public ExerciseBase
{
    Q_OBJECT

public:
    /** constructor */
    explicit ExerciseCompare(QWidget * parent = nullptr);

    /** destructor */
    ~ExerciseCompare();

    /** force the creation of a new task */
    void forceNewTask() Q_DECL_OVERRIDE;

    void setQuestionMixed(bool value);

    void update();

Q_SIGNALS:
    /** class emits this signal, if the task was solved correctly by the user */
    void signalExerciseSolvedCorrect();

    /** class emits this signal, if the task was skipped by the user */
    void signalExerciseSkipped();

    /** class emits this signal, if the task was solved not correctly by the user
     * */
    void signalExerciseSolvedWrong();

private:
    short m_currentState;

    bool m_questionMixed;

    RatioWidget * m_firstRatioWidget;
    RatioWidget * m_secondRatioWidget;

    // Buttons to select the comparison
    QPushButton * m_skipButton;
    QPushButton * m_moreButton;
    QPushButton * m_minorButton;
    QPushButton * m_equalButton;

    Ratio m_firstRatio;
    Ratio m_secondRatio;

    // Layout Structures
    QGridLayout * checkLayout;
    QGridLayout * taskLayout;
    QGridLayout * baseGrid;

    enum SignButtonState {
        lessThen,
        greaterThen,
        equalTo
    };
    SignButtonState m_signButtonState;

    QWidget * checkWidget;
    QWidget * taskWidget;

    ResultWidget* m_resultWidget;

    void createTask();
    void showResult();
    void nextTask();

    void slotSkipButtonClicked();
    void slotMinorButtonClicked();
    void slotMoreButtonClicked();
    void slotEqualButtonClicked();
};

#endif
