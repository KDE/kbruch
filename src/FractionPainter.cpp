/*
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tiago Porangaba <tiago.porangaba@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "FractionPainter.h"

/* these includes are needed for Qt support */
#include <QPainter>
#include <QPaintEvent>

#ifdef DEBUG
#include <QDebug>
#endif

FractionPainter::FractionPainter(QWidget * parent) :
    FractionBaseWidget(parent)
{
#ifdef DEBUG
    qDebug() << "constructor FractionPainter";
#endif
}

FractionPainter::~FractionPainter()
{
#ifdef DEBUG
    qDebug() << "destructor FractionPainter";
#endif
}

void FractionPainter::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paintWidget(paint);
}

void FractionPainter::paintFraction(const QString & op, Ratio & lr, int lm,
                                    Ratio & rr, int rm)
{
    str_operation = op;
    leftRatio = lr;
    leftMult = lm;
    rightRatio = rr;
    rightMult = rm;

    update();
}

void FractionPainter::paintWidget(QPainter & paint)
{
    // our x position, we paint from left to right;
    // we don't want to start directly on the border, so add the margin
    int old_x = _MARGIN_X;
    int old_y = 0;

    // ratios and operation signs are painted with the same font
    m_font.setPointSize(24);
    m_font.setBold(true);
    paint.setFont(m_font);

    // set the pen for painting
    QPen pen(Qt::SolidLine);
    pen.setWidth(0);
    paint.setPen(pen);

    // get the font height; the font height doesn't change while painting
    QFontMetrics  fm(paint.fontMetrics());

    // now we can correctly set the height of the widget
    setMinimumHeight(2 * fm.lineSpacing() + 10);
    setMaximumHeight(2 * fm.lineSpacing() + 10);

    // get the current ratio and paint it
    paintRatio(paint, Ratio(leftRatio.numerator() * leftMult, leftRatio.denominator() * leftMult, false), old_x, old_y, fm);

    if (leftRatio.denominator() * leftMult < 10)
        old_x += 40;
    else
        old_x += 25;

    // paint the operation
    paintMiddle(paint, str_operation, old_x, old_y, fm, m_colorOperation);

    if (rightRatio.denominator() * rightMult < 10)
        old_x += 40;
    else
        old_x += 25;

    // get the current ratio and paint it
    paintRatio(paint, Ratio(rightRatio.numerator() * rightMult, rightRatio.denominator() * rightMult, false), old_x, old_y, fm);

    // stop the painter
    paint.end();

    // the space we needed for painting is the minimum width of the widget
    setMinimumWidth(old_x);
}

/* ------ public slots ------ */

void FractionPainter::update()
{
    updateAndRepaint();

    // update for itself
    ((QWidget *) this)->update();
}
