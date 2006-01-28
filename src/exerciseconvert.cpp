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

#include "exerciseconvert.h"
#include "exerciseconvert.moc"

/* these includes are needed for KDE support */
#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <knumvalidator.h>

/* these includes are needed for Qt support */
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/* standard C++ library includes */
#include <stdlib.h>

#include "rationalwidget.h"
#include "resultwidget.h"

/* ----- public member functions ----- */

/* constructor */
ExerciseConvert::ExerciseConvert(QWidget * parent, const char * name):
		ExerciseBase(parent, name)
{
#ifdef DEBUG
	kdDebug() << "constructor ExerciseConvert()" << endl;
#endif

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

	// first left is the rational widget
	m_rationalWidget = new RationalWidget(baseWidget, "m_rationalWidget", m_number, m_periodStart, m_periodLength);
// 	taskLineHBoxLayout->addWidget(m_rationalWidget);

	// now we have the input fields aligned in a VBox
	QVBoxLayout * inputLayout = new QVBoxLayout(5, "inputLayout");
//	taskLineHBoxLayout->addLayout(inputLayout);

	// to validate, that the input is an int
	KIntValidator *valnum = new KIntValidator( this );

	/* add input box so the user can enter numerator */
	numer_edit = new QLineEdit(baseWidget, "numer_edit");
	numer_edit->setValidator( valnum ); // use the int validator
	QToolTip::add(numer_edit, i18n("Enter the numerator of your result"));
	inputLayout->addWidget(numer_edit);

	/* add a line between the edit boxes */
	edit_line = new QFrame(baseWidget, "edit_line");
	edit_line->setGeometry(QRect(100, 100, 20, 20));
	edit_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	inputLayout->addWidget(edit_line);

	/* add input box so the user can enter denominator */
	deno_edit = new QLineEdit(baseWidget, "deno_edit");
	deno_edit->setValidator( valnum ); // use the int validator
	QToolTip::add(deno_edit, i18n("Enter the denominator of your result"));
	inputLayout->addWidget(deno_edit);

	// next is the result widget
	m_resultWidget = new ResultWidget(baseWidget, "m_resultWidget", m_result);
// 	taskLineHBoxLayout->addWidget(m_resultWidget);
	m_resultWidget->hide();

	// at the right end we have a label just showing CORRECT or WRONG
	result_label = new QLabel(baseWidget, "result_lable");
	result_label->setText(i18n("WRONG"));
// 	taskLineHBoxLayout->addWidget(result_label);
	result_label->hide();

	// add another spacer in the middle of the VBox
	v_spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
// 	taskLineHBoxLayout->addItem(v_spacer);

	// --- that is the end of the horizontal line ---

	// in RTL desktops, we still need to allign the
	// execise to the left. On Qt4, you can set the direction
	// of the layout to LTR (instead of inherit), but on Qt3
	// the only way of fixing it is inserting the widgets in reversed
	// order to the layout.
	//
	// if you need help with this feel free to contact me - Diego <elcuco@kde.org> )
	// This should fix parts of bug #116831
	if (QApplication::reverseLayout())
	{
		taskLineHBoxLayout->addItem(v_spacer);
		taskLineHBoxLayout->addWidget(result_label);
		taskLineHBoxLayout->addWidget(m_resultWidget);
		taskLineHBoxLayout->addLayout(inputLayout);
		taskLineHBoxLayout->addWidget(m_rationalWidget);
	}
	else
	{
		taskLineHBoxLayout->addWidget(m_rationalWidget);
		taskLineHBoxLayout->addLayout(inputLayout);
		taskLineHBoxLayout->addWidget(m_resultWidget);
		taskLineHBoxLayout->addWidget(result_label);
		taskLineHBoxLayout->addItem(v_spacer);
	}
	
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
	QToolTip::add(m_checkButton, i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));
	lowerHBox->addWidget(m_checkButton, 1, Qt::AlignRight);
	QObject::connect(m_checkButton, SIGNAL(clicked()), this, SLOT(slotCheckButtonClicked()));

	// that the user can start typing without moving the focus
	numer_edit->setFocus();

	// show the whole layout
	baseWidget->show();

	// add tooltip and qwhatsthis help to the widget
	QToolTip::add(this, i18n("In this exercise you have to convert a number into a fraction."));
	QWhatsThis::add(this, i18n("In this exercise you have to convert a given number into a fraction by entering numerator and denominator. Do not forget to reduce the result!"));
}

/* destructor */
ExerciseConvert::~ExerciseConvert()
{
#ifdef DEBUG
	kdDebug() << "destructor ExerciseConvert()" << endl;
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExerciseConvert::forceNewTask()
{
#ifdef DEBUG
	kdDebug() << "forceNewTask ExerciseConvert()" << endl;
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

void ExerciseConvert::update()
{
	// call update of components
	m_rationalWidget->updateAndRepaint();
	m_resultWidget->updateAndRepaint();

	// update for itself
	((QWidget *) this)->update();
}


/* ------ private member functions ------ */

void ExerciseConvert::createTask()
{
	// the tasks are hardcoded here; there are some algorithms to convert
	// rational numbers to fractions, but it is not worth the effort here
	switch(int((double(rand()) / RAND_MAX) * 18 + 1))
	{
		case  0 :   m_number = KGlobal::locale()->formatNumber(0.5, 1);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(1, 2);
						break;
		case  1 :   m_number = KGlobal::locale()->formatNumber(0.3, 1);
						m_periodStart = 2;
						m_periodLength = 1;
						m_result = ratio(1, 3);
						break;
		case  2 :   m_number = KGlobal::locale()->formatNumber(0.6, 1);
						m_periodStart = 2;
						m_periodLength = 1;
						m_result = ratio(2, 3);
						break;
		case  3 :   m_number = KGlobal::locale()->formatNumber(0.25, 2);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(1, 4);
						break;
		case  4 :   m_number = KGlobal::locale()->formatNumber(0.75, 2);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(3, 4);
						break;
		case  5 :   m_number = KGlobal::locale()->formatNumber(0.2, 1);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(1, 5);
						break;
		case  6 :   m_number = KGlobal::locale()->formatNumber(0.4, 1);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(2, 5);
						break;
		case  7 :   m_number = KGlobal::locale()->formatNumber(0.6, 1);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(3, 5);
						break;
		case  8 :   m_number = KGlobal::locale()->formatNumber(0.8, 1);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(4, 5);
						break;
		case  9 :   m_number = KGlobal::locale()->formatNumber(0.16, 2);
						m_periodStart = 3;
						m_periodLength = 1;
						m_result = ratio(1, 6);
						break;
		case 10 :   m_number = KGlobal::locale()->formatNumber(0.142857, 6);
						m_periodStart = 2;
						m_periodLength = 6;
						m_result = ratio(1, 7);
						break;
		case 11 :   m_number = KGlobal::locale()->formatNumber(0.125, 3);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(1, 8);
						break;
		case 12 :   m_number = KGlobal::locale()->formatNumber(0.375, 3);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(3, 8);
						break;
		case 13 :   m_number = KGlobal::locale()->formatNumber(0.1, 1);
						m_periodStart = 2;
						m_periodLength = 1;
						m_result = ratio(1, 9);
						break;
		case 14 :   m_number = KGlobal::locale()->formatNumber(0.1, 1);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(1, 10);
						break;
		case 15 :   m_number = KGlobal::locale()->formatNumber(0.05, 2);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(1, 20);
						break;
		case 16 :   m_number = KGlobal::locale()->formatNumber(0.01, 2);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(1, 100);
						break;
		case 17 :   m_number = KGlobal::locale()->formatNumber(0.83, 2);
						m_periodStart = 3;
						m_periodLength = 1;
						m_result = ratio(5, 6);
						break;
		default :
		case 18 :   m_number = KGlobal::locale()->formatNumber(0.001, 3);
						m_periodStart = 2;
						m_periodLength = 0;
						m_result = ratio(1, 1000);
						break;
	}

	return;
}

/**	- checks, if the user solved the task correctly
		- emits signals if task was solved correctly or wrong */
void ExerciseConvert::showResult()
{
	QString tmp_str; /* to build a string for a label */
	QPalette pal;
	QColorGroup cg;
	ratio entered_result;

	// change the tooltip of the check button
	QToolTip::add(m_checkButton, i18n("Click on this button to get to the next task."));

	numer_edit->setEnabled(false);
	deno_edit->setEnabled(false);

	m_resultWidget->setResult(m_result);
	m_resultWidget->show();

	// an empty numerator field will be interpreted as 0
	if (numer_edit->text().isEmpty() == true)
		numer_edit->setText("0");

	// an empty denominator field will be interpreted as 1
	if (deno_edit->text().isEmpty() == true)
		deno_edit->setText("1");

	/* store the entered result to check it, but without reducing */
	entered_result.setNumerator(numer_edit->text().toInt(), false);
	entered_result.setDenominator(deno_edit->text().toInt(), false);

	// check the entered result; 0/1 == 0/5 -> true,
	// but 0/1 == 0/0 -> false
	// a 0 for denominator is never allowed (always counted as wrong)
	//
	// we have to get the 0 directly from the input field, because
	// Ratio::setDenominator(0, false) will set the denominator to 1 to ensure
	// the Ratio is valid
	if ( (deno_edit->text().toInt() != 0) && ((entered_result == m_result) ||
		  (m_result.numerator() == 0 && entered_result.numerator() == 0)) )
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

		// if the user entered a 0 for the denominator (division by 0) we have to
		// get the 0 directly from the input field, because
		// Ratio::setDenominator(0, true) will set the denominator to 1 to ensure
		// the Ratio is valid
		if (deno_edit->text().toInt() == 0)
		{
			KMessageBox::information(this,
			                         i18n("You entered a 0 as the denominator. This means division by zero, which is not allowed. This task will be counted as not correctly solved."));
		} else {
			/* maybe the entered ratio was not reduced */
			entered_result.reduce();
			if (entered_result == m_result)
				KMessageBox::information(this,
				                         i18n("You entered the correct result, but not reduced.\nAlways enter your results as reduced. This task will be counted as not correctly solved."));
		}
	} /* if (entered_result == result) */

	return;
}

/** generate the next task and show it to the user */
void ExerciseConvert::nextTask()
{
	// change the tooltip of the check button
	QToolTip::add(m_checkButton, i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));

	numer_edit->setEnabled(true);
	deno_edit->setEnabled(true);

	result_label->hide(); /* do not show the result at the end of the task */
	m_resultWidget->hide();

	/* clear user input */
	deno_edit->setText("");
	numer_edit->setText("");
	numer_edit->setFocus();

	/* create a new task */
	QApplication::setOverrideCursor(waitCursor); /* show the sand clock */
	createTask();
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// update the task widget
	m_rationalWidget->setRational(m_number, m_periodStart, m_periodLength);

	return;
}

/* ------ private slots ------ */

void ExerciseConvert::slotCheckButtonClicked()
{
	if (m_currentState == _CHECK_TASK)
	{
		// if nothing has been entered by the user, we don't check the result yet
		if (numer_edit->text().isEmpty() == true && deno_edit->text().isEmpty() ==
true)
			return;
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
