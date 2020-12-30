/*
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "StatisticsBarWidget.h"

/* these includes are needed for Qt support */
#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>

#ifdef DEBUG
#include <QDebug>
#endif

#include "settingsclass.h"

/* constructor */
StatisticsBarWidget::StatisticsBarWidget(QWidget * parent) :
    QWidget(parent)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor StatisticsBarWidget()");
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
    progressPalette.setColor(QPalette::Window, QColor(192, 234, 194));
    progressPalette.setColor(QPalette::Highlight, Qt::green);
    m_correctBar->setPalette(progressPalette);
    m_correctBar->setMinimumSize(QSize(210, 14));
    m_correctBar->setRange(0, 100);
    m_correctBar->setValue(0);

    m_incorrectBar = new QProgressBar(this);
    m_incorrectBar->setTextVisible(false);
    progressPalette.setColor(QPalette::Window, QColor(234, 192, 192));
    progressPalette.setColor(QPalette::Highlight, Qt::red);
    m_incorrectBar->setPalette(progressPalette);
    m_incorrectBar->setMinimumSize(QSize(210, 14));
    m_incorrectBar->setRange(0, 100);
    m_incorrectBar->setValue(0);

    m_skippedBar = new QProgressBar(this);
    m_skippedBar->setTextVisible(false);
    progressPalette.setColor(QPalette::Window, QColor(233, 234, 192));
    progressPalette.setColor(QPalette::Highlight, Qt::yellow);
    m_skippedBar->setPalette(progressPalette);
    m_skippedBar->setMinimumSize(QSize(210, 14));
    m_skippedBar->setRange(0, 100);
    m_skippedBar->setValue(0);

    m_correctLabel = new QLabel(QStringLiteral("0%"), this);
    m_correctLabel->setFont(defaultFont);
    m_correctLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_incorrectLabel = new QLabel(QStringLiteral("0%"), this);
    m_incorrectLabel->setFont(defaultFont);
    m_incorrectLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_skippedLabel = new QLabel(QStringLiteral("0%"), this);
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
    qDebug() << QStringLiteral("destructor StatisticsBarWidget()");
#endif
}

void StatisticsBarWidget::updateBar(int correct, int skipped, int total)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("StatisticsBarWidget::updateBar()");
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
