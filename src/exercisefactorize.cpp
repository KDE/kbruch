/***************************************************************************
                              exercisefactorize.cpp
                             -------------------
    begin                : 2004/06/04
    copyright            : (C) 2004 by Sebastian Stein
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

#include "exercisefactorize.h"
#include "exercisefactorize.moc"

/* these includes are needed for KDE support */
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <knumvalidator.h>

/* these includes are needed for Qt support */
#include <qapplication.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
 #include <QToolButton>
 #include <qpainter.h>
 
//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QPaintEvent>

/* standard C++ library includes */
#include <stdlib.h>

#include "primenumber.h"
#include "rationalwidget.h"
#include "resultwidget.h"
#include "settingsclass.h"

/* ----- public member functions ----- */

/* constructor */
ExerciseFactorize::ExerciseFactorize(QWidget * parent):
		ExerciseBase(parent)
{
#ifdef DEBUG
	kDebug() << "constructor ExerciseFactorize()";
#endif

	/* create a new task */
	QApplication::setOverrideCursor(Qt::WaitCursor); /* show the sand clock */
	createTask();
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// the next thing to do on a button click would be to check the entered
	// result
	m_currentState = _CHECK_TASK;

	QFont defaultFont = SettingsClass::taskFont();

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
	taskLayout->setRowStretch(6,1);
	taskLayout->setColumnStretch(0,1);
	taskLayout->setColumnStretch(7,1);

	checkLayout = new QGridLayout(this);
	checkLayout->setObjectName( "checkLayout" );

	m_taskLabel = new QLabel( this );
	m_taskLabel->setObjectName( "taskLabel" );
	m_taskLabel->setFont(defaultFont);	
	taskLayout->addWidget( m_taskLabel, 1, 1 );

	defaultFont.setBold( true );
	defaultFont.setPointSize(16);

	m_equalSignLabel = new QLabel( this );
	m_equalSignLabel->setObjectName( "equalSignLabel" );
	m_equalSignLabel->setText("=");
	m_equalSignLabel->setFont(defaultFont);
	taskLayout->addWidget( m_equalSignLabel, 1, 2 );

	m_factorsEnteredEdit = new KLineEdit( this );
	m_factorsEnteredEdit->setObjectName( "factorsEnteredEdit" );
	m_factorsEnteredEdit->setReadOnly(true);
	QPalette pal;
	pal.setColor( m_factorsEnteredEdit->foregroundRole(), QColor(0, 0, 0) );
	m_factorsEnteredEdit->setPalette(pal);
	m_factorsEnteredEdit->setFont(defaultFont);
	m_factorsEnteredEdit->setFixedSize(320,35);	
	taskLayout->addWidget( m_factorsEnteredEdit, 1, 3, 1, 5 );

	defaultFont.setPointSize(10);

	m_factor2Button = new QPushButton( this );
	m_factor2Button->setObjectName( "factor2Button" );
	m_factor2Button->setFixedSize(50,35);	
	m_factor2Button->setFont(defaultFont);
	taskLayout->addWidget( m_factor2Button, 4, 3 );
	
	m_factor3Button = new QPushButton( this );
	m_factor3Button->setObjectName( "factor3Button" );
	m_factor3Button->setFixedSize(50,35);		
	m_factor3Button->setFont(defaultFont);
	taskLayout->addWidget( m_factor3Button, 4, 4 );

	m_factor5Button = new QPushButton( this );
	m_factor5Button->setObjectName( "factor5Button" );
	m_factor5Button->setFixedSize(50,35);
	m_factor5Button->setFont(defaultFont);
	taskLayout->addWidget( m_factor5Button, 4, 5 );

	m_factor7Button = new QPushButton( this );
	m_factor7Button->setObjectName( "factor7Button" );
	m_factor7Button->setFixedSize(50,35);
	m_factor7Button->setFont(defaultFont);
	taskLayout->addWidget( m_factor7Button, 4, 6 );

	m_factor11Button = new QPushButton( this );
	m_factor11Button->setObjectName( "factor11Button" );
	m_factor11Button->setFixedSize(50,35);
	m_factor11Button->setFont(defaultFont);
	taskLayout->addWidget( m_factor11Button, 5, 3 );
	
	m_factor13Button = new QPushButton( this );
	m_factor13Button->setObjectName( "factor13Button" );
	m_factor13Button->setFixedSize(50,35);
	m_factor13Button->setFont(defaultFont);
	taskLayout->addWidget( m_factor13Button, 5, 4 );

	m_factor17Button = new QPushButton( this );
	m_factor17Button->setObjectName( "factor17Button" );
	m_factor17Button->setFixedSize(50,35);
	m_factor17Button->setFont(defaultFont);
	taskLayout->addWidget( m_factor17Button, 5, 5 );

	m_factor19Button = new QPushButton( this );
	m_factor19Button->setObjectName( "factor19Button" );
	m_factor19Button->setFixedSize(50,35);
	m_factor19Button->setFont(defaultFont);
	taskLayout->addWidget( m_factor19Button, 5, 6 );

	m_removeLastFactorButton = new QPushButton( this );
	m_removeLastFactorButton->setObjectName( "removeLastFactor" );
	m_removeLastFactorButton->setFixedSize(115,35);
	m_removeLastFactorButton->setFont(defaultFont);
	m_removeLastFactorButton->setText(i18n("Backspace"));
	taskLayout->addWidget( m_removeLastFactorButton, 3, 4, 1, 3, Qt::AlignRight );

	defaultFont.setPointSize(10);

	m_skipButton = new QPushButton( this );
	m_skipButton->setObjectName( "m_skipButton" );
	m_skipButton->setText(i18n("&Skip"));
	m_skipButton->setToolTip(i18n("Click on this button to skip this question."));
	m_skipButton->setFont(defaultFont);		
	QObject::connect(m_skipButton, SIGNAL(clicked()), this, SLOT(slotSkipButtonClicked()));
	checkLayout->addWidget(m_skipButton, 1, 1);	

	// the check task button
	m_checkButton = new QPushButton( this );
	m_checkButton->setObjectName( "m_checkButton" );
	m_checkButton->setText( i18n( "&Check" ) );
	m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));
	QObject::connect(m_checkButton, SIGNAL(clicked()), this, SLOT(slotCheckButtonClicked()));
	m_checkButton->setFont(defaultFont);		
	checkLayout->addWidget(m_checkButton, 1, 0);			
	m_checkButton->setDefault(true); // is the default button of the dialog

	// next is the result widget
	m_resultWidget = new ResultWidget( this,  m_factorsResult );
	m_resultWidget->setObjectName("m_resultWidget");
	checkLayout->addWidget(m_resultWidget, 0, 0, 1, 2);

	// the current task
	QString tmp_str;
	tmp_str.setNum(m_taskNumber);
	m_taskLabel->setText(tmp_str);

	// now set the color for the task label
	pal = QPalette();
	pal.setColor( m_taskLabel->foregroundRole(), SettingsClass::numberColor() );
	m_taskLabel->setPalette( pal );

	// now set the color for the equal sign
	pal = QPalette();
	pal.setColor( m_equalSignLabel->foregroundRole(), SettingsClass::operationColor() );
	m_equalSignLabel->setPalette( pal );

	// the prime factor buttons
	m_factor2Button->setText( i18n( "2" ) );
	m_factor3Button->setText( i18n( "3" ) );
	m_factor5Button->setText( i18n( "5" ) );
	m_factor7Button->setText( i18n( "7" ) );
	m_factor11Button->setText( i18n( "11" ) );
	m_factor13Button->setText( i18n( "13" ) );
	m_factor17Button->setText( i18n( "17" ) );
	m_factor19Button->setText( i18n( "19" ) );
	QObject::connect(m_factor2Button, SIGNAL(clicked()), this, SLOT(slotFactor2ButtonClicked()));
	QObject::connect(m_factor3Button, SIGNAL(clicked()), this, SLOT(slotFactor3ButtonClicked()));
	QObject::connect(m_factor5Button, SIGNAL(clicked()), this, SLOT(slotFactor5ButtonClicked()));
	QObject::connect(m_factor7Button, SIGNAL(clicked()), this, SLOT(slotFactor7ButtonClicked()));
	QObject::connect(m_factor11Button, SIGNAL(clicked()), this, SLOT(slotFactor11ButtonClicked()));
	QObject::connect(m_factor13Button, SIGNAL(clicked()), this, SLOT(slotFactor13ButtonClicked()));
	QObject::connect(m_factor17Button, SIGNAL(clicked()), this, SLOT(slotFactor17ButtonClicked()));
	QObject::connect(m_factor19Button, SIGNAL(clicked()), this, SLOT(slotFactor19ButtonClicked()));

	// add tooltips to the factor buttons
	m_factor2Button->setToolTip(i18n("Add prime factor 2."));
	m_factor3Button->setToolTip(i18n("Add prime factor 3."));
	m_factor5Button->setToolTip(i18n("Add prime factor 5."));
	m_factor7Button->setToolTip(i18n("Add prime factor 7."));
	m_factor11Button->setToolTip(i18n("Add prime factor 11."));
	m_factor13Button->setToolTip(i18n("Add prime factor 13."));
	m_factor17Button->setToolTip(i18n("Add prime factor 17."));
	m_factor19Button->setToolTip(i18n("Add prime factor 19."));

	// the remove last factor button
	m_removeLastFactorButton->setEnabled(false);
	QObject::connect(m_removeLastFactorButton, SIGNAL(clicked()), this, SLOT(slotRemoveLastFactorButtonClicked()));
	m_removeLastFactorButton->setToolTip(i18n("Removes the last entered prime factor."));
	
	m_factor2Button->setFocusPolicy( Qt::NoFocus );
	m_factor3Button->setFocusPolicy( Qt::NoFocus );
	m_factor5Button->setFocusPolicy( Qt::NoFocus );
	m_factor7Button->setFocusPolicy( Qt::NoFocus );
	m_factor11Button->setFocusPolicy( Qt::NoFocus );
	m_factor13Button->setFocusPolicy( Qt::NoFocus );
	m_factor17Button->setFocusPolicy( Qt::NoFocus );
	m_factor19Button->setFocusPolicy( Qt::NoFocus );	
	m_removeLastFactorButton->setFocusPolicy( Qt::NoFocus );	
	m_skipButton->setFocusPolicy( Qt::NoFocus );	
	m_checkButton->setFocusPolicy( Qt::NoFocus );	

	m_checkButton->setDefault( true );

	setLayout(baseGrid);
	taskWidget->setLayout(taskLayout);
  	checkWidget->setLayout(checkLayout);

	// add tooltip and qwhatsthis help to the widget
	setToolTip(i18n("In this exercise you have to factorize a given number."));
	setWhatsThis( i18n("In this exercise you have to factorize a given number. You have to enter all prime factors of the number. You can add a prime factor by clicking on the corresponding button. The chosen prime factors will be shown in the input field. Do not forget to enter all prime factors, even when a prime factor repeats several times. Click the 'Backspace' button to remove the last entered prime factor."));

	m_edit = true;
}

/* destructor */
ExerciseFactorize::~ExerciseFactorize()
{
#ifdef DEBUG
	kDebug() << "destructor ExerciseFactorize()";
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExerciseFactorize::forceNewTask()
{
#ifdef DEBUG
	kDebug() << "forceNewTask ExerciseFactorize()";
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

void ExerciseFactorize::update()
{
	// now set the color for the task label
	QPalette pal;
	pal.setColor( m_taskLabel->foregroundRole(), SettingsClass::numberColor() );
	m_taskLabel->setPalette( pal );

	// now set the color for the equal sign
	pal = QPalette();
	pal.setColor( m_equalSignLabel->foregroundRole(), SettingsClass::operationColor() );
	m_equalSignLabel->setPalette( pal );

	// and the factors
	m_resultWidget->update();

	// update for itself
	((QWidget *) this)->update();
}


/* ------ private member functions ------ */

void ExerciseFactorize::createTask()
{
	uint uint_number;
	primenumber tmp_primenumber;

	// just pick one number out of the possible numbers to factorize
	m_taskNumber = possibleTasks[uint((double(rand()) / RAND_MAX) * numberPossibleTasks)];

	// now get the primefactors of the taskNumber
	m_factorsResult.clear();
	uint_number = m_taskNumber;
	tmp_primenumber.move_first();
	do
	{
		// check if the current primenumber is a divisor
		if (uint_number % tmp_primenumber.get_current() != 0)
		{
			// no, it is not a divisor, go on with next primenumber
			tmp_primenumber.move_forward();
		} else {
			// current primenumber is a divisor
			m_factorsResult.append(tmp_primenumber.get_current());
			uint_number = uint(uint_number / tmp_primenumber.get_current());
		}
	} while (uint_number != 1);

	return;
}

/**	- checks, if the user solved the task correctly
		- emits signals if task was solved correctly or wrong */
void ExerciseFactorize::showResult()
{
	QString tmp_str, tmp_str2; /* to build a string for a label */
	QPalette pal;
	uint uint_result = 0;

	// change the tooltip of the check button
	m_checkButton->setToolTip(i18n("Click on this button to get to the next question."));

	// disable prime factor buttons
	m_factor2Button->setEnabled(false);
	m_factor3Button->setEnabled(false);
	m_factor5Button->setEnabled(false);
	m_factor7Button->setEnabled(false);
	m_factor11Button->setEnabled(false);
	m_factor13Button->setEnabled(false);
	m_factor17Button->setEnabled(false);
	m_factor19Button->setEnabled(false);
	m_skipButton->setEnabled(false);		

	// disable factor removal button as well
	m_removeLastFactorButton->setEnabled(false);

	// show the result
	m_resultWidget->setFactors(m_factorsResult);

	// now calculate the product of the prime factors entered by the user
	for (int tmp_uint = 0; tmp_uint < m_factorsEntered.count(); tmp_uint++)
	{
		if (tmp_uint == 0)
		{
			uint_result = m_factorsEntered[0];
		} else {
			uint_result *= m_factorsEntered[tmp_uint];
		}
	}

	if (uint_result == m_taskNumber)
	{
		// emit the signal for correct
		signalExerciseSolvedCorrect();

		/* yes, the user entered the correct result */
		m_resultWidget->setResult( ratio(), 1 );
	} else {
		// emit the signal for wrong
		signalExerciseSolvedWrong();

		/* no, the user entered the wrong result */
		m_resultWidget->setResult( ratio(), 0 );
	} /* if (entered_result == result) */

	m_edit = false;

	return;
}

/** generate the next task and show it to the user */
void ExerciseFactorize::nextTask()
{
	// change the tooltip of the check button
	m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));

	// enable prime factor buttons
	m_factor2Button->setEnabled(true);
	m_factor3Button->setEnabled(true);
	m_factor5Button->setEnabled(true);
	m_factor7Button->setEnabled(true);
	m_factor11Button->setEnabled(true);
	m_factor13Button->setEnabled(true);
	m_factor17Button->setEnabled(true);
	m_factor19Button->setEnabled(true);
	m_skipButton->setEnabled(true);	

	// disable the factor removal button, there are no factors to be removed yet
	m_removeLastFactorButton->setEnabled(false);

	m_resultWidget->setResult( ratio(), -1 );

	/* clear user input */
	m_factorsEntered.clear();
	m_factorsResult.clear();
	m_factorsEnteredEdit->setText("");

	m_factorsEnteredEdit->setFocus();

	/* create a new task */
	QApplication::setOverrideCursor(Qt::WaitCursor); /* show the sand clock */
	createTask();
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// update the task widget
	QString tmp_str;
	tmp_str.setNum(m_taskNumber);
	m_taskLabel->setText(tmp_str);

	m_edit = true;

	return;
}

void ExerciseFactorize::addFactor(uint factor)
{
	// add the new entered factor
	m_factorsEntered.append(factor);

	// a factor was added, so the user can can remove one again
	m_removeLastFactorButton->setEnabled(true);

	// update the line edit
	updateEnteredEdit();

	return;
}

void ExerciseFactorize::updateEnteredEdit()
{
	// the string to be shown in the entered edit
	QString str_output = "";
	QString str_tmp;

	// find the end of the list
	uintList::iterator it;


	for (it = m_factorsEntered.begin(); it != m_factorsEntered.end(); ++it)
	{
		// convert the factor into a string
		str_tmp.setNum(*it);

		// add the factor with a *
		if (it == m_factorsEntered.begin())
		{
			str_output = str_tmp;
		} else {
			str_output += " * " + str_tmp;
		}
	}

	// set the text into the line edit
	m_factorsEnteredEdit->setText(str_output);

	return;
}

/* ------ private slots ------ */

void ExerciseFactorize::slotCheckButtonClicked()
{
	if (m_currentState == _CHECK_TASK)
	{
		// if nothing has been entered by the user, we don't check the result yet
		if (m_factorsEntered.count() == 0)
			return;
		m_currentState = _NEXT_TASK;
		m_checkButton->setText(i18n("N&ext"));
		(void) showResult();
	} else {
		m_currentState = _CHECK_TASK;
		m_checkButton->setText(i18n("&Check"));
		(void) nextTask();
	}

	// update the line edit
	updateEnteredEdit();
	return;
}

void ExerciseFactorize::slotSkipButtonClicked()
{
	forceNewTask();
	// update the line edit
	updateEnteredEdit();

	return;
}

void ExerciseFactorize::slotFactor2ButtonClicked()
{
	addFactor(2);

	return;
}

void ExerciseFactorize::slotFactor3ButtonClicked()
{
	addFactor(3);

	return;
}

void ExerciseFactorize::slotFactor5ButtonClicked()
{
	addFactor(5);

	return;
}

void ExerciseFactorize::slotFactor7ButtonClicked()
{
	addFactor(7);

	return;
}

void ExerciseFactorize::slotFactor11ButtonClicked()
{
	addFactor(11);

	return;
}

void ExerciseFactorize::slotFactor13ButtonClicked()
{
	addFactor(13);

	return;
}

void ExerciseFactorize::slotFactor17ButtonClicked()
{
	addFactor(17);

	return;
}

void ExerciseFactorize::slotFactor19ButtonClicked()
{
	addFactor(19);

	return;
}

void ExerciseFactorize::slotRemoveLastFactorButtonClicked()
{
	if (m_factorsEntered.count() > 0)
	{
		// find the end of the list
		uintList::iterator it = m_factorsEntered.end();
		--it;

		// remove last item
		m_factorsEntered.erase(it);
	}

	// if the list is empty, we have to disable the delete button
	if (m_factorsEntered.count() == 0)
		m_removeLastFactorButton->setEnabled(false);

	// update the line edit
	updateEnteredEdit();

	return;
}

/* ------ protected events ------ */
void ExerciseFactorize::keyPressEvent(QKeyEvent * e)
{
	if (m_edit == false)
		return;

	switch (e->key())
	{
		case Qt::Key_1:
			if (m_buffer != 1)
			{
				m_buffer = 1;
			}
			else
			{
				m_buffer = 0;
				addFactor(11);
			}
			break;

		case Qt::Key_2:
			m_buffer = 0;
			addFactor(2);
			break;

		case Qt::Key_3:
			if (m_buffer != 1)
			{
				addFactor(3);
			}
			else
			{
				addFactor(13);
			}
			m_buffer = 0;
			break;

		case Qt::Key_5:
			m_buffer = 0;
			addFactor(5);
			break;

		case Qt::Key_7:
			if (m_buffer == 1)
			{
				addFactor(17);
			}
			else
			{
				addFactor(7);
			}
			m_buffer = 0;
			break;

		case Qt::Key_9:
			if (m_buffer == 1)
			{
				addFactor(19);
			}
			m_buffer = 0;
			break;

		case Qt::Key_Delete:
		case Qt::Key_Backspace:
			m_buffer = 0;
			slotRemoveLastFactorButtonClicked();
			break;

		case Qt::Key_Return:
			m_buffer = 0;
			slotCheckButtonClicked();
			break;

		default:
			m_buffer = 0;
			break;
	}
}

void ExerciseFactorize::showEvent( QShowEvent * event )
{
#ifdef DEBUG
	kDebug() << "ExerciseFactorize::showEvent()";
#endif

	if ( isVisible() )
		m_factorsEnteredEdit->setFocus();
}
