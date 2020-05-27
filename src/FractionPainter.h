/***************************************************************************
                              FractionPainter.h
                             -------------------
    begin                : 2008/09/18
    copyright            : (C) 2008 by Danilo Balzaque
    email                : danilo.balzaque@ltia.fc.unesp.br

    copyright            : (C) 2008 by Tadeu Araujo, tadeu.araujo@ltia.fc.unesp.br
    copyright            : (C) 2008 by Tiago Porangaba, tiago.porangaba@ltia.fc.unesp.br
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FRACTIONPAINTER_H
#define FRACTIONPAINTER_H

#include "Ratio.h"
#include "RatioWidget.h"

#include <QWidget>

class QPainter;
class QPaintEvent;

/*! Constructs a QWidget bla bla bla
 *  \author Danilo Balzaque
 * */
class FractionPainter : public FractionBaseWidget
{
    Q_OBJECT
public:
    /** constructor */
    explicit FractionPainter(QWidget * parent = nullptr);

    /** destructor */
    ~FractionPainter();

    void paintFraction(const QString & str_operation, Ratio & leftRatio, int leftMult,
                       Ratio & rightRatio, int rightMult);
    void update();
protected:
    /** Function is called every time the screen need to be painted.
     **/
    void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;
    void paintWidget(QPainter & paint);

private:
    QString str_operation;
    Ratio leftRatio;
    int leftMult;
    Ratio rightRatio;
    int rightMult;
};

#endif
