/***************************************************************************
                          mainqtwidget.cpp  -  The main Qt/KDE window
                             -------------------
    begin                : Tue Mar 16 00:00:00 CET 2003
    copyright            : (C) 2003-2004 by Sebastian Stein
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
#include "mainqtwidget.h"

#include <config.h>

#include <kaccel.h>
#include <kapplication.h>
#include <kdebug.h>
#include <kaction.h>
#include <klocale.h>

#include <qsplitter.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#include "taskview.h"
#include "statisticsview.h"

#include "settingsclass.h"

/* ------ public member functions ------ */

MainQtWidget::MainQtWidget()
{
#ifdef DEBUG
	kdDebug() << "constructor MainQtWidget" << endl;
#endif

	// get the settings
	readOptions();

	// creating KActions, used by the kbruchui.rc file
	setupActions();

	createGUI(0L);

	// we split the main view into 2 parts, one for the tasks, one for the
	// statistics
	QSplitter* splitter = new QSplitter(QSplitter::Horizontal, this,"QSplitter");
	setCentralWidget(splitter);

	// create the statistic view
	m_statview = new StatisticsView(splitter, "StatisticsView");

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

void MainQtWidget::readOptions()
{
	m_addSub = SettingsClass::addsub();
	m_mulDiv = SettingsClass::muldiv();
	m_nrRatios = SettingsClass::number_ratios();
	m_maxMainDenominator = SettingsClass::max_main_denominator();
}

void MainQtWidget::writeOptions()
{
	SettingsClass::setAddsub(m_addSub);
	SettingsClass::setMuldiv(m_mulDiv);
	SettingsClass::setNumber_ratios(m_nrRatios);
	SettingsClass::setMax_main_denominator(m_maxMainDenominator);
	SettingsClass::writeConfig();
}

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
	m_NrOfTermsBox->setCurrentItem(m_nrRatios - 2);
	QToolTip::add( m_NrOfTermsBox, i18n( "the number of terms you want" ) );
	QWhatsThis::add( m_NrOfTermsBox, i18n( "Choose the number of terms (2, 3, 4 or 5) you want for calculating fractions" ) );
	m_NrOfTermsBoxAction = new KWidgetAction(m_NrOfTermsBox, i18n("Number of Terms"), ALT+Key_T, this, SLOT(NrOfTermsBoxSlot()), actionCollection(), "NrOfTermsBoxAction");

	// now connect the ComboBox's signal textChanged() to the slot function
	QObject::connect(m_NrOfTermsBox, SIGNAL(activated(int)), this, SLOT(NrOfTermsBoxSlot()));

	// a label just describing the max. main denominator ComboBox
	m_MaxMainDenominatorLabel = new QLabel(i18n("Max. main denominator:"), 0, "kde toolbar widget");
	m_MaxMainDenominatorLabelAction = new KWidgetAction(m_MaxMainDenominatorLabel, i18n("Max. main denominator:"), ALT+Key_T,
							    this, SLOT(MaxMainDenominatorBoxSlot()),
							    actionCollection(), "MaxMainDenominatorLabelAction");

	// the ComboBox holding possible values for the max. main denominator
	m_MaxMainDenominatorBox = new QComboBox(this);
	m_MaxMainDenominatorBox->insertItem("10");
	m_MaxMainDenominatorBox->insertItem("20");
	m_MaxMainDenominatorBox->insertItem("30");
	m_MaxMainDenominatorBox->insertItem("50");
	QToolTip::add( m_MaxMainDenominatorBox, i18n( "the maximum number you can have as denominator" ) );
	QWhatsThis::add( m_MaxMainDenominatorBox, i18n( "Choose the number which will be the maximum for the denominator: 10, 20, 30, 40 or 50" ) );
	switch (m_maxMainDenominator)
	{
		case 10 : m_MaxMainDenominatorBox->setCurrentItem(0);
					 break;
		case 20 : m_MaxMainDenominatorBox->setCurrentItem(1);
					 break;
		case 30 : m_MaxMainDenominatorBox->setCurrentItem(2);
					 break;
		case 50 : m_MaxMainDenominatorBox->setCurrentItem(3);
					 break;
	}
	m_MaxMainDenominatorBoxAction = new KWidgetAction(m_MaxMainDenominatorBox, i18n("Maximal Main Denominator"), ALT+Key_T, this, SLOT(MaxMainDenominatorBoxSlot()), actionCollection(), "MaxMainDenominatorBoxAction");

	// now connect the ComboBox's signal textChanged() to the slot function
	QObject::connect(m_MaxMainDenominatorBox, SIGNAL(activated(int)),
			 this, SLOT(MaxMainDenominatorBoxSlot()));

	// a label just describing the operation ComboBox
	m_OperationLabel = new QLabel(i18n("Operations:"), 0, "kde toolbar widget");
	m_OperationLabelAction = new KWidgetAction(m_OperationLabel, i18n("Operations:"), ALT+Key_O,
						   this, SLOT(OperationBoxSlot()),
						   actionCollection(), "OperationLabelAction");

	// the ComboBox holding possible combinations for operations
	m_OperationBox = new QComboBox(this);
	m_OperationBox->insertItem(i18n("Addition/Subtraction"));
	m_OperationBox->insertItem(i18n("Multiplication/Division"));
	m_OperationBox->insertItem(i18n("All Operations Mixed"));
	if (m_addSub == true && m_mulDiv == false)
	{
		m_OperationBox->setCurrentItem(0);
	} else if (m_addSub == false && m_mulDiv == true) {
		m_OperationBox->setCurrentItem(1);
	} else if (m_addSub == true && m_mulDiv == true) {
		m_OperationBox->setCurrentItem(2);
	}
	QToolTip::add( m_OperationBox, i18n( "the operations you want" ) );
	QWhatsThis::add( m_OperationBox, i18n( "Choose the type of operations you want for calculating fractions: Addition/Substraction, Multiplication/Division or All Operations Mixed. If you choose All Operations Mixed, the program will randomly chooseaddition, substraction, multiplication and/or division." ) );
	m_OperationBoxAction = new KWidgetAction(m_OperationBox, i18n("Operations:"), ALT+Key_O, this, SLOT(OperationBoxSlot()), actionCollection(), "OperationBoxAction");

	// now connect the ComboBox's signal textChanged() to the slot function
	QObject::connect(m_OperationBox, SIGNAL(activated(int)), this, SLOT(OperationBoxSlot()));
}


/* ------ private slots ------ */

void MainQtWidget::NewTask()
{
	// generate a new task
	(void) m_taskview->forceNewTask();
}

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
	//I think clicking on new task after setting all parameters is the best way
	//to ensure good usability -> everything else would be hard to explain to the
	//user!
	//a change of terms should trigger a new task with the correct terms number
	//(void) m_taskview->forceNewTask();
	//TODO (annma suggestion) in that case, the statistic view should stay the
	//same i.e the non done task should not be counted as wrong
}

void MainQtWidget::MaxMainDenominatorBoxSlot()
{
	// get the max. size from the ComboBox, convert it to a number and store
	// it in the private member
	QString curr_md = m_MaxMainDenominatorBox->currentText();
	m_maxMainDenominator = curr_md.toUInt();

	// set the new task parameters
	(void) m_taskview->setTaskParameters(m_addSub, m_mulDiv, m_nrRatios, m_maxMainDenominator);
}

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

bool MainQtWidget::queryExit()
{
	writeOptions();
	return true;
}

#include "mainqtwidget.moc"
