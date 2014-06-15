/***************************************************************************
                              ExerciseConvert.cpp
                             -------------------
    begin                : 2004/06/04
    copyright            : (C) 2004 by Sebastian Stein
    email                : seb.kde@hpfsc.de

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

#include "ExerciseConvert.h"
#include "ExerciseConvert.moc"

/* these includes are needed for KDE support */
#include <kglobal.h>
#include <klineedit.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <knumvalidator.h>

/* these includes are needed for Qt support */
#include <QApplication>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>

//Added by qt3to4:
#include <QGridLayout>

/* standard C++ library includes */
#include <stdlib.h>

#include "RationalWidget.h"
#include "ResultWidget.h"
#include "settingsclass.h"

/* ----- public member functions ----- */

/* constructor */
ExerciseConvert::ExerciseConvert(QWidget * parent) :
    ExerciseBase(parent)
{
#ifdef DEBUG
    kDebug() << "constructor ExerciseConvert()";
#endif

    /* create a new task */
    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */
    createTask();
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    // to validate, that the input is an int
    KIntValidator *valnum = new KIntValidator(this);

    QFont defaultFont = SettingsClass::taskFont();
    defaultFont.setBold(true);
    defaultFont.setPointSize(18);

    m_currentState = _CHECK_TASK;

    // Create layout
    taskWidget = new QWidget(this);
    taskWidget->setObjectName("taskWidget");
    checkWidget = new QWidget(this);
    checkWidget->setObjectName("checkWidget");

    baseGrid = new QGridLayout(this);
    baseGrid->setObjectName("baseGrid");
    baseGrid->setColumnStretch(0, 1);

    baseGrid->addWidget(taskWidget, 0, 0);
    baseGrid->addWidget(checkWidget, 0, 1);

    taskLayout = new QGridLayout(taskWidget);
    taskLayout->setObjectName("taskLayout");
    taskLayout->setRowStretch(0, 1);
    taskLayout->setRowStretch(4, 1);
    taskLayout->setColumnStretch(0, 1);
    taskLayout->setColumnStretch(3, 1);

    checkLayout = new QGridLayout(checkWidget);
    checkLayout->setObjectName("checkLayout");

    // first left is the rational widget
    m_rationalWidget = new RationalWidget(taskWidget, m_number, m_periodStart, m_periodLength);
    m_rationalWidget->setObjectName("m_rationalWidget");
    taskLayout->addWidget(m_rationalWidget, 1, 1, 3, 1);

    /* add input box so the user can enter numerator */
    numer_edit = new KLineEdit(taskWidget);
    numer_edit->setObjectName("numer_edit");
    numer_edit->setValidator(valnum);   // use the int validator
    numer_edit->setToolTip(i18n("Enter the numerator of your result"));
    numer_edit->setFixedSize(85, 42);
    numer_edit->setAlignment(Qt::AlignHCenter);
    numer_edit->setFont(defaultFont);
    QObject::connect(numer_edit, SIGNAL(returnPressed(QString)), this,
                     SLOT(numeratorReturnPressed(QString)));
    taskLayout->addWidget(numer_edit, 1, 2);

    /* add a line between the edit boxes */
    edit_line = new QFrame(taskWidget);
    edit_line->setGeometry(QRect(100, 100, 20, 20));
    edit_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    taskLayout->addWidget(edit_line, 2, 2);

    /* add input box so the user can enter denominator */
    deno_edit = new KLineEdit(taskWidget);
    deno_edit->setObjectName("deno_edit");
    deno_edit->setValidator(valnum);   // use the int validator
    deno_edit->setToolTip(i18n("Enter the denominator of your result"));
    deno_edit->setFixedSize(85, 42);
    deno_edit->setAlignment(Qt::AlignHCenter);
    deno_edit->setFont(defaultFont);
    QObject::connect(deno_edit, SIGNAL(returnPressed(QString)), this,
                     SLOT(denominatorReturnPressed(QString)));
    taskLayout->addWidget(deno_edit, 3, 2);

    // next is the result widget
    m_resultWidget = new ResultWidget(checkWidget, m_result);
    m_resultWidget->setObjectName("m_resultWidget");
    checkLayout->addWidget(m_resultWidget, 0, 0, 1, 2);

    defaultFont.setPointSize(10);

    // the right aligned button
    m_checkButton = new QPushButton(checkWidget);
    m_checkButton->setObjectName("m_checkButton");
    m_checkButton->setText(i18n("&Check"));
    m_checkButton->setDefault(true);  // is the default button of the dialog
    m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));
    m_checkButton->setFont(defaultFont);
    QObject::connect(m_checkButton, SIGNAL(clicked()), this, SLOT(slotCheckButtonClicked()));
    checkLayout->addWidget(m_checkButton, 1, 0);

    // the right aligned button
    m_skipButton = new QPushButton(checkWidget);
    m_skipButton->setObjectName("m_skipButton");
    m_skipButton->setText(i18n("&Skip"));
    m_skipButton->setToolTip(i18n("Click on this button to skip this question."));
    m_skipButton->setFont(defaultFont);
    QObject::connect(m_skipButton, SIGNAL(clicked()), this, SLOT(slotSkipButtonClicked()));
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
    kDebug() << "destructor ExerciseConvert()";
#endif

    /* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExerciseConvert::forceNewTask()
{
#ifdef DEBUG
    kDebug() << "forceNewTask ExerciseConvert()";
#endif

    if (m_currentState == _CHECK_TASK) {
        // emit the signal for skipped
        signalExerciseSkipped();
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
    switch (int ((double(rand()) / RAND_MAX) * 18 + 1)) {
    case  0 :   m_number = KGlobal::locale()->formatNumber(0.5, 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 2);
        break;
    case  1 :   m_number = KGlobal::locale()->formatNumber(0.3, 1);
        m_periodStart = 2;
        m_periodLength = 1;
        m_result = Ratio(1, 3);
        break;
    case  2 :   m_number = KGlobal::locale()->formatNumber(0.6, 1);
        m_periodStart = 2;
        m_periodLength = 1;
        m_result = Ratio(2, 3);
        break;
    case  3 :   m_number = KGlobal::locale()->formatNumber(0.25, 2);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 4);
        break;
    case  4 :   m_number = KGlobal::locale()->formatNumber(0.75, 2);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(3, 4);
        break;
    case  5 :   m_number = KGlobal::locale()->formatNumber(0.2, 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 5);
        break;
    case  6 :   m_number = KGlobal::locale()->formatNumber(0.4, 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(2, 5);
        break;
    case  7 :   m_number = KGlobal::locale()->formatNumber(0.6, 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(3, 5);
        break;
    case  8 :   m_number = KGlobal::locale()->formatNumber(0.8, 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(4, 5);
        break;
    case  9 :   m_number = KGlobal::locale()->formatNumber(0.16, 2);
        m_periodStart = 3;
        m_periodLength = 1;
        m_result = Ratio(1, 6);
        break;
    case 10 :   m_number = KGlobal::locale()->formatNumber(0.142857, 6);
        m_periodStart = 2;
        m_periodLength = 6;
        m_result = Ratio(1, 7);
        break;
    case 11 :   m_number = KGlobal::locale()->formatNumber(0.125, 3);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 8);
        break;
    case 12 :   m_number = KGlobal::locale()->formatNumber(0.375, 3);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(3, 8);
        break;
    case 13 :   m_number = KGlobal::locale()->formatNumber(0.1, 1);
        m_periodStart = 2;
        m_periodLength = 1;
        m_result = Ratio(1, 9);
        break;
    case 14 :   m_number = KGlobal::locale()->formatNumber(0.1, 1);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 10);
        break;
    case 15 :   m_number = KGlobal::locale()->formatNumber(0.05, 2);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 20);
        break;
    case 16 :   m_number = KGlobal::locale()->formatNumber(0.01, 2);
        m_periodStart = 2;
        m_periodLength = 0;
        m_result = Ratio(1, 100);
        break;
    case 17 :   m_number = KGlobal::locale()->formatNumber(0.83, 2);
        m_periodStart = 3;
        m_periodLength = 1;
        m_result = Ratio(5, 6);
        break;
    default :
    case 18 :   m_number = KGlobal::locale()->formatNumber(0.001, 3);
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
    QString tmp_str; /* to build a string for a label */
    QPalette pal;
    Ratio entered_result;

    // change the tooltip of the check button
    m_checkButton->setToolTip(i18n("Click on this button to get to the next question."));

    numer_edit->setEnabled(false);
    deno_edit->setEnabled(false);
    m_skipButton->setEnabled(false);

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
    if ((deno_edit->text().toInt() != 0) && ((entered_result == m_result) ||
            (m_result.numerator() == 0 && entered_result.numerator() == 0))) {
        // emit the signal for correct
        signalExerciseSolvedCorrect();

        /* yes, the user entered the correct result */
        m_resultWidget->setResult(m_result, 1);
    } else {
        // emit the signal for wrong
        signalExerciseSolvedWrong();

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
    deno_edit->setText("");
    numer_edit->setText("");
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

void ExerciseConvert::numeratorReturnPressed(const QString &)
{
    deno_edit->setFocus();
}

void ExerciseConvert::denominatorReturnPressed(const QString &)
{
    slotCheckButtonClicked();
}

/* ------ protected events ------ */
void ExerciseConvert::showEvent(QShowEvent *)
{

    // that the user can start typing without moving the focus
    numer_edit->setFocus();

}