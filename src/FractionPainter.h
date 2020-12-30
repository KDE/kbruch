/*
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tiago Porangaba <tiago.porangaba@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
