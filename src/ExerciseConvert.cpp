/*
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ExerciseConvert.h"

/* these includes are needed for KDE support */
#include <KMessageBox>
#include <KLocalizedString>

/* these includes are needed for Qt support */
#include <QApplication>
#include <QIntValidator>
#include <QLayout>
#include <QLineEdit>
#include <QLocale>
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
ExerciseConvert::ExerciseConvert(QWidget * parent) :
    ExerciseBase(parent)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor ExerciseConvert()");
#endif

    /* create a new task */
    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */
    createTask();
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    // to validate, that the input is an int
    QIntValidator *valnum = new QIntValidator(this);

    QFont defaultFont = SettingsClass::taskFont();
    defaultFont.setBold(true);
    defaultFont.setPointSize(18);

    m_currentState = _CHECK_TASK;

    // Create layout
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

    // first left is the rational widget
    m_rationalWidget = new RationalWidget(taskWidget, m_number, m_periodStart, m_periodLength);
    m_rationalWidget->setObjectName(QStringLiteral("m_rationalWidget"));
    taskLayout->addWidget(m_rationalWidget, 1, 1, 3, 1);

    /* add input box so the user can enter numerator */
    numer_edit = new QLineEdit(taskWidget);
    numer_edit->setObjectName(QStringLiteral("numer_edit"));
    numer_edit->setValidator(valnum);   // use the int validator
    numer_edit->setToolTip(i18n("Enter the numerator of your result"));
    numer_edit->setFixedSize(85, 42);
    numer_edit->setAlignment(Qt::AlignHCenter);
    numer_edit->setFont(defaultFont);
    QObject::connect(numer_edit, &QLineEdit::returnPressed, this, &ExerciseConvert::numeratorReturnPressed);
    taskLayout->addWidget(numer_edit, 1, 2);

    /* add a line between the edit boxes */
    edit_line = new QFrame(taskWidget);
    edit_line->setGeometry(QRect(100, 100, 20, 20));
    edit_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    taskLayout->addWidget(edit_line, 2, 2);

    /* add input box so the user can enter denominator */
    deno_edit = new QLineEdit(taskWidget);
    deno_edit->setObjectName(QStringLiteral("deno_edit"));
    deno_edit->setValidator(valnum);   // use the int validator
    deno_edit->setToolTip(i18n("Enter the denominator of your result"));
    deno_edit->setFixedSize(85, 42);
    deno_edit->setAlignment(Qt::AlignHCenter);
    deno_edit->setFont(defaultFont);
    QObject::connect(deno_edit, &QLineEdit::returnPressed, this, &ExerciseConvert::denominatorReturnPressed);
    taskLayout->addWidget(deno_edit, 3, 2);

    // next is the result widget
    m_resultWidget = new ResultWidget(checkWidget, m_result);
    m_resultWidget->setObjectName(QStringLiteral("m_resultWidget"));
    checkLayout->addWidget(m_resultWidget, 0, 0, 1, 2);

    defaultFont.setPointSize(10);

    // the right aligned button
    m_checkButton = new QPushButton(checkWidget);
    m_checkButton->setObjectName(QStringLiteral("m_checkButton"));
    m_checkButton->setText(i18n("&Check"));
    m_checkButton->setDefault(true);  // is the default button of the dialog
    m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));
    m_checkButton->setFont(defaultFont);
    QObject::connect(m_checkButton, &QPushButton::clicked, this, &ExerciseConvert::slotCheckButtonClicked);
    checkLayout->addWidget(m_checkButton, 1, 0);

    // the right aligned button
    m_skipButton = new QPushButton(checkWidget);
    m_skipButton->setObjectName(QStringLiteral("m_skipButton"));
    m_skipButton->setText(i18n("&Skip"));
    m_skipButton->setToolTip(i18n("Click on this button to skip this question."));
    m_skipButton->setFont(defaultFont);
    QObject::connect(m_skipButton, &QPushButton::clicked, this, &ExerciseConvert::slotSkipButtonClicked);
    checkLayout->addWidget(m_skipButton, 1, 1);

    setLayout(baseGrid);
    taskWidget->setLayout(taskLayout);
    checkWidget->setLayout(checkLayout);

    // add tooltip and qwhatsthis help to the widget
    setToolTip(i18n("In this exercise you have to convert a number into a fraction."));
    setWhatsThis(i18n("In this exercise you have to convert a given number into a fraction by entering a numerator and denominator. Do not forget to reduce the result."));
}

/* destructor */
ExerciseConvert::~ExerciseConvert()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("destructor ExerciseConvert()");
#endif

    /* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExerciseConvert::forceNewTask()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("forceNewTask ExerciseConvert()");
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

void ExerciseConvert::update()
{
    // call update of components
    m_rationalWidget->updateAndRepaint();
    m_resultWidget->updateAndRepaint();

    // update for itself
    ((QWidget *) this)->update();
}


/* ------ private member functions ------ */

void ExerciseConvert::createTask()
{
    // the tasks are hardcoded here; there are some algorithms to convert
    // rational numbers to fractions, but it is not worth the effort here
    switch (int ((double(qrand()) / RAND_MAX) * 19 )) {
    case  0 :   m_number = QLocale().toString(0.5, 'f', 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 2);
        break;
    case  1 :   m_number = QLocale().toString(0.3, 'f', 1);
        m_periodStart = 2;
        m_periodLength = 1;
        m_result = Ratio(1, 3);
        break;
    case  2 :   m_number = QLocale().toString(0.6, 'f', 1);
        m_periodStart = 2;
        m_periodLength = 1;
        m_result = Ratio(2, 3);
        break;
    case  3 :   m_number = QLocale().toString(0.25, 'f', 2);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 4);
        break;
    case  4 :   m_number = QLocale().toString(0.75, 'f', 2);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(3, 4);
        break;
    case  5 :   m_number = QLocale().toString(0.2, 'f', 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 5);
        break;
    case  6 :   m_number = QLocale().toString(0.4, 'f', 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(2, 5);
        break;
    case  7 :   m_number = QLocale().toString(0.6, 'f', 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(3, 5);
        break;
    case  8 :   m_number = QLocale().toString(0.8, 'f', 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(4, 5);
        break;
    case  9 :   m_number = QLocale().toString(0.16, 'f', 2);
        m_periodStart = 3;
        m_periodLength = 1;
        m_result = Ratio(1, 6);
        break;
    case 10 :   m_number = QLocale().toString(0.142857, 'f', 6);
        m_periodStart = 2;
        m_periodLength = 6;
        m_result = Ratio(1, 7);
        break;
    case 11 :   m_number = QLocale().toString(0.125, 'f', 3);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 8);
        break;
    case 12 :   m_number = QLocale().toString(0.375, 'f', 3);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(3, 8);
        break;
    case 13 :   m_number = QLocale().toString(0.1, 'f', 1);
        m_periodStart = 2;
        m_periodLength = 1;
        m_result = Ratio(1, 9);
        break;
    case 14 :   m_number = QLocale().toString(0.1, 'f', 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 10);
        break;
    case 15 :   m_number = QLocale().toString(0.05, 'f', 2);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 20);
        break;
    case 16 :   m_number = QLocale().toString(0.01, 'f', 2);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 100);
        break;
    case 17 :   m_number = QLocale().toString(0.83, 'f', 2);
        m_periodStart = 3;
        m_periodLength = 1;
        m_result = Ratio(5, 6);
        break;
    default :
    case 18 :   m_number = QLocale().toString(0.001, 'f', 3);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 1000);
        break;
    }

    return;
}

/** - checks, if the user solved the task correctly
        - emits signals if task was solved correctly or wrong */
void ExerciseConvert::showResult()
{
    Ratio entered_result;

    // change the tooltip of the check button
    m_checkButton->setToolTip(i18n("Click on this button to get to the next question."));

    numer_edit->setEnabled(false);
    deno_edit->setEnabled(false);
    m_skipButton->setEnabled(false);

    // an empty numerator field will be interpreted as 0
    if (numer_edit->text().isEmpty() == true)
        numer_edit->setText(QStringLiteral("0"));

    // an empty denominator field will be interpreted as 1
    if (deno_edit->text().isEmpty() == true)
        deno_edit->setText(QStringLiteral("1"));

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
    if ((deno_edit->text().toInt() != 0) && ((entered_result == m_result) ||
            (m_result.numerator() == 0 && entered_result.numerator() == 0))) {
        // emit the signal for correct
        Q_EMIT signalExerciseSolvedCorrect();

        /* yes, the user entered the correct result */
        m_resultWidget->setResult(m_result, 1);
    } else {
        // emit the signal for wrong
        Q_EMIT signalExerciseSolvedWrong();

        /* no, the user entered the wrong result */
        m_resultWidget->setResult(m_result, 0);

        // if the user entered a 0 for the denominator (division by 0) we have to
        // get the 0 directly from the input field, because
        // Ratio::setDenominator(0, true) will set the denominator to 1 to ensure
        // the Ratio is valid
        if (deno_edit->text().toInt() == 0) {
            KMessageBox::information(this,
                                     i18n("You entered a 0 as the denominator. This means division by zero, which is not allowed. This question will be counted as not correctly solved."));
        } else {
            /* maybe the entered ratio was not reduced */
            entered_result.reduce();
            if (entered_result == m_result)
                KMessageBox::information(this,
                                         i18n("You entered the correct result, but not reduced.\nAlways enter your results as reduced. This question will be counted as not correctly solved."));
        }
    } /* if (entered_result == result) */

    return;
}

/** generate the next task and show it to the user */
void ExerciseConvert::nextTask()
{
    // change the tooltip of the check button
    m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));

    numer_edit->setEnabled(true);
    deno_edit->setEnabled(true);
    m_skipButton->setEnabled(true);

    m_resultWidget->setResult(m_result, -1);

    /* clear user input */
    deno_edit->clear();
    numer_edit->clear();
    numer_edit->setFocus();

    /* create a new task */
    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */
    createTask();
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    // update the task widget
    m_rationalWidget->setRational(m_number, m_periodStart, m_periodLength);

    return;
}

/* ------ private slots ------ */

void ExerciseConvert::slotCheckButtonClicked()
{
    if (m_currentState == _CHECK_TASK) {
        // if nothing has been entered by the user, we don't check the result yet
        if (numer_edit->text().isEmpty() == true && deno_edit->text().isEmpty() ==
                true)
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

void ExerciseConvert::slotSkipButtonClicked()
{
    forceNewTask();
}

/* ------ private slots ------ */

void ExerciseConvert::numeratorReturnPressed()
{
    deno_edit->setFocus();
}

void ExerciseConvert::denominatorReturnPressed()
{
    slotCheckButtonClicked();
}

/* ------ protected events ------ */
void ExerciseConvert::showEvent(QShowEvent *)
{

    // that the user can start typing without moving the focus
    numer_edit->setFocus();

}
