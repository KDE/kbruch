/***************************************************************************
                          mainqtwidget.cpp  -  The main Qt/KDE window
                             -------------------
    begin                : Tue Mar 16 00:00:00 CET 2003
    copyright            : (C) 2003 by Sebastian Stein
    email                : kbruch@hpfsc.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "mainqtwidget.h"

#include <config.h>

#include <kaccel.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kmenubar.h>
#include <kpopupmenu.h>
#include <kaction.h>
#include <kstdaction.h>

#include <qsplitter.h>
#include <qcombobox.h>

#include "taskview.h"
#include "statisticsview.h"

/* ------ public member functions ------ */

MainQtWidget::MainQtWidget()
{
#ifdef DEBUG
	kdDebug() << "constructor MainQtWidget" << endl;
#endif
	// creating KActions, used by the kbruchui.rc file
	setupActions();

	createGUI(0L);

	// we split the main view into 2 parts, one for the tasks, one for the
	// statistics
	QSplitter* splitter = new QSplitter(QSplitter::Horizontal, this,"QSplitter");
	setCentralWidget(splitter);

	// create the statistic view
	m_statview = new StatisticsView(splitter, "StatisticsView");

	// setting default values for the first task
	m_addSub = true;
	m_mulDiv = false;
	m_nrRatios = 2;
	m_maxMainDenominator = 10;

	// create the task view with the given defaults
	m_taskview = new TaskView(splitter,"TaskView", m_addSub, m_mulDiv, m_nrRatios, m_maxMainDenominator);

	splitter->setResizeMode(m_statview, QSplitter::FollowSizeHint);

	// connect TaskView and StatisticView, so that StatisticView gets informed
	// about how the user solved a given task (wrong or correct)
	QObject::connect(m_taskview, SIGNAL(signalTaskSolvedCorrect()), m_statview, SLOT(addCorrect()));
	QObject::connect(m_taskview, SIGNAL(signalTaskSolvedWrong()), m_statview, SLOT(addWrong()));

	resize(QSize(QMAX(toolBar()->sizeHint().width(), sizeHint().width()), sizeHint().height()));

}

MainQtWidget::~MainQtWidget()
{
}


/* ------ private member functions ------ */

void MainQtWidget::setupActions()
{
	// new task action
	m_NewTaskAction = new KAction(i18n("&New Task"), "filenew", KStdAccel::shortcut(KStdAccel::New), 
				      this, SLOT(NewTask()), 
				      actionCollection(), "NewTask");

	// quit action
	KStdAction::quit(kapp, SLOT(quit()), actionCollection());

	// a label just describing the Number of terms ComboBox
	m_NrOfTermsLabel = new QLabel(i18n("Terms:"), 0, "kde toolbar widget");
	m_NrOfTermsLabelAction = new KWidgetAction(m_NrOfTermsLabel, i18n("Terms:"), ALT+Key_T, 
						   this, SLOT(NrOfTermsBoxSlot()), 
						   actionCollection(), "NrOfTermsLabelAction");

	// the ComboBox holding possible values for term number
	m_NrOfTermsBox = new QComboBox();
	m_NrOfTermsBox->insertItem("2");
	m_NrOfTermsBox->insertItem("3");
	m_NrOfTermsBox->insertItem("4");
	m_NrOfTermsBox->insertItem("5");
	m_NrOfTermsBoxAction = new KWidgetAction(m_NrOfTermsBox, i18n("Number of terms"), ALT+Key_T, 
						 this, SLOT(NrOfTermsBoxSlot()), 
						 actionCollection(), "NrOfTermsBoxAction");

	// now connect the ComboBox's signal textChanged() to the slot function
	QObject::connect(m_NrOfTermsBox, SIGNAL(activated(int)), this, SLOT(NrOfTermsBoxSlot()));

	// a label just describing the max. main denominator ComboBox
	m_MaxMainDenominatorLabel = new QLabel(i18n("Max. Main Denominator:"), 0, "kde toolbar widget");
	m_MaxMainDenominatorLabelAction = new KWidgetAction(m_MaxMainDenominatorLabel, i18n("Max. Main Denominator:"), ALT+Key_T, 
							    this, SLOT(MaxMainDenominatorBoxSlot()), 
							    actionCollection(), "MaxMainDenominatorLabelAction");

	// the ComboBox holding possible values for the max. main denominator
	m_MaxMainDenominatorBox = new QComboBox();
	m_MaxMainDenominatorBox->insertItem("10");
	m_MaxMainDenominatorBox->insertItem("20");
	m_MaxMainDenominatorBox->insertItem("30");
	m_MaxMainDenominatorBox->insertItem("50");
	m_MaxMainDenominatorBoxAction = new KWidgetAction(m_MaxMainDenominatorBox, i18n("Maximal main denominator"), ALT+Key_T, 
							  this, SLOT(MaxMainDenominatorBoxSlot()), 
							  actionCollection(), "MaxMainDenominatorBoxAction");

	// now connect the ComboBox's signal textChanged() to the slot function
	QObject::connect(m_MaxMainDenominatorBox, SIGNAL(activated(int)),
			 this, SLOT(MaxMainDenominatorBoxSlot()));

	// a label just describing the operation ComboBox
	m_OperationLabel = new QLabel(i18n("Operations:"), 0, "kde toolbar widget");
	m_OperationLabelAction = new KWidgetAction(m_OperationLabel, i18n("Operations:"), ALT+Key_O,
						   this, SLOT(OperationBoxSlot()), 
						   actionCollection(), "OperationLabelAction");

	// the ComboBox holding possible combinations for operations
	m_OperationBox = new QComboBox();
	m_OperationBox->insertItem(i18n("Addition/Subtraction"));
	m_OperationBox->insertItem(i18n("Multiplication/Division"));
	m_OperationBox->insertItem(i18n("All Operations Mixed"));
	m_OperationBoxAction = new KWidgetAction(m_OperationBox, i18n("Operations:"), ALT+Key_O,
						 this, SLOT(OperationBoxSlot()), 
						 actionCollection(), "OperationBoxAction");

	// now connect the ComboBox's signal textChanged() to the slot function
	QObject::connect(m_OperationBox, SIGNAL(activated(int)), this, SLOT(OperationBoxSlot()));
}


/* ------ private slots ------ */

/** called to force generation of a new task */
void MainQtWidget::NewTask()
{
	// generate a new task
	(void) m_taskview->forceNewTask();
}

/** called, when the user changes the number of terms in the ComboBox */
void MainQtWidget::NrOfTermsBoxSlot()
{
#ifdef DEBUG
	kdDebug() << "MainQtWidget::NrOfTermsBoxSlot()" << endl;
#endif
	QString curr_nr = m_NrOfTermsBox->currentText();
	m_MaxMainDenominatorBox->clear();

	if (curr_nr == "2")
	{
		m_MaxMainDenominatorBox->insertItem("10");
		m_MaxMainDenominatorBox->insertItem("20");
		m_MaxMainDenominatorBox->insertItem("30");
		m_MaxMainDenominatorBox->insertItem("50");
		m_nrRatios = 2;
	} else if (curr_nr == "3") {
		m_MaxMainDenominatorBox->insertItem("20");
		m_MaxMainDenominatorBox->insertItem("30");
		m_MaxMainDenominatorBox->insertItem("50");
		m_nrRatios = 3;
	} else if (curr_nr == "4") {
		m_MaxMainDenominatorBox->insertItem("20");
		m_MaxMainDenominatorBox->insertItem("30");
		m_MaxMainDenominatorBox->insertItem("50");
		m_nrRatios = 4;
	} else {
		m_MaxMainDenominatorBox->insertItem("50");
		m_nrRatios = 5;
	}
	// set the new task parameters
	(void) m_taskview->setTaskParameters(m_addSub, m_mulDiv, m_nrRatios, m_maxMainDenominator);
}

/** called, when the user changes the max. size of the main denominator in
 * the ComboBox */
void MainQtWidget::MaxMainDenominatorBoxSlot()
{
	// get the max. size from the ComboBox, convert it to a number and store
	// it in the private member
	QString curr_md = m_MaxMainDenominatorBox->currentText();
	m_maxMainDenominator = curr_md.toUInt();

	// set the new task parameters
	(void) m_taskview->setTaskParameters(m_addSub, m_mulDiv, m_nrRatios, m_maxMainDenominator);
}

/** called, when the user changes the wished task's operations */
void MainQtWidget::OperationBoxSlot()
{
	int index = m_OperationBox->currentItem(); // get selected item

	// user has selected the operations for the next task, so store it in the
	// private members
	if (index == 0)
	{
		m_addSub = true;
		m_mulDiv = false;
	} else if (index == 1) {
		m_addSub = false;
		m_mulDiv = true;
	} else {
		m_addSub = true;
		m_mulDiv = true;
	}

	// set the new task parameters
	(void) m_taskview->setTaskParameters(m_addSub, m_mulDiv, m_nrRatios, m_maxMainDenominator);
}

#include "mainqtwidget.moc"
