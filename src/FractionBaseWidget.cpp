/***************************************************************************
                 FractionBaseWidget.cpp  -  base fraction painting
                             -------------------
    begin                : 2004/05/30
    copyright            : (C) 2004 by Sebastian Stein
    email                : seb.kde@hpfsc.de

    copyright            : (C) 2008 by Tiago Porangaba, tiago.porangaba@ltia.fc.unesp.br
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

#include "FractionBaseWidget.h"

/* these includes are needed for KDE support */

/* these includes are needed for Qt support */
#include <QPainter>

#ifdef DEBUG
#include <QDebug>
#endif

#include "settingsclass.h"

FractionBaseWidget::FractionBaseWidget(QWidget * parent) :
    QWidget(parent)
{
#ifdef DEBUG
    qDebug() << "constructor FractionBaseWidget";
#endif

    // set colors and font used for task displaying
    setColorAndFont();
}

FractionBaseWidget::~FractionBaseWidget()
{
#ifdef DEBUG
    qDebug() << "destructor FractionBaseWidget";
#endif
}

void FractionBaseWidget::updateAndRepaint()
{
    setColorAndFont();
    update();
}

void FractionBaseWidget::paintRatio(QPainter & paint, const Ratio &tmp_ratio, int & x_pos, int & y_pos, QFontMetrics & fm, bool show_mixed, bool addMargin, bool show_center)
{
    QPen pen = paint.pen(); // get the pen
    int fontHeight = fm.lineSpacing(); // get the font height

    int int_numerator, int_denominator, int_mixed;
    QString str_numerator, str_denominator;
    QString str_mixed;

    int fontWidth; // to store the width of the last thing painted
    int tmp_int;

    // check if we have to show the ratio as mixed number
    //         11            1
    // if yes, -- becomes  2 -
    //         5             5
    int_numerator = tmp_ratio.numerator();
    int_denominator = tmp_ratio.denominator();
    if (show_mixed == true && qAbs(int_numerator) >= qAbs(int_denominator)) {
        // calculate the mixed number
        int_mixed = int (int_numerator / int_denominator);

        // the negative sign is in front of the mixed number
        int_numerator = qAbs(int_numerator);
        int_denominator = qAbs(int_denominator);

        // we have to reduce the numerator by the mixed number * denominator
        int_numerator = int_numerator % int_denominator;

        // now we can convert the numbers into strings for painting
        str_mixed.setNum(int_mixed);
        str_numerator.setNum(int_numerator);
        str_denominator.setNum(int_denominator);

        // paint the front part of the mixed number
        paintMiddle(paint, str_mixed, x_pos, y_pos, fm, m_colorNumber);
    } else {

        // don't show the ratio as mixed number
        str_numerator.setNum(int_numerator);
        str_denominator.setNum(int_denominator);
    } // if (show_mixed == true && qAbs(int_numerator) > qAbs(int_denominator))

    // get the text width of the current ratio
    fontWidth = fm.width(str_numerator);
    tmp_int = fm.width(str_denominator);
    if (tmp_int > fontWidth)
        fontWidth = tmp_int;

    // show numerator and denominator in m_colorNumber
    pen.setColor(m_colorNumber);
    paint.setPen(pen);

    // make sure we don't display something like:   0
    //                                            7 -
    //                                              3
    if (!(show_mixed == true && int_numerator == 0)) {
        if (show_center == true)
            x_pos = 80 - fontWidth / 2;
        if (!(int_denominator == 1 && show_mixed == false)) {
            // paint the numerator
            paint.drawText(x_pos, y_pos, fontWidth, fontHeight, Qt::AlignCenter, str_numerator);
            // paint the fraction line between numerator and denominator
            paint.fillRect(x_pos, y_pos + fontHeight + 4, fontWidth, 2, m_colorLine);
            // paint the denominator
            paint.drawText(x_pos, y_pos + fontHeight + 10, fontWidth, fontHeight, Qt::AlignCenter, str_denominator);
        } else {
            // paint the numerator and move the y position down to align with the + signal
            paint.drawText(x_pos, y_pos + fontHeight - fontHeight / 2, fontWidth,
                           fontHeight, Qt::AlignCenter, str_numerator);
        }

        // move the x position to the right by adding the width used for painting
        // the ratio and a margin
        x_pos += fontWidth;

        if (addMargin == true)
            x_pos += _MARGIN_X;
    }

    return;
}

void FractionBaseWidget::paintMiddle(QPainter & paint,
                                     const QString &paint_str,
                                     int & x_pos, int & y_pos, QFontMetrics & fm,
                                     const QColor &color, bool addMargin)
{
    // get the pen, font height and font width
    QPen pen = paint.pen();
    int fontHeight = fm.lineSpacing();
    int fontWidth = fm.width(paint_str);

    // paint the string
    pen.setColor(color);
    paint.setPen(pen);
    paint.drawText(x_pos, y_pos + fontHeight - fontHeight / 2, fontWidth, fontHeight, Qt::AlignCenter, paint_str);

    // move the x position to the right by adding the width used for
    // painting the string and a margin
    x_pos += fontWidth;

    if (addMargin == true)
        x_pos += _MARGIN_X;

    return;
}

void FractionBaseWidget::setColorAndFont()
{
    /* set colors */
    m_colorNumber = SettingsClass::numberColor();
    m_colorLine = SettingsClass::fractionBarColor();
    m_colorOperation = SettingsClass::operationColor();

    /* set font */
    m_font = SettingsClass::taskFont();

    // repaint
    update();

    return;
}
