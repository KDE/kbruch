/***************************************************************************
                              exerciseconvert.h
                             -------------------
    begin                : 2004/06/04
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

#ifndef EXERCISECONVERT_H
#define EXERCISECONVERT_H

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "exercisebase.h"
#include "ratio.h"
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>

class RationalWidget;
class ResultWidget;

class KLineEdit;

class QGridLayout;
class QLabel;
class QPushButton;
class QString;
class QVBoxLayout;

/*! Construct the exercise widget to convert rational numbers into fractions
 *
 *  \author Sebastian Stein
 * */
class ExerciseConvert : public ExerciseBase
{
    Q_OBJECT

public:
    /** constructor */
    explicit ExerciseConvert(QWidget * parent = 0);

    /** destructor */
    ~ExerciseConvert();

    /** force the creation of a new task */
    void forceNewTask();

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

    QString m_number;
    uint m_periodStart;
    uint m_periodLength;
    ratio m_result;

    // Visible components
    RationalWidget * m_rationalWidget;
    ResultWidget * m_resultWidget;
    KLineEdit * numer_edit;
    QFrame * edit_line;
    KLineEdit * deno_edit;

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

private slots:
    void slotCheckButtonClicked();
    void slotSkipButtonClicked();
    void numeratorReturnPressed(const QString &);
    void denominatorReturnPressed(const QString &);

protected:
    void showEvent(QShowEvent * event);
};

#endif

