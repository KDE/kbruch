/*
    StatisticsView.cpp  -  Header File
    SPDX-FileCopyrightText: 2001-2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2001-2004 Eva Brucherseifer
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tiago Porangaba <tiago.porangaba@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef STATISTICSVIEW_H
#define STATISTICSVIEW_H

#include <QLabel>
#include <QWidget>

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

class StatisticsBarWidget;


/*!
  * StatisticsView takes care of the statistics of a test.
  * It saves the number of correct and wrong answers and
  * displays this data to the user.
  * \author Sebastian Stein
  * \author Eva Brucherseifer
  */

class StatisticsView : public QFrame
{
    Q_OBJECT
public:
    /** constructor */
    explicit StatisticsView(QWidget * parent = nullptr);

    /** destructor */
    ~StatisticsView();

    /** increment number of correct answers */
    void addCorrect();

    /** increment number of skipped answers */
    void addSkipped();

    /** increment number of wrong answers */
    void addWrong();

    /** set statistics to zero.
     *  Triggered by internal button or when a new test is started
     */
    void resetStatistics();
private:
    /** calculate percentages and update view */
    void calc();

    unsigned int m_count;
    unsigned int m_correct;
    unsigned int m_skipped;

    QHBoxLayout * buttonLayout;
    QVBoxLayout * layout1;
    QGridLayout * labelGrid;
    QLabel * result1Label;
    QLabel * result2Label;
    QLabel * result3Label;
    QLabel * result4Label;
    QLabel * info1Label;
    QLabel * info2Label;
    QLabel * info3Label;
    QLabel * info4Label;

    QFont defaultFont;

    QPushButton * resetButton;

    StatisticsBarWidget * statisticsBar;
};

#endif
