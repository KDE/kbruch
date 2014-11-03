/***************************************************************************
                              StatisticsBarWidget.h
                             -------------------
    begin                : 2008/09/02
    copyright            : (C) 2008 by Danilo Balzaque
    email                : danilo.balzaque@ltia.fc.unesp.br

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
    explicit StatisticsBarWidget(QWidget * parent = 0);

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
