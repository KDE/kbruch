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
#include <klocale.h>

/* these includes are needed for Qt support */
#include <qapplication.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>

//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

/* standard C++ library includes */
#include <stdlib.h>

/* ----- public member functions ----- */

/* constructor */
ExerciseCompare::ExerciseCompare(QWidget * parent):
		ExerciseBase(parent)
{
#ifdef DEBUG
	kDebug() << "constructor ExerciseCompare()";
#endif

	/* create a new task */
	QApplication::setOverrideCursor(Qt::WaitCursor); /* show the sand clock */
	createTask();
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// the next thing to do on a button click would be to check the entered
	// result
	m_currentState = _CHECK_TASK;

	baseWidget = new QWidget(this);
	baseWidget->setObjectName("baseWidget");
	baseGrid = new QGridLayout( this );
        baseGrid->setObjectName( "baseGrid");
	baseGrid->addWidget(baseWidget, 0, 0);

	// this is a VBox
	realLayout = new QVBoxLayout(baseWidget);
	realLayout->setMargin(5);
	realLayout->setSpacing(5);
	realLayout->setObjectName("realLayout");

	// add a spacer at the top of the VBox
	QSpacerItem * v_spacer = new QSpacerItem(1, 1);
	realLayout->addItem(v_spacer);

	// now a line holding the task, input fields and result
	QHBoxLayout * taskLineHBoxLayout = new QHBoxLayout();
	taskLineHBoxLayout->setObjectName("taskLineHBoxLayout");
	taskLineHBoxLayout->setSpacing(5);
	realLayout->addLayout(taskLineHBoxLayout);

	// spacer
	v_spacer = new QSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// first the first ratio widget
	m_firstRatioWidget = new RatioWidget(baseWidget, m_firstRatio);
	m_firstRatioWidget->setObjectName("m_firstRatioWidget");
	taskLineHBoxLayout->addWidget(m_firstRatioWidget);

	// spacer
	v_spacer = new QSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// now the button where the user has to choose the comparison sign
	m_signButton = new QPushButton(baseWidget);
	m_signButton->setObjectName("m_signButton");
	m_signButton->setText("<");
	m_signButtonState = lessThen;
	taskLineHBoxLayout->addWidget(m_signButton);
	QObject::connect(m_signButton, SIGNAL(clicked()), this, SLOT(slotSignButtonClicked()));
	m_signButton->setToolTip(i18n("Click on this button to change the comparison sign."));

	// spacer
	v_spacer = new QSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// now the second ratio widget
	m_secondRatioWidget = new RatioWidget(baseWidget, m_secondRatio);
	m_secondRatioWidget->setObjectName("m_secondRatioWidget");
	taskLineHBoxLayout->addWidget(m_secondRatioWidget);

	// spacer
	v_spacer = new QSpacerItem(1, 1);
	taskLineHBoxLayout->addItem(v_spacer);

	// at the right end we have a label just showing CORRECT or WRONG
	result_label = new QLabel(baseWidget);
	result_label->setObjectName("result_label");
	result_label->setText(i18n("WRONG"));
	taskLineHBoxLayout->addWidget(result_label);
	result_label->hide();

	// --- that is the end of the horizontal line ---

	// add another spacer in the middle of the VBox
	v_spacer = new QSpacerItem(1, 1);
	realLayout->addItem(v_spacer);

	// the lower part of the VBox holds just a right aligned button
	QHBoxLayout * lowerHBox = new QHBoxLayout();
	lowerHBox->setObjectName("lowerHBox");
	lowerHBox->setSpacing(1);
	realLayout->addLayout(lowerHBox);
	lowerHBox->addStretch(100);

	// the right aligned button
	m_checkButton = new QPushButton( baseWidget );
	m_checkButton->setObjectName( "m_checkButton" );
	m_checkButton->setText(i18n("&Check Task"));
	m_checkButton->setDefault(true); // is the default button of the dialog
	m_checkButton->setToolTip(i18n("Click on this button to check your result."));
	lowerHBox->addWidget(m_checkButton, 1, Qt::AlignRight);
	QObject::connect(m_checkButton, SIGNAL(clicked()), this, SLOT(slotCheckButtonClicked()));

	// that the user can start typing without moving the focus
	m_signButton->setFocus();

	// show the whole layout
	baseWidget->show();

	// add tooltip and qwhatsthis help to the widget
	setToolTip(i18n("In this exercise you have to compare 2 given fractions."));
	setWhatsThis( i18n("In this exercise you have to compare 2 given fractions by choosing the correct comparison sign. You can change the comparison sign by just clicking on the button showing the sign."));
}

/* destructor */
ExerciseCompare::~ExerciseCompare()
{
#ifdef DEBUG
	kDebug() << "destructor ExerciseCompare()";
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExerciseCompare::forceNewTask()
{
#ifdef DEBUG
	kDebug() << "forceNewTask ExerciseCompare()";
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
	bool result = m_firstRatio < m_secondRatio;

	// change the tooltip of the check button
	m_checkButton->setToolTip(i18n("Click on this button to get to the next task."));

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
        pal.setColor(QPalette::Active, QPalette::Foreground, QColor(6, 179, 0));
        pal.setColor(QPalette::Inactive, QPalette::Foreground, QColor(6, 179, 0));
		result_label->setPalette(pal);
		result_label->show(); /* show the result at the end of the task */
	} else {
		// emit the signal for wrong
		signalExerciseSolvedWrong();

		/* no, the user entered the wrong result */
		result_label->setText(i18n("WRONG"));
		pal = result_label->palette(); /* set red font color */
        pal.setColor(QPalette::Active, QPalette::Foreground, QColor(Qt::red));
        pal.setColor(QPalette::Inactive, QPalette::Foreground, QColor(Qt::red));
		result_label->setPalette(pal);

		result_label->show(); /* show the result at the end of the task */
	} /* if (entered_result == result) */

	return;
}

/** generate the next task and show it to the user */
void ExerciseCompare::nextTask()
{
	// change the tooltip of the check button
	m_checkButton->setToolTip(i18n("Click on this button to check your result."));

	// enable sign button
	m_signButton->setEnabled(true);

	result_label->hide(); /* do not show the result at the end of the task */

	// reset the signButton
	m_signButton->setText("<");
	m_signButtonState = lessThen;

	/* create a new task */
	QApplication::setOverrideCursor(Qt::WaitCursor); /* show the sand clock */
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
