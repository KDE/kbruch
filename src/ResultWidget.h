/*
    RatioWidget.h  -  paint a ratio
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    ResultWidget(QWidget * parent, const Ratio &para_result);
    ResultWidget(QWidget * parent, const uintList &para_factors);
    explicit ResultWidget(QWidget * parent = nullptr);

    /** destructor */
    ~ResultWidget();

    /** set the task to be displayed */
    void setResult(const Ratio &para_result, int k);
    void setResult(int k);

    int KindView();

    /** set the task to be displayed */
    void setFactors(const uintList &para_factors);

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

    /** overriding the paint event of FractionBaseWidget */
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;

    void Init();
    void showResult()   ;
};

#endif
