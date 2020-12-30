/*
    RationalWidget.h  -  paint a rational number
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef RATIONALWIDGET_H
#define RATIONALWIDGET_H

#include "FractionBaseWidget.h"

class QPaintEvent;

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

    /** overriding the paint event of FractionBaseWidget */
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;
};

#endif
