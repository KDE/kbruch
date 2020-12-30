/*
    RatioWidget.h  -  paint a ratio
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef RATIOWIDGET_H
#define RATIOWIDGET_H

#include "FractionBaseWidget.h"
#include "Ratio.h"

#include <QWidget>

class QPaintEvent;

/*! class to paint the ratio
 *
 *  \author Sebastian Stein */
class RatioWidget : public FractionBaseWidget
{
    Q_OBJECT
public:
    /** constructor */
    explicit RatioWidget(QWidget * parent = nullptr, const Ratio &para_ratio = Ratio());

    /** destructor */
    ~RatioWidget();

    /** set the task to be displayed */
    void setRatio(const Ratio &para_ratio);

    void setQuestionMixed(bool value);

private:

    /** the ratio to be displayed */
    Ratio m_ratio;

    /*identify if the fraction will appear in mixed form or not*/
    bool m_questionMixed;

    /** overriding the paint event of FractionBaseWidget */
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
};

#endif
