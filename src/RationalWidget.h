/***************************************************************************
                          RationalWidget.h  -  paint a rational number
                             -------------------
    begin                : 2004/06/04
    copyright            : (C) 2004 by Sebastian Stein
    email                : seb.kde@hpfsc.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef RATIONALWIDGET_H
#define RATIONALWIDGET_H

#define DEBUG

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "FractionBaseWidget.h"
//Added by qt3to4:
#include <QPaintEvent>

class QString;

/*! class to paint a rational number
 *
 *  \author Sebastian Stein */
class RationalWidget : public FractionBaseWidget
{
    Q_OBJECT

public:
    /** constructor */
    RationalWidget(QWidget * parent, const QString &pnumber, const int pperiodStart = 1, const int pperiodLength = 0);

    /** destructor */
    ~RationalWidget();

    /** set the task to be displayed */
    void setRational(const QString &pnumber, const int pperiodStart = 1, const int pperiodLength = 0);

private:

    /** the rational number to be displayed */
    QString m_number;

    /** starting of the period */
    int m_periodStart;

    /** length of the period */
    int m_periodLength;

    /** overrideing the paint event of FractionBaseWidget */
    void paintEvent(QPaintEvent*);
};

#endif
