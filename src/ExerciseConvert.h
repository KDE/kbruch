/*
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXERCISECONVERT_H
#define EXERCISECONVERT_H

#include "ExerciseBase.h"
#include "Ratio.h"

#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>

class RationalWidget;
class ResultWidget;

class QLineEdit;
class QPushButton;
class QString;

/*! Construct the exercise widget to convert rational numbers into fractions
 *
 *  \author Sebastian Stein
 * */
class ExerciseConvert : public ExerciseBase
{
    Q_OBJECT

public:
    /** constructor */
    explicit ExerciseConvert(QWidget * parent = nullptr);

    /** destructor */
    ~ExerciseConvert();

    /** force the creation of a new task */
    void forceNewTask() Q_DECL_OVERRIDE;

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

    QString m_number;
    uint m_periodStart;
    uint m_periodLength;
    Ratio m_result;

    // Visible components
    RationalWidget * m_rationalWidget;
    ResultWidget * m_resultWidget;
    QLineEdit * numer_edit;
    QFrame * edit_line;
    QLineEdit * deno_edit;

    QPushButton* m_checkButton;
    QPushButton* m_skipButton;

    // Layout Structures
    QGridLayout* checkLayout;
    QGridLayout* taskLayout;
    QGridLayout* baseGrid;

    QWidget * checkWidget;
    QWidget * taskWidget;

    // Operation functions
    void createTask();
    void showResult();
    void nextTask();

    void slotCheckButtonClicked();
    void slotSkipButtonClicked();
    void numeratorReturnPressed();
    void denominatorReturnPressed();

protected:
    void showEvent(QShowEvent * event) Q_DECL_OVERRIDE;
};

#endif

