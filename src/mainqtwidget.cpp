/***************************************************************************
                          mainqtwidget.cpp  -  The main Qt/KDE window
                             -------------------
    begin                : Tue Mar 16 00:00:00 CET 2003
    copyright            : (C) 2003-2004 by Sebastian Stein
    email                : seb.kde@hpfsc.de

    copyright            : (C) 2008 by Tiago Porangaba, tiago.porangaba@ltia.fc.unesp.br
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
#include <kstandarddirs.h>
#include <kglobalsettings.h>

#include <qlayout.h>
#include <qsplitter.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qwidget.h>
#include <qframe.h>
#include <QToolButton>
#include <QPushButton>
 
#include <math.h>

#include "exercisecompare.h"
#include "exerciseconvert.h"
#include "exercisefactorize.h"
#include "exercisepercentage.h"
#include "taskview.h"
#include "ui_taskcolorsbase.h"
#include "ui_taskfontsbase.h"
#include "statisticsview.h"
#include "AppMenuWidget.h"

#include "settingsclass.h"
#include <kpageview.h>

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

	// Create layout	
	QGridLayout * layoutExercises = new QGridLayout();
	layoutExercises->setObjectName( "layoutExercises" );
	layoutExercises->setRowStretch(2, 1);
	QGridLayout * layoutOptions = new QGridLayout();
	layoutOptions->setObjectName( "layoutOptions" );
	layoutOptions->setRowStretch(0, 1);
	layoutOptions->setRowStretch(6, 1);
	QGridLayout * layoutQuestion = new QGridLayout();
	layoutQuestion->setObjectName( "layoutQuestion" );
	layoutQuestion->setColumnMinimumWidth(0, 110);
	layoutQuestion->setColumnStretch(1, 1);
	QGridLayout * layoutAnswer = new QGridLayout();
	layoutAnswer->setObjectName( "layoutAnswer" );
	layoutAnswer->setColumnMinimumWidth(0, 110);
	layoutAnswer->setColumnStretch(1, 1);
	QGridLayout * layoutOperations = new QGridLayout();
	layoutOperations->setObjectName( "layoutOperations" );
	layoutOperations->setColumnMinimumWidth(0, 110);
	layoutOperations->setColumnStretch(1, 1);
	QGridLayout * layoutSolution = new QGridLayout();
	layoutSolution->setObjectName( "layoutSolution" );
	layoutSolution->setColumnMinimumWidth(0, 110);
	layoutSolution->setColumnStretch(1, 1);

	QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
	splitter->setObjectName("QSplitter");
	setCentralWidget(splitter);

	QWidget * pageExercises = new QWidget();
	QWidget * pageOptions = new QWidget();
	pageOptions->setFixedWidth(185);

	// Create visible components
	m_QuestionGroup = new QGroupBox(i18n("Question:"), pageOptions);
	m_AnswerGroup = new QGroupBox(i18n("Answer:"), pageOptions);
	m_SolutionGroup = new QGroupBox(i18n("Solution:"), pageOptions);
	m_OperationsGroup = new QGroupBox(i18n("Operations:"), pageOptions);

	defaultFont = SettingsClass::taskFont();
	defaultFont.setBold( TRUE );
	defaultFont.setPointSize( 14 );

	m_TitleLabel = new QLabel(i18n("Arithmetic"), pageExercises);
	m_TitleLabel->setAlignment(Qt::AlignLeft);
	m_TitleLabel->setFont(defaultFont);
	m_TitleLabel->setFixedHeight(35);
	m_TitleLabel->setAlignment(Qt::AlignVCenter);

	m_statview = new StatisticsView;
	m_statview->setObjectName("StatisticsView");
	m_statview->setFixedHeight(120);

	m_footerline = new QFrame(pageExercises);
	m_footerline->setGeometry(QRect(pageExercises->width(), 100, 20, 20));
	m_footerline->setFrameStyle(QFrame::HLine | QFrame::Sunken);

	m_headerline = new QFrame(pageExercises);
	m_headerline->setGeometry(QRect(pageExercises->width(), 100, 20, 20));
	m_headerline->setFrameStyle(QFrame::HLine | QFrame::Sunken);

	m_taskview = new TaskView(pageExercises, m_addAdd, m_addDiv, m_addMult, m_addSub, m_nrRatios, m_maxMainDenominator);
	m_taskview->setObjectName("TaskView");
	m_taskview->hide();

	m_exerciseCompare = new ExerciseCompare(pageExercises);
	m_exerciseCompare->setObjectName("ExerciseCompare");
	m_exerciseCompare->hide();

	m_exerciseConvert = new ExerciseConvert(pageExercises);
	m_exerciseConvert->setObjectName("ExerciseConvert");
	m_exerciseConvert->hide();

	m_exerciseFactorize = new ExerciseFactorize(pageExercises);
	m_exerciseFactorize->setObjectName("ExerciseFactorize");
	m_exerciseFactorize->hide();

	m_exercisePercentage = new ExercisePercentage(pageExercises);
	m_exercisePercentage->setObjectName("ExercisePercentage");
	m_exercisePercentage->hide();

	m_OptionsLabel = new QLabel(i18n("Options:"), pageOptions);
	m_OptionsLabel->setToolTip(i18n("Set the options to solve the exercises."));
	m_OptionsLabel->setObjectName("OptionsLabel");
	m_OptionsLabel->setFont(defaultFont);

	m_QuestionMixedLabel = new QLabel(i18n("Mixed number:"), pageOptions);
	m_QuestionMixedLabel->setToolTip(i18n("Set if the fractions will appear in mixed form or not in the question expression ( mixed form example: 1 5/4 = 9/4 )."));
	m_QuestionMixedLabel->setObjectName("QuestionMixedLabel");
	m_QuestionMixedLabel->setAlignment(Qt::AlignRight);
	
	m_QuestionMixedCheck = new QCheckBox(pageOptions);
	m_QuestionMixedCheck->setObjectName("QuestionMixedCheck");
	m_QuestionMixedCheck->setChecked(m_questionMixed);
	m_taskview->setQuestionMixed(m_questionMixed);
	QObject::connect(m_QuestionMixedCheck, SIGNAL(stateChanged(int)), this, SLOT(QuestionMixedCheckSlot()));

	m_AnswerMixedLabel = new QLabel(i18n("Mixed number:"), pageOptions);
	m_AnswerMixedLabel->setToolTip(i18n("Set if the fractions will appear in mixed form or not in the answer ( mixed form example: 1 5/4 = 9/4 )."));
	m_AnswerMixedLabel->setObjectName("AnswerMixedLabel");
	m_AnswerMixedLabel->setAlignment(Qt::AlignRight);
	
	m_AnswerMixedCheck = new QCheckBox(pageOptions);
	m_AnswerMixedCheck->setObjectName("AnswerMixedCheck");
	m_AnswerMixedCheck->setChecked(m_answerMixed);
	m_taskview->setAnswerMixed(m_answerMixed);
	QObject::connect(m_AnswerMixedCheck, SIGNAL(stateChanged(int)), this, SLOT(AnswerMixedCheckSlot()));

	m_SolutionMixedLabel = new QLabel(i18n("Mixed number:"), pageOptions);
	m_SolutionMixedLabel->setToolTip(i18n("Set if the fractions will appear in mixed form or not in the solution ( mixed form example: 1 5/4 = 9/4 )."));
	m_SolutionMixedLabel->setObjectName("SolutionMixedLabel");
	m_SolutionMixedLabel->setAlignment(Qt::AlignRight);
	
	m_SolutionMixedCheck = new QCheckBox(pageOptions);
	m_SolutionMixedCheck->setObjectName("SolutionMixedCheck");
	m_SolutionMixedCheck->setChecked(m_solutionMixed);
	m_taskview->setSolutionMixed(m_solutionMixed);
	QObject::connect(m_SolutionMixedCheck, SIGNAL(stateChanged(int)), this, SLOT(SolutionMixedCheckSlot()));

	m_AdditionLabel = new QLabel(i18n("Addition:"), pageOptions);
	m_AdditionLabel->setToolTip(i18n("Check this to use addition operator."));
	m_AdditionLabel->setObjectName("AdditionLabel");
	m_AdditionLabel->setAlignment(Qt::AlignRight);	

	m_AdditionCheck = new QCheckBox(pageOptions);
	m_AdditionCheck->setObjectName("AdditionCheck");
	m_AdditionCheck->setChecked(m_addAdd);
	QObject::connect(m_AdditionCheck, SIGNAL(stateChanged(int)), this, SLOT(AdditionCheckSlot()));

	m_SubtractionLabel = new QLabel(i18n("Subtraction:"), pageOptions);
	m_SubtractionLabel->setToolTip(i18n("Check this to use subtraction operator."));
	m_SubtractionLabel->setObjectName("SubtractionLabel");
	m_SubtractionLabel->setAlignment(Qt::AlignRight);		

	m_SubtractionCheck = new QCheckBox(pageOptions);
	m_SubtractionCheck->setObjectName("SubtractionCheck");
	m_SubtractionCheck->setChecked(m_addSub);
	QObject::connect(m_SubtractionCheck, SIGNAL(stateChanged(int)), this, SLOT(SubtractionCheckSlot()));

	m_MultiplicationLabel = new QLabel(i18n("Multiplication:"), pageOptions);
	m_MultiplicationLabel->setToolTip(i18n("Check this to use multiplication operator."));
	m_MultiplicationLabel->setObjectName("MultiplicationLabel");
	m_MultiplicationLabel->setAlignment(Qt::AlignRight);			

	m_MultiplicationCheck = new QCheckBox(pageOptions);
	m_MultiplicationCheck->setObjectName("MultiplicationCheck");
	m_MultiplicationCheck->setChecked(m_addMult);
	QObject::connect(m_MultiplicationCheck, SIGNAL(stateChanged(int)), this, SLOT(MultiplicationCheckSlot()));

	m_DivisionLabel = new QLabel(i18n("Division:"), pageOptions);
	m_DivisionLabel->setToolTip(i18n("Check this to use division operator."));
	m_DivisionLabel->setObjectName("DivisionLabel");
	m_DivisionLabel->setAlignment(Qt::AlignRight);	

	m_DivisionCheck = new QCheckBox(pageOptions);
	m_DivisionCheck->setObjectName("DivisionCheck");
	m_DivisionCheck->setChecked(m_addDiv);
	QObject::connect(m_DivisionCheck, SIGNAL(stateChanged(int)), this, SLOT(DivisionCheckSlot()));

	m_ReducedLabel = new QLabel(i18n("Reduced form:"), pageOptions);
	m_ReducedLabel->setToolTip(i18n("Check this to force the use of the reduced form."));
	m_ReducedLabel->setObjectName("ReducedLabel");
	m_ReducedLabel->setAlignment(Qt::AlignRight);		

	m_ReducedCheck = new QCheckBox(pageOptions);
	m_ReducedCheck->setObjectName("ReducedCheck");
	m_ReducedCheck->setChecked(m_reducedForm);
	m_taskview->setReducedForm(m_reducedForm);
	QObject::connect(m_ReducedCheck, SIGNAL(stateChanged(int)), this, SLOT(ReducedFormCheckSlot()));

	m_NrOfTermsLabel = new QLabel(i18n("Number of terms:"), pageOptions);
	m_NrOfTermsLabel->setToolTip( i18n( "The number of \nterms you want" ) );
	m_NrOfTermsLabel->setObjectName("NrOfTermsLabel");
	m_NrOfTermsLabel->setAlignment(Qt::AlignRight);

	m_NrOfTermsBox = new KComboBox(pageOptions);
	m_NrOfTermsBox->addItem("2");
	m_NrOfTermsBox->addItem("3");
	m_NrOfTermsBox->addItem("4");
	m_NrOfTermsBox->addItem("5");
	m_NrOfTermsBox->setCurrentIndex(m_nrRatios - 2);
	m_NrOfTermsBox->setToolTip( i18n( "The number of \nterms you want" ) );
	m_NrOfTermsBox->setWhatsThis( i18n( "Choose the number of terms (2, 3, 4 or 5) you \nwant for calculating fractions." ) );
	QObject::connect(m_NrOfTermsBox, SIGNAL(activated(int)), this, SLOT(NrOfTermsBoxSlot()));

	m_MaxMainDenominatorLabel = new QLabel(i18n("Maximum denominator:"), pageOptions);
	m_MaxMainDenominatorLabel->setObjectName("MaxMainDenominatorLabel");
	m_MaxMainDenominatorLabel->setAlignment(Qt::AlignRight);
	m_MaxMainDenominatorLabel->setWordWrap( TRUE );

	m_MaxMainDenominatorBox = new KComboBox(pageOptions);
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
	QObject::connect(m_MaxMainDenominatorBox, SIGNAL(activated(int)), this, SLOT(MaxMainDenominatorBoxSlot()));

	
	layoutExercises->addWidget(m_TitleLabel, 0, 0);
	layoutExercises->addWidget(m_headerline, 1, 0);
	layoutExercises->addWidget(m_taskview, 2, 0);
	layoutExercises->addWidget(m_exerciseCompare, 2, 0);
	layoutExercises->addWidget(m_exerciseConvert, 2, 0);
	layoutExercises->addWidget(m_exerciseFactorize, 2, 0);
	layoutExercises->addWidget(m_exercisePercentage, 2, 0);	
	layoutExercises->addWidget(m_footerline, 3, 0);
	layoutExercises->addWidget(m_statview, 4, 0);

	layoutOptions->addWidget(m_OptionsLabel, 1, 0);
	layoutOptions->addWidget(m_QuestionGroup, 2, 0);
    	layoutOptions->addWidget(m_AnswerGroup, 3, 0);
    	layoutOptions->addWidget(m_SolutionGroup, 4, 0);
    	layoutOptions->addWidget(m_OperationsGroup, 5, 0);

	layoutQuestion->addWidget(m_QuestionMixedLabel,0,0);
	layoutQuestion->addWidget(m_QuestionMixedCheck,0,1);
	layoutQuestion->addWidget(m_NrOfTermsLabel,1,0);
	layoutQuestion->addWidget(m_NrOfTermsBox,1,1);
	layoutQuestion->addWidget(m_MaxMainDenominatorLabel,2,0);
	layoutQuestion->addWidget(m_MaxMainDenominatorBox,2,1);
	
	layoutAnswer->addWidget(m_AnswerMixedLabel,0,0);
	layoutAnswer->addWidget(m_AnswerMixedCheck,0,1);
	layoutAnswer->addWidget(m_ReducedLabel,1,0);
	layoutAnswer->addWidget(m_ReducedCheck,1,1);

	layoutSolution->addWidget(m_SolutionMixedLabel,0,0);
	layoutSolution->addWidget(m_SolutionMixedCheck,0,1);

	layoutOperations->addWidget(m_AdditionLabel,0,0);
	layoutOperations->addWidget(m_AdditionCheck,0,1);
	layoutOperations->addWidget(m_SubtractionLabel,1,0);
	layoutOperations->addWidget(m_SubtractionCheck,1,1);
	layoutOperations->addWidget(m_MultiplicationLabel,2,0);
	layoutOperations->addWidget(m_MultiplicationCheck,2,1);
	layoutOperations->addWidget(m_DivisionLabel,3,0);
	layoutOperations->addWidget(m_DivisionCheck,3,1);

	m_QuestionGroup->setLayout(layoutQuestion);
	m_AnswerGroup->setLayout(layoutAnswer);
	m_SolutionGroup->setLayout(layoutSolution);
	m_OperationsGroup->setLayout(layoutOperations);
  	pageOptions->setLayout(layoutOptions);
  	pageExercises->setLayout(layoutExercises);

	splitter->addWidget(pageOptions);
	splitter->addWidget(pageExercises);

	// connect signals of the exercises and StatisticView, so that StatisticView
	// gets informed about how the user solved a given task (wrong or correct)
	QObject::connect(m_taskview, SIGNAL(signalTaskSolvedCorrect()), m_statview, SLOT(addCorrect()));
	QObject::connect(m_taskview, SIGNAL(signalTaskSkipped()), m_statview, SLOT(addSkipped()));
	QObject::connect(m_taskview, SIGNAL(signalTaskSolvedWrong()), m_statview, SLOT(addWrong()));
	QObject::connect(m_exerciseCompare, SIGNAL(signalExerciseSolvedCorrect()), m_statview, SLOT(addCorrect()));
	QObject::connect(m_exerciseCompare, SIGNAL(signalExerciseSkipped()), m_statview, SLOT(addSkipped()));
	QObject::connect(m_exerciseCompare, SIGNAL(signalExerciseSolvedWrong()), m_statview, SLOT(addWrong()));
	QObject::connect(m_exerciseConvert, SIGNAL(signalExerciseSolvedCorrect()), m_statview, SLOT(addCorrect()));
	QObject::connect(m_exerciseConvert, SIGNAL(signalExerciseSkipped()), m_statview, SLOT(addSkipped()));
	QObject::connect(m_exerciseConvert, SIGNAL(signalExerciseSolvedWrong()), m_statview, SLOT(addWrong()));
	QObject::connect(m_exerciseFactorize, SIGNAL(signalExerciseSolvedCorrect()), m_statview, SLOT(addCorrect()));
	QObject::connect(m_exerciseFactorize, SIGNAL(signalExerciseSkipped()), m_statview, SLOT(addSkipped()));
	QObject::connect(m_exerciseFactorize, SIGNAL(signalExerciseSolvedWrong()), m_statview, SLOT(addWrong()));
	QObject::connect(m_exercisePercentage, SIGNAL(signalExerciseSolvedCorrect()), m_statview, SLOT(addCorrect()));
	QObject::connect(m_exercisePercentage, SIGNAL(signalExerciseSkipped()), m_statview, SLOT(addSkipped()));
	QObject::connect(m_exercisePercentage, SIGNAL(signalExerciseSolvedWrong()), m_statview, SLOT(addWrong()));
	
	// Get and set the page of last exercise
	selectedTask = (ExerciseType)SettingsClass::activeExercise();
	switch (selectedTask)
	{
		case Arithmetics:
			SelectArithmetics();
			break;
		case Comparison:
			SelectComparison();
			break;
		case Conversion:
			SelectConversion();
			break;
		case Factorization:
			SelectFactorization();
			break;
		case Percentage:
			SelectPercentage();
			break;			
		default:
			SelectArithmetics();
			break;
	}	
	move(50, 50);	
}

MainQtWidget::~MainQtWidget()
{
}

QFont MainQtWidget::DefaultFont()
{
	QFont defaultFont = KGlobalSettings::generalFont();
	defaultFont.setPointSize( 24 );
	defaultFont.setBold( TRUE );
	return defaultFont;
}

/* ------ private member functions ------ */

void MainQtWidget::readOptions()
{
#ifdef DEBUG
	kDebug() << "readOptions MainQtWidget";
#endif

	m_addSub = SettingsClass::addsub();
	m_addAdd = SettingsClass::addadd();
	m_addDiv = SettingsClass::adddiv();
	m_addMult = SettingsClass::addmult();	

	m_addMult = SettingsClass::addmult();	
	m_addMult = SettingsClass::addmult();	
	m_addMult = SettingsClass::addmult();	
	
	m_nrRatios = SettingsClass::number_ratios();
	m_maxMainDenominator = SettingsClass::max_main_denominator();

	m_reducedForm = SettingsClass::reduceForm();
	m_solutionMixed = SettingsClass::solutionMixed();
	m_questionMixed = SettingsClass::questionMixed();

	/* make sure that we can load config files with corrupted values */
	if ((m_addMult == true && m_addDiv == true) && pow(2.0, (double)m_nrRatios) > m_maxMainDenominator)
	{
		m_nrRatios = 2;
		m_maxMainDenominator = 10;
	}
}

void MainQtWidget::writeOptions()
{
	SettingsClass::setActiveExercise( selectedTask );
	
	// save settings for exercise solve task with fractions
	SettingsClass::setAddsub(m_addSub);
	SettingsClass::setAddadd(m_addAdd);
	SettingsClass::setAdddiv(m_addDiv);
	SettingsClass::setAddmult(m_addMult);		
	SettingsClass::setNumber_ratios(m_nrRatios);
	SettingsClass::setMax_main_denominator(m_maxMainDenominator);
	SettingsClass::setReduceForm(m_reducedForm);
	SettingsClass::setSolutionMixed(m_solutionMixed);
	SettingsClass::setQuestionMixed(m_questionMixed);
	SettingsClass::self()->writeConfig();	
}

void MainQtWidget::setupActions()
{
#ifdef DEBUG
	kDebug() << "setupActions MainQtWidget";
#endif

 	QString css;
	css += "QToolButton {";
	css +=		"background-position: top center;";
	css +=		"background-repeat: none;";
	css +=	"}";	
	css += "QToolButton#ArithmeticsButton {";
	css +=		"background-image: url(";
	css +=		KStandardDirs::locate("data", "kbruch/pics/exercise_arithmetics.png");
	css +=		");";
	css +=	"}";
	css += "QToolButton#ComparisonButton {";
	css +=		"background-image: url(";
	css +=		KStandardDirs::locate("data", "kbruch/pics/exercise_compare.png");
	css +=		");";
	css +=	"}";
	css += "QToolButton#ConversionButton {";
	css +=		"background-image: url(";
	css +=		KStandardDirs::locate("data", "kbruch/pics/exercise_conversion.png");
	css +=		");";
	css +=	"}";
	css += "QToolButton#Factorization {";
	css +=		"background-image: url(";
	css +=		KStandardDirs::locate("data", "kbruch/pics/exercise_factorization.png");
	css +=		");";
	css +=	"}";
	css += "QToolButton#Percentage {";
	css +=		"background-image: url(";
	css +=		KStandardDirs::locate("data", "kbruch/pics/exercise_percentage.png");
	css +=		");";
	css +=	"}";		

	setStyleSheet(css);

	m_NewTaskButton = new QToolButton( this );
	m_NewTaskButton->setObjectName("NewTaskButton");
	m_NewTaskButton->setFixedSize( 84,65 );
	m_NewTaskButton->setText( i18n("New") );	
	m_NewTaskButton->setIcon(KIcon("document-new"));
	m_NewTaskButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	m_BackTaskButton = new QToolButton( this );
	m_BackTaskButton->setObjectName("BackTaskButton");
	m_BackTaskButton->setFixedSize( 84,65 );
	m_BackTaskButton->setText( i18n("Back") );	
	m_BackTaskButton->setIcon(KIcon("go-previous"));
	m_BackTaskButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	m_ArithmeticsButton = new QToolButton( this );
	m_ArithmeticsButton->setObjectName("ArithmeticsButton");
	m_ArithmeticsButton->setFixedSize( 84,65 );
	m_ArithmeticsButton->setText( i18n("\n\nArithmetics") );	

	m_ComparisonButton = new QToolButton( this );
	m_ComparisonButton->setObjectName("ComparisonButton");
	m_ComparisonButton->setFixedSize( 84,65 );
	m_ComparisonButton->setText( i18n("\n\nComparison") );	

	m_ConversionButton = new QToolButton( this );
	m_ConversionButton->setObjectName("ConversionButton");
	m_ConversionButton->setFixedSize( 84,65 );
	m_ConversionButton->setText( i18n("\n\nConversion") );	

	m_FactorizationButton = new QToolButton( this );
	m_FactorizationButton->setObjectName("Factorization");
	m_FactorizationButton->setFixedSize( 84,65 );
	m_FactorizationButton->setText( i18n("\n\nFactorization") );	

	m_PercentageButton = new QToolButton( this );
	m_PercentageButton->setObjectName("Percentage");
	m_PercentageButton->setFixedSize( 84,65 );
	m_PercentageButton->setText( i18n("\n\nPercentage") );

   	m_NewTaskActionMenu  = new KAction(KIcon("document-new"), i18nc("@action opens a new question", "&New"), this);
   	actionCollection()->addAction("NewTaskMenu", m_NewTaskActionMenu );
	connect(m_NewTaskActionMenu, SIGNAL(triggered(bool) ), SLOT(NewTask()));
	m_NewTaskActionMenu->setShortcut(KStandardShortcut::shortcut(KStandardShortcut::New));

   	m_NewTaskActionTool  = new KAction(i18nc("@action opens a new question", "New"), this);
   	actionCollection()->addAction("NewTaskTool", m_NewTaskActionTool );
	connect(m_NewTaskButton, SIGNAL(clicked()), SLOT(NewTask()));
	m_NewTaskActionTool->setDefaultWidget( m_NewTaskButton );
        
   	m_BackActionMenu  = new KAction(KIcon("go-previous"), i18nc("@action go to the main screen", "&Back"), this);
   	actionCollection()->addAction("BackMenu", m_BackActionMenu );
	connect(m_BackActionMenu, SIGNAL(triggered(bool) ), SLOT(GoBack()));
	m_BackActionMenu->setShortcut(KStandardShortcut::shortcut(KStandardShortcut::Back));

   	m_BackActionTool  = new KAction(i18nc("@action go to the main screen", "Back"), this);
   	actionCollection()->addAction("BackTool", m_BackActionTool );
	connect(m_BackTaskButton, SIGNAL(clicked()), SLOT(GoBack()));
	m_BackActionTool->setDefaultWidget( m_BackTaskButton );

	m_ArithmeticsAction  = new KAction(i18nc("Arithmetics Exercise", "Arithmetic"), this);
   	actionCollection()->addAction("Arithmetic", m_ArithmeticsAction );
	connect(m_ArithmeticsButton, SIGNAL(clicked()), SLOT(SelectArithmetics()));
	m_ArithmeticsAction->setDefaultWidget( m_ArithmeticsButton );
                  
	m_ComparisonAction  = new KAction(i18nc("Comparison Exercise", "Comparison"), this);
   	actionCollection()->addAction("Comparison", m_ComparisonAction );
	connect(m_ComparisonButton, SIGNAL(clicked()), SLOT(SelectComparison()));
	m_ComparisonAction->setDefaultWidget( m_ComparisonButton );
        
	m_ConversionAction  = new KAction(i18nc("Conversion Exercise", "Conversion"), this);
   	actionCollection()->addAction("Conversion", m_ConversionAction );
	connect(m_ConversionButton, SIGNAL(clicked()), SLOT(SelectConversion()));
	m_ConversionAction->setDefaultWidget( m_ConversionButton );
        
	m_FactorizationAction  = new KAction(i18nc("Factorization Exercise", "Factorization"), this);
   	actionCollection()->addAction("Factorization", m_FactorizationAction );
	connect(m_FactorizationButton, SIGNAL(clicked()), SLOT(SelectFactorization()));
	m_FactorizationAction->setDefaultWidget( m_FactorizationButton );
        
	m_PercentageAction  = new KAction(i18nc("Percentage Exercise", "Percentage"), this);
   	actionCollection()->addAction("Percentage", m_PercentageAction );
	connect(m_PercentageButton, SIGNAL(clicked()), SLOT(SelectPercentage()));
	m_PercentageAction->setDefaultWidget( m_PercentageButton );

	// quit action
	KStandardAction::quit(kapp, SLOT(quit()), actionCollection());

	KStandardAction::preferences(this,  SLOT( slotPrefs() ), actionCollection());

	if (!initialGeometrySet())
		resize( QSize(742, 520).expandedTo(minimumSizeHint()));

	setupGUI(ToolBar | Keys | StatusBar | Create);
	setAutoSaveSettings();
}


/* ------ private slots ------ */

void MainQtWidget::NewTask()
{
#ifdef DEBUG
	kDebug() << "NewTask MainQtWidget";
#endif
	// check which page should generate a new task
	switch (selectedTask)
	{
		case Arithmetics:
			m_taskview->forceNewTask();
			break;
		case Comparison:
			m_exerciseCompare->forceNewTask();
			break;
		case Conversion:
			m_exerciseConvert->forceNewTask();
			break;
		case Factorization:
			m_exerciseFactorize->forceNewTask();
			break;
		case Percentage:
			m_exercisePercentage->forceNewTask();
			break;			
	}
	m_statview->resetStatistics();
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

	if (m_addDiv == TRUE || m_addMult == TRUE)
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
	} 
	
	// set the new task parameters
	(void) m_taskview->setTaskParameters(m_addAdd, m_addDiv, m_addMult, m_addSub, m_nrRatios, m_maxMainDenominator);
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
	(void) m_taskview->setTaskParameters(m_addAdd, m_addDiv, m_addMult, m_addSub, m_nrRatios, m_maxMainDenominator);
}

void MainQtWidget::AnswerMixedCheckSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::AnswerMixedCheckSlot()";
#endif
	m_answerMixed = m_AnswerMixedCheck->isChecked();
	m_taskview->setAnswerMixed(m_answerMixed);
}

void MainQtWidget::SolutionMixedCheckSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::SolutionMixedCheckSlot()";
#endif
	m_solutionMixed = m_SolutionMixedCheck->isChecked();
	m_taskview->setSolutionMixed(m_solutionMixed);
}

void MainQtWidget::QuestionMixedCheckSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::QuestionMixedCheckSlot()";
#endif
	m_questionMixed = m_QuestionMixedCheck->isChecked();
	if ( selectedTask == Arithmetics )		
		m_taskview->setQuestionMixed(m_questionMixed);
	else if ( selectedTask == Comparison )
		m_exerciseCompare->setQuestionMixed(m_questionMixed);
}

void MainQtWidget::ReducedFormCheckSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::ChkReducedFormSlot()";
#endif
	m_reducedForm = m_ReducedCheck->isChecked();
	m_taskview->setReducedForm(m_reducedForm);
}

void MainQtWidget::AdditionCheckSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::AdditionCheckSlot()";
#endif
	if ( OperationsCheck() == TRUE ) {
		if (m_AdditionCheck->checkState() == Qt::Checked)
			m_addAdd = TRUE;
		else
			m_addAdd = FALSE;		 
	} else {
		m_addAdd = TRUE;
		m_AdditionCheck->setCheckState(Qt::Checked);
	}
	(void) m_taskview->setTaskParameters(m_addAdd, m_addDiv, m_addMult, m_addSub, m_nrRatios, m_maxMainDenominator);		
}

void MainQtWidget::SubtractionCheckSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::SubtractionCheckSlot()";
#endif
	if ( OperationsCheck() == TRUE ) {
		if (m_SubtractionCheck->checkState() == Qt::Checked)
			m_addSub = TRUE;
		else
			m_addSub = FALSE;		 
	} else {
		m_addSub = TRUE;
		m_SubtractionCheck->setCheckState(Qt::Checked);
	}
	(void) m_taskview->setTaskParameters(m_addAdd, m_addDiv, m_addMult, m_addSub, m_nrRatios, m_maxMainDenominator);		
}

void MainQtWidget::MultiplicationCheckSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::MultiplicationCheckSlot()";
#endif
	if ( OperationsCheck() == TRUE ) {
		if (m_MultiplicationCheck->checkState() == Qt::Checked)
			m_addMult = TRUE;
		else
			m_addMult = FALSE;		 
	} else {
		m_addMult = TRUE;
		m_MultiplicationCheck->setCheckState(Qt::Checked);
	}
	(void) m_taskview->setTaskParameters(m_addAdd, m_addDiv, m_addMult, m_addSub, m_nrRatios, m_maxMainDenominator);	
}

void MainQtWidget::DivisionCheckSlot()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::DivisionCheckSlot()";
#endif
	if ( OperationsCheck() == TRUE ) {
		if (m_DivisionCheck->checkState() == Qt::Checked)
			m_addDiv = TRUE;
		else
			m_addDiv = FALSE;		 
	} else {
		m_addDiv = TRUE;
		m_DivisionCheck->setCheckState(Qt::Checked);
	}
	(void) m_taskview->setTaskParameters(m_addAdd, m_addDiv, m_addMult, m_addSub, m_nrRatios, m_maxMainDenominator);	
}

void MainQtWidget::slotPrefs()
{
#ifdef DEBUG
	kDebug() << "slotPrefs MainQtWidget";
#endif
	// do not show dialog twice
	if (KConfigDialog::showDialog("settings"))
		return;

	//KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog* configDialog = new KConfigDialog( this, "settings", SettingsClass::self() );

	TaskColors * taskColors = new TaskColors(0);
	configDialog->addPage(taskColors, i18n("Colors"), "preferences-desktop-color");

	TaskFonts * taskFonts = new TaskFonts(0);
	configDialog->addPage(taskFonts, i18n("Fonts"), "preferences-desktop-font");
	// User edited the configuration - update your local copies of the
	// configuration data
	connect(configDialog, SIGNAL(settingsChanged( const QString &)), this, SLOT(slotApplySettings()) );
	configDialog->show();

	return;
}

void MainQtWidget::slotApplySettings()
{
#ifdef DEBUG
	kDebug() << "slotApplySettings MainQtWidget";
#endif
	// update the task view
	m_taskview->update();
	m_exerciseCompare->update();
	m_exerciseConvert->update();
	m_exerciseFactorize->update();

	return;
}

void MainQtWidget::slotAboutToShowPage()
{
#ifdef DEBUG
	kDebug() << "slotAboutToShowPage MainQtWidget";
#endif
	// check which page to show

	switch ( selectedTask ) {
	  case Arithmetics:
		m_QuestionMixedCheck->setEnabled( TRUE );
		m_NrOfTermsBox->setEnabled( TRUE );
		m_MaxMainDenominatorBox->setEnabled( TRUE );		
		m_AnswerMixedCheck->setEnabled( TRUE );		
		m_ReducedCheck->setEnabled( TRUE );		
		m_SolutionMixedCheck->setEnabled( TRUE );		
		m_SubtractionCheck->setEnabled( TRUE );		
		m_DivisionCheck->setEnabled( TRUE );		
		m_MultiplicationCheck->setEnabled( TRUE );		
		m_AdditionCheck->setEnabled( TRUE );								
	    break;
	  case Percentage:	    
	  case Factorization:	  
	  case Conversion:
		m_QuestionMixedCheck->setEnabled( FALSE );
		m_NrOfTermsBox->setEnabled( FALSE );
		m_MaxMainDenominatorBox->setEnabled( FALSE );		
		m_AnswerMixedCheck->setEnabled( FALSE );		
		m_ReducedCheck->setEnabled( FALSE );		
		m_SolutionMixedCheck->setEnabled( FALSE );		
		m_SubtractionCheck->setEnabled( FALSE );		
		m_DivisionCheck->setEnabled( FALSE );		
		m_MultiplicationCheck->setEnabled( FALSE );		
		m_AdditionCheck->setEnabled( FALSE );								
	    break;
	  case Comparison:
		m_QuestionMixedCheck->setEnabled( TRUE );
		m_NrOfTermsBox->setEnabled( FALSE );
		m_MaxMainDenominatorBox->setEnabled( FALSE );		
		m_AnswerMixedCheck->setEnabled( FALSE );		
		m_ReducedCheck->setEnabled( FALSE );		
		m_SolutionMixedCheck->setEnabled( FALSE );		
		m_SubtractionCheck->setEnabled( FALSE );		
		m_DivisionCheck->setEnabled( FALSE );		
		m_MultiplicationCheck->setEnabled( FALSE );		
		m_AdditionCheck->setEnabled( FALSE );	
	    break;
	}
	return;
}

bool MainQtWidget::queryExit()
{
	writeOptions();
	return true;
}

void MainQtWidget::SelectPercentage()
{
#ifdef DEBUG
	kDebug() << "SelectPercentage MainQtWidget";
#endif
	m_TitleLabel->setText(i18n("Percentage"));
	selectedTask = Percentage;
	m_taskview->hide();
	m_exerciseCompare->hide();
	m_exerciseConvert->hide();
	m_exerciseFactorize->hide();
	m_exercisePercentage->show();
	slotAboutToShowPage();
}

void MainQtWidget::SelectArithmetics()
{
#ifdef DEBUG
	kDebug() << "SelectArithmetics MainQtWidget";
#endif
	m_TitleLabel->setText(i18n("Arithmetic"));
	selectedTask = Arithmetics;
	m_taskview->show();
	m_exerciseCompare->hide();
	m_exerciseConvert->hide();
	m_exerciseFactorize->hide();
	m_exercisePercentage->hide();	
	slotAboutToShowPage();
}

void MainQtWidget::SelectComparison()
{
#ifdef DEBUG
	kDebug() << "SelectComparison MainQtWidget";
#endif
	m_TitleLabel->setText(i18n("Comparison"));
	selectedTask = Comparison;
	m_taskview->hide();
	m_exerciseCompare->show();
	m_exerciseConvert->hide();
	m_exerciseFactorize->hide();
	m_exercisePercentage->hide();	
	slotAboutToShowPage();
}

void MainQtWidget::SelectConversion()
{
#ifdef DEBUG
	kDebug() << "SelectConversion MainQtWidget";
#endif
	m_TitleLabel->setText(i18n("Conversion"));
	selectedTask = Conversion;
	m_taskview->hide();
	m_exerciseCompare->hide();
	m_exerciseConvert->show();
	m_exerciseFactorize->hide();
	m_exercisePercentage->hide();		
	slotAboutToShowPage();
}

void MainQtWidget::SelectFactorization()
{
#ifdef DEBUG
	kDebug() << "SelectFactorization MainQtWidget";
#endif
	m_TitleLabel->setText(i18n("Factorization"));
	selectedTask = Factorization;
	m_taskview->hide();
	m_exerciseCompare->hide();
	m_exerciseConvert->hide();
	m_exercisePercentage->hide();		
	m_exerciseFactorize->show();
	slotAboutToShowPage();
}

bool MainQtWidget::OperationsCheck()
{
#ifdef DEBUG
	kDebug() << "MainQtWidget::OperationsCheck()";
#endif

        if ( m_AdditionCheck->checkState() == Qt::Unchecked &&
	     m_SubtractionCheck->checkState() == Qt::Unchecked && 
	     m_DivisionCheck->checkState() == Qt::Unchecked &&
	     m_MultiplicationCheck->checkState() == Qt::Unchecked )
		return FALSE;
	else
		return TRUE;
}	     

void MainQtWidget::GoBack()
{
	kbruchApp = new AppMenuWidget();
	kbruchApp->show();
	close();
}

#include "mainqtwidget.moc"
