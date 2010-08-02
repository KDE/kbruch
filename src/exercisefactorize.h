/***************************************************************************
                              exercisefactorize.h
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

#ifndef EXERCISEFACTORIZE_H
#define EXERCISEFACTORIZE_H

#define DEBUG

#ifdef DEBUG
#include <kdebug.h>
#endif

#include <tqvaluelist.h>
#include <tqvariant.h>
#include <tqwidget.h>

#include "exercisebase.h"
#include "ratio.h"

class ResultWidget;
class FactorizedWidget;

class TQVBoxLayout;
class TQHBoxLayout;
class TQGridLayout;
class TQSpacerItem;
class TQLabel;
class TQLineEdit;
class TQPushButton;

// a list containing uints
typedef TQValueList<uint> uintList;

// set a macro how much numbers are given to factorize
#define numberPossibleTasks 45

// set all possible numbers to factorize
const uint possibleTasks[numberPossibleTasks] = {4, 6, 8, 9, 10, 12, 14, 15, 16,
18, 20, 21, 22, 24, 25, 26, 27, 30, 32, 33, 34, 35, 38, 39, 49, 50, 51, 54, 55,
57, 60, 65, 70, 77, 75, 85, 95, 98, 121, 125, 169, 242, 250, 289, 361};

/*! Construct the exercise widget to factorize a given number
 *
 *  \author Sebastian Stein
 * */
class ExerciseFactorize : public ExerciseBase
{
	Q_OBJECT

public:
	/** constructor */
	ExerciseFactorize(TQWidget * parent = 0, const char * name = 0);

	/** destructor */
	~ExerciseFactorize();


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

	uint m_taskNumber;
	uintList m_factorsEntered;
	uintList m_factorsResult;

	TQLabel* m_taskLabel;
	TQLabel* m_equalSignLabel;
	TQLineEdit* m_factorsEnteredEdit;
	FactorizedWidget* m_factorsWidget;
	TQLabel* result_label;

	

	// buttons for the different prime factors
	TQPushButton* m_factor2Button;
	TQPushButton* m_factor3Button;
	TQPushButton* m_factor5Button;
	TQPushButton* m_factor7Button;
	TQPushButton* m_factor11Button;
	TQPushButton* m_factor13Button;
	TQPushButton* m_factor17Button;
	TQPushButton* m_factor19Button;

	// button to remove the last entered factor
	TQPushButton* m_removeLastFactorButton;
	TQPushButton* m_checkButton;


	//
	// the following two methods were added to fix
	// bug #116831 (reverse layout in RTL desktops)
	// Amit Ramon amit.ramon@kdemail.net
	//

	/** create the factor widgets layout */
	TQHBoxLayout* createFactorsLayout();

	/** create the (answer) buttons layout */
	TQGridLayout* createButtonsLayout();
	

	void createTask();
	void showResult();
	void nextTask();

	void addFactor(uint factor);
	void updateEnteredEdit();

protected:
	TQVBoxLayout* Form1Layout;
	TQVBoxLayout* layout9;
	TQSpacerItem* spacer4;
	TQHBoxLayout* layout4;
	TQSpacerItem* spacer1;
	TQSpacerItem* spacer2;
	TQVBoxLayout* layout2;
	TQGridLayout* layout1;
	TQHBoxLayout* layout7;
	TQSpacerItem* spacer3;

private slots:
	void slotCheckButtonClicked();

	void slotFactor2ButtonClicked();
	void slotFactor3ButtonClicked();
	void slotFactor5ButtonClicked();
	void slotFactor7ButtonClicked();
	void slotFactor11ButtonClicked();
	void slotFactor13ButtonClicked();
	void slotFactor17ButtonClicked();
	void slotFactor19ButtonClicked();
	void slotRemoveLastFactorButtonClicked();
};

#endif
