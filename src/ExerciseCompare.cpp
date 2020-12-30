/*
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ExerciseCompare.h"

/* these includes are needed for KDE support */
#include <KLocalizedString>

/* these includes are needed for Qt support */
#include <QApplication>
#include <QLayout>
#include <QPushButton>

#ifdef DEBUG
#include <QDebug>
#endif

/* standard C++ library includes */
#include <cstdlib>

#include "settingsclass.h"

/* ----- public member functions ----- */

/* constructor */
ExerciseCompare::ExerciseCompare(QWidget * parent) :
    ExerciseBase(parent)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor ExerciseCompare()");
#endif

    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */
    createTask();
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    m_currentState = _CHECK_TASK;

    QFont defaultFont = SettingsClass::taskFont();
    defaultFont.setBold(true);
    defaultFont.setPointSize(12);

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
    taskLayout->setColumnStretch(4, 1);

    checkLayout = new QGridLayout(checkWidget);
    checkLayout->setObjectName(QStringLiteral("checkLayout"));

    // first the first ratio widget
    m_firstRatioWidget = new RatioWidget(taskWidget, m_firstRatio);
    m_firstRatioWidget->setObjectName(QStringLiteral("m_firstRatioWidget"));
    taskLayout->addWidget(m_firstRatioWidget, 1, 1, 3, 1);

    // now the second ratio widget
    m_secondRatioWidget = new RatioWidget(taskWidget, m_secondRatio);
    m_secondRatioWidget->setObjectName(QStringLiteral("m_secondRatioWidget"));
    taskLayout->addWidget(m_secondRatioWidget, 1, 3, 3, 1);

    // Create compare buttons
    m_moreButton = new QPushButton(taskWidget);
    m_moreButton->setObjectName(QStringLiteral("m_moreButton"));
    m_moreButton->setText(i18n(">"));
    m_moreButton->setFixedSize(74, 30);
    m_moreButton->setFont(defaultFont);
    QObject::connect(m_moreButton, &QPushButton::clicked, this, &ExerciseCompare::slotMoreButtonClicked);
    m_moreButton->setToolTip(i18n("Click on this button to select the 'greater than' sign."));
    taskLayout->addWidget(m_moreButton, 1, 2);

    m_minorButton = new QPushButton(taskWidget);
    m_minorButton->setObjectName(QStringLiteral("m_minorButton"));
    m_minorButton->setText(i18n("<"));
    m_minorButton->setFixedSize(74, 30);
    m_minorButton->setFont(defaultFont);
    QObject::connect(m_minorButton, &QPushButton::clicked, this, &ExerciseCompare::slotMinorButtonClicked);
    m_minorButton->setToolTip(i18n("Click on this button to select the 'less than' sign."));
    taskLayout->addWidget(m_minorButton, 2, 2);

    m_equalButton = new QPushButton(taskWidget);
    m_equalButton->setObjectName(QStringLiteral("m_equalButton"));
    m_equalButton->setText(i18n("="));
    m_equalButton->setFixedSize(74, 30);
    m_equalButton->setFont(defaultFont);
    QObject::connect(m_equalButton, &QPushButton::clicked, this, &ExerciseCompare::slotEqualButtonClicked);
    m_equalButton->setToolTip(i18n("Click on this button to select the 'equals' sign."));
    taskLayout->addWidget(m_equalButton, 3, 2);

    // Create Skip and Check buttons
    m_resultWidget = new ResultWidget(checkWidget);
    m_resultWidget->setObjectName(QStringLiteral("m_resultWidget"));
    checkLayout->addWidget(m_resultWidget, 0, 0);

    defaultFont.setPointSize(10);

    m_skipButton = new QPushButton(checkWidget);
    m_skipButton->setObjectName(QStringLiteral("m_skipButton"));
    m_skipButton->setText(i18n("&Skip"));
    m_skipButton->setToolTip(i18n("Click on this button to skip this question."));
    m_skipButton->setFont(defaultFont);
    QObject::connect(m_skipButton, &QPushButton::clicked, this, &ExerciseCompare::slotSkipButtonClicked);
    checkLayout->addWidget(m_skipButton, 1, 0);

    m_equalButton->setFocusPolicy(Qt::NoFocus);
    m_moreButton->setFocusPolicy(Qt::NoFocus);
    m_minorButton->setFocusPolicy(Qt::NoFocus);
    m_skipButton->setFocusPolicy(Qt::NoFocus);

    setLayout(baseGrid);
    taskWidget->setLayout(taskLayout);
    checkWidget->setLayout(checkLayout);

    // add tooltip and qwhatsthis help to the widget
    setToolTip(i18n("In this exercise you have to compare two given fractions."));
    setWhatsThis(i18n("In this exercise you have to compare two given fractions and choose the correct comparison sign."));

    // that the user can start typing without moving the focus
    m_equalButton->setFocus();
}

/* destructor */
ExerciseCompare::~ExerciseCompare()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("destructor ExerciseCompare()");
#endif

    /* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExerciseCompare::forceNewTask()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("forceNewTask ExerciseCompare()");
#endif

    if (m_currentState == _CHECK_TASK) {
        // emit the signal for skipped
        Q_EMIT signalExerciseSkipped();
    }
    m_currentState = _CHECK_TASK;

    // generate next task
    (void) nextTask();
}

void ExerciseCompare::setQuestionMixed(bool value)
{
    m_firstRatioWidget->setQuestionMixed(value);
    m_secondRatioWidget->setQuestionMixed(value);
}

/* ------ public slots ------ */

void ExerciseCompare::update()
{
    // call update of components
    m_firstRatioWidget->updateAndRepaint();
    m_secondRatioWidget->updateAndRepaint();

    // update for itself
    ((QWidget *) this)->update();
}


/* ------ private member functions ------ */

void ExerciseCompare::createTask()
{
    // generate the first ratio
    m_firstRatio = Ratio(int ((double(qrand()) / RAND_MAX) * 10 + 1), int ((double(qrand()) / RAND_MAX) * 10 + 1));

    // now the second ratio, but make sure, the second ratio is different from
    // the first one
    do {
        m_secondRatio = Ratio(int ((double(qrand()) / RAND_MAX) * 10 + 1), int ((double(qrand()) / RAND_MAX) * 10 + 1));
    } while (m_firstRatio == m_secondRatio);

    return;
}

/** - checks, if the user solved the task correctly
        - emits signals if task was solved correctly or wrong */
void ExerciseCompare::showResult()
{
    SignButtonState result;

    if (m_firstRatio < m_secondRatio)
        result = lessThen;
    else if (m_firstRatio > m_secondRatio)
        result = greaterThen;
    else
        result = equalTo;


    // disable sign button
    m_minorButton->setEnabled(false);
    m_moreButton->setEnabled(false);
    m_equalButton->setEnabled(false);

    if (m_signButtonState == result) {
        // emit the signal for correct
        Q_EMIT signalExerciseSolvedCorrect();

        /* yes, the user entered the correct result */
        m_resultWidget->setResult(1);
    } else {
        // emit the signal for wrong
        Q_EMIT signalExerciseSolvedWrong();

        /* no, the user entered the wrong result */

        m_resultWidget->setResult(2);
    } /* if (entered_result == result) */

    return;
}

/** generate the next task and show it to the user */
void ExerciseCompare::nextTask()
{
    // change the tooltip of the check button
    m_equalButton->setToolTip(i18n("Click on this button to check your result."));
    m_moreButton->setToolTip(i18n("Click on this button to check your result."));
    m_minorButton->setToolTip(i18n("Click on this button to check your result."));
    // enable sign button
    m_equalButton->setEnabled(true);
    m_minorButton->setEnabled(true);
    m_moreButton->setEnabled(true);

    // reset the signButton
    m_signButtonState = lessThen;

    /* create a new task */
    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */
    createTask();
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    // set the ratio widgets with the new ratios
    m_firstRatioWidget->setRatio(m_firstRatio);
    m_secondRatioWidget->setRatio(m_secondRatio);

    return;
}

/* ------ private slots ------ */

void ExerciseCompare::slotSkipButtonClicked()
{
#ifdef DEBUG
    qDebug() << "ExerciseCompare::slotSkipButtonClicked()";
#endif
    if (m_currentState == _CHECK_TASK) {
        forceNewTask();
    } else {
        m_currentState = _CHECK_TASK;
        m_skipButton->setText(i18n("&Skip"));
        m_resultWidget->setResult(m_firstRatio, -1);
        nextTask();
    }

    return;
}

void ExerciseCompare::slotMinorButtonClicked()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("ExerciseCompare::slotMinorButtonClicked()");
#endif

    m_currentState = _NEXT_TASK;
    m_skipButton->setText(i18n("&Next"));
    m_signButtonState = lessThen;
    showResult();

    return;
}

void ExerciseCompare::slotMoreButtonClicked()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("ExerciseCompare::slotMoreButtonClicked()");
#endif

    m_currentState = _NEXT_TASK;
    m_skipButton->setText(i18n("&Next"));
    m_signButtonState = greaterThen;
    showResult();

    return;
}

void ExerciseCompare::slotEqualButtonClicked()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("ExerciseCompare::slotEqualButtonClicked()");
#endif

    m_currentState = _NEXT_TASK;
    m_skipButton->setText(i18n("&Next"));
    m_signButtonState = equalTo;
    showResult();

    return;
}
