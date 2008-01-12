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

#include <kaction.h>
#include <kcombobox.h>
#include <kdebug.h>
#include <kiconloader.h>
#include <kconfigdialog.h>
#include <klocale.h>
#include <KStandardShortcut>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <kvbox.h>
#include <kpagewidgetmodel.h>
#include <kicon.h>
#include <kapplication.h>

#include <qsplitter.h>
#include <qlabel.h>

#include <qwidget.h>

#include <math.h>

#include "exercisecompare.h"
#include "exerciseconvert.h"
#include "exercisefactorize.h"
#include "taskview.h"
#include "ui_taskgeneralbase.h"
#include "ui_taskcolorsbase.h"
#include "ui_taskfontsbase.h"
#include "statisticsview.h"

#include "settingsclass.h"
#include <kpageview.h>

class TaskGeneral : public QWidget, public Ui::TaskGeneralBase
{
	public:
		TaskGeneral( QWidget * parent ) : QWidget( parent )
		{
			setupUi(this);
		}
};

class TaskColors : public QWidget, public Ui::TaskColorsBase
{
	public:
		TaskColors( QWidget * parent ) : QWidget( parent )
		{
			setupUi(this);
		}
};

class TaskFonts : public QWidget, public Ui::TaskFontsBase
{
	public:
		TaskFonts( QWidget * parent ) : QWidget( parent )
		{
			setupUi(this);
		}
};

/* ------ public member functions ------ */

MainQtWidget::MainQtWidget()
{
#ifdef DEBUG
	kDebug() << "constructor MainQtWidget";
#endif

	// get the settings
	readOptions();

	// creating KActions, used by the kbruchui.rc file
	setupActions();

	createGUI(0L);

	// we split the main view into 2 parts, one for the tasks, one for the
	// statistics
	QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
	splitter->setObjectName("QSplitter");
	setCentralWidget(splitter);

	// the iconlist, where the user can choose the different exercises
	m_exercises = new KPageDialog;
	m_exercises->setFaceType(KPageDialog::List);
	m_exercises->setToolTip(i18n("Choose another exercise by clicking on an icon."));
	m_exercises->setWhatsThis( i18n("Click on the different icons to choose another exercise. The exercises help you to practice different aspects of calculating with fractions."));

	// create the statistic view
	m_statview = new StatisticsView;
	m_statview->setObjectName("StatisticsView");

	// add the pages
	//
	// we have the exercise to solve fraction tasks
	KVBox * page = new KVBox();
	pageItemFraction = new KPageWidgetItem( page, i18n("Fraction Task") );
	pageItemFraction->setIcon( KIcon("kbruch_exercise_common"));
	m_exercises->addPage(pageItemFraction);

	m_taskview = new TaskView((QWidget *) page, m_addSub, m_mulDiv, m_nrRatios, m_maxMainDenominator);
	m_taskview->setObjectName("TaskView");

	// we have the exercise to compare ratios
    page = new KVBox();
    pageItemComparison = new KPageWidgetItem( page, i18n("Comparison") );
    pageItemComparison->setIcon( KIcon("kbruch_exercise_compare"));
    m_exercises->addPage(pageItemComparison);

	m_exerciseCompare = new ExerciseCompare((QWidget *) page);
	m_exerciseCompare->setObjectName("ExerciseCompare");

	// we have the exercise to convert rational numbers into ratios
    page = new KVBox();
    pageItemConversion = new KPageWidgetItem( page, i18n("Conversion") );
    pageItemConversion->setIcon( KIcon("kbruch_exercise_conversion"));
    m_exercises->addPage(pageItemConversion);

	m_exerciseConvert = new ExerciseConvert((QWidget *) page);
	m_exerciseConvert->setObjectName("ExerciseConvert");

	// we have the exercise to factorize a given number into prime factors
    page = new KVBox();
    pageItemFactorization = new KPageWidgetItem( page, i18n("Factorization") );
    pageItemFactorization->setIcon( KIcon("kbruch_exercise_factorisation"));
    m_exercises->addPage(pageItemFactorization );

	m_exerciseFactorize = new ExerciseFactorize((QWidget *) page);
	m_exerciseFactorize->setObjectName("ExerciseFactorize");
	
	splitter->addWidget(m_exercises->mainWidget());
	splitter->addWidget(m_statview);

	// we must change the status of the menubar before another page is shown
	QObject::connect(m_exercises, SIGNAL(currentPageChanged( KPageWidgetItem *, KPageWidgetItem * )), this, SLOT(slotAboutToShowPage(KPageWidgetItem *)));

	// connect signals of the exercises and StatisticView, so that StatisticView
	// gets informed about how the user solved a given task (wrong or correct)
	QObject::connect(m_taskview, SIGNAL(signalTaskSolvedCorrect()), m_statview, SLOT(addCorrect()));
	QObject::connect(m_taskview, SIGNAL(signalTaskSolvedWrong()), m_statview, SLOT(addWrong()));
	QObject::connect(m_exerciseCompare, SIGNAL(signalExerciseSolvedCorrect()), m_statview, SLOT(addCorrect()));
	QObject::connect(m_exerciseCompare, SIGNAL(signalExerciseSolvedWrong()), m_statview, SLOT(addWrong()));
	QObject::connect(m_exerciseConvert, SIGNAL(signalExerciseSolvedCorrect()), m_statview, SLOT(addCorrect()));
	QObject::connect(m_exerciseConvert, SIGNAL(signalExerciseSolvedWrong()), m_statview, SLOT(addWrong()));
	QObject::connect(m_exerciseFactorize, SIGNAL(signalExerciseSolvedCorrect()), m_statview, SLOT(addCorrect()));
	QObject::connect(m_exerciseFactorize, SIGNAL(signalExerciseSolvedWrong()), m_statview, SLOT(addWrong()));
	
	pageItems[0] = pageItemFraction;
	pageItems[1] = pageItemComparison;
	pageItems[2] = pageItemConversion;
	pageItems[3] = pageItemFactorization;
	
	// now show the last exercise
	KPageWidgetItem * currentPage = pageItems[SettingsClass::activeExercise()];
	m_exercises->setCurrentPage( currentPage );
	slotAboutToShowPage( currentPage );
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

	/* make sure that we can load config files with corrupted values */
	if (m_mulDiv == true && pow(2, m_nrRatios) > m_maxMainDenominator)
	{
		m_nrRatios = 2;
		m_maxMainDenominator = 10;
	}
}

void MainQtWidget::writeOptions()
{
	// Get current index
	int index;
	for ( index=0; index<4; ++index )
	{
		if ( pageItems[index] == m_exercises->currentPage() )
			break;
	}
	SettingsClass::setActiveExercise( index );
	
	// save settings for exercise solve task with fractions
	SettingsClass::setAddsub(m_addSub);
	SettingsClass::setMuldiv(m_mulDiv);
	SettingsClass::setNumber_ratios(m_nrRatios);
	SettingsClass::setMax_main_denominator(m_maxMainDenominator);

	SettingsClass::self()->writeConfig();
}

void MainQtWidget::setupActions()
{
	// new task action
    m_NewTaskAction  = new KAction(KIcon("document-new"), i18nc("@action opens a new task", "&New"), this);
    actionCollection()->addAction("NewTask", m_NewTaskAction );
	connect(m_NewTaskAction, SIGNAL(triggered(bool) ), SLOT(NewTask()));
	m_NewTaskAction->setShortcut(KStandardShortcut::shortcut(KStandardShortcut::New));

	// quit action
	KStandardAction::quit(kapp, SLOT(quit()), actionCollection());

	//
	KStandardAction::preferences(this,  SLOT( slotPrefs() ), actionCollection());

	// a label just describing the Number of terms ComboBox
	m_NrOfTermsLabel = new QLabel(i18n("Terms:"), 0);
	m_NrOfTermsLabel->setObjectName("kde toolbar widget");
    m_NrOfTermsLabelAction  = new KAction(i18n("Terms:"), this);
    actionCollection()->addAction("NrOfTermsLabelAction", m_NrOfTermsLabelAction );
	connect(m_NrOfTermsLabelAction, SIGNAL(triggered(bool)), SLOT(NrOfTermsBoxSlot()));
	m_NrOfTermsLabelAction->setShortcut(QKeySequence(Qt::ALT+Qt::Key_E));
        m_NrOfTermsLabelAction->setDefaultWidget( m_NrOfTermsLabel );

	// the ComboBox holding possible values for term number
	m_NrOfTermsBox = new KComboBox();
	m_NrOfTermsBox->addItem("2");
	m_NrOfTermsBox->addItem("3");
	m_NrOfTermsBox->addItem("4");
	m_NrOfTermsBox->addItem("5");
	m_NrOfTermsBox->setCurrentIndex(m_nrRatios - 2);
	m_NrOfTermsBox->setToolTip( i18n( "The number of terms you want" ) );
	m_NrOfTermsBox->setWhatsThis( i18n( "Choose the number of terms (2, 3, 4 or 5) you want for calculating fractions." ) );
    m_NrOfTermsBoxAction  = new KAction(i18n("Number of Terms"), this);
    actionCollection()->addAction("NrOfTermsBoxAction", m_NrOfTermsBoxAction );
	connect(m_NrOfTermsBoxAction, SIGNAL(triggered(bool)), SLOT(NrOfTermsBoxSlot()));
	m_NrOfTermsBoxAction->setShortcut(QKeySequence(Qt::ALT+Qt::Key_E));
        m_NrOfTermsBoxAction->setDefaultWidget( m_NrOfTermsBox );

	// now connect the ComboBox's signal textChanged() to the slot function
	QObject::connect(m_NrOfTermsBox, SIGNAL(activated(int)), this, SLOT(NrOfTermsBoxSlot()));

	// a label just describing the max. main denominator ComboBox
	m_MaxMainDenominatorLabel = new QLabel(i18n("Max. main denominator:"), 0);
	m_MaxMainDenominatorLabel->setObjectName("kde toolbar widget");
    m_MaxMainDenominatorLabelAction  = new KAction(i18n("Max. main denominator:"), this);
    actionCollection()->addAction("MaxMainDenominatorLabelAction", m_MaxMainDenominatorLabelAction );
	connect(m_MaxMainDenominatorLabelAction, SIGNAL(triggered(bool)), SLOT(MaxMainDenominatorBoxSlot()));
	m_MaxMainDenominatorLabelAction->setShortcut(QKeySequence(Qt::ALT+Qt::Key_D));
        m_MaxMainDenominatorLabelAction->setDefaultWidget( m_MaxMainDenominatorLabel );

	// the ComboBox holding possible values for the max. main denominator
	m_MaxMainDenominatorBox = new KComboBox(this);
	m_MaxMainDenominatorBox->addItem("10");
	m_MaxMainDenominatorBox->addItem("20");
	m_MaxMainDenominatorBox->addItem("30");
	m_MaxMainDenominatorBox->addItem("50");
	m_MaxMainDenominatorBox->setToolTip( i18n( "The maximum number you can have as main denominator" ) );
	m_MaxMainDenominatorBox->setWhatsThis( i18n( "Choose the number which will be the maximum for the main denominator: 10, 20, 30, 40 or 50." ) );
	switch (m_maxMainDenominator)
	{
		case 10 : m_MaxMainDenominatorBox->setCurrentIndex(0);
					 break;
		case 20 : m_MaxMainDenominatorBox->setCurrentIndex(1);
					 break;
		case 30 : m_MaxMainDenominatorBox->setCurrentIndex(2);
					 break;
		case 50 : m_MaxMainDenominatorBox->setCurrentIndex(3);
					 break;
	}
    m_MaxMainDenominatorBoxAction  = new KAction(i18n("Maximal Main Denominator"), this);
    actionCollection()->addAction("MaxMainDenominatorBoxAction", m_MaxMainDenominatorBoxAction );
	connect(m_MaxMainDenominatorBoxAction, SIGNAL(triggered(bool)), SLOT(MaxMainDenominatorBoxSlot()));
	m_MaxMainDenominatorBoxAction->setShortcut(QKeySequence(Qt::ALT+Qt::Key_D));
        m_MaxMainDenominatorBoxAction->setDefaultWidget( m_MaxMainDenominatorBox );

	// now connect the ComboBox's signal textChanged() to the slot function
	QObject::connect(m_MaxMainDenominatorBox, SIGNAL(activated(int)),
			 this, SLOT(MaxMainDenominatorBoxSlot()));

	// a label just describing the operation ComboBox
	m_OperationLabel = new QLabel(i18n("Operations:"), 0);
	m_OperationLabel->setObjectName("kde toolbar widget");
    m_OperationLabelAction  = new KAction(i18n("Operations:"), this);
    actionCollection()->addAction("OperationLabelAction", m_OperationLabelAction );
	connect(m_OperationLabelAction, SIGNAL(triggered(bool)), SLOT(OperationBoxSlot()));
	m_OperationLabelAction->setShortcut(QKeySequence(Qt::ALT+Qt::Key_O));
        m_OperationLabelAction->setDefaultWidget( m_OperationLabel );

	// the ComboBox holding possible combinations for operations
	m_OperationBox = new KComboBox(this);
	m_OperationBox->addItem(i18n("Addition/Subtraction"));
	m_OperationBox->addItem(i18n("Multiplication/Division"));
	m_OperationBox->addItem(i18n("All Operations Mixed"));
	if (m_addSub == true && m_mulDiv == false)
	{
		m_OperationBox->setCurrentIndex(0);
	} else if (m_addSub == false && m_mulDiv == true) {
		m_OperationBox->setCurrentIndex(1);
	} else if (m_addSub == true && m_mulDiv == true) {
		m_OperationBox->setCurrentIndex(2);
	}
	m_OperationBox->setToolTip( i18n( "The operations you want" ) );
	m_OperationBox->setWhatsThis( i18n( "Choose the type of operations you want for calculating fractions: Addition/Substraction, Multiplication/Division or All Operations Mixed. If you choose All Operations Mixed, the program will randomly choose addition, substraction, multiplication and/or division." ) );
    m_OperationBoxAction  = new KAction(i18n("Operations:"), this);
    actionCollection()->addAction("OperationBoxAction", m_OperationBoxAction );
	connect(m_OperationBoxAction, SIGNAL(triggered(bool)), SLOT(OperationBoxSlot()));
	m_OperationBoxAction->setShortcut(QKeySequence(Qt::ALT+Qt::Key_O));
        m_OperationBoxAction->setDefaultWidget( m_OperationBox );

	// now connect the ComboBox's signal textChanged() to the slot function
	QObject::connect(m_OperationBox, SIGNAL(activated(int)), this, SLOT(OperationBoxSlot()));

	if (!initialGeometrySet())
		resize( QSize(725, 330).expandedTo(minimumSizeHint()));
	setupGUI(ToolBar | Keys | StatusBar | Create);
	setAutoSaveSettings();
}


/* ------ private slots ------ */

void MainQtWidget::NewTask()
{
#ifdef DEBUG
	kDebug() << "NewTask MainQtWidget";
	//kDebug() << "pageIndex(m_taskview): " << m_exercises->pageIndex(m_taskview);
	//kDebug() << "pageIndex(m_exerciseCompare): " << m_exercises->pageIndex(m_exerciseCompare);
	//kDebug() << "pageIndex(m_exerciseConvert): " << m_exercises->pageIndex(m_exerciseConvert);
#endif

	// check which page should generate a new task
        KPageWidgetItem*currentItem = m_exercises->currentPage();
        if ( currentItem==pageItemFraction )
        {
            m_taskview->forceNewTask();
        }
        else if( currentItem==pageItemComparison )
        {
            m_exerciseCompare->forceNewTask();
        }
        else if ( currentItem==pageItemConversion )
        {
            m_exerciseConvert->forceNewTask();
        }
        else if ( currentItem == pageItemFactorization )
        {
            m_exerciseFactorize->forceNewTask();
        }


/* this doesn't seem to work, because pageIndex always returns 0

	if (m_exercises->activePageIndex() == m_exercises->pageIndex(m_taskview))
	{
		m_taskview->forceNewTask();
		return;
	}
	if (m_exercises->activePageIndex() == m_exercises->pageIndex(m_exerciseCompare))
	{
		m_exerciseCompare->forceNewTask();
		return;
	}
*/

/* this even do not compile, but I don't know why

	switch (m_exercises->activePageIndex())
	{
		case m_exercises->pageIndex(m_taskview):
					break;
		case m_exercises->pageIndex(m_exerciseCompare):
					m_exerciseCompare->forceNewTask();
					break;
	}
*/

	return;
}

void MainQtWidget::NrOfTermsBoxSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::NrOfTermsBoxSlot()";
#endif
	int currentMaxDenom = m_MaxMainDenominatorBox->currentText().toInt();
	QString curr_nr = m_NrOfTermsBox->currentText();
	m_MaxMainDenominatorBox->clear();

	if (m_mulDiv == true)
	{
		if (curr_nr == "2")
		{
			m_MaxMainDenominatorBox->addItem("10");
			m_MaxMainDenominatorBox->addItem("20");
			m_MaxMainDenominatorBox->addItem("30");
			m_MaxMainDenominatorBox->addItem("50");
		} else if (curr_nr == "3") {
			m_MaxMainDenominatorBox->addItem("20");
			m_MaxMainDenominatorBox->addItem("30");
			m_MaxMainDenominatorBox->addItem("50");
		} else if (curr_nr == "4") {
			m_MaxMainDenominatorBox->addItem("20");
			m_MaxMainDenominatorBox->addItem("30");
			m_MaxMainDenominatorBox->addItem("50");
		} else {
			m_MaxMainDenominatorBox->addItem("50");
		}
		m_nrRatios = curr_nr.toInt();
		int index = m_MaxMainDenominatorBox->findText(QString::number(currentMaxDenom));
		m_MaxMainDenominatorBox->setCurrentIndex(index > -1 ? index : 0);
		m_maxMainDenominator = m_MaxMainDenominatorBox->currentText().toInt();
	} else {
		/* no multiplication or division allowed, so we add the default values */
		m_MaxMainDenominatorBox->addItem("10");
		m_MaxMainDenominatorBox->addItem("20");
		m_MaxMainDenominatorBox->addItem("30");
		m_MaxMainDenominatorBox->addItem("50");
		int index = m_MaxMainDenominatorBox->findText(QString::number(currentMaxDenom));
		m_MaxMainDenominatorBox->setCurrentIndex(index > -1 ? index : 0);
		m_nrRatios = curr_nr.toInt();
	} // if (m_mulDiv == true)

	// set the new task parameters
	(void) m_taskview->setTaskParameters(m_addSub, m_mulDiv, m_nrRatios, m_maxMainDenominator);
}

void MainQtWidget::MaxMainDenominatorBoxSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::MaxMainDenominatorBoxSlot()";
#endif

	// get the max. size from the ComboBox, convert it to a number and store
	// it in the private member
	QString curr_md = m_MaxMainDenominatorBox->currentText();
	m_maxMainDenominator = curr_md.toUInt();

	// set the new task parameters
	(void) m_taskview->setTaskParameters(m_addSub, m_mulDiv, m_nrRatios, m_maxMainDenominator);
}

void MainQtWidget::OperationBoxSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::OperationBoxSlot()";
#endif

	int index = m_OperationBox->currentIndex(); // get selected item

	// user has selected the operations for the next task, so store it in the
	// private members
	if (index == 0)
	{
		m_addSub = true;
		m_mulDiv = false;

		/* set the number of terms box and max main denominator box correctly */
		NrOfTermsBoxSlot();
	} else if (index == 1) {
		m_addSub = false;
		m_mulDiv = true;

		/* set the number of terms box and max main denominator box correctly */
		NrOfTermsBoxSlot();
	} else {
		m_addSub = true;
		m_mulDiv = true;

		/* set the number of terms box and max main denominator box correctly */
		NrOfTermsBoxSlot();
	}

	// set the new task parameters
	(void) m_taskview->setTaskParameters(m_addSub, m_mulDiv, m_nrRatios, m_maxMainDenominator);
}

void MainQtWidget::slotPrefs()
{
	// do not show dialog twice
	if (KConfigDialog::showDialog("settings"))
		return;

	//KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog* configDialog = new KConfigDialog( this, "settings", SettingsClass::self() );

	TaskGeneral * taskGeneral = new TaskGeneral(0);
	configDialog->addPage(taskGeneral, i18n("General"), "preferences-desktop-other");

	TaskColors * taskColors = new TaskColors(0);
	configDialog->addPage(taskColors, i18n("Colors"), "preferences-desktop-color");

	TaskFonts * taskFonts = new TaskFonts(0);
	configDialog->addPage(taskFonts, i18n("Fonts"), "preferences-desktop-font");
	// User edited the configuration - update your local copies of the
	// configuration data
	connect(configDialog, SIGNAL(settingsChanged( const QString &)), this, SLOT(slotApplySettings()) );
        configDialog->setHelp("kbruch/index.html");
	configDialog->show();
/*
	SettingsDialog * dlg = new SettingsDialog(this);
	connect(dlg, SIGNAL(configChanged()), this, SLOT(slotApplySettings()));

	dlg->exec();

	delete dlg;
	dlg = NULL;

*/
	return;
}

void MainQtWidget::slotApplySettings()
{
	// update the task view
	m_taskview->update();
	m_exerciseCompare->update();
	m_exerciseConvert->update();
	m_exerciseFactorize->update();

	return;
}

void MainQtWidget::slotAboutToShowPage(KPageWidgetItem *current)
{
#ifdef DEBUG
	kDebug() << "slotAboutToShowPage MainQtWidget";
	//kDebug() << "pageIndex(m_taskview): " << m_exercises->pageIndex(m_taskview);
	//kDebug() << "pageIndex(m_exerciseCompare): " << m_exercises->pageIndex(m_exerciseCompare);
	//kDebug() << "pageIndex(m_exerciseConvert): " << m_exercises->pageIndex(m_exerciseConvert);
#endif

	// check which page to show
	if (current ==pageItemFraction)
	{
		// exercise solve task with fraction (taskview.h)
		m_NrOfTermsBox->setEnabled(true);
		m_MaxMainDenominatorBox->setEnabled(true);
		m_OperationBox->setEnabled(true);
	} else {
		m_NrOfTermsBox->setEnabled(false);
		m_MaxMainDenominatorBox->setEnabled(false);
		m_OperationBox->setEnabled(false);
	}

	return;
}

bool MainQtWidget::queryExit()
{
	writeOptions();
	return true;
}

#include "mainqtwidget.moc"
