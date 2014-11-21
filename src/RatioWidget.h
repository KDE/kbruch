/***************************************************************************
                          RatioWidget.h  -  paint a ratio
                             -------------------
    begin                : 2004/06/03
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
public:
    /** constructor */
    explicit RatioWidget(QWidget * parent = 0, const Ratio para_ratio = Ratio());

    /** destructor */
    ~RatioWidget();

    /** set the task to be displayed */
    void setRatio(const Ratio para_ratio);

    void setQuestionMixed(bool value);

private:

    /** the ratio to be displayed */
    Ratio m_ratio;

    /*identify if the fraction will appear in mixed form or not*/
    bool m_questionMixed;

    /** overrideing the paint event of FractionBaseWidget */
    void paintEvent(QPaintEvent*);
};

#endif
