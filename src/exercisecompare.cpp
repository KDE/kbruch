/***************************************************************************
                             exercisecompare.cpp
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

#include "exercisecompare.h"
#include "exercisecompare.moc"

/* these includes are needed for KDE support */
#include <kapplication.h>
#include <klocale.h>

/* these includes are needed for Qt support */
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtooltip.h>

/* standard C++ library includes */
#include <stdlib.h>
#include <time.h>

/* ----- public member functions ----- */

/* constructor */
ExerciseCompare::ExerciseCompare(QWidget * parent, const char * name):
		ExerciseBase(parent, name)
{
#ifdef DEBUG
	kdDebug() << "constructor ExerciseCompare()" << endl;
#endif
	
	// init random generator
	srand(time(NULL));

	/* create a new task */
	QApplication::setOverrideCursor(waitCursor); /* show the sand clock */
	createTask();
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// the next thing to do on a button click would be to check the entered
	// result
	m_currentState = _CHECK_TASK;

	baseWidget = new QWidget(this, "baseWidget");
	baseGrid = new QGridLayout(this, 1, 1, 0, -1, "baseGrid"); 
	baseGrid->addWidget(baseWidget, 0, 0);

	// this is a VBox
	realLayout = new QVBoxLayout(baseWidget, 5, 5, "realLayout");

	// add a spacer at the top of the VBox
	QSpacerItem * v_spacer = new QSpacerItem(1, 1);
	realLayout->addItem(v_spacer);

	// now a line holding the task, input fields and result
	QHBoxLayout * taskLineHBoxLayout = new QHBoxLayout(5, "taskLineHBoxLayout");
	realLayout->addLayout(taskLineHBoxLayout);

	// spacer
	v_spacer = new QSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// first the first ratio widget
	m_firstRatioWidget = new RatioWidget(baseWidget, "m_firstRatioWidget", m_firstRatio);
	taskLineHBoxLayout->addWidget(m_firstRatioWidget);

	// spacer
	v_spacer = new QSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// now the button where the user has to choose the comparison sign
	m_signButton = new QPushButton(baseWidget, "m_signButton");
	m_signButton->setText("<");
	m_signButtonState = lessThen;
	taskLineHBoxLayout->addWidget(m_signButton);
	QObject::connect(m_signButton, SIGNAL(clicked()), this, SLOT(slotSignButtonClicked()));

	// spacer
	v_spacer = new QSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// now the second ratio widget
	m_secondRatioWidget = new RatioWidget(baseWidget, "m_secondRatioWidget", m_secondRatio);
	taskLineHBoxLayout->addWidget(m_secondRatioWidget);

	// spacer
	v_spacer = new QSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// at the right end we have a label just showing CORRECT or WRONG
	result_label = new QLabel(baseWidget, "result_label");
	result_label->setText(i18n("WRONG"));
	taskLineHBoxLayout->addWidget(result_label);
	result_label->hide();

	// --- that is the end of the horizontal line ---
	
	// add another spacer in the middle of the VBox
	v_spacer = new QSpacerItem(1, 1);
	realLayout->addItem(v_spacer);

	// the lower part of the VBox holds just a right aligned button
	QHBoxLayout * lowerHBox = new QHBoxLayout(1, "lowerHBox");
	realLayout->addLayout(lowerHBox);
	lowerHBox->addStretch(100);

	// the right aligned button
	m_checkButton = new QPushButton( baseWidget, "m_checkButton" );
	m_checkButton->setText(i18n("&Check Task"));
	m_checkButton->setDefault(true); // is the default button of the dialog
	lowerHBox->addWidget(m_checkButton, 1, Qt::AlignRight);
	QObject::connect(m_checkButton, SIGNAL(clicked()), this, SLOT(slotCheckButtonClicked()));

	// that the user can start typing without moving the focus
	m_signButton->setFocus();

	// show the whole layout
	baseWidget->show();
}

/* destructor */
ExerciseCompare::~ExerciseCompare()
{
#ifdef DEBUG
	kdDebug() << "destructor ExerciseCompare()" << endl;
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExerciseCompare::forceNewTask()
{
#ifdef DEBUG
	kdDebug() << "forceNewTask ExerciseCompare()" << endl;
#endif

	if (m_currentState == _CHECK_TASK)
	{
		// emit the signal for wrong
		signalExerciseSolvedWrong();
	}
	m_currentState = _CHECK_TASK;
	m_checkButton->setText(i18n("&Check Task"));

	// generate next task
	(void) nextTask();
}


/* ------ public slots ------ */

void ExerciseCompare::update()
{
	// call update of components
	m_firstRatioWidget->updateAndRepaint();
	m_secondRatioWidget->updateAndRepaint();

	// update for itself
	((QWidget *) this)->update();
}


/* ------ private member functions ------ */

void ExerciseCompare::createTask()
{
	// generate the first ratio
	m_firstRatio = ratio(int((double(rand()) / RAND_MAX) * 10 + 1), int((double(rand()) / RAND_MAX) * 10 + 1));

	// now the second ratio, but make sure, the second ratio is different from
	// the first one
	do
	{
		m_secondRatio = ratio(int((double(rand()) / RAND_MAX) * 10 + 1), int((double(rand()) / RAND_MAX) * 10 + 1));
	} while (m_firstRatio == m_secondRatio);

	return;
}

/**	- checks, if the user solved the task correctly
		- emits signals if task was solved correctly or wrong */
void ExerciseCompare::showResult()
{
	QPalette pal;
	QColorGroup cg;
	bool result = m_firstRatio < m_secondRatio;

kdDebug() << "result: " << result << endl;
kdDebug() << "m_signButtonState: " << m_signButtonState << endl;

	if ((m_signButtonState == lessThen && result == true) ||
		 (m_signButtonState == greaterThen && result == false))
	{
		// emit the signal for correct
		signalExerciseSolvedCorrect();

		/* yes, the user entered the correct result */
		result_label->setText(i18n("CORRECT"));
		pal = result_label->palette(); /* set green font color */
		cg = pal.active();
		cg.setColor(QColorGroup::Foreground, QColor(6, 179, 0));
		pal.setActive(cg);
		cg = pal.inactive();
		cg.setColor(QColorGroup::Foreground, QColor(6, 179, 0));
		pal.setInactive(cg);
		result_label->setPalette(pal);
		result_label->show(); /* show the result at the end of the task */
	} else {
		// emit the signal for wrong
		signalExerciseSolvedWrong();

		/* no, the user entered the wrong result */
		result_label->setText(i18n("WRONG"));
		pal = result_label->palette(); /* set red font color */
		cg = pal.active();
		cg.setColor(QColorGroup::Foreground, QColor(red));
		pal.setActive(cg);
		cg = pal.inactive();
		cg.setColor(QColorGroup::Foreground, QColor(red));
		pal.setInactive(cg);
		result_label->setPalette(pal);

		result_label->show(); /* show the result at the end of the task */
	} /* if (entered_result == result) */

	return;
}

/** generate the next task and show it to the user */
void ExerciseCompare::nextTask()
{
	result_label->hide(); /* do not show the result at the end of the task */

	// reset the signButton
	m_signButton->setText("<");
	m_signButtonState = lessThen;

	/* create a new task */
	QApplication::setOverrideCursor(waitCursor); /* show the sand clock */
	createTask();
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// set the ratio widgets with the new ratios
	m_firstRatioWidget->setRatio(m_firstRatio);
	m_secondRatioWidget->setRatio(m_secondRatio);

	return;
}

/* ------ private slots ------ */

void ExerciseCompare::slotCheckButtonClicked()
{
	if (m_currentState == _CHECK_TASK)
	{
		m_currentState = _NEXT_TASK;
		m_checkButton->setText(i18n("N&ext Task"));
		(void) showResult();
	} else {
		m_currentState = _CHECK_TASK;
		m_checkButton->setText(i18n("&Check Task"));
		(void) nextTask();
	}

	return;
}

void ExerciseCompare::slotSignButtonClicked()
{
	if (m_signButtonState == lessThen)
	{
		m_signButton->setText(">");
		m_signButtonState = greaterThen;
	} else {
		m_signButton->setText("<");
		m_signButtonState = lessThen;
	}

	return;
}