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
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqpushbutton.h>
#include <tqtooltip.h>
#include <tqwhatsthis.h>

/* standard C++ library includes */
#include <stdlib.h>

/* ----- public member functions ----- */

/* constructor */
ExerciseCompare::ExerciseCompare(TQWidget * parent, const char * name):
		ExerciseBase(parent, name)
{
#ifdef DEBUG
	kdDebug() << "constructor ExerciseCompare()" << endl;
#endif

	/* create a new task */
	TQApplication::setOverrideCursor(waitCursor); /* show the sand clock */
	createTask();
	TQApplication::restoreOverrideCursor(); /* show the normal cursor */

	// the next thing to do on a button click would be to check the entered
	// result
	m_currentState = _CHECK_TASK;

	baseWidget = new TQWidget(this, "baseWidget");
	baseGrid = new TQGridLayout(this, 1, 1, 0, -1, "baseGrid"); 
	baseGrid->addWidget(baseWidget, 0, 0);

	// this is a VBox
	realLayout = new TQVBoxLayout(baseWidget, 5, 5, "realLayout");

	// add a spacer at the top of the VBox
	TQSpacerItem * v_spacer = new TQSpacerItem(1, 1);
	realLayout->addItem(v_spacer);

	// now a line holding the task, input fields and result
	TQHBoxLayout * taskLineHBoxLayout = new TQHBoxLayout(5, "taskLineHBoxLayout");
	realLayout->addLayout(taskLineHBoxLayout);

	// spacer
	v_spacer = new TQSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// first the first ratio widget
	m_firstRatioWidget = new RatioWidget(baseWidget, "m_firstRatioWidget", m_firstRatio);
	taskLineHBoxLayout->addWidget(m_firstRatioWidget);

	// spacer
	v_spacer = new TQSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// now the button where the user has to choose the comparison sign
	m_signButton = new TQPushButton(baseWidget, "m_signButton");

	// RTL BUG, see slotSignButtonClicked() for more information
	m_signButton->setText( TQApplication::reverseLayout()?">":"<");

	m_signButtonState = lessThen;
	taskLineHBoxLayout->addWidget(m_signButton);
	TQObject::connect(m_signButton, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotSignButtonClicked()));
	TQToolTip::add(m_signButton, i18n("Click on this button to change the comparison sign."));

	// spacer
	v_spacer = new TQSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// now the second ratio widget
	m_secondRatioWidget = new RatioWidget(baseWidget, "m_secondRatioWidget", m_secondRatio);
	taskLineHBoxLayout->addWidget(m_secondRatioWidget);

	// spacer
	v_spacer = new TQSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// at the right end we have a label just showing CORRECT or WRONG
	result_label = new TQLabel(baseWidget, "result_label");
	result_label->setText(i18n("WRONG"));
	taskLineHBoxLayout->addWidget(result_label);
	result_label->hide();

	// --- that is the end of the horizontal line ---
	
	// add another spacer in the middle of the VBox
	v_spacer = new TQSpacerItem(1, 1);
	realLayout->addItem(v_spacer);

	// the lower part of the VBox holds just a right aligned button
	TQHBoxLayout * lowerHBox = new TQHBoxLayout(1, "lowerHBox");
	realLayout->addLayout(lowerHBox);
	lowerHBox->addStretch(100);

	// the right aligned button
	m_checkButton = new TQPushButton( baseWidget, "m_checkButton" );
	m_checkButton->setText(i18n("&Check Task"));
	m_checkButton->setDefault(true); // is the default button of the dialog
	TQToolTip::add(m_checkButton, i18n("Click on this button to check your result."));
	lowerHBox->addWidget(m_checkButton, 1, Qt::AlignRight);
	TQObject::connect(m_checkButton, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotCheckButtonClicked()));

	// that the user can start typing without moving the focus
	m_signButton->setFocus();

	// show the whole layout
	baseWidget->show();

	// add tooltip and qwhatsthis help to the widget
	TQToolTip::add(this, i18n("In this exercise you have to compare 2 given fractions."));
	TQWhatsThis::add(this, i18n("In this exercise you have to compare 2 given fractions by choosing the correct comparison sign. You can change the comparison sign by just clicking on the button showing the sign."));
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
	((TQWidget *) this)->update();
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
	TQPalette pal;
	TQColorGroup cg;
	bool result = m_firstRatio < m_secondRatio;

	// change the tooltip of the check button
	TQToolTip::add(m_checkButton, i18n("Click on this button to get to the next task."));

	// disable sign button
	m_signButton->setEnabled(false);

	if ((m_signButtonState == lessThen && result == true) ||
		 (m_signButtonState == greaterThen && result == false))
	{
		// emit the signal for correct
		signalExerciseSolvedCorrect();

		/* yes, the user entered the correct result */
		result_label->setText(i18n("CORRECT"));
		pal = result_label->palette(); /* set green font color */
		cg = pal.active();
		cg.setColor(TQColorGroup::Foreground, TQColor(6, 179, 0));
		pal.setActive(cg);
		cg = pal.inactive();
		cg.setColor(TQColorGroup::Foreground, TQColor(6, 179, 0));
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
		cg.setColor(TQColorGroup::Foreground, TQColor(red));
		pal.setActive(cg);
		cg = pal.inactive();
		cg.setColor(TQColorGroup::Foreground, TQColor(red));
		pal.setInactive(cg);
		result_label->setPalette(pal);

		result_label->show(); /* show the result at the end of the task */
	} /* if (entered_result == result) */

	return;
}

/** generate the next task and show it to the user */
void ExerciseCompare::nextTask()
{
	// change the tooltip of the check button
	TQToolTip::add(m_checkButton, i18n("Click on this button to check your result."));

	// enable sign button
	m_signButton->setEnabled(true);

	result_label->hide(); /* do not show the result at the end of the task */

	// reset the signButton
	// RTL BUG, see slotSignButtonClicked() for more information
	m_signButton->setText( TQApplication::reverseLayout()?">":"<");
	m_signButtonState = lessThen;

	/* create a new task */
	TQApplication::setOverrideCursor(waitCursor); /* show the sand clock */
	createTask();
	TQApplication::restoreOverrideCursor(); /* show the normal cursor */

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
	// in RTL desktops, we still need to allign the
	// execise to the left. On Qt4, you can set the direction
	// of the layout to LTR (instead of inherit), but on Qt3
	// the only way of fixing it is inserting the widgets in reversed
	// order to the layout.
	// 
	// But... as an ugly hack, we can also display the "other" operation
	// thats damm ugly, but will work as well :)
	// 
	// See also taskview.cpp for the same bug.
	//
	// (if you need help with this feel free to contact
	// (me - Diego <elcuco@kde.org> )
	// This shuold fix parts of bug #116831, 

	if (m_signButtonState == lessThen)
	{
		m_signButton->setText( TQApplication::reverseLayout()?"<":">");
		m_signButtonState = greaterThen;
	} else {
		m_signButton->setText( TQApplication::reverseLayout()?">":"<");
		m_signButtonState = lessThen;
	}

	return;
}
