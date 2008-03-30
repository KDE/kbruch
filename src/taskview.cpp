/***************************************************************************
                          taskview.cpp  -  The task window
                             -------------------
    begin                : Tue Feb 08 13:41:00 CET 2002
    copyright            : (C) 2001 - 2004 by Sebastian Stein
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

#include "taskview.h"
#include "taskview.moc"

/* these includes are needed for KDE support */
#include <klocale.h>
#include <kmessagebox.h>
#include <knumvalidator.h>
#include <klineedit.h>

/* these includes are needed for Qt support */
#include <qapplication.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>

//Added by qt3to4:
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

/* standard C++ library includes */
#include <math.h>

/* ----- public member functions ----- */

/* constructor */
TaskView::TaskView(QWidget * parent, bool padd_sub,
                  bool pmul_div, unsigned int pnr_ratios, unsigned int pmax_md):
		ExerciseBase(parent), add_sub(padd_sub), mul_div(pmul_div),
		nr_ratios(pnr_ratios), max_md(pmax_md)
{
#ifdef DEBUG
	kDebug() << "constructor TaskView()";
#endif

	curr_nr_ratios = nr_ratios;

	/* create a new task */
	QApplication::setOverrideCursor(Qt::WaitCursor); /* show the sand clock */
	current_task.create_task(max_md, nr_ratios, add_sub, mul_div);
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// the next thing to do on a button click would be to check the entered
	// result
	m_currentState = _CHECK_TASK;

	baseWidget = new QWidget(this);
	baseWidget->setObjectName("baseWidget");
	baseGrid = new QGridLayout(this );
	baseGrid->setObjectName( "baseGrid" );
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
	taskLineHBoxLayout->setMargin(5);
	taskLineHBoxLayout->setObjectName("taskLineHBoxLayout");
	realLayout->addLayout(taskLineHBoxLayout);

	// first left is the task widget
	m_taskWidget = new TaskWidget(baseWidget, current_task);
	m_taskWidget->setObjectName("m_taskWidget");
	taskLineHBoxLayout->addWidget(m_taskWidget);

	// now we have the input fields aligned in a VBox
	QVBoxLayout * inputLayout = new QVBoxLayout();
	inputLayout->setObjectName("inputLayout");
	inputLayout->setSpacing(5);
	taskLineHBoxLayout->addLayout(inputLayout);

	// to validate, that the input is an int
	KIntValidator *valnum = new KIntValidator( this );

	/* add input box so the user can enter numerator */
	numer_edit = new KLineEdit(baseWidget);
	numer_edit->setObjectName("numer_edit");
	numer_edit->setValidator( valnum ); // use the int validator
	numer_edit->setToolTip(i18n("Enter the numerator of your result"));
	inputLayout->addWidget(numer_edit);

	/* add a line between the edit boxes */
	edit_line = new QFrame(baseWidget);
	edit_line->setGeometry(QRect(100, 100, 20, 20));
	edit_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	inputLayout->addWidget(edit_line);

	/* add input box so the user can enter denominator */
	deno_edit = new KLineEdit(baseWidget);
	deno_edit->setObjectName("deno_edit");
	deno_edit->setValidator( valnum ); // use the int validator
	deno_edit->setToolTip(i18n("Enter the denominator of your result"));
	inputLayout->addWidget(deno_edit);

	// next is the result widget
	m_resultWidget = new ResultWidget(baseWidget, ratio());
	m_resultWidget->setObjectName("m_resultWidget");
	taskLineHBoxLayout->addWidget(m_resultWidget);

	// at the right end we have a label just showing CORRECT or WRONG
	result_label = new QLabel(baseWidget);
	result_label->setObjectName("result_label");
	//result_label->setText(i18n("WRONG")); // we don't need this, as it's hidden anyway (jpw)
	taskLineHBoxLayout->addWidget(result_label);
	result_label->hide();

	// add another spacer in the middle of the VBox
	v_spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
	taskLineHBoxLayout->addItem(v_spacer);

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
	m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));
	lowerHBox->addWidget(m_checkButton, 1, Qt::AlignRight);
	QObject::connect(m_checkButton, SIGNAL(clicked()), this, SLOT(slotCheckButtonClicked()));

	// that the user can start typing without moving the focus
	numer_edit->setFocus();

	// show the whole layout
	baseWidget->show();

	// show the whole layout
	m_taskWidget->show();
	m_resultWidget->hide();

	// add tooltip and qwhatsthis help to the widget
	setToolTip(i18n("In this exercise you have to solve a given task with fractions."));
	setWhatsThis( i18n("In this exercise you have to solve the generated task. You have to enter numerator and denominator. You can adjust the difficulty of the task with the boxes in the toolbar. Do not forget to reduce the result."));
}

/* destructor */
TaskView::~TaskView()
{
#ifdef DEBUG
	kDebug() << "destructor TaskView()";
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

/** the parameters of task generation can be set with this function */
void TaskView::setTaskParameters(bool padd_sub, bool pmul_div, unsigned int pnr_ratios, unsigned int pmax_md)
{
	// at least one operation must be enabled
	if ((padd_sub == false) && (pmul_div == false))
		padd_sub = true;

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
	add_sub = padd_sub;
	mul_div = pmul_div;
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
		// emit the signal for wrong
		signalTaskSolvedWrong();
	}
	m_currentState = _CHECK_TASK;
	m_checkButton->setText(i18n("&Check Task"));

	// generate next task
	(void) nextTask();
}


/* ------ public slots ------ */

void TaskView::update()
{
	// call update of components
	m_taskWidget->updateAndRepaint();
	m_resultWidget->updateAndRepaint();

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

	// change the tooltip of the check button
	m_checkButton->setToolTip(i18n("Click on this button to get to the next task."));

	numer_edit->setEnabled(false);
	deno_edit->setEnabled(false);

	result = current_task.solve();
	m_resultWidget->setResult(result);
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
	if ( (deno_edit->text().toInt() != 0) && ((entered_result == result) ||
		  (result.numerator() == 0 && entered_result.numerator() == 0)) )
	{
		// emit the signal for correct
		signalTaskSolvedCorrect();

		/* yes, the user entered the correct result */
		result_label->setText(i18nc("@info:status the answer given was correct", "CORRECT"));
		pal = result_label->palette(); /* set green font color */
        pal.setColor(QPalette::Active, QPalette::Foreground, QColor(6, 179, 0));
        pal.setColor(QPalette::Inactive, QPalette::Foreground, QColor(6, 179, 0));
		result_label->setPalette(pal);
		result_label->show(); /* show the result at the end of the task */
	} else {
		// emit the signal for wrong
		signalTaskSolvedWrong();

		/* no, the user entered the wrong result */
		result_label->setText(i18nc("@info:status the answer given was incorrect", "WRONG"));
		pal = result_label->palette(); /* set red font color */
        pal.setColor(QPalette::Active, QPalette::Foreground, QColor(Qt::red));
        pal.setColor(QPalette::Inactive, QPalette::Foreground, QColor(Qt::red));
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
			if (entered_result == result)
				KMessageBox::information(this,
				                         i18n("You entered the correct result, but not reduced.\nAlways enter your results as reduced. This task will be counted as not correctly solved."));
		}
	} /* if (entered_result == result) */
}

/** generate the next task and show it to the user */
void TaskView::nextTask()
{
	// change the tooltip of the check button
	m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));

	numer_edit->setEnabled(true);
	deno_edit->setEnabled(true);

	result_label->hide(); /* do not show the result at the end of the task */
	m_resultWidget->hide();

	/* clear user input */
	deno_edit->setText("");
	numer_edit->setText("");
	numer_edit->setFocus();

	/* create a new task */
	QApplication::setOverrideCursor(Qt::WaitCursor); /* show the sand clock */
	current_task.create_task(max_md, nr_ratios, add_sub, mul_div);
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	// update the task widget
	m_taskWidget->setTask((const task) (current_task));
}

/* ------ private slots ------ */

void TaskView::slotCheckButtonClicked()
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
}
