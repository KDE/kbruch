/***************************************************************************
                              exerciseconvert.h
                             -------------------
    begin                : 2004/06/04
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

#ifndef EXERCISECONVERT_H
#define EXERCISECONVERT_H

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "exercisebase.h"
#include "ratio.h"

class RationalWidget;
class ResultWidget;

class QFrame;
class QGridLayout;
class QLabel;
class QLineEdit;
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
	ExerciseConvert(QWidget * parent = 0, const char * name = 0);

	/** destructor */
	~ExerciseConvert();

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

	QString m_number;
	uint m_periodStart;
	uint m_periodLength;
	ratio m_result;

	RationalWidget* m_rationalWidget;
	ResultWidget* m_resultWidget;
	QLineEdit* numer_edit;
	QFrame* edit_line;
	QLineEdit* deno_edit;

	QPushButton* m_checkButton;
	QLabel* result_label;

	QGridLayout* baseGrid;
	QWidget* baseWidget;
	QVBoxLayout* realLayout;

	void createTask();
	void showResult();
	void nextTask();

private slots:
	void slotCheckButtonClicked();
};

#endif
