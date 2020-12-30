/*
    MainQtWidget.cpp  -  The main Qt/KDE window
    SPDX-FileCopyrightText: 2003-2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tiago Porangaba <tiago.porangaba@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAINQTWIDGET_H
#define MAINQTWIDGET_H

#include <KXmlGuiWindow>
#include <QRadioButton>

class QAction;
class QWidgetAction;
class QComboBox;

class QLabel;
class QCheckBox;
class QGroupBox;
class QGridLayout;
class QFrame;
class QToolButton;

class ExerciseCompare;
class ExerciseConvert;
class ExerciseFactorize;
class ExerciseMixedNumbers;
class ExercisePercentage;
class StatisticsView;
class TaskView;
class AppMenuWidget;
class ExerciseMixedNumbers;


class QGridLayout;


enum ExerciseType {
    Arithmetic,
    Comparison,
    Conversion,
    MixedNumbers,
    Factorization,
    Percentage
};

/** Constructs the main window and presents the user interface.
 *  The window is separated into 2 sections. In the left section is the
 *  statistic view and in the right section the task view.
 *  \author Sebastian Stein
 *  \author Eva Brucherseifer
 **/
class MainQtWidget : public KXmlGuiWindow
{
    Q_OBJECT

public:

    /** constructor */
    MainQtWidget();

    /** destructor */
    ~MainQtWidget();

    static QFont DefaultFont();

private:

    /** read the config file */
    void readOptions();

    /** read the config file */
    void writeOptions();

    /** pointing to the statistics view */
    StatisticsView * m_statview;

    /** pointing to the exercise solving a task with fractions */
    TaskView * m_taskview;

    /** pointing to the exercise comparing ratios */
    ExerciseCompare * m_exerciseCompare;

    /** pointing to the exercise convert rational number */
    ExerciseConvert * m_exerciseConvert;

    /** pointing to the exercise for converting mixed numbers to ratios */
    ExerciseMixedNumbers * m_exerciseMixedNumbers;

    /** pointing to the exercise factorize a given number */
    ExerciseFactorize * m_exerciseFactorize;

    /** pointing to the exercise percentage */
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
    bool m_reducedForm;
    QFont defaultFont;

    // Visible Components
    QLabel * m_OptionsLabel;
    QLabel * m_TitleLabel;


    QGroupBox * m_QuestionGroup;
    QGroupBox * m_AnswerGroup;
    QGroupBox * m_OperationsGroup;

    QToolButton * m_ArithmeticButton;
    QToolButton * m_ComparisonButton;
    QToolButton * m_ConversionButton;
    QToolButton * m_MixedNumbersButton;
    QToolButton * m_FactorizationButton;
    QToolButton * m_NewTaskButton;
    QToolButton * m_BackTaskButton;
    QToolButton * m_PercentageButton;

    // Question Group
    QComboBox * m_NrOfTermsBox;
    QLabel * m_NrOfTermsLabel;

    QComboBox * m_MaxMainDenominatorBox;
    QLabel * m_MaxMainDenominatorLabel;

    QCheckBox * m_QuestionMixedCheck;
    QLabel * m_QuestionMixedLabel;


    // Answer group
    QRadioButton * m_AnswerMixedCheck;
    QLabel * m_AnswerMixedLabel;

    QRadioButton * m_ReducedCheck;
    QLabel * m_ReducedLabel;


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
    QWidgetAction * m_ArithmeticAction;
    QWidgetAction * m_ComparisonAction;
    QWidgetAction * m_ConversionAction;
    QWidgetAction * m_MixedNumbersAction;
    QWidgetAction * m_FactorizationAction;
    QWidgetAction * m_NewTaskActionMenu;
    QWidgetAction * m_NewTaskActionTool;
    QWidgetAction * m_BackActionTool;
    QWidgetAction * m_BackActionMenu;
    QAction * m_HintAction;
    QWidgetAction * m_PercentageAction;

    QFrame * m_footerline;
    QFrame * m_headerline;

    AppMenuWidget * kbruchApp;

    void setupActions();

private Q_SLOTS:
    bool OperationsCheck();

    /**
     * Tasks selection
     */
    void SelectArithmetic();
    void SelectComparison();
    void SelectConversion();
    void SelectMixedNumbers();
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
    void newTask();


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
    void goBack();

protected:
    /** Function is called when user closes the window.
     *  Used to save the current statistics and settings before exiting.
     */
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
};

#endif // MainQtWidget
