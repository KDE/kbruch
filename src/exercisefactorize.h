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

#include <qvaluelist.h>
#include <qvariant.h>
#include <qwidget.h>

#include "exercisebase.h"
#include "ratio.h"

class ResultWidget;
class FactorizedWidget;

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QPushButton;

// a list containing uints
typedef QValueList<uint> uintList;

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
	ExerciseFactorize(QWidget * parent = 0, const char * name = 0);

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

	QLabel* m_taskLabel;
	QLabel* m_equalSignLabel;
	QLineEdit* m_factorsEnteredEdit;
	FactorizedWidget* m_factorsWidget;
	QLabel* result_label;

	

	// buttons for the different prime factors
	QPushButton* m_factor2Button;
	QPushButton* m_factor3Button;
	QPushButton* m_factor5Button;
	QPushButton* m_factor7Button;
	QPushButton* m_factor11Button;
	QPushButton* m_factor13Button;
	QPushButton* m_factor17Button;
	QPushButton* m_factor19Button;

	// button to remove the last entered factor
	QPushButton* m_removeLastFactorButton;
	QPushButton* m_checkButton;


	//
	// the following two methods were added to fix
	// bug #116831 (reverse layout in RTL desktops)
	// Amit Ramon amit.ramon@kdemail.net
	//

	/** create the factor widgets layout */
	QHBoxLayout* createFactorsLayout();

	/** create the (answer) buttons layout */
	QGridLayout* createButtonsLayout();
	

	void createTask();
	void showResult();
	void nextTask();

	void addFactor(uint factor);
	void updateEnteredEdit();

protected:
	QVBoxLayout* Form1Layout;
	QVBoxLayout* layout9;
	QSpacerItem* spacer4;
	QHBoxLayout* layout4;
	QSpacerItem* spacer1;
	QSpacerItem* spacer2;
	QVBoxLayout* layout2;
	QGridLayout* layout1;
	QHBoxLayout* layout7;
	QSpacerItem* spacer3;

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
