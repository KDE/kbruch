/*
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef STATISTICSBARWIDGET_H
#define STATISTICSBARWIDGET_H

#include <QWidget>

class QProgressBar;
class QLabel;

/*! Constructs a QWidget bla bla bla
 *  \author Danilo Balzaque
 * */
class StatisticsBarWidget : public QWidget
{
    Q_OBJECT
public:
    /** constructor */
    explicit StatisticsBarWidget(QWidget * parent = nullptr);

    /** destructor */
    ~StatisticsBarWidget();

    /** Update statisticsBar */
    void updateBar(int correct, int skipped, int total);

protected:

private:
    int m_correct;
    int m_skipped;
    int m_total;

    QFont defaultFont;

    QLabel * m_correctLabel;
    QLabel * m_skippedLabel;
    QLabel * m_incorrectLabel;

    QProgressBar * m_correctBar;
    QProgressBar * m_skippedBar;
    QProgressBar * m_incorrectBar;
};

#endif
