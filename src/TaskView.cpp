/*
    StatisticsView.cpp  -  the statistic window
    SPDX-FileCopyrightText: 2001-2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "TaskView.h"

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

#include "settingsclass.h"

/* standard C++ library includes */
#include <cmath>

/* ----- public member functions ----- */

/* constructor */
TaskView::TaskView(QWidget * parent,
                   bool padd_add, bool padd_div, bool padd_mult, bool padd_sub,
                   unsigned int pnr_ratios, unsigned int pmax_md) :
    ExerciseBase(parent), m_addAdd(padd_add), m_addDiv(padd_div),
    m_addMult(padd_mult), m_addSub(padd_sub),
    nr_ratios(pnr_ratios), max_md(pmax_md)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor TaskView()");
#endif
    curr_nr_ratios = nr_ratios;

    /* create a new task */
    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */
    current_task.create_task(max_md, nr_ratios, m_addAdd, m_addDiv, m_addMult, m_addSub);
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    // to validate, that the input is an int
    QIntValidator *valnum = new QIntValidator(this);

    // the next thing to do on a button click would be to check the entered
    // result
    m_currentState = _CHECK_TASK;

    // Create the base of Widget
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
    taskLayout->setColumnStretch(5, 1);

    checkLayout = new QGridLayout(checkWidget);
    checkLayout->setObjectName(QStringLiteral("checkLayout"));

    QFont defaultFont = SettingsClass::taskFont();
    defaultFont.setBold(true);
    defaultFont.setPointSize(18);

    // first left is the task widget
    m_taskWidget = new TaskWidget(taskWidget, current_task);
    m_taskWidget->setObjectName(QStringLiteral("m_taskWidget"));
    taskLayout->addWidget(m_taskWidget, 1, 1, 3, 1);

    /* add input box so the user can enter the integer par of the fraction */
    integer_edit = new QLineEdit(taskWidget);
    integer_edit->setObjectName(QStringLiteral("integer_edit"));
    integer_edit->setValidator(valnum);   // use the int validator
    integer_edit->setToolTip(i18n("Enter the integer part of the fraction"));
    integer_edit->setFont(defaultFont);
    integer_edit->setFixedSize(85, 42);
    integer_edit->setAlignment(Qt::AlignHCenter);
    QObject::connect(integer_edit, &QLineEdit::returnPressed, this, &TaskView::integerReturnPressed);
    taskLayout->addWidget(integer_edit, 1, 3, 3, 1, Qt::AlignVCenter | Qt::AlignRight);


    /* add input box so the user can enter numerator */
    numer_edit = new QLineEdit(taskWidget);
    numer_edit->setObjectName(QStringLiteral("numer_edit"));
    numer_edit->setValidator(valnum);   // use the int validator
    numer_edit->setToolTip(i18n("Enter the numerator of your result"));
    numer_edit->setFont(defaultFont);
    numer_edit->setFixedSize(85, 42);
    numer_edit->setAlignment(Qt::AlignHCenter);
    QObject::connect(numer_edit, &QLineEdit::returnPressed, this, &TaskView::numeratorReturnPressed);
    taskLayout->addWidget(numer_edit, 1, 4);

    /* add a line between the edit boxes */
    edit_line = new QFrame(taskWidget);
    edit_line->setGeometry(QRect(100, 100, 20, 20));
    edit_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    taskLayout->addWidget(edit_line, 2, 4);

    /* add input box so the user can enter denominator */
    deno_edit = new QLineEdit(taskWidget);
    deno_edit->setObjectName(QStringLiteral("deno_edit"));
    deno_edit->setValidator(valnum);   // use the int validator
    deno_edit->setToolTip(i18n("Enter the denominator of your result"));
    deno_edit->setFont(defaultFont);
    deno_edit->setFixedSize(85, 42);
    deno_edit->setAlignment(Qt::AlignHCenter);
    QObject::connect(deno_edit, &QLineEdit::returnPressed, this, &TaskView::denominatorReturnPressed);
    taskLayout->addWidget(deno_edit, 3, 4);

    // next is the result widget
    m_resultWidget = new ResultWidget(checkWidget, Ratio());
    m_resultWidget->setObjectName(QStringLiteral("m_resultWidget"));
    checkLayout->addWidget(m_resultWidget, 0, 0, 1, 2);

    defaultFont.setPointSize(10);

    // the right aligned button
    m_checkButton = new QPushButton(checkWidget);
    m_checkButton->setObjectName(QStringLiteral("m_checkButton"));
    m_checkButton->setText(i18n("&Check"));
    m_checkButton->setDefault(true);  // is the default button of the dialog
    m_checkButton->setToolTip(i18n("Click this button to check your result. The button will not work if you have not entered a result yet."));
    m_checkButton->setFont(defaultFont);
    QObject::connect(m_checkButton, &QPushButton::clicked, this, &TaskView::slotCheckButtonClicked);
    checkLayout->addWidget(m_checkButton, 1, 0);

    // the right aligned button
    m_skipButton = new QPushButton(checkWidget);
    m_skipButton->setObjectName(QStringLiteral("m_skipButton"));
    m_skipButton->setText(i18n("&Skip"));
    m_skipButton->setToolTip(i18n("Click this button to skip this question."));
    m_skipButton->setFont(defaultFont);
    QObject::connect(m_skipButton, &QPushButton::clicked, this, &TaskView::slotSkipButtonClicked);
    checkLayout->addWidget(m_skipButton, 1, 1);

    // show the whole layout
    m_taskWidget->show();

    // add tooltip and qwhatsthis help to the widget
    setToolTip(i18n("In this exercise you have to solve a given question with fractions."));
    setWhatsThis(i18n("In this exercise you have to solve the generated question. You have to enter the integer part of the fraction and the numerator and the denominator. You can adjust the difficulty of the question in the options window part. Do not forget to reduce the result, if the use of the reduced form is forced."));
}

/* destructor */
TaskView::~TaskView()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("destructor TaskView()");
#endif

    /* no need to delete any child widgets, Qt does it by itself */
}

void TaskView::setReducedForm(bool value)
{
    m_reducedForm = value;
}

void TaskView::setQuestionMixed(bool value)
{
#ifdef DEBUG
    qDebug() << "TaskView::setQuestionMixed()";
#endif
    m_taskWidget->setQuestionMixed(value);
}

void TaskView::setAnswerMixed(bool value)
{
#ifdef DEBUG
    qDebug() << "TaskView::setAnswerMixed()";
#endif
    m_answerMixed = value;
    integer_edit->setVisible(value);
    m_resultWidget->setAnswerMixed(value);
}

/** the parameters of task generation can be set with this function */
void TaskView::setTaskParameters(bool padd_add, bool padd_div,
                                 bool padd_mult, bool padd_sub,
                                 unsigned int pnr_ratios, unsigned int pmax_md)
{
    // at least one operation must be enabled
    //if ((padd_add == false) && (padd_div == false) && (padd_mult == false) && (padd_sub == false))
    //  padd_add = true;

    // we need at least 2 ratios
    if (pnr_ratios < 2)
        pnr_ratios = 2;

    // we can only visualize 5 ratios, so we have to limit it
    if (pnr_ratios > 5)
        pnr_ratios = 5;

    // the main denominator must be at least 2^pnr_ratios
    if (pow(2.0, (double) pnr_ratios) > pmax_md)
        pmax_md = (unsigned int) pow(2.0, (double) pnr_ratios);

    // so everything seems to be fine, lets set the internal values to the given
    // ones
    m_addSub = padd_sub;
    m_addAdd = padd_add;
    m_addMult = padd_mult;
    m_addDiv = padd_div;
    max_md = pmax_md;

    nr_ratios = pnr_ratios;

    return;
}

/** resets the current state, creates a new task and count the last task as
 * wrong, if it wasn't solved (in _NEXT_TASK state) yet
 * mainly used after changing the task parameters */
void TaskView::forceNewTask()
{
#ifdef DEBUG
    qDebug() << "forceNewTask TaskView()";
#endif

    if (m_currentState == _CHECK_TASK) {
        // emit the signal for skipped
        Q_EMIT signalTaskSkipped();
    }
    m_currentState = _CHECK_TASK;
    m_checkButton->setText(i18n("&Check"));

    // generate next task
    (void) nextTask();
}


/* ------ private member functions ------ */

/** - checks the entered result and compares it to the task's result
        - shows the correct result and informs the user if he was right or wrong
        - if the user entered the result unreduced, he will be informed about it
        - if the user entered a 0 for the denominator, he will be informed about
          it (division by zero)
        - emits signals if task was solved right or wrong */
void TaskView::showResult()
{
    bool wrong = false;

    // change the tooltip of the check button
    m_checkButton->setToolTip(i18n("Click this button to get the next question."));

    numer_edit->setEnabled(false);
    deno_edit->setEnabled(false);
    integer_edit->setEnabled(false);
    m_skipButton->setEnabled(false);

    // an empty numerator field will be interpreted as 0
    if (numer_edit->text().isEmpty() == true)
        numer_edit->setText(QStringLiteral("0"));
    int int_numerator = numer_edit->text().toInt();

    // an empty denominator field will be interpreted as 1
    if (deno_edit->text().isEmpty() == true)
        deno_edit->setText(QStringLiteral("1"));
    int int_denominator = deno_edit->text().toInt();

    // get the par (integer) value in case mixed input is enabled
    int int_mixed = 0;
    if (m_answerMixed) {
        // an empty par (integer) field  will be interpreted as 0
        if (integer_edit->text().isEmpty() == true)
            integer_edit->setText(QStringLiteral("0"));
        int_mixed = integer_edit->text().toInt();
    }

    // get the solution for the current task
    solution = current_task.solve();

    // calculate the combined numerator and preserve prefix sign
    int tmp_num = qAbs(int_mixed * int_denominator) + qAbs(int_numerator);
    if (int_mixed < 0)
        tmp_num *= -1;
    if (int_numerator < 0)
        tmp_num *= -1;

    /* store the entered result to check it, but without reducing it */
    entered_result.setNumerator(tmp_num, false);
    entered_result.setDenominator(int_denominator, false);

    /* check whether we expect the result in reduced form
     * if not, we reduce it for the user
     *
     * we also have to update some temp variables to be reduced
     *
     * we don't recalculate the mixed part, because this should not change
     * by reducing it */
    if (!m_reducedForm) {
        entered_result.reduce();
        int_numerator = entered_result.numerator() % entered_result.denominator();
        int_denominator = entered_result.denominator();

    }

    /* compare entered result and solution ratio
     * if they are equal it still might be that the user didn't entered a mixed result */
    if (!(entered_result == solution))
        wrong = true;

    /* we do not allow entering a denominator of 0 (division by zero!) */
    if (deno_edit->text().toInt() == 0)
        wrong = true;

    /* now we have to check if the solution was entered as a mixed number if required
     *
     * we only have to do that in case the answer is not already marked as wrong
     *
     * fortunately, we don't have to care about the prefix sign, because that is already
     * handled above */
    if (m_answerMixed == true && wrong == false) {
        int int_solution_mixed = qAbs(int (solution.numerator() / solution.denominator()));
        int int_solution_numerator = qAbs(solution.numerator() % solution.denominator());

        if (!(int_solution_mixed == qAbs(int_mixed) && int_solution_numerator == qAbs(int_numerator)))
            wrong = true;
    }

    // in case the user entered the wrong result, try to give some hints
    if (wrong == true) {
        // emit the signal for wrong
        Q_EMIT signalTaskSolvedWrong();
        m_resultWidget->setResult(solution, 0);

        // if the user entered a 0 for the denominator (division by 0) we have to
        // get the 0 directly from the input field, because
        // Ratio::setDenominator(0, true) will set the denominator to 1 to ensure
        // the Ratio is valid
        if (deno_edit->text().toInt() == 0) {
            KMessageBox::information(this,
                                     i18n("You entered 0 as the denominator. This means division by zero, which is not allowed. This question will be counted as not correctly solved."));
        } else {
            /* maybe the entered ratio was not reduced */
            Ratio tmp_result = Ratio(entered_result);
            tmp_result.reduce();
            if ((tmp_result == solution) && !(tmp_result == entered_result)) {
                KMessageBox::information(this,
                                         i18n("You entered the correct result, but not reduced. This question will be counted as not correctly solved."));
            } else if (tmp_result == solution && tmp_result == entered_result) {
                KMessageBox::information(this,
                                         i18n("You entered the correct result, but not in the mixed number notation. This question will be counted as not correctly solved."));
            }
        }
    } else {
        // emit the signal for correct
        Q_EMIT signalTaskSolvedCorrect();
        m_resultWidget->setResult(solution, 1);

    }

    m_resultWidget->show();
}

/** generate the next task and show it to the user */
void TaskView::nextTask()
{
    // change the tooltip of the check button
    m_checkButton->setToolTip(i18n("Click this button to check your result. The button will not work if you have not entered a result yet."));

    numer_edit->setEnabled(true);
    deno_edit->setEnabled(true);
    integer_edit->setEnabled(true);
    if (m_answerMixed == true)
        integer_edit->setEnabled(true);
    m_skipButton->setEnabled(true);

    m_resultWidget->setResult(solution, -1);

    /* clear user input */
    deno_edit->clear();
    numer_edit->clear();
    integer_edit->clear();
    if (m_answerMixed == true)
        integer_edit->setFocus();
    else
        numer_edit->setFocus();

    /* create a new task */
    QApplication::setOverrideCursor(Qt::WaitCursor);  /* show the sand clock */

    current_task.create_task(max_md, nr_ratios, m_addAdd, m_addDiv, m_addMult, m_addSub);
    QApplication::restoreOverrideCursor(); /* show the normal cursor */

    // update the task widget
    m_taskWidget->setTask((const Task)(current_task));
}

/* ------ private slots ------ */

void TaskView::integerReturnPressed()
{
    numer_edit->setFocus();
}

void TaskView::numeratorReturnPressed()
{
    deno_edit->setFocus();
}

void TaskView::denominatorReturnPressed()
{
    slotCheckButtonClicked();
}

void TaskView::slotCheckButtonClicked()
{
    if (m_currentState == _CHECK_TASK) {
        // if nothing has been entered by the user, we don't check the result yet
        if (numer_edit->text().isEmpty() == true && deno_edit->text().isEmpty() == true && integer_edit->text().isEmpty() == true)
            return;
        m_currentState = _NEXT_TASK;
        m_checkButton->setText(i18n("N&ext"));
        (void) showResult();
    } else {
        m_currentState = _CHECK_TASK;
        m_checkButton->setText(i18n("&Check"));
        (void) nextTask();
    }
}

void TaskView::slotSkipButtonClicked()
{
    forceNewTask();
}

/* ------ protected events ------ */
void TaskView::showEvent(QShowEvent *)
{

    // that the user can start typing without moving the focus
    if (m_answerMixed == true)
        integer_edit->setFocus();
    else
        numer_edit->setFocus();

}
