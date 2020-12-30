/*
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef EXERCISEFACTORIZE_H
#define EXERCISEFACTORIZE_H

#include <QList>
#include <QVariant>
#include <QWidget>

#include "ExerciseBase.h"
#include "ResultWidget.h"
#include "Ratio.h"
#include "PrimeFactorsLineEdit.h"

class ResultWidget;
class QGridLayout;
class QLabel;
class QPushButton;

// a list containing uints
typedef QList<uint> uintList;

// set a macro how much numbers are given to factorize
#define numberPossibleTasks 45

// set all possible numbers to factorize
const uint possibleTasks[numberPossibleTasks] = {4, 6, 8, 9, 10, 12, 14, 15, 16,
        18, 20, 21, 22, 24, 25, 26, 27, 30, 32, 33, 34, 35, 38, 39, 49, 50, 51, 54, 55,
        57, 60, 65, 70, 77, 75, 85, 95, 98, 121, 125, 169, 242, 250, 289, 361
                                                };

/*! Construct the exercise widget to factorize a given number
 *
 *  \author Sebastian Stein
 * */
class ExerciseFactorize : public ExerciseBase
{
    Q_OBJECT

public:
    /** constructor */
    explicit ExerciseFactorize(QWidget * parent = nullptr);

    /** destructor */
    ~ExerciseFactorize();

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

    bool m_edit;

    // Get the last number typped by keyboard for keyboard handler
    uint m_buffer;

    uint m_taskNumber;
    uintList m_factorsEntered;
    uintList m_factorsResult;

    QLabel * m_taskLabel;
    QLabel * m_equalSignLabel;
    PrimeFactorsLineEdit * m_factorsEnteredEdit;

    // buttons for the different prime factors
    QPushButton * m_factor2Button;
    QPushButton * m_factor3Button;
    QPushButton * m_factor5Button;
    QPushButton * m_factor7Button;
    QPushButton * m_factor11Button;
    QPushButton * m_factor13Button;
    QPushButton * m_factor17Button;
    QPushButton * m_factor19Button;

    // button to remove the last entered factor
    QPushButton * m_removeLastFactorButton;
    QPushButton * m_checkButton;
    QPushButton * m_skipButton;

    QGridLayout * checkLayout;
    QGridLayout * taskLayout;
    QGridLayout * baseGrid;

    QWidget * checkWidget;
    QWidget * taskWidget;

    ResultWidget * m_resultWidget;

    void createTask();
    void showResult();
    void nextTask();

    void addFactor(uint factor);
    void updateEnteredEdit();

    void slotFactorsEditReturnPressed();

    void slotCheckButtonClicked();
    void slotSkipButtonClicked();

    void slotFactor2ButtonClicked();
    void slotFactor3ButtonClicked();
    void slotFactor5ButtonClicked();
    void slotFactor7ButtonClicked();
    void slotFactor11ButtonClicked();
    void slotFactor13ButtonClicked();
    void slotFactor17ButtonClicked();
    void slotFactor19ButtonClicked();
    void slotRemoveLastFactorButtonClicked();

    void editContentChanged(bool correct);
    void setButtonsEnabled(bool enabled);

    protected:
    void showEvent(QShowEvent * event) Q_DECL_OVERRIDE;
};

#endif
