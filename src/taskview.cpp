/***************************************************************************
                          taskview.cpp  -  The task window
                             -------------------
    begin                : Tue Feb 08 13:41:00 CET 2002
    copyright            : (C) 2001 - 2004 by Sebastian Stein
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

#include "taskview.h"
#include "taskview.moc"

/* these includes are needed for KDE support */
#include <klocale.h>
#include <kmessagebox.h>
#include <knumvalidator.h>
#include <klineedit.h>
#include <KStandardDirs>

/* these includes are needed for Qt support */
#include <qapplication.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>

//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "settingsclass.h"

/* standard C++ library includes */
#include <math.h>

/* ----- public member functions ----- */

/* constructor */
TaskView::TaskView(QWidget * parent,
		 bool padd_add, bool padd_div, bool padd_mult, bool padd_sub,
		 unsigned int pnr_ratios, unsigned int pmax_md):
		ExerciseBase(parent), m_addAdd(padd_add), m_addDiv(padd_div), 
		m_addMult(padd_mult), m_addSub(padd_sub),
		nr_ratios(pnr_ratios), max_md(pmax_md)
{
#ifdef DEBUG
	kDebug() << "constructor TaskView()";
#endif
	curr_nr_ratios = nr_ratios;

	/* create a new task */
	QApplication::setOverrideCursor(Qt::WaitCursor); /* show the sand clock */
	current_task.create_task(max_md, nr_ratios, m_addAdd, m_addDiv, m_addMult, m_addSub);
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// to validate, that the input is an int
	KIntValidator *valnum = new KIntValidator( this );

	// the next thing to do on a button click would be to check the entered
	// result
	m_currentState = _CHECK_TASK;

	// Create the base of Widget
	taskWidget = new QWidget(this);
	taskWidget->setObjectName("taskWidget");
	checkWidget = new QWidget(this);
	checkWidget->setObjectName("checkWidget");
	
	baseGrid = new QGridLayout(this);
	baseGrid->setObjectName( "baseGrid" );
	baseGrid->setColumnStretch(0,1);

	baseGrid->addWidget(taskWidget, 0, 0);
	baseGrid->addWidget(checkWidget, 0, 1);
	
	taskLayout = new QGridLayout(this);
	taskLayout->setObjectName( "taskLayout" );
	taskLayout->setRowStretch(0,1);
	taskLayout->setRowStretch(4,1);
	taskLayout->setColumnStretch(0,1);
	taskLayout->setColumnStretch(5,1);

	checkLayout = new QGridLayout(this);
	checkLayout->setObjectName( "checkLayout" );

	QFont defaultFont = SettingsClass::taskFont();
	defaultFont.setBold( true );
	defaultFont.setPointSize( 18 );
	
	// first left is the task widget
	m_taskWidget = new TaskWidget(taskWidget, current_task);
	m_taskWidget->setObjectName("m_taskWidget");
	taskLayout->addWidget(m_taskWidget, 1, 1, 3, 1);

	/* add input box so the user can enter the integer par of the fraction */
	integer_edit = new KLineEdit(taskWidget);
	integer_edit->setObjectName("integer_edit");
	integer_edit->setValidator( valnum ); // use the int validator
	integer_edit->setToolTip(i18n("Enter the integer part of the fraction"));
	integer_edit->setFont(defaultFont);
	integer_edit->setFixedSize(85,42);
	integer_edit->setAlignment(Qt::AlignHCenter);
	QObject::connect(integer_edit, SIGNAL(returnPressed(const QString &)), this,
		SLOT(integerReturnPressed(const QString &)));
	taskLayout->addWidget(integer_edit, 1, 3, 3, 1, Qt::AlignVCenter | Qt::AlignRight);


	/* add input box so the user can enter numerator */
	numer_edit = new KLineEdit(taskWidget);
	numer_edit->setObjectName("numer_edit");
	numer_edit->setValidator( valnum ); // use the int validator
	numer_edit->setToolTip(i18n("Enter the numerator of your result"));
	numer_edit->setFont(defaultFont);
	numer_edit->setFixedSize(85,42);
	numer_edit->setAlignment(Qt::AlignHCenter);
	QObject::connect(numer_edit, SIGNAL(returnPressed(const QString &)), this,
		SLOT(numeratorReturnPressed(const QString &)));
	taskLayout->addWidget(numer_edit, 1, 4);

	/* add a line between the edit boxes */
	edit_line = new QFrame(taskWidget);
	edit_line->setGeometry(QRect(100, 100, 20, 20));
	edit_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	taskLayout->addWidget(edit_line, 2, 4);

	/* add input box so the user can enter denominator */
	deno_edit = new KLineEdit(taskWidget);
	deno_edit->setObjectName("deno_edit");
	deno_edit->setValidator( valnum ); // use the int validator
	deno_edit->setToolTip(i18n("Enter the denominator of your result"));
	deno_edit->setFont(defaultFont);
	deno_edit->setFixedSize(85,42);
	deno_edit->setAlignment(Qt::AlignHCenter);
	QObject::connect(deno_edit, SIGNAL(returnPressed(const QString &)), this,
		SLOT(denominatorReturnPressed(const QString &)));
	taskLayout->addWidget(deno_edit, 3, 4);
	
	// next is the result widget
	m_resultWidget = new ResultWidget(checkWidget, ratio());
	m_resultWidget->setObjectName("m_resultWidget");
	checkLayout->addWidget(m_resultWidget, 0, 0, 1, 2);

	defaultFont.setPointSize(10);

	// the right aligned button
	m_checkButton = new QPushButton( checkWidget );
	m_checkButton->setObjectName( "m_checkButton" );
	m_checkButton->setText(i18n("&Check"));
	m_checkButton->setDefault(true); // is the default button of the dialog
	m_checkButton->setToolTip(i18n("Click this button to check your result. The button will not work if you have not entered a result yet."));
	m_checkButton->setFont(defaultFont);	
	QObject::connect(m_checkButton, SIGNAL(clicked()), this, SLOT(slotCheckButtonClicked()));
	checkLayout->addWidget(m_checkButton, 1, 0);	

	// the right aligned button
	m_skipButton = new QPushButton( checkWidget );
	m_skipButton->setObjectName( "m_skipButton" );
	m_skipButton->setText(i18n("&Skip"));
	m_skipButton->setToolTip(i18n("Click this button to skip this question."));
	m_skipButton->setFont(defaultFont);	
	QObject::connect(m_skipButton, SIGNAL(clicked()), this, SLOT(slotSkipButtonClicked()));
	checkLayout->addWidget(m_skipButton, 1, 1);	

	setLayout(baseGrid);
	taskWidget->setLayout(taskLayout);
  	checkWidget->setLayout(checkLayout);

    // show the whole layout
	m_taskWidget->show();

	// add tooltip and qwhatsthis help to the widget
	setToolTip(i18n("In this exercise you have to solve a given question with fractions."));
	setWhatsThis( i18n("In this exercise you have to solve the generated question. You have to enter the numerator and the denominator. You can adjust the difficulty of the question with the boxes in the toolbar. Do not forget to reduce the result."));
}

/* destructor */
TaskView::~TaskView()
{
#ifdef DEBUG
	kDebug() << "destructor TaskView()";
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

void TaskView::setReducedForm(bool value)
{
	m_reducedForm = value;
}

void TaskView::setSolutionMixed(bool value)
{
	m_resultWidget->setSolutionMixed(value);
}

void TaskView::setQuestionMixed(bool value)
{
	m_taskWidget->setQuestionMixed(value);
}

void TaskView::setAnswerMixed(bool value)
{
	m_answerMixed = value;
	integer_edit->setVisible( value );
}

/** the parameters of task generation can be set with this function */
void TaskView::setTaskParameters(bool padd_add, bool padd_div, 
				bool padd_mult, bool padd_sub, 
				unsigned int pnr_ratios, unsigned int pmax_md)
{
	// at least one operation must be enabled
	//if ((padd_add == false) && (padd_div == false) && (padd_mult == false) && (padd_sub == false))
	//	padd_add = true;

	// we need at least 2 ratios
	if (pnr_ratios < 2)
		pnr_ratios = 2;

	// we can only visualize 5 ratios, so we have to limit it
	if (pnr_ratios > 5)
		pnr_ratios = 5;

	// the main denominator must be at least 2^pnr_ratios
	if (pow(2.0, (double)pnr_ratios) > pmax_md)
		pmax_md = (unsigned int) pow(2.0, (double)pnr_ratios);

	// so everything seems to be fine, lets set the internal values to the given
	// ones
	m_addSub = padd_sub;
	m_addAdd = padd_add;
	m_addMult = padd_mult;
	m_addDiv = padd_div;			
	max_md = pmax_md;

	nr_ratios = pnr_ratios;

	return;
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void TaskView::forceNewTask()
{
#ifdef DEBUG
	kDebug() << "forceNewTask TaskView()";
#endif

	if (m_currentState == _CHECK_TASK)
	{
		// emit the signal for skipped
		signalTaskSkipped();
	}
	m_currentState = _CHECK_TASK;
	m_checkButton->setText(i18n("&Check"));

	// generate next task
	(void) nextTask();
}


/* ------ public slots ------ */
void TaskView::update()
{
	// call update of components
	m_taskWidget->updateAndRepaint();
	m_resultWidget->update();

	// update for itself
	((QWidget *) this)->update();
}


/* ------ private member functions ------ */

/**	- checks the entered result and compares it to the task's result
		- shows the correct result and informs the user if he was right or wrong
		- if the user entered the result unreduced, he will be informed about it
		- if the user entered a 0 for the denominator, he will be informed about
		  it (division by zero)
		- emits signals if task was solved right or wrong */
void TaskView::showResult()
{
	QString tmp_str; /* to build a string for a label */
	QPalette pal;
	bool wrong = false;

	// change the tooltip of the check button
	m_checkButton->setToolTip(i18n("Click this button to get the next question."));

	numer_edit->setEnabled(false);
	deno_edit->setEnabled(false);
	integer_edit->setEnabled(false);

	// an empty numerator field will be interpreted as 0
	if (numer_edit->text().isEmpty() == true)
		numer_edit->setText("0");

	// an empty denominator field will be interpreted as 1
	if (deno_edit->text().isEmpty() == true)
		deno_edit->setText("1");

	result = current_task.solve();

	if (m_answerMixed == true) {

		// an empty denominator field will be interpreted as 1
		if (integer_edit->text().isEmpty() == true)
			integer_edit->setText("0");

		int int_mixed, int_numerator, int_denominator;

		int_numerator = qAbs(result.numerator());
		int_denominator = qAbs(result.denominator());
		int_mixed = 0;

		if (qAbs(result.numerator()) >= qAbs(result.denominator()))
		{
			int_mixed = int(result.numerator() / result.denominator());
			int_numerator = int_numerator % int_denominator;
		}

		if ( (deno_edit->text().toInt() != 0) && (int_numerator == numer_edit->text().toInt())
			&& (int_denominator == deno_edit->text().toInt()) 
			&& (int_mixed == integer_edit->text().toInt()) )
		{
			// emit the signal for correct
			signalTaskSolvedCorrect();
			m_resultWidget->setResult(result, 1);
		} else 
			wrong = true;	
	} else {

		/* store the entered result to check it, but without reducing */
		entered_result.setNumerator(numer_edit->text().toInt(), false);
		entered_result.setDenominator(deno_edit->text().toInt(), false);

		if (!m_reducedForm)
			entered_result.reduce();

		// check the entered result; 0/1 == 0/5 -> true,
		// but 0/1 == 0/0 -> false
		// a 0 for denominator is never allowed (always counted as wrong)
		//
		// we have to get the 0 directly from the input field, because
		// Ratio::setDenominator(0, false) will set the denominator to 1 to ensure
		// the Ratio is valid

		if ( (deno_edit->text().toInt() != 0) && ((entered_result == result) ||
			  (result.numerator() == 0 && entered_result.numerator() == 0)) )
		{
			// emit the signal for correct
			signalTaskSolvedCorrect();
			m_resultWidget->setResult(result, 1);
		} else 
			wrong = true;
	}

	if (wrong == true) {
		// emit the signal for wrong
		signalTaskSolvedWrong();
		m_resultWidget->setResult(result, 0);

		// if the user entered a 0 for the denominator (division by 0) we have to
		// get the 0 directly from the input field, because
		// Ratio::setDenominator(0, true) will set the denominator to 1 to ensure
		// the Ratio is valid
		if (deno_edit->text().toInt() == 0)
		{
			KMessageBox::information(this,
			                         i18n("You entered 0 as the denominator. This means division by zero, which is not allowed. This question will be counted as not correctly solved."));
		} else {
			/* maybe the entered ratio was not reduced */
			entered_result.reduce();
			if (entered_result == result)
				KMessageBox::information(this,
				                         i18n("You entered the correct result, but not reduced.\nAlways enter your results as reduced. This question will be counted as not correctly solved."));
		}
	} 
	m_resultWidget->show();
}

/** generate the next task and show it to the user */
void TaskView::nextTask()
{
	// change the tooltip of the check button
	m_checkButton->setToolTip(i18n("Click this button to check your result. The button will not work if you have not entered a result yet."));

	numer_edit->setEnabled(true);
	deno_edit->setEnabled(true);
	integer_edit->setEnabled(true);
	if ( m_answerMixed == true )
		integer_edit->setEnabled(true);
	m_skipButton->setEnabled(true);
	
	m_resultWidget->setResult( result, -1);
	
	/* clear user input */
	deno_edit->setText("");
	numer_edit->setText("");
	integer_edit->setText("");
	if ( m_answerMixed == true )
		integer_edit->setFocus();
	else
		numer_edit->setFocus();

	/* create a new task */
	QApplication::setOverrideCursor(Qt::WaitCursor); /* show the sand clock */

	current_task.create_task(max_md, nr_ratios, m_addAdd, m_addDiv, m_addMult, m_addSub);
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// update the task widget
	m_taskWidget->setTask((const task) (current_task));
}

/* ------ private slots ------ */

void TaskView::integerReturnPressed(const QString &)
{
	numer_edit->setFocus();
}


void TaskView::numeratorReturnPressed(const QString &)
{
	deno_edit->setFocus();
}

void TaskView::denominatorReturnPressed(const QString &)
{
	slotCheckButtonClicked();
}

void TaskView::slotCheckButtonClicked()
{
	if (m_currentState == _CHECK_TASK)
	{
		// if nothing has been entered by the user, we don't check the result yet
		if (numer_edit->text().isEmpty() == true && deno_edit->text().isEmpty() == true && integer_edit->text().isEmpty() == true)
			return;
		m_currentState = _NEXT_TASK;
		m_checkButton->setText(i18n("N&ext"));
		(void) showResult();
	} else {
		m_currentState = _CHECK_TASK;
		m_checkButton->setText(i18n("&Check"));
		(void) nextTask();
	}
}

void TaskView::slotSkipButtonClicked()
{
	forceNewTask();
}

/* ------ protected events ------ */
void TaskView::showEvent ( QShowEvent * event ) {

	// that the user can start typing without moving the focus
	if ( m_answerMixed == true )
		integer_edit->setFocus();
	else 
		numer_edit->setFocus();

}
