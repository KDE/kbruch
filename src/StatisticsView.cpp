/*
    StatisticsView.cpp  -  the statistic window
    SPDX-FileCopyrightText: 2001-2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2001-2004 Eva Brucherseifer
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tiago Porangaba <tiago.porangaba@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "StatisticsView.h"

/* the includes are needed for KDE support */
#include <KLocalizedString>

/* the includes are needed for Qt support */
#include <QGridLayout>
#include <QLayout>
#include <QPushButton>

#ifdef DEBUG
#include <QDebug>
#endif

#include "settingsclass.h"
#include "StatisticsBarWidget.h"

/* constructor */
StatisticsView::StatisticsView(QWidget * parent) :
    QFrame(parent), m_count(0), m_correct(0)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor StatisticsView()");
#endif
    // load statistics from config file
    m_count = SettingsClass::count();
    m_correct = SettingsClass::correct();
    m_skipped = SettingsClass::skipped();

    defaultFont = SettingsClass::taskFont();
    defaultFont.setBold(true);
    defaultFont.setPointSize(28);

    /* create a grid to show the labels */
    labelGrid = new QGridLayout();
    setLayout(labelGrid);

    labelGrid->setColumnStretch(0, 1);
    labelGrid->setColumnStretch(6, 1);

    labelGrid->setColumnMinimumWidth(5, 220);
    labelGrid->setColumnMinimumWidth(2, 30);

    result1Label = new QLabel(this);
    labelGrid->addWidget(result1Label, 1, 1, 2, 1);
    result1Label->setFont(defaultFont);
    result1Label->setAlignment(Qt::AlignCenter);
    result1Label->setToolTip(
        i18n("This is the current total number of solved tasks."));

    defaultFont.setBold(false);
    defaultFont.setPointSize(10);

    info1Label = new QLabel(this);
    info1Label->setText(i18n("Questions:"));
    info1Label->setFont(defaultFont);
    info1Label->setAlignment(Qt::AlignCenter);
    labelGrid->addWidget(info1Label, 0, 1);

    info2Label = new QLabel(this);
    info2Label->setText(i18nc("@info:status the number of correct answers", "Correct:"));
    info2Label->setFont(defaultFont);
    info2Label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    labelGrid->addWidget(info2Label, 0, 3);

    info4Label = new QLabel(this);
    info4Label->setText(i18nc("@info:status the number of incorrect answers", "Incorrect:"));
    info4Label->setFont(defaultFont);
    info4Label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    labelGrid->addWidget(info4Label, 1, 3);

    info3Label = new QLabel(this);
    info3Label->setText(i18nc("@info:status the number of skipped answers", "Skipped:"));
    info3Label->setFont(defaultFont);
    info3Label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    labelGrid->addWidget(info3Label, 2, 3);

    defaultFont.setBold(true);

    result2Label = new QLabel(this);
    labelGrid->addWidget(result2Label, 0, 4);
    result2Label->setFont(defaultFont);
    result2Label->setToolTip(
        i18n("This is the current total number of correctly solved tasks."));

    result4Label = new QLabel(this);
    labelGrid->addWidget(result4Label, 1, 4);
    result4Label->setFont(defaultFont);
    result4Label->setToolTip(
        i18n("This is the current total number of unsolved tasks."));

    result3Label = new QLabel(this);
    labelGrid->addWidget(result3Label, 2, 4);
    result3Label->setFont(defaultFont);
    result3Label->setToolTip(
        i18n("This is the current total number of skipped tasks."));

    // add tooltip and qwhatsthis help to the widget
    setToolTip(i18n("This part of the window shows the statistics."));
    setWhatsThis(i18n("This part of the window shows the statistics. Each exercise you do is counted. You can reset the statistics by clicking on the 'New' button in the toolbar or by selecting 'New' from the 'File' menu"));

    QBoxLayout * cLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    statisticsBar = new StatisticsBarWidget(this);
    labelGrid->addWidget(statisticsBar, 0, 5, 3, 1);
    labelGrid->addLayout(cLayout, 0, 5, Qt::AlignCenter);

    // add reset button and connect
    resetButton = new QPushButton(this);
    resetButton->setObjectName(QStringLiteral("resetButton"));
    resetButton->setText(i18n("&Reset"));
    resetButton->setToolTip(i18n("Click this button to reset the statistics."));
    resetButton->setFont(defaultFont);
    QObject::connect(resetButton, &QPushButton::clicked, this, &StatisticsView::resetStatistics);
    labelGrid->addWidget(resetButton, 5, 5, Qt::AlignRight);

    /* calculate the statistics */
    (void) calc();
}

/* destructor */
StatisticsView::~StatisticsView()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("destructor StatisticsView()");
#endif
    // save statistics for next run
    SettingsClass::setCount(m_count);
    SettingsClass::setCorrect(m_correct);
    SettingsClass::setSkipped(m_skipped);
    SettingsClass::self()->save();

    /* no need to delete any child widgets, Qt does it by itself */
}

/* called, if a task solved correctly */
void StatisticsView::addCorrect()
{
    ++m_count;
    ++m_correct;
    (void) calc(); /* repaint the statistics */
}

/* called, if a task is skipped */
void StatisticsView::addSkipped()
{
    ++m_count;
    ++m_skipped;
    (void) calc(); /* repaint the statistics */
}

/* called, if a task was solved wrong */
void StatisticsView::addWrong()
{
    ++m_count;
    (void) calc(); /* repaint the statistics */
}


/* ------ private member functions ------ */

/* recalculates the statistics and changes the corresponding labels */
void StatisticsView::calc()
{
    statisticsBar->updateBar(m_correct, m_skipped, m_count);

    QString new_text;

    new_text = QStringLiteral("<b>%1</b>").arg(m_count);
    result1Label->setText(new_text);

    /* we have to be careful with division by 0 */
    if (m_count == 0) {
        result2Label->setText(QStringLiteral("0 (0 %)"));
        result3Label->setText(QStringLiteral("0 (0 %)"));
        result4Label->setText(QStringLiteral("0 (0 %)"));
    } else {
        /* set the correct label */
        new_text = QStringLiteral("%1").arg(m_correct);
        result2Label->setText(new_text);

        /* set the correct label */
        new_text = QStringLiteral("%1").arg(m_skipped);
        result3Label->setText(new_text);

        /* set the incorrect label */
        new_text = QStringLiteral("%1").arg(m_count - m_correct - m_skipped);
        result4Label->setText(new_text);
    }
}

/* ------ public slots ------ */

/* called by the reset button */
void StatisticsView::resetStatistics()
{
    m_count = 0;
    m_correct = 0;
    m_skipped = 0;
    (void) calc();
}
