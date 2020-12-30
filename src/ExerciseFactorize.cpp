/*
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ExerciseFactorize.h"

/* these includes are needed for KDE support */
#include <KLocalizedString>
#include <KMessageBox>

/* these includes are needed for Qt support */
#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QPainter>

#ifdef DEBUG
#include <QDebug>
#endif

/* standard C++ library includes */
#include <cstdlib>

#include "PrimeNumber.h"
#include "RationalWidget.h"
#include "settingsclass.h"

/* ----- public member functions ----- */

/* constructor */
ExerciseFactorize::ExerciseFactorize(QWidget * parent) :
    ExerciseBase(parent)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor ExerciseFactorize()");
#endif

    /* create a new task */
    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */
    createTask();
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    // the next thing to do on a button click would be to check the entered
    // result
    m_currentState = _CHECK_TASK;

    QFont defaultFont = SettingsClass::taskFont();

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
    taskLayout->setRowStretch(6, 1);
    taskLayout->setColumnStretch(0, 1);
    taskLayout->setColumnStretch(7, 1);

    checkLayout = new QGridLayout(checkWidget);
    checkLayout->setObjectName(QStringLiteral("checkLayout"));

    m_taskLabel = new QLabel(this);
    m_taskLabel->setObjectName(QStringLiteral("taskLabel"));
    m_taskLabel->setFont(defaultFont);
    taskLayout->addWidget(m_taskLabel, 1, 1);

    defaultFont.setBold(true);

    m_equalSignLabel = new QLabel(this);
    m_equalSignLabel->setObjectName(QStringLiteral("equalSignLabel"));
    m_equalSignLabel->setText(QStringLiteral("="));
    m_equalSignLabel->setFont(defaultFont);
    taskLayout->addWidget(m_equalSignLabel, 1, 2);

    m_factorsEnteredEdit = new PrimeFactorsLineEdit(this);
    m_factorsEnteredEdit->setObjectName(QStringLiteral("factorsEnteredEdit"));
    QPalette pal;
    pal.setColor(m_factorsEnteredEdit->foregroundRole(), QColor(0, 0, 0));
    //m_factorsEnteredEdit->setPalette (pal);
    m_factorsEnteredEdit->setFont(defaultFont);
    m_factorsEnteredEdit->setFixedSize(320, 35);
    taskLayout->addWidget(m_factorsEnteredEdit, 1, 3, 1, 5);

    connect(m_factorsEnteredEdit, &PrimeFactorsLineEdit::contentIsRight, this, &ExerciseFactorize::editContentChanged);
    connect(m_factorsEnteredEdit, &PrimeFactorsLineEdit::returnPressed, this, &ExerciseFactorize::slotFactorsEditReturnPressed);

    defaultFont.setPointSize(10);

    m_factor2Button = new QPushButton(this);
    m_factor2Button->setObjectName(QStringLiteral("factor2Button"));
    m_factor2Button->setFixedSize(50, 35);
    m_factor2Button->setFont(defaultFont);
    taskLayout->addWidget(m_factor2Button, 4, 3);

    m_factor3Button = new QPushButton(this);
    m_factor3Button->setObjectName(QStringLiteral("factor3Button"));
    m_factor3Button->setFixedSize(50, 35);
    m_factor3Button->setFont(defaultFont);
    taskLayout->addWidget(m_factor3Button, 4, 4);

    m_factor5Button = new QPushButton(this);
    m_factor5Button->setObjectName(QStringLiteral("factor5Button"));
    m_factor5Button->setFixedSize(50, 35);
    m_factor5Button->setFont(defaultFont);
    taskLayout->addWidget(m_factor5Button, 4, 5);

    m_factor7Button = new QPushButton(this);
    m_factor7Button->setObjectName(QStringLiteral("factor7Button"));
    m_factor7Button->setFixedSize(50, 35);
    m_factor7Button->setFont(defaultFont);
    taskLayout->addWidget(m_factor7Button, 4, 6);

    m_factor11Button = new QPushButton(this);
    m_factor11Button->setObjectName(QStringLiteral("factor11Button"));
    m_factor11Button->setFixedSize(50, 35);
    m_factor11Button->setFont(defaultFont);
    taskLayout->addWidget(m_factor11Button, 5, 3);

    m_factor13Button = new QPushButton(this);
    m_factor13Button->setObjectName(QStringLiteral("factor13Button"));
    m_factor13Button->setFixedSize(50, 35);
    m_factor13Button->setFont(defaultFont);
    taskLayout->addWidget(m_factor13Button, 5, 4);

    m_factor17Button = new QPushButton(this);
    m_factor17Button->setObjectName(QStringLiteral("factor17Button"));
    m_factor17Button->setFixedSize(50, 35);
    m_factor17Button->setFont(defaultFont);
    taskLayout->addWidget(m_factor17Button, 5, 5);

    m_factor19Button = new QPushButton(this);
    m_factor19Button->setObjectName(QStringLiteral("factor19Button"));
    m_factor19Button->setFixedSize(50, 35);
    m_factor19Button->setFont(defaultFont);
    taskLayout->addWidget(m_factor19Button, 5, 6);

    m_removeLastFactorButton = new QPushButton(this);
    m_removeLastFactorButton->setObjectName(QStringLiteral("removeLastFactor"));
    m_removeLastFactorButton->setFixedSize(115, 35);
    m_removeLastFactorButton->setFont(defaultFont);
    m_removeLastFactorButton->setText(i18n("Backspace"));
    taskLayout->addWidget(m_removeLastFactorButton, 3, 4, 1, 3, Qt::AlignRight);

    defaultFont.setPointSize(10);

    m_skipButton = new QPushButton(this);
    m_skipButton->setObjectName(QStringLiteral("m_skipButton"));
    m_skipButton->setText(i18n("&Skip"));
    m_skipButton->setToolTip(i18n("Click on this button to skip this question."));
    m_skipButton->setFont(defaultFont);
    QObject::connect(m_skipButton, &QPushButton::clicked, this, &ExerciseFactorize::slotSkipButtonClicked);
    checkLayout->addWidget(m_skipButton, 1, 1);

    // the check task button
    m_checkButton = new QPushButton(this);
    m_checkButton->setObjectName(QStringLiteral("m_checkButton"));
    m_checkButton->setText(i18n("&Check"));
    m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));
    QObject::connect(m_checkButton, &QPushButton::clicked, this, &ExerciseFactorize::slotCheckButtonClicked);
    m_checkButton->setFont(defaultFont);
    m_checkButton->setDefault(true);  // is the default button of the dialog
    checkLayout->addWidget(m_checkButton, 1, 0);

    // next is the result widget
    m_resultWidget = new ResultWidget(this,  m_factorsResult);
    m_resultWidget->setObjectName(QStringLiteral("m_resultWidget"));
    checkLayout->addWidget(m_resultWidget, 0, 0, 1, 2);

    // the current task
    QString tmp_str;
    tmp_str.setNum(m_taskNumber);
    m_taskLabel->setText(tmp_str);

    // now set the color for the task label
    pal = QPalette();
    pal.setColor(m_taskLabel->foregroundRole(), SettingsClass::numberColor());
    m_taskLabel->setPalette(pal);

    // now set the color for the equal sign
    pal = QPalette();
    pal.setColor(m_equalSignLabel->foregroundRole(), SettingsClass::operationColor());
    m_equalSignLabel->setPalette(pal);

    // the prime factor buttons
    m_factor2Button->setText(i18n("2"));
    m_factor3Button->setText(i18n("3"));
    m_factor5Button->setText(i18n("5"));
    m_factor7Button->setText(i18n("7"));
    m_factor11Button->setText(i18n("11"));
    m_factor13Button->setText(i18n("13"));
    m_factor17Button->setText(i18n("17"));
    m_factor19Button->setText(i18n("19"));
    QObject::connect(m_factor2Button, &QPushButton::clicked, this, &ExerciseFactorize::slotFactor2ButtonClicked);
    QObject::connect(m_factor3Button, &QPushButton::clicked, this, &ExerciseFactorize::slotFactor3ButtonClicked);
    QObject::connect(m_factor5Button, &QPushButton::clicked, this, &ExerciseFactorize::slotFactor5ButtonClicked);
    QObject::connect(m_factor7Button, &QPushButton::clicked, this, &ExerciseFactorize::slotFactor7ButtonClicked);
    QObject::connect(m_factor11Button, &QPushButton::clicked, this, &ExerciseFactorize::slotFactor11ButtonClicked);
    QObject::connect(m_factor13Button, &QPushButton::clicked, this, &ExerciseFactorize::slotFactor13ButtonClicked);
    QObject::connect(m_factor17Button, &QPushButton::clicked, this, &ExerciseFactorize::slotFactor17ButtonClicked);
    QObject::connect(m_factor19Button, &QPushButton::clicked, this, &ExerciseFactorize::slotFactor19ButtonClicked);

    // add tooltips to the factor buttons
    m_factor2Button->setToolTip(i18n("Add prime factor 2."));
    m_factor3Button->setToolTip(i18n("Add prime factor 3."));
    m_factor5Button->setToolTip(i18n("Add prime factor 5."));
    m_factor7Button->setToolTip(i18n("Add prime factor 7."));
    m_factor11Button->setToolTip(i18n("Add prime factor 11."));
    m_factor13Button->setToolTip(i18n("Add prime factor 13."));
    m_factor17Button->setToolTip(i18n("Add prime factor 17."));
    m_factor19Button->setToolTip(i18n("Add prime factor 19."));

    // the remove last factor button
    m_removeLastFactorButton->setEnabled(false);
    QObject::connect(m_removeLastFactorButton, &QPushButton::clicked, this, &ExerciseFactorize::slotRemoveLastFactorButtonClicked);
    m_removeLastFactorButton->setToolTip(i18n("Removes the last entered prime factor."));

    m_factor2Button->setFocusPolicy(Qt::NoFocus);
    m_factor3Button->setFocusPolicy(Qt::NoFocus);
    m_factor5Button->setFocusPolicy(Qt::NoFocus);
    m_factor7Button->setFocusPolicy(Qt::NoFocus);
    m_factor11Button->setFocusPolicy(Qt::NoFocus);
    m_factor13Button->setFocusPolicy(Qt::NoFocus);
    m_factor17Button->setFocusPolicy(Qt::NoFocus);
    m_factor19Button->setFocusPolicy(Qt::NoFocus);
    m_removeLastFactorButton->setFocusPolicy(Qt::NoFocus);

    setLayout(baseGrid);
    taskWidget->setLayout(taskLayout);
    checkWidget->setLayout(checkLayout);

    // add tooltip and qwhatsthis help to the widget
    setToolTip(i18n("In this exercise you have to factorize a given number."));
    setWhatsThis(i18n("In this exercise you have to factorize a given number. You have to enter all prime factors of the number. You can add a prime factor by clicking on the corresponding button. The chosen prime factors will be shown in the input field. Do not forget to enter all prime factors, even when a prime factor repeats several times. Click the 'Backspace' button to remove the last entered prime factor."));

    m_edit = true;
}

/* destructor */
ExerciseFactorize::~ExerciseFactorize()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("destructor ExerciseFactorize()");
#endif

    /* no need to delete any child widgets, Qt does it by itself */
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void ExerciseFactorize::forceNewTask()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("forceNewTask ExerciseFactorize()");
#endif

    if (m_currentState == _CHECK_TASK) {
        // emit the signal for skipped
        Q_EMIT signalExerciseSkipped();
    }
    m_currentState = _CHECK_TASK;
    m_checkButton->setText(i18n("&Check"));

    // Maybe the skip button was clicked with while
    // wrong content was int the QLineEdit so...
    // Set the QLineEdit's background to white
    QPalette palette;
    palette.setColor(QPalette::Base, Qt::white);
    m_factorsEnteredEdit->setPalette(palette);
    // and enable all buttons that could be disabled.
    setButtonsEnabled(true);

    // generate next task
    (void) nextTask();
}


/* ------ public slots ------ */

void ExerciseFactorize::update()
{
    // now set the color for the task label
    QPalette pal;
    pal.setColor(m_taskLabel->foregroundRole(), SettingsClass::numberColor());
    m_taskLabel->setPalette(pal);

    // now set the color for the equal sign
    pal = QPalette();
    pal.setColor(m_equalSignLabel->foregroundRole(), SettingsClass::operationColor());
    m_equalSignLabel->setPalette(pal);

    // and the factors
    m_resultWidget->update();

    // update for itself
    ((QWidget *) this)->update();
}


/* ------ private member functions ------ */

void ExerciseFactorize::createTask()
{
    uint uint_number;
    PrimeNumber tmp_primenumber;

    // just pick one number out of the possible numbers to factorize
    m_taskNumber = possibleTasks[uint((double(qrand()) / RAND_MAX) * numberPossibleTasks)];

    // now get the primefactors of the taskNumber
    m_factorsResult.clear();
    uint_number = m_taskNumber;
    tmp_primenumber.move_first();
    do {
        // check if the current primenumber is a divisor
        if (uint_number % tmp_primenumber.get_current() != 0) {
            // no, it is not a divisor, go on with next primenumber
            tmp_primenumber.move_forward();
        } else {
            // current primenumber is a divisor
            m_factorsResult.append(tmp_primenumber.get_current());
            uint_number = uint(uint_number / tmp_primenumber.get_current());
        }
    } while (uint_number != 1);

    return;
}

/** - checks, if the user solved the task correctly
        - emits signals if task was solved correctly or wrong */
void ExerciseFactorize::showResult()
{
    uint uint_result = 0;

    // change the tooltip of the check button
    m_checkButton->setToolTip(i18n("Click on this button to get to the next question."));

    // disable prime factor buttons
    m_factor2Button->setEnabled(false);
    m_factor3Button->setEnabled(false);
    m_factor5Button->setEnabled(false);
    m_factor7Button->setEnabled(false);
    m_factor11Button->setEnabled(false);
    m_factor13Button->setEnabled(false);
    m_factor17Button->setEnabled(false);
    m_factor19Button->setEnabled(false);
    m_skipButton->setEnabled(false);

    // disable factor removal button as well
    m_removeLastFactorButton->setEnabled(false);

    // show the result
    m_resultWidget->setFactors(m_factorsResult);

    // now calculate the product of the prime factors entered by the user
    for (int tmp_uint = 0; tmp_uint < m_factorsEntered.count(); tmp_uint++) {
        if (tmp_uint == 0) {
            uint_result = m_factorsEntered[0];
        } else {
            uint_result *= m_factorsEntered[tmp_uint];
        }
    }

    if (uint_result == m_taskNumber) {
        // emit the signal for correct
        Q_EMIT signalExerciseSolvedCorrect();

        /* yes, the user entered the correct result */
        m_resultWidget->setResult(Ratio(), 1);
    } else {
        // emit the signal for wrong
        Q_EMIT signalExerciseSolvedWrong();

        /* no, the user entered the wrong result */
        m_resultWidget->setResult(Ratio(), 0);
    } /* if (entered_result == result) */

    m_edit = false;

    return;
}

/** generate the next task and show it to the user */
void ExerciseFactorize::nextTask()
{
    // change the tooltip of the check button
    m_checkButton->setToolTip(i18n("Click on this button to check your result. The button will not work if you have not entered a result yet."));

    // enable prime factor buttons
    m_factor2Button->setEnabled(true);
    m_factor3Button->setEnabled(true);
    m_factor5Button->setEnabled(true);
    m_factor7Button->setEnabled(true);
    m_factor11Button->setEnabled(true);
    m_factor13Button->setEnabled(true);
    m_factor17Button->setEnabled(true);
    m_factor19Button->setEnabled(true);
    m_skipButton->setEnabled(true);

    // disable the factor removal button, there are no factors to be removed yet
    m_removeLastFactorButton->setEnabled(false);

    m_resultWidget->setResult(Ratio(), -1);

    /* clear user input */
    m_factorsEntered.clear();
    m_factorsResult.clear();
    m_factorsEnteredEdit->clear();

    m_factorsEnteredEdit->setFocus();

    /* create a new task */
    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */
    createTask();
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    // update the task widget
    QString tmp_str;
    tmp_str.setNum(m_taskNumber);
    m_taskLabel->setText(tmp_str);

    m_edit = true;

    return;
}

void ExerciseFactorize::addFactor(uint factor)
{
    // add the new entered factor
    m_factorsEntered.append(factor);

    // a factor was added, so the user can can remove one again
    m_removeLastFactorButton->setEnabled(true);

    // update the line edit
    updateEnteredEdit();

    return;
}

void ExerciseFactorize::updateEnteredEdit()
{
    // the string to be shown in the entered edit
    QString str_output;
    QString str_tmp;

    // find the end of the list
    uintList::iterator it;


    for (it = m_factorsEntered.begin(); it != m_factorsEntered.end(); ++it) {
        // convert the factor into a string
        str_tmp.setNum(*it);

        // add the factor with a *
        if (it == m_factorsEntered.begin()) {
            str_output = str_tmp;
        } else {
            str_output += QStringLiteral(" x ") + str_tmp;
        }
    }

    // set the text into the line edit
    m_factorsEnteredEdit->setText(str_output);

    return;
}

/* ------ private slots ------ */

void ExerciseFactorize::slotFactorsEditReturnPressed()
{
    if (m_checkButton->isEnabled()) {
        slotCheckButtonClicked();
    }
}

void ExerciseFactorize::slotCheckButtonClicked()
{
    if (m_currentState == _CHECK_TASK) {
        // if nothing has been entered by the user, we don't check the result yet
        if (m_factorsEntered.count() == 0)
            return;
        m_currentState = _NEXT_TASK;
        m_checkButton->setText(i18n("N&ext"));
        (void) showResult();
    } else {
        m_currentState = _CHECK_TASK;
        m_checkButton->setText(i18n("&Check"));
        (void) nextTask();
    }

    // update the line edit
    updateEnteredEdit();
    return;
}

void ExerciseFactorize::slotSkipButtonClicked()
{
    forceNewTask();
    // update the line edit
    updateEnteredEdit();

    return;
}

void ExerciseFactorize::slotFactor2ButtonClicked()
{
    addFactor(2);

    return;
}

void ExerciseFactorize::slotFactor3ButtonClicked()
{
    addFactor(3);

    return;
}

void ExerciseFactorize::slotFactor5ButtonClicked()
{
    addFactor(5);

    return;
}

void ExerciseFactorize::slotFactor7ButtonClicked()
{
    addFactor(7);

    return;
}

void ExerciseFactorize::slotFactor11ButtonClicked()
{
    addFactor(11);

    return;
}

void ExerciseFactorize::slotFactor13ButtonClicked()
{
    addFactor(13);

    return;
}

void ExerciseFactorize::slotFactor17ButtonClicked()
{
    addFactor(17);

    return;
}

void ExerciseFactorize::slotFactor19ButtonClicked()
{
    addFactor(19);

    return;
}

void ExerciseFactorize::slotRemoveLastFactorButtonClicked()
{
    if (m_factorsEntered.count() > 0) {
        // find the end of the list
        uintList::iterator it = m_factorsEntered.end();
        --it;

        // remove last item
        m_factorsEntered.erase(it);
    }

    // if the list is empty, we have to disable the delete button
    if (m_factorsEntered.count() == 0)
        m_removeLastFactorButton->setEnabled(false);

    // update the line edit
    updateEnteredEdit();

    return;
}

void ExerciseFactorize::showEvent(QShowEvent *)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("ExerciseFactorize::showEvent()");
#endif

    if (isVisible())
        m_factorsEnteredEdit->setFocus();
}

void ExerciseFactorize::editContentChanged(bool correct)
{
    if (correct) {
        const QStringList factors = m_factorsEnteredEdit->getFactors();

        m_factorsEntered.clear();
        for (const QString & auxStr : factors) {
            m_factorsEntered.append(auxStr.toUInt());
        }
    }
    setButtonsEnabled(correct);
}

void ExerciseFactorize::setButtonsEnabled(bool enabled)
{
    // set the state of the buttons involved in factorization
    m_checkButton->setEnabled(enabled);
    m_factor2Button->setEnabled(enabled);
    m_factor3Button->setEnabled(enabled);
    m_factor5Button->setEnabled(enabled);
    m_factor7Button->setEnabled(enabled);
    m_factor11Button->setEnabled(enabled);
    m_factor13Button->setEnabled(enabled);
    m_factor17Button->setEnabled(enabled);
    m_factor19Button->setEnabled(enabled);
    m_removeLastFactorButton->setEnabled(enabled);
}


