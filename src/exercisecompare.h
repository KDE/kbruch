/***************************************************************************
                              exercisecompare.h
                             -------------------
    begin                : 2004/06/03
    copyright            : (C) 2004 by Sebastian Stein
    email                : seb.kde@hpfsc.de
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

#include "exercisebase.h"
#include "ratio.h"
#include "ratiowidget.h"

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
	ExerciseCompare(QWidget * parent = 0, const char * name = 0);

	/** destructor */
	~ExerciseCompare();

	/** force the creation of a new task */
	void forceNewTask();

public slots:
	void update();

signals:
	/** class emits this signal, if the task was solved correctly by the user */
	void signalExerciseSolvedCorrect();

	/** class emits this signal, if the task was solved not correctly by the user
	 * */
	void signalExerciseSolvedWrong();

private:
	short m_currentState;

	RatioWidget* m_firstRatioWidget;
	RatioWidget* m_secondRatioWidget;
	QPushButton* m_checkButton;
	QPushButton* m_signButton;
	QLabel* result_label;

	QGridLayout* baseGrid;
	QWidget* baseWidget;
	QVBoxLayout* realLayout;

	ratio m_firstRatio;
	ratio m_secondRatio;

	enum SignButtonState
	{
		lessThen,
		greaterThen
	};
	SignButtonState m_signButtonState;

	void createTask();
	void showResult();
	void nextTask();

private slots:
	void slotCheckButtonClicked();
	void slotSignButtonClicked();
};

#endif
