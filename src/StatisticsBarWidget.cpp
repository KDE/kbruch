/***************************************************************************
                          StatisticsBarWidget.cpp
                             -------------------
    begin                : 2008/09/02
    copyright            : (C) 2008 by Danilo Balzaque
    email                : danilo.balzaque@gmail.com

    copyright            : (C) 2008 by Tadeu Araujo, tadeu.araujo@ltia.fc.unesp.br
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "StatisticsBarWidget.h"
#include "StatisticsBarWidget.moc"

/* these includes are needed for KDE support */
#include <klocale.h>

/* these includes are needed for Qt support */
#include <qapplication.h>
#include <QProgressBar>
#include <QLabel>

//Added by qt3to4:

#include "settingsclass.h"

/* constructor */
StatisticsBarWidget::StatisticsBarWidget(QWidget * parent) :
    QWidget(parent)
{
#ifdef DEBUG
    kDebug() << "constructor StatisticsBarWidget()";
#endif
    m_correct = 0;
    m_skipped = 0;
    m_total = 0;

    defaultFont = SettingsClass::taskFont();
    defaultFont.setBold(true);
    defaultFont.setPointSize(10);

    QGridLayout * barLayout = new QGridLayout();

    m_correctBar = new QProgressBar(this);
    m_correctBar->setTextVisible(false);
    QPalette progressPalette;
    progressPalette.setColor(QPalette::Background, QColor(192, 234, 194));
    progressPalette.setColor(QPalette::Highlight, Qt::green);
    m_correctBar->setPalette(progressPalette);
    m_correctBar->setMinimumSize(QSize(210, 14));
    m_correctBar->setRange(0, 100);
    m_correctBar->setValue(0);

    m_incorrectBar = new QProgressBar(this);
    m_incorrectBar->setTextVisible(false);
    progressPalette.setColor(QPalette::Background, QColor(234, 192, 192));
    progressPalette.setColor(QPalette::Highlight, Qt::red);
    m_incorrectBar->setPalette(progressPalette);
    m_incorrectBar->setMinimumSize(QSize(210, 14));
    m_incorrectBar->setRange(0, 100);
    m_incorrectBar->setValue(0);

    m_skippedBar = new QProgressBar(this);
    m_skippedBar->setTextVisible(false);
    progressPalette.setColor(QPalette::Background, QColor(233, 234, 192));
    progressPalette.setColor(QPalette::Highlight, Qt::yellow);
    m_skippedBar->setPalette(progressPalette);
    m_skippedBar->setMinimumSize(QSize(210, 14));
    m_skippedBar->setRange(0, 100);
    m_skippedBar->setValue(0);

    m_correctLabel = new QLabel("0%", this);
    m_correctLabel->setFont(defaultFont);
    m_correctLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_incorrectLabel = new QLabel("0%", this);
    m_incorrectLabel->setFont(defaultFont);
    m_incorrectLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_skippedLabel = new QLabel("0%", this);
    m_skippedLabel->setFont(defaultFont);
    m_skippedLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    barLayout->addWidget(m_correctBar, 0, 0, Qt::AlignVCenter | Qt::AlignLeft);
    barLayout->addWidget(m_incorrectBar, 1, 0, Qt::AlignVCenter | Qt::AlignLeft);
    barLayout->addWidget(m_skippedBar, 2, 0, Qt::AlignVCenter | Qt::AlignLeft);

    barLayout->addWidget(m_correctLabel, 0, 1);
    barLayout->addWidget(m_incorrectLabel, 1, 1);
    barLayout->addWidget(m_skippedLabel, 2, 1);

    setLayout(barLayout);
}

/* destructor */
StatisticsBarWidget::~StatisticsBarWidget()
{
#ifdef DEBUG
    kDebug() << "destructor StatisticsBarWidget()";
#endif
}

void StatisticsBarWidget::updateBar(int correct, int skipped, int total)
{
#ifdef DEBUG
    kDebug() << "StatisticsBarWidget::updateBar()";
#endif
    m_correct = correct;
    m_skipped = skipped;
    m_total = total;

    m_correctBar->setRange(0, m_total);
    m_correctBar->setValue(correct);

    m_incorrectBar->setRange(0, m_total);
    m_incorrectBar->setValue(m_total - m_correct - m_skipped);

    m_skippedBar->setRange(0, m_total);
    m_skippedBar->setValue(m_skipped);

    m_incorrectLabel->setText(m_incorrectBar->text());
    m_skippedLabel->setText(m_skippedBar->text());
    m_correctLabel->setText(m_correctBar->text());

    update();
}
