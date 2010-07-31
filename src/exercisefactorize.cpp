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

#include "exercisefactorize.h"
#include "exercisefactorize.moc"

/* these includes are needed for KDE support */
#include <kapplication.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <knumvalidator.h>

/* these includes are needed for Qt support */
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqlineedit.h>
#include <tqpushbutton.h>
#include <tqtooltip.h>
#include <tqwhatsthis.h>

/* standard C++ library includes */
#include <stdlib.h>

#include "factorizedwidget.h"
#include "primenumber.h"
#include "rationalwidget.h"
#include "resultwidget.h"
#include "settingsclass.h"

/* ----- public member functions ----- */

/* constructor */
ExerciseFactorize::ExerciseFactorize(TQWidget * parent, const char * name):
		ExerciseBase(parent, name)
{
	TQPalette pal;
	TQColorGroup cg;
#ifdef DEBUG
	kdDebug() << "constructor ExerciseFactorize()" << endl;
#endif

	/* create a new task */
	TQApplication::setOverrideCursor(waitCursor); /* show the sand clock */
	createTask();
	TQApplication::restoreOverrideCursor(); /* show the normal cursor */

	// the next thing to do on a button click would be to check the entered
	// result
	m_currentState = _CHECK_TASK;

	Form1Layout = new TQVBoxLayout( this, 11, 6, "Form1Layout"); 

	layout9 = new TQVBoxLayout( 0, 0, 6, "layout9"); 
	
	// The following method fix the problem in
	// bug  #116831, reverse order in RTL desktops.
	// Amit Ramon amit.ramon@kdemail.net
	layout4 = createFactorsLayout();
	layout9->addLayout(layout4);
	
	spacer2 = new TQSpacerItem( 20, 21, TQSizePolicy::Minimum, TQSizePolicy::Expanding );
	layout9->addItem( spacer2 );
	
	layout2 = new TQVBoxLayout( 0, 0, 6, "layout2"); 
	
	// The following method fix the problem in
	// bug  #116831, reverse order in RTL desktops.
	// Amit Ramon amit.ramon@kdemail.net
	layout1 = createButtonsLayout();
	layout2->addLayout(layout1);

	m_removeLastFactorButton = new TQPushButton( this, "m_removeLastFactorButton" );
	layout2->addWidget( m_removeLastFactorButton );
	layout9->addLayout( layout2 );

	spacer4 = new TQSpacerItem( 20, 21, TQSizePolicy::Minimum, TQSizePolicy::Expanding );
	layout9->addItem( spacer4 );
	
	layout7 = new TQHBoxLayout( 0, 0, 6, "layout7"); 
	spacer3 = new TQSpacerItem( 361, 20, TQSizePolicy::Expanding, TQSizePolicy::Minimum );
	layout7->addItem( spacer3 );
	
	m_checkButton = new TQPushButton( this, "m_checkButton" );
	layout7->addWidget( m_checkButton );
	layout9->addLayout( layout7 );
	Form1Layout->addLayout( layout9 );

	// the current task
	TQString tmp_str;
	tmp_str.setNum(m_taskNumber);
	m_taskLabel->setText(tmp_str);

	// now set the color for the task label
	m_taskLabel->setPaletteForegroundColor(SettingsClass::numberColor());

	// the equal sign
	m_equalSignLabel->setText("=");

	// now set the color for the equal sign
	m_equalSignLabel->setPaletteForegroundColor(SettingsClass::operationColor());

	// the wrong/correct label, we hide it
	result_label->setText(i18n("WRONG"));
	result_label->hide();

	// the prime factor buttons
	m_factor2Button->setText( i18n( "2" ) );
	m_factor3Button->setText( i18n( "3" ) );
	m_factor5Button->setText( i18n( "5" ) );
	m_factor7Button->setText( i18n( "7" ) );
	m_factor11Button->setText( i18n( "11" ) );
	m_factor13Button->setText( i18n( "13" ) );
	m_factor17Button->setText( i18n( "17" ) );
	m_factor19Button->setText( i18n( "19" ) );
	TQObject::connect(m_factor2Button, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotFactor2ButtonClicked()));
	TQObject::connect(m_factor3Button, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotFactor3ButtonClicked()));
	TQObject::connect(m_factor5Button, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotFactor5ButtonClicked()));
	TQObject::connect(m_factor7Button, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotFactor7ButtonClicked()));
	TQObject::connect(m_factor11Button, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotFactor11ButtonClicked()));
	TQObject::connect(m_factor13Button, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotFactor13ButtonClicked()));
	TQObject::connect(m_factor17Button, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotFactor17ButtonClicked()));
	TQObject::connect(m_factor19Button, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotFactor19ButtonClicked()));

	// add tooltips to the factor buttons
	TQToolTip::add(m_factor2Button, i18n("Add prime factor 2."));
	TQToolTip::add(m_factor3Button, i18n("Add prime factor 3."));
	TQToolTip::add(m_factor5Button, i18n("Add prime factor 5."));
	TQToolTip::add(m_factor7Button, i18n("Add prime factor 7."));
	TQToolTip::add(m_factor11Button, i18n("Add prime factor 11."));
	TQToolTip::add(m_factor13Button, i18n("Add prime factor 13."));
	TQToolTip::add(m_factor17Button, i18n("Add prime factor 17."));
	TQToolTip::add(m_factor19Button, i18n("Add prime factor 19."));

	// the remove last factor button
	m_removeLastFactorButton->setText( i18n( "&Remove Last Factor" ) );
	m_removeLastFactorButton->setEnabled(false);
	TQObject::connect(m_removeLastFactorButton, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotRemoveLastFactorButtonClicked()));
	TQToolTip::add(m_removeLastFactorButton, i18n("Removes the last entered prime factor."));

	// the check task button
	m_checkButton->setText( i18n( "&Check Task" ) );
	TQToolTip::add(m_checkButton, i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));
	TQObject::connect(m_checkButton, TQT_SIGNAL(clicked()), this, TQT_SLOT(slotCheckButtonClicked()));
	m_checkButton->setDefault(true); // is the default button of the dialog

	// that the user can start choosing the prime factors
	m_factor2Button->setFocus();

	// set the tab order
	setTabOrder(m_factor2Button, m_factor3Button);
	setTabOrder(m_factor3Button, m_factor5Button);
	setTabOrder(m_factor5Button, m_factor7Button);
	setTabOrder(m_factor7Button, m_factor11Button);
	setTabOrder(m_factor13Button, m_factor17Button);
	setTabOrder(m_factor17Button, m_factor19Button);
	setTabOrder(m_factor19Button, m_removeLastFactorButton);

	// add tooltip and qwhatsthis help to the widget
	TQToolTip::add(this, i18n("In this exercise you have to factorize a given number."));
	TQWhatsThis::add(this, i18n("In this exercise you have to factorize a given number. You have to enter all prime factors of the number. You can add a prime factor by clicking on the corresponding button. The chosen prime factors will be shown in the input field. Do not forget to enter all prime factors, even when a prime factor repeats several times!"));
}

/* destructor */
ExerciseFactorize::~ExerciseFactorize()
{
#ifdef DEBUG
	kdDebug() << "destructor ExerciseFactorize()" << endl;
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExerciseFactorize::forceNewTask()
{
#ifdef DEBUG
	kdDebug() << "forceNewTask ExerciseFactorize()" << endl;
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

void ExerciseFactorize::update()
{
	// now set the color for the task label
	m_taskLabel->setPaletteForegroundColor(SettingsClass::numberColor());

	// now set the color for the equal sign
	m_equalSignLabel->setPaletteForegroundColor(SettingsClass::operationColor());

	// and the factors
	m_factorsWidget->updateAndRepaint();

	// update for itself
	((TQWidget *) this)->update();
}


/* ------ private member functions ------ */

//
// The following method was added to fix
// bug #116831 (reverse layout in RTL desktops)
// Amit Ramon amit.ramon@kdemail.net
//

/** Create the layout that hold the exercise widgets
 */
TQHBoxLayout* ExerciseFactorize::createFactorsLayout()
{
  // first create all widgets
  TQHBoxLayout* layout = new TQHBoxLayout( 0, 0, 6, "layout4"); 
  
  m_taskLabel = new TQLabel( this, "m_taskLabel" );

  m_equalSignLabel = new TQLabel( this, "m_equalSignLabel" );

  m_factorsEnteredEdit = new TQLineEdit( this, "m_factorsEnteredEdit" );
  m_factorsEnteredEdit->setReadOnly(true);
  m_factorsEnteredEdit->setEnabled(false);
  m_factorsEnteredEdit->setPaletteForegroundColor(TQColor(0, 0, 0));
	
  m_factorsWidget =
    new FactorizedWidget( this, "m_factorsWidget", m_factorsResult);

  m_factorsWidget->hide();
  
  result_label = new TQLabel( this, "result_label" );

  spacer1 =  new TQSpacerItem( 25, 20, TQSizePolicy::Expanding,
				    TQSizePolicy::Minimum );

  // now add the widgets to the layout.
  // if we are in a RTL desktop, add them
  // in a reverse order
  if (TQApplication::reverseLayout())
    {
      layout->addItem( spacer1 );
      layout->addWidget( result_label );
      layout->addWidget( m_factorsWidget );
      layout->addWidget( m_factorsEnteredEdit );
      layout->addWidget( m_equalSignLabel );
      layout->addWidget( m_taskLabel );
    }
  else
    {
      layout->addWidget( m_taskLabel );
      layout->addWidget( m_equalSignLabel );
      layout->addWidget( m_factorsEnteredEdit );
      layout->addWidget( m_factorsWidget );
      layout->addWidget( result_label );
      layout->addItem( spacer1 );
    }
  
  return layout;
}


//
// The following method was added to fix
// bug #116831 (reverse layout in RTL desktops)
// Amit Ramon amit.ramon@kdemail.net
//

/** Create the layout that hold the exercise widgets
 */
TQGridLayout* ExerciseFactorize::createButtonsLayout()
{
  const int _COLS = 4; // number of buttons in each row
  const int _ROWS = 2; // number of rows

  TQGridLayout* layout = new TQGridLayout( 0, 1, 1, 0, 6, "layout1"); 
	
  // first row buttons
  m_factor2Button = new TQPushButton( this, "m_factor2Button" );
  m_factor3Button = new TQPushButton( this, "m_factor3Button" );
  m_factor5Button = new TQPushButton( this, "m_factor5Button" );
  m_factor7Button = new TQPushButton( this, "m_factor7Button" );

  // second row buttons
  m_factor11Button = new TQPushButton( this, "m_factor11Button" );
  m_factor13Button = new TQPushButton( this, "m_factor13Button" );	
  m_factor17Button = new TQPushButton( this, "m_factor17Button" );
  m_factor19Button = new TQPushButton( this, "m_factor19Button" );

  // temp array to help with adding the buttons
  // to the grid
  TQPushButton* buttons[_ROWS][_COLS] = 
    {
      {
	m_factor2Button,
	m_factor3Button,
	m_factor5Button,
	m_factor7Button
      },
      {
	m_factor11Button,
	m_factor13Button,
	m_factor17Button,
	m_factor19Button
      }
    };
  

  int buttonIdxStart = 0;
  int step = 1;

  // if we are in a RTL desktop, this helps adding the
  // buttons in a reverse order
  if (TQApplication::reverseLayout())
    {
      buttonIdxStart = _COLS - 1;
      step = -1;
    }

  // now add the buttons to the grid
  for (int row = 0; row < _ROWS; row++)
    {
      int buttonIdx = buttonIdxStart;
      
      for (int col = 0; col < _COLS; col++)
	{
	  layout->addWidget(buttons[row][buttonIdx], row, col );
	  buttonIdx += step;
	}
    }
  
  return layout;
}


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
	TQString tmp_str, tmp_str2; /* to build a string for a label */
	TQPalette pal;
	TQColorGroup cg;
	uint uint_result = 0;

	// change the tooltip of the check button
	TQToolTip::add(m_checkButton, i18n("Click on this button to get to the next task."));

	// disable prime factor buttons
	m_factor2Button->setEnabled(false);
	m_factor3Button->setEnabled(false);
	m_factor5Button->setEnabled(false);
	m_factor7Button->setEnabled(false);
	m_factor11Button->setEnabled(false);
	m_factor13Button->setEnabled(false);
	m_factor17Button->setEnabled(false);
	m_factor19Button->setEnabled(false);

	// disable factor removal button as well
	m_removeLastFactorButton->setEnabled(false);

	// show the result
	m_factorsWidget->setFactors(m_factorsResult);
	m_factorsWidget->show();

	// now calculate the product of the prime factors entered by the user
	for (uint tmp_uint = 0; tmp_uint < m_factorsEntered.count(); tmp_uint++)
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
		result_label->setText(i18n("CORRECT"));
		pal = result_label->palette(); /* set green font color */
		cg = pal.active();
		cg.setColor(TQColorGroup::Foreground, TQColor(6, 179, 0));
		pal.setActive(cg);
		cg = pal.inactive();
		cg.setColor(TQColorGroup::Foreground, TQColor(6, 179, 0));
		pal.setInactive(cg);
		result_label->setPalette(pal);
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

	} /* if (entered_result == result) */

	result_label->show(); /* show the result at the end of the task */

	return;
}

/** generate the next task and show it to the user */
void ExerciseFactorize::nextTask()
{
	// change the tooltip of the check button
	TQToolTip::add(m_checkButton, i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));

	// enable prime factor buttons
	m_factor2Button->setEnabled(true);
	m_factor3Button->setEnabled(true);
	m_factor5Button->setEnabled(true);
	m_factor7Button->setEnabled(true);
	m_factor11Button->setEnabled(true);
	m_factor13Button->setEnabled(true);
	m_factor17Button->setEnabled(true);
	m_factor19Button->setEnabled(true);

	// disable the factor removal button, there are no factors to be removed yet
	m_removeLastFactorButton->setEnabled(false);

	result_label->hide(); /* do not show the result at the end of the task */
	m_factorsWidget->hide();

	/* clear user input */
	m_factorsEntered.clear();
	m_factorsResult.clear();
	m_factorsEnteredEdit->setText("");

	m_factor2Button->setFocus();

	/* create a new task */
	TQApplication::setOverrideCursor(waitCursor); /* show the sand clock */
	createTask();
	TQApplication::restoreOverrideCursor(); /* show the normal cursor */

	// update the task widget
	TQString tmp_str;
	tmp_str.setNum(m_taskNumber);
	m_taskLabel->setText(tmp_str);

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
	TQString str_output = "";
	TQString str_tmp;

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
		m_checkButton->setText(i18n("N&ext Task"));
		(void) showResult();
	} else {
		m_currentState = _CHECK_TASK;
		m_checkButton->setText(i18n("&Check Task"));
		(void) nextTask();
	}

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
		m_factorsEntered.remove(it);
	}

	// if the list is empty, we have to disable the delete button
	if (m_factorsEntered.count() == 0)
		m_removeLastFactorButton->setEnabled(false);

	// update the line edit
	updateEnteredEdit();

	return;
}
