/***************************************************************************
                          ResultWidget.h  -  paint the result
                             -------------------
    begin                : 2004/05/30
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

#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include "FractionBaseWidget.h"
#include "Ratio.h"

#include <QWidget>

class QGridLayout;
class QLabel;
class QPaintEvent;

// a list containing uints
typedef QList<uint> uintList;

/*! class to paint the result
 *
 *  \author Sebastian Stein */
class ResultWidget : public FractionBaseWidget
{
    Q_OBJECT

public:
    /** constructors */
    ResultWidget(QWidget * parent, const Ratio para_result);
    ResultWidget(QWidget * parent, const uintList para_factors);
    explicit ResultWidget(QWidget * parent = 0);

    /** destructor */
    ~ResultWidget();

    /** set the task to be displayed */
    void setResult(const Ratio para_result, int k);
    void setResult(int k);

    int KindView();

    /** set the task to be displayed */
    void setFactors(const uintList para_factors);

    void setAnswerMixed(bool value);

private:
    bool m_answerMixed;
    /** the prime factors of the number */
    uintList m_factors;

    QFont defaultFont;

    QLabel * m_primaryText;
    QLabel * m_secondaryText;

    QGridLayout * layout;

    // 1 = Correct, 0 = Incorrect (with solution), 2 = Incorrect (without solution),
    short m_kindView;

    short m_ExerciseView;

    /** the ratio to be displayed */
    Ratio m_result;

    /** overrideing the paint event of FractionBaseWidget */
    void paintEvent(QPaintEvent*);

    void Init();
    void showResult()   ;
};

#endif
