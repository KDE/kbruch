/***************************************************************************
                          mainqtwidget.h  -  The main Qt/KDE window
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
#ifndef MAINQTWIDGET_H
#define MAINQTWIDGET_H

#include <kxmlguiwindow.h>
#include <QGridLayout>
#include <kapplication.h>
class KAction;
class KComboBox;

class QLabel;
class QCheckBox;
class QGroupBox;
class QGridLayout;
class QFrame;
class QToolButton;
class QPushButton;

class ExerciseCompare;
class ExerciseConvert;
class ExerciseFactorize;
class ExercisePercentage;
class StatisticsView;
class TaskView;
class AppMenuWidget;

class QGridLayout;
/** Constructs the main window and presents the user interface.
 *  The window is separated into 2 sections. In the left section is the
 *  statistic view and in the right section the task view.
 *  \author Sebastian Stein
 *  \author Eva Brucherseifer
 **/

enum ExerciseType
{
	Arithmetics,
	Comparison,
	Conversion,
	Factorization,
	Percentage
};

class MainQtWidget : public KXmlGuiWindow
{
	Q_OBJECT
			  
	public:

		/** constructor */
		MainQtWidget();

		/** destructor */
		~MainQtWidget();

	private:
  
		/** read the config file */
		void readOptions();

		/** read the config file */
		void writeOptions();

		/** pointing to the statistics view */
		StatisticsView * m_statview;

		/** pointing to the exercise solving a task with fractions; added as page
		 * to the iconlist */
		TaskView * m_taskview;

		/** pointing to the exercise comparing ratios; added as page to the
		 * iconlist */ 
		ExerciseCompare * m_exerciseCompare;

		/** pointing to the exercise convert rational number; added as page to the
		 * iconlist */ 
		ExerciseConvert * m_exerciseConvert;
	
		/** pointing to the exercise factorize a given number; added as page to
		 * the iconlist */ 
		ExerciseFactorize * m_exerciseFactorize;

		/** pointing to the exercise percentage; added as page to the icon list */
		ExercisePercentage * m_exercisePercentage;

		ExerciseType selectedTask;	
		bool m_addSub;
		bool m_addAdd;
		bool m_addMult;
		bool m_addDiv;
		unsigned int m_nrRatios;
		unsigned int m_maxMainDenominator;
		bool m_questionMixed;
		bool m_answerMixed;
		bool m_solutionMixed;
		bool m_reducedForm;
		QFont defaultFont;

		// Visible Components
		QLabel * m_OptionsLabel;
		QLabel * m_TitleLabel;


		QGroupBox * m_QuestionGroup;
		QGroupBox * m_AnswerGroup;
		QGroupBox * m_SolutionGroup;
		QGroupBox * m_OperationsGroup;

		QToolButton * m_ArithmeticsButton;
		QToolButton * m_ComparisonButton;
		QToolButton * m_ConversionButton;
		QToolButton * m_FactorizationButton;
		QToolButton * m_NewTaskButton;
		QToolButton * m_BackTaskButton;
		QToolButton * m_PercentageButton;
		
		// Question Group
		KComboBox * m_NrOfTermsBox;
		QLabel * m_NrOfTermsLabel;

		KComboBox * m_MaxMainDenominatorBox;
		QLabel * m_MaxMainDenominatorLabel;

		QCheckBox * m_QuestionMixedCheck;
		QLabel * m_QuestionMixedLabel;

		
		// Answer group
		QCheckBox * m_AnswerMixedCheck;
		QLabel * m_AnswerMixedLabel;

		QCheckBox * m_ReducedCheck;	
		QLabel * m_ReducedLabel;

		
		// Solution group
		QCheckBox * m_SolutionMixedCheck;
		QLabel * m_SolutionMixedLabel;


		// Operations group
		QCheckBox * m_AdditionCheck;
		QLabel * m_AdditionLabel;

		QCheckBox * m_SubtractionCheck;
		QLabel * m_SubtractionLabel;

		QCheckBox * m_MultiplicationCheck;
		QLabel * m_MultiplicationLabel;

		QCheckBox * m_DivisionCheck;
		QLabel * m_DivisionLabel;
			
		// Menu exercise actions
		KAction * m_ArithmeticsAction;
		KAction * m_ComparisonAction;
		KAction * m_ConversionAction;
		KAction * m_FactorizationAction;
		KAction * m_NewTaskActionMenu;
		KAction * m_NewTaskActionTool;
		KAction * m_BackAction;
		KAction * m_HintAction;
		KAction * m_PercentageAction;
		
		QFrame * m_footerline;
		QFrame * m_headerline;

		AppMenuWidget * kbruchApp;

		void setupActions();

	private slots:

		bool OperationsCheck();

		/** 
		 * Tasks selection 
		 */
		void SelectArithmetics();
		void SelectComparison();
		void SelectConversion();
		void SelectFactorization();
		void SelectPercentage();
		
		/** 
		 * Options Slot changes
		 */
		// called, when the user changes the state of reduced form checkbox
		void ReducedFormCheckSlot();
		// called, when the user changes the state of mixed number checkbox of question group
		void QuestionMixedCheckSlot();
		// called, when the user changes the state of mixed number checkbox of answer group
		void AnswerMixedCheckSlot();
		// called, when the user changes the state of mixed number checkbox of solution group
		void SolutionMixedCheckSlot();
		// called, when the user changes the state of addition task operation
		void AdditionCheckSlot();
		// called, when the user changes the state of subtraction task operation
		void SubtractionCheckSlot();
		// called, when the user changes the state of multiplication task operation
		void MultiplicationCheckSlot();
		// called, when the user changes the state of division task operation
		void DivisionCheckSlot();
		 // called when the user changes the number of terms in the ComboBox 
		void NrOfTermsBoxSlot();
		 // called, when the user changes the max. size of the main denominator in the ComboBox 
		void MaxMainDenominatorBoxSlot();


		/** 
		 * called to force generation of a new task 
		 */
		void NewTask();


		/**
		 * calls the settings dialog
		 */
		void slotPrefs();


		/**
		 * makes sure, all parts of the UI update to new settings
		 */
		void slotApplySettings();


		/**
		 * called just before another page is shown
		 */
		void slotAboutToShowPage();
		
		
		/**
		 * called to go back to the main screen and choose another mode
		 */
		void GoBack();

	protected:
		/** Function is called when user calls termination.
		 *  Used to save the current statistics and settings before exiting.
		 **/
		bool queryExit();
};

#endif // MainQtWidget
