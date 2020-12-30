/*
    SPDX-FileCopyrightText: 2008 Tiago Porangaba <tiago.porangaba@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ExercisePercentage.h"

/* these includes are needed for KDE support */
#include <KLocalizedString>
#include <KMessageBox>

/* these includes are needed for Qt support */
#include <QApplication>
#include <QGridLayout>
#include <QIntValidator>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>

#ifdef DEBUG
#include <QDebug>
#endif

/* standard C++ library includes */
#include <cstdlib>

#include "RationalWidget.h"
#include "ResultWidget.h"
#include "settingsclass.h"

/* ----- public member functions ----- */

/* constructor */
ExercisePercentage::ExercisePercentage(QWidget * parent) :
    ExerciseBase(parent)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor ExercisePercentage()");
#endif

    /* create a new task */
    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */
    createTask();
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    // to validate, that the input is an int
    QIntValidator *valnum = new QIntValidator(this);

    QFont defaultFont = SettingsClass::taskFont();
    defaultFont.setBold(true);
    defaultFont.setPointSize(10);

    // the next thing to do on a button click would be to check the entered
    // result
    m_currentState = _CHECK_TASK;

    taskWidget = new QWidget(this);
    taskWidget->setObjectName(QStringLiteral("taskWidget"));
    checkWidget = new QWidget(this);
    checkWidget->setObjectName(QStringLiteral("checkWidget"));

    baseGrid = new QGridLayout(this);
    baseGrid->setObjectName(QStringLiteral("baseGrid"));
    baseGrid->setColumnStretch(0, 1);

    baseGrid->addWidget(taskWidget, 0, 0);
    baseGrid->addWidget(checkWidget, 0, 1);

    taskLayout = new QGridLayout(taskWidget);
    taskLayout->setObjectName(QStringLiteral("taskLayout"));
    taskLayout->setRowStretch(0, 1);
    taskLayout->setRowStretch(4, 1);
    taskLayout->setColumnStretch(0, 1);
    taskLayout->setColumnStretch(3, 1);

    checkLayout = new QGridLayout(checkWidget);
    checkLayout->setObjectName(QStringLiteral("checkLayout"));

    /* Task: percentage question */
    defaultFont.setPointSize(16);
    m_taskLabel = new QLabel(this);
    m_taskLabel->setObjectName(QStringLiteral("m_taskLabel"));
    m_taskLabel->setFont(defaultFont);
    m_taskLabel->setText(i18nc("%1 percentage of %2", "%1% of %2 = ", m_numberPercentage ,  m_numberPercentageOf));
    taskLayout->addWidget(m_taskLabel, 1, 1, 2, 1);

    /* Input question: result of question */
    answer_edit = new QLineEdit(taskWidget);
    answer_edit->setObjectName(QStringLiteral("answer_edit"));
    answer_edit->setValidator(valnum);   // use the int validator
    answer_edit->setToolTip(i18n("Enter the result of percentage question"));
    answer_edit->setFixedSize(85, 42);
    answer_edit->setAlignment(Qt::AlignHCenter);
    answer_edit->setFont(defaultFont);
    QObject::connect(answer_edit, &QLineEdit::returnPressed, this, &ExercisePercentage::answerReturnPressed);
    taskLayout->addWidget(answer_edit, 1, 2, 2, 1);

    // next is the result widget
    m_resultWidget = new ResultWidget(checkWidget, m_result);
    m_resultWidget->setObjectName(QStringLiteral("m_resultWidget"));
    checkLayout->addWidget(m_resultWidget, 0, 0, 1, 2);

    // the right aligned button
    defaultFont.setPointSize(10);
    m_checkButton = new QPushButton(checkWidget);
    m_checkButton->setObjectName(QStringLiteral("m_checkButton"));
    m_checkButton->setText(i18n("&Check"));
    m_checkButton->setDefault(true);  // is the default button of the dialog
    m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));
    m_checkButton->setFont(defaultFont);
    QObject::connect(m_checkButton, &QPushButton::clicked, this, &ExercisePercentage::slotCheckButtonClicked);
    checkLayout->addWidget(m_checkButton, 2, 0);

    // the right aligned button
    m_skipButton = new QPushButton(checkWidget);
    m_skipButton->setObjectName(QStringLiteral("m_skipButton"));
    m_skipButton->setText(i18n("&Skip"));
    m_skipButton->setToolTip(i18n("Click on this button to skip this question."));
    m_skipButton->setFont(defaultFont);
    QObject::connect(m_skipButton, &QPushButton::clicked, this, &ExercisePercentage::slotSkipButtonClicked);
    checkLayout->addWidget(m_skipButton, 2, 1);

    // that the user can start typing without moving the focus
    //numer_edit->setFocus();
    answer_edit->setFocus();

    setLayout(baseGrid);
    taskWidget->setLayout(taskLayout);
    checkWidget->setLayout(checkLayout);

    // add tooltip and qwhatsthis help to the widget
    setToolTip(i18n("In this exercise you have to work with percentage questions."));
    setWhatsThis(i18n("In this exercise you have to enter the fraction of the given number."));

}

/* destructor */
ExercisePercentage::~ExercisePercentage()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("destructor ExercisePercentage()");
#endif

    /* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExercisePercentage::forceNewTask()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("forceNewTask ExercisePercentage()");
#endif

    if (m_currentState == _CHECK_TASK) {
        // emit the signal for skipped
        Q_EMIT signalExerciseSkipped();
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
    switch (int ((double(qrand()) / RAND_MAX) * 19)) {
    case  0 :
        //m_number = QLocale().toString(0.5, 'f', 1);
        m_numberPercentage = QStringLiteral("75");
        m_numberPercentageOf = QStringLiteral("1900");
        m_resultPercentage = QStringLiteral("1425");
        break;
    case  1 :
        //m_number = QLocale().toString(0.3, 'f', 1);
        m_numberPercentage = QStringLiteral("50");
        m_numberPercentageOf = QStringLiteral("1800");
        m_resultPercentage = QStringLiteral("900");
        break;
    case  2 :
        //m_number = QLocale().toString(0.6, 'f', 1);
        m_numberPercentage = QStringLiteral("1");
        m_numberPercentageOf = QStringLiteral("1200");
        m_resultPercentage = QStringLiteral("12");
        break;
    case  3 :
        //m_number = QLocale().toString(0.25, 'f', 2);
        m_numberPercentage = QStringLiteral("10");
        m_numberPercentageOf = QStringLiteral("900");
        m_resultPercentage = QStringLiteral("90");
        break;
    case  4 :
        //m_number = QLocale().toString(0.75, 'f', 2);
        m_numberPercentage = QStringLiteral("100");
        m_numberPercentageOf = QStringLiteral("800");
        m_resultPercentage = QStringLiteral("800");
        break;
    case  5 :
        //m_number = QLocale().toString(0.2, 'f', 1);
        m_numberPercentage = QStringLiteral("75");
        m_numberPercentageOf = QStringLiteral("300");
        m_resultPercentage = QStringLiteral("225");
        break;
    case  6 :
        //m_number = QLocale().toString(0.4, 'f', 1);
        m_numberPercentage = QStringLiteral("10");
        m_numberPercentageOf = QStringLiteral("1500");
        m_resultPercentage = QStringLiteral("150");
        break;
    case  7 :
        //m_number = QLocale().toString(0.6, 'f', 1);
        m_numberPercentage = QStringLiteral("10");
        m_numberPercentageOf = QStringLiteral("100");
        m_resultPercentage = QStringLiteral("10");
        break;
    case  8 :
        //m_number = QLocale().toString(0.8, 'f', 1);
        m_numberPercentage = QStringLiteral("25");
        m_numberPercentageOf = QStringLiteral("400");
        m_resultPercentage = QStringLiteral("100");
        break;
    case  9 :
        //m_number = QLocale().toString(0.16, 'f', 2);
        m_numberPercentage = QStringLiteral("50");
        m_numberPercentageOf = QStringLiteral("800");
        m_resultPercentage = QStringLiteral("400");
        break;
    case 10 :
        //m_number = QLocale().toString(0.142857, 'f', 6);
        m_numberPercentage = QStringLiteral("1");
        m_numberPercentageOf = QStringLiteral("400");
        m_resultPercentage = QStringLiteral("4");
        break;
    case 11 :
        //m_number = QLocale().toString(0.125, 'f', 3);
        m_numberPercentage = QStringLiteral("50");
        m_numberPercentageOf = QStringLiteral("600");
        m_resultPercentage = QStringLiteral("300");
        break;
    case 12 :
        //m_number = QLocale().toString(0.375, 'f', 3);
        m_numberPercentage = QStringLiteral("100");
        m_numberPercentageOf = QStringLiteral("1300");
        m_resultPercentage = QStringLiteral("1300");
        break;
    case 13 :
        //m_number = QLocale().toString(0.1, 'f', 1);
        m_numberPercentage = QStringLiteral("100");
        m_numberPercentageOf = QStringLiteral("800");
        m_resultPercentage = QStringLiteral("800");
        break;
    case 14 :
        //m_number = QLocale().toString(0.1, 'f', 1);
        m_numberPercentage = QStringLiteral("25");
        m_numberPercentageOf = QStringLiteral("1400");
        m_resultPercentage = QStringLiteral("350");
        break;
    case 15 :
        //m_number = QLocale().toString(0.05, 'f', 2);
        m_numberPercentage = QStringLiteral("10");
        m_numberPercentageOf = QStringLiteral("1400");
        m_resultPercentage = QStringLiteral("140");
        break;
    case 16 :
        //m_number = QLocale().toString(0.01, 'f', 2);
        m_numberPercentage = QStringLiteral("1");
        m_numberPercentageOf = QStringLiteral("2000");
        m_resultPercentage = QStringLiteral("20");
        break;
    case 17 :
        //m_number = QLocale().toString(0.83, 'f', 2);
        m_numberPercentage = QStringLiteral("75");
        m_numberPercentageOf = QStringLiteral("1000");
        m_resultPercentage = QStringLiteral("750");
        break;
    default :
    case 18 :
        //m_number = QLocale().toString(0.001, 'f', 3);
        m_numberPercentage = QStringLiteral("75");
        m_numberPercentageOf = QStringLiteral("1100");
        m_resultPercentage = QStringLiteral("825");
        break;
    }

    return;
}

/** - checks, if the user solved the task correctly
        - emits signals if task was solved correctly or wrong */
void ExercisePercentage::showResult()
{
    int tmp_result;
    Ratio entered_result;
    Ratio correct_result;

    // change the tooltip of the check button
    m_checkButton->setToolTip(i18n("Click on this button to get to the next question."));

    answer_edit->setEnabled(false);
    m_skipButton->setEnabled(false);

    //an empty answer field will be interpreted as 0
    if (answer_edit->text().isEmpty() == true)
        answer_edit->setText(QStringLiteral("0"));


    tmp_result = answer_edit->text().toInt();
    entered_result.setNumerator(tmp_result, false);
    entered_result.setDenominator(1, false);
    correct_result.setNumerator(m_resultPercentage.toInt());
    correct_result.setDenominator(1, false);
    if (tmp_result == m_resultPercentage.toInt()) {
        // emit the signal for correct
        Q_EMIT signalExerciseSolvedCorrect();
        /* yes, the user entered the correct result */
        m_resultWidget->setResult(entered_result, 1);
    } else {
        // emit the signal for incorrect
        Q_EMIT signalExerciseSolvedWrong();
        /* no, the user entered the wrong result */
        m_resultWidget->setResult(correct_result, 0);
    }

    return;
}

/** generate the next task and show it to the user */
void ExercisePercentage::nextTask()
{
    // change the tooltip of the check button
    m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));

    answer_edit->setEnabled(true);
    m_skipButton->setEnabled(true);

    m_resultWidget->setResult(m_result, -1);

    /* clear user input */
    answer_edit->clear();
    answer_edit->setFocus();

    /* create a new task */
    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */
    createTask();
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    // update the task widget
    QString tempTask = i18nc("%1 percentage of %2", "%1% of %2 = ", m_numberPercentage, m_numberPercentageOf);
    m_taskLabel->setText(tempTask);

    return;
}

/* ------ private slots ------ */

void ExercisePercentage::slotCheckButtonClicked()
{
    if (m_currentState == _CHECK_TASK) {
        // if nothing has been entered by the user, we don't check the result yet
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

void ExercisePercentage::answerReturnPressed()
{
    slotCheckButtonClicked();
}
