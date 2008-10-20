/***************************************************************************
                              exercisepercentage.h
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

#include "exercisepercentage.h"
#include "exercisepercentage.moc"

/* these includes are needed for KDE support */
#include <kglobal.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <knumvalidator.h>

/* these includes are needed for Qt support */
#include <QApplication>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>

//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

/* standard C++ library includes */
#include <stdlib.h>

#include "rationalwidget.h"
#include "resultwidget.h"
#include "settingsclass.h"

/* ----- public member functions ----- */

/* constructor */
ExercisePercentage::ExercisePercentage(QWidget * parent):
		ExerciseBase(parent)
{
#ifdef DEBUG
	kDebug() << "constructor ExercisePercentage()";
#endif

	/* create a new task */
	QApplication::setOverrideCursor(Qt::WaitCursor); /* show the sand clock */
	createTask();     
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// to validate, that the input is an int
	KIntValidator *valnum = new KIntValidator( this );

	QFont defaultFont = SettingsClass::taskFont();
	defaultFont.setBold( TRUE );

	// the next thing to do on a button click would be to check the entered
	// result
	m_currentState = _CHECK_TASK;

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
	taskLayout->setColumnStretch(3,1);

	checkLayout = new QGridLayout(this);
	checkLayout->setObjectName( "checkLayout" );

	// first left is the rational widget
	//m_rationalWidget = new RationalWidget(taskWidget, m_number, m_periodStart, m_periodLength);
	//m_rationalWidget->setObjectName("m_rationalWidget");
	//taskLayout->addWidget(m_rationalWidget, 1, 1, 3, 1);

	/* Task: percentage question */
	m_taskLabel = new QLabel( this );
	m_taskLabel->setObjectName( "m_taskLabel" );
	m_taskLabel->setFont(defaultFont);	
	m_taskLabel->setText(m_numberPercentage + " % of " + m_numberPercentageOf + " = ");
	taskLayout->addWidget( m_taskLabel, 1, 0);	

	/* Input question: result of question */
	answer_edit = new KLineEdit(taskWidget);
	answer_edit->setObjectName("answer_edit");
	answer_edit->setValidator( valnum ); // use the int validator
	answer_edit->setToolTip(i18n("Enter the result of percentage question"));
	answer_edit->setFixedSize(85,42);
	answer_edit->setAlignment(Qt::AlignHCenter);
	answer_edit->setFont(defaultFont);	
	//QObject::connect(answer_edit, SIGNAL(returnPressed(const QString &)), this,
	//	SLOT(numeratorReturnPressed(const QString &)));	
	taskLayout->addWidget(answer_edit, 1, 1);

	/* add input box so the user can enter numerator */
	//numer_edit = new KLineEdit(taskWidget);
	//numer_edit->setObjectName("numer_edit");
	//numer_edit->setValidator( valnum ); // use the int validator
	//numer_edit->setToolTip(i18n("Enter the numerator of your result"));
	//numer_edit->setFixedSize(85,42);
	//numer_edit->setAlignment(Qt::AlignHCenter);
	//numer_edit->setFont(defaultFont);	
	//QObject::connect(numer_edit, SIGNAL(returnPressed(const QString &)), this,
	//	SLOT(numeratorReturnPressed(const QString &)));	
	//taskLayout->addWidget(numer_edit, 1, 2);

	/* add a line between the edit boxes */
	//edit_line = new QFrame(taskWidget);
	//edit_line->setGeometry(QRect(100, 100, 20, 20));
	//edit_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	//taskLayout->addWidget(edit_line, 2, 2);

	/* add input box so the user can enter denominator */
	//deno_edit = new KLineEdit(taskWidget);
	//deno_edit->setObjectName("deno_edit");
	//deno_edit->setValidator( valnum ); // use the int validator
	//deno_edit->setToolTip(i18n("Enter the denominator of your result"));
	//deno_edit->setFixedSize(85,42);
	//deno_edit->setAlignment(Qt::AlignHCenter);
	//deno_edit->setFont(defaultFont);	
	//QObject::connect(deno_edit, SIGNAL(returnPressed(const QString &)), this,
	//	SLOT(numeratorReturnPressed(const QString &)));	
	//taskLayout->addWidget(deno_edit, 3, 2);

	// next is the result widget
	m_resultWidget = new ResultWidget(checkWidget, m_result);
	m_resultWidget->setObjectName("m_resultWidget");
	checkLayout->addWidget(m_resultWidget, 0, 0, 1, 2);

	defaultFont.setPointSize(10);

	// the right aligned button
	m_checkButton = new QPushButton( checkWidget );
	m_checkButton->setObjectName( "m_checkButton" );
	m_checkButton->setText(i18n("&Check"));
	m_checkButton->setDefault(true); // is the default button of the dialog
	m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));
	m_checkButton->setFixedSize(74,30);
	m_checkButton->setFont(defaultFont);		
		QObject::connect(m_checkButton, SIGNAL(clicked()), this, SLOT(slotCheckButtonClicked()));
	checkLayout->addWidget(m_checkButton, 2, 0);	
	
	// the right aligned button
	m_skipButton = new QPushButton( checkWidget );
	m_skipButton->setObjectName( "m_skipButton" );
	m_skipButton->setText(i18n("&Skip"));
	m_skipButton->setToolTip(i18n("Click on this button to skip this question."));
	m_skipButton->setFixedSize(74,30);
	m_skipButton->setFont(defaultFont);		
	QObject::connect(m_skipButton, SIGNAL(clicked()), this, SLOT(slotSkipButtonClicked()));
	checkLayout->addWidget(m_skipButton, 2, 1);	

	// that the user can start typing without moving the focus
	//numer_edit->setFocus();
         answer_edit->setFocus();

	setLayout(baseGrid);
	taskWidget->setLayout(taskLayout);
  	checkWidget->setLayout(checkLayout);

	// add tooltip and qwhatsthis help to the widget
        //TODO: change messages below 
	setToolTip(i18n("In this exercise you have to work with percentage questions."));
	//setWhatsThis( i18n("In this exercise you have to convert a given number into a fraction by entering numerator and denominator. Do not forget to reduce the result."));
}

/* destructor */
ExercisePercentage::~ExercisePercentage()
{
#ifdef DEBUG
	kDebug() << "destructor ExercisePercentage()";
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExercisePercentage::forceNewTask()
{
#ifdef DEBUG
	kDebug() << "forceNewTask ExercisePercentage()";
#endif

	if (m_currentState == _CHECK_TASK)
	{
		// emit the signal for skipped
		signalExerciseSkipped();
	}
	m_currentState = _CHECK_TASK;
	m_checkButton->setText(i18n("&Check"));

	// generate next task
	(void) nextTask();
}


/* ------ public slots ------ */

void ExercisePercentage::update()
{
	// call update of components
	//m_rationalWidget->updateAndRepaint();
	m_resultWidget->updateAndRepaint();

	// update for itself
	((QWidget *) this)->update();
}


/* ------ private member functions ------ */

void ExercisePercentage::createTask()
{
	// the tasks are hardcoded here; there are some algorithms to convert
	// rational numbers to fractions, but it is not worth the effort here
	switch(int((double(rand()) / RAND_MAX) * 18 + 1))
	{
		case  0 :   
			//m_number = KGlobal::locale()->formatNumber(0.5, 1);
			//m_periodStart = 2;
			//m_periodLength = 0;
			//m_result = ratio(1, 2);			
			m_numberPercentage = "75";
			m_numberPercentageOf = "1900";
			m_resultPercentage = "1425" ;
			break;
		case  1 :   
			//m_number = KGlobal::locale()->formatNumber(0.3, 1);
			//m_periodStart = 2;
			//m_periodLength = 1;
			//m_result = ratio(1, 3);			
			m_numberPercentage = "50";
			m_numberPercentageOf = "1800";
			m_resultPercentage = "900" ;
			break;
		case  2 :   
			//m_number = KGlobal::locale()->formatNumber(0.6, 1);
			//m_periodStart = 2;
			//m_periodLength = 1;
			m_result = ratio(2, 3);
			m_numberPercentage = "1";
			m_numberPercentageOf = "1200";
			m_resultPercentage = "12" ;


			break;
		case  3 :   
			m_number = KGlobal::locale()->formatNumber(0.25, 2);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(1, 4);

			m_numberPercentage = "10";
			m_numberPercentageOf = "900";
			m_resultPercentage = "90" ;


			break;
		case  4 :   
			m_number = KGlobal::locale()->formatNumber(0.75, 2);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(3, 4);

			m_numberPercentage = "100";
			m_numberPercentageOf = "800";
			m_resultPercentage = "800" ;


			break;
		case  5 :   
			m_number = KGlobal::locale()->formatNumber(0.2, 1);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(1, 5);
			
			m_numberPercentage = "75";
			m_numberPercentageOf = "300";
			m_resultPercentage = "225" ;

			break;
		case  6 :   
			m_number = KGlobal::locale()->formatNumber(0.4, 1);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(2, 5);
			
			m_numberPercentage = "10";
			m_numberPercentageOf = "1500";
			m_resultPercentage = "150" ;

			break;
		case  7 :   
			m_number = KGlobal::locale()->formatNumber(0.6, 1);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(3, 5);

			m_numberPercentage = "10";
			m_numberPercentageOf = "100";
			m_resultPercentage = "10" ;

			break;
		case  8 :   
			m_number = KGlobal::locale()->formatNumber(0.8, 1);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(4, 5);

			m_numberPercentage = "25";
			m_numberPercentageOf = "400";
			m_resultPercentage = "100" ;

			break;
		case  9 :
			m_number = KGlobal::locale()->formatNumber(0.16, 2);
			m_periodStart = 3;
			m_periodLength = 1;
			m_result = ratio(1, 6);

			m_numberPercentage = "50";
			m_numberPercentageOf = "800";
			m_resultPercentage = "400" ;

			break;
		case 10 :
			m_number = KGlobal::locale()->formatNumber(0.142857, 6);
			m_periodStart = 2;
			m_periodLength = 6;
			m_result = ratio(1, 7);

			m_numberPercentage = "1";
			m_numberPercentageOf = "400";
			m_resultPercentage = "1" ;

			break;
		case 11 :   
			m_number = KGlobal::locale()->formatNumber(0.125, 3);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(1, 8);

			m_numberPercentage = "50";
			m_numberPercentageOf = "600";
			m_resultPercentage = "300" ;

			break;
		case 12 :
			m_number = KGlobal::locale()->formatNumber(0.375, 3);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(3, 8);

			m_numberPercentage = "100";
			m_numberPercentageOf = "1300";
			m_resultPercentage = "1300" ;

			break;
		case 13 :  
			m_number = KGlobal::locale()->formatNumber(0.1, 1);
			m_periodStart = 2;
			m_periodLength = 1;
			m_result = ratio(1, 9);

			m_numberPercentage = "100";
			m_numberPercentageOf = "800";
			m_resultPercentage = "800" ;

			break;
		case 14 :   
			m_number = KGlobal::locale()->formatNumber(0.1, 1);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(1, 10);

			m_numberPercentage = "25";
			m_numberPercentageOf = "1400";
			m_resultPercentage = "350" ;

			break;
		case 15 :   
			m_number = KGlobal::locale()->formatNumber(0.05, 2);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(1, 20);

			m_numberPercentage = "10";
			m_numberPercentageOf = "1400";
			m_resultPercentage = "140" ;

			break;
		case 16 :   
			m_number = KGlobal::locale()->formatNumber(0.01, 2);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(1, 100);

			m_numberPercentage = "1";
			m_numberPercentageOf = "2000";
			m_resultPercentage = "20" ;

			break;
		case 17 :   
			m_number = KGlobal::locale()->formatNumber(0.83, 2);
			m_periodStart = 3;
			m_periodLength = 1;
			m_result = ratio(5, 6);

			m_numberPercentage = "75";
			m_numberPercentageOf = "1000";
			m_resultPercentage = "750" ;

			break;
		default :
		case 18 :   
			m_number = KGlobal::locale()->formatNumber(0.001, 3);
			m_periodStart = 2;
			m_periodLength = 0;
			m_result = ratio(1, 1000);			

			m_numberPercentage = "75";
			m_numberPercentageOf = "1100";
			m_resultPercentage = "825" ;

			break;
	}

	return;
}

/**	- checks, if the user solved the task correctly
		- emits signals if task was solved correctly or wrong */
void ExercisePercentage::showResult()
{
	QString tmp_str; /* to build a string for a label */
	QPalette pal;
	int tmp_result;
	ratio entered_result;
	ratio correct_result;

	// change the tooltip of the check button
	m_checkButton->setToolTip(i18n("Click on this button to get to the next question."));

	//numer_edit->setEnabled(false);
	//deno_edit->setEnabled(false);
	answer_edit->setEnabled(false);
	m_skipButton->setEnabled(false);	

	// an empty numerator field will be interpreted as 0
	//if (numer_edit->text().isEmpty() == true)
	//	numer_edit->setText("0");

	// an empty denominator field will be interpreted as 1
	//if (deno_edit->text().isEmpty() == true)
	//	deno_edit->setText("1");

	//an empty answer field will be interpreted as 0
	if (answer_edit->text().isEmpty() == true)
		answer_edit->setText("0");

	/* store the entered result to check it, but without reducing */
	//entered_result.setNumerator(numer_edit->text().toInt(), false);
	//entered_result.setDenominator(deno_edit->text().toInt(), false);

	// check the entered result; 0/1 == 0/5 -> true,
	// but 0/1 == 0/0 -> false
	// a 0 for denominator is never allowed (always counted as wrong)
	//
	// we have to get the 0 directly from the input field, because
	// Ratio::setDenominator(0, false) will set the denominator to 1 to ensure
	// the Ratio is valid

	tmp_result = answer_edit->text().toInt();
	entered_result.setNumerator(tmp_result, false);
	entered_result.setDenominator(1, false);
	correct_result.setNumerator(m_resultPercentage.toInt());
	correct_result.setDenominator(1, false);
	if (tmp_result == m_resultPercentage.toInt()) {
		// emit the signal for correct
		signalExerciseSolvedCorrect();
		/* yes, the user entered the correct result */
		m_resultWidget->setResult(entered_result, 1);
	} else {
		// emit the signal for incorrect
		signalExerciseSolvedWrong();
		/* no, the user entered the wrong result */
		m_resultWidget->setResult(correct_result, 0);
	}

	//if ( (deno_edit->text().toInt() != 0) && ((entered_result == m_result) ||
	//	  (m_result.numerator() == 0 && entered_result.numerator() == 0)) )
	//{
	//	// emit the signal for correct
	//	signalExerciseSolvedCorrect();

	//	/* yes, the user entered the correct result */
	//	m_resultWidget->setResult(m_result, 1);
	//} else {
	//	// emit the signal for wrong
	//	signalExerciseSolvedWrong();

	//	/* no, the user entered the wrong result */
	//	m_resultWidget->setResult(m_result, 0);

	//	// if the user entered a 0 for the denominator (division by 0) we have to
	//	// get the 0 directly from the input field, because
	//	// Ratio::setDenominator(0, true) will set the denominator to 1 to ensure
	//	// the Ratio is valid
	//	if (deno_edit->text().toInt() == 0)
	//	{
	//		KMessageBox::information(this,
	//		                         i18n("You entered a 0 as the denominator. This means division by zero, which is not allowed. This question will be counted as not correctly solved."));
	//	} else {
	//		/* maybe the entered ratio was not reduced */
	//		entered_result.reduce();
	//		if (entered_result == m_result)
	//			KMessageBox::information(this,
	//			                         i18n("You entered the correct result, but not reduced.\nAlways enter your results as reduced. This question will be counted as not correctly solved."));
	//	}
	//} /* if (entered_result == result) */

	return;
}

/** generate the next task and show it to the user */
void ExercisePercentage::nextTask()
{
	// change the tooltip of the check button
	m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));

	//numer_edit->setEnabled(true);
	//deno_edit->setEnabled(true);
	answer_edit->setEnabled(true);
	m_skipButton->setEnabled(true);		

	m_resultWidget->setResult( m_result, -1);

	/* clear user input */
	//deno_edit->setText("");
	//numer_edit->setText("");
	answer_edit->setText("");
	answer_edit->setFocus();
	//numer_edit->setFocus();

	/* create a new task */
	QApplication::setOverrideCursor(Qt::WaitCursor); /* show the sand clock */
	createTask();
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// update the task widget
	//m_rationalWidget->setRational(m_number, m_periodStart, m_periodLength);
	QString tempTask(m_numberPercentage + " % of " + m_numberPercentageOf + " = ");	
	m_taskLabel->setText(tempTask);

	return;
}

/* ------ private slots ------ */

void ExercisePercentage::slotCheckButtonClicked()
{
	if (m_currentState == _CHECK_TASK)
	{
		// if nothing has been entered by the user, we don't check the result yet
		//if (numer_edit->text().isEmpty() == true && deno_edit->text().isEmpty() ==
//true)
		if (answer_edit->text().isEmpty() == true)
			return;
		m_currentState = _NEXT_TASK;
		m_checkButton->setText(i18n("N&ext"));
		(void) showResult();
	} else {
		m_currentState = _CHECK_TASK;
		m_checkButton->setText(i18n("&Check"));
		(void) nextTask();
	}

	return;
}

void ExercisePercentage::slotSkipButtonClicked()
{
	forceNewTask();
}