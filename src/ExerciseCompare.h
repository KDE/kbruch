/***************************************************************************
                              ExerciseCompare.h
                             -------------------
    begin                : 2004/06/03
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

#ifndef EXERCISECOMPARE_H
#define EXERCISECOMPARE_H

#define DEBUG

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "ExerciseBase.h"
#include "Ratio.h"
#include "RatioWidget.h"
#include "ResultWidget.h"
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>

class QGridLayout;
class QLabel;
class QPushButton;
class QVBoxLayout;


#include <qwidget.h>

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
    explicit ExerciseCompare(QWidget * parent = 0);

    /** destructor */
    ~ExerciseCompare();

    /** force the creation of a new task */
    void forceNewTask();

    void setQuestionMixed(bool value);

public slots:
    void update();

signals:
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

private slots:
    void slotSkipButtonClicked();
    void slotMinorButtonClicked();
    void slotMoreButtonClicked();
    void slotEqualButtonClicked();
};

#endif
