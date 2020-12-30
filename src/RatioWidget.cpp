/*
    RatioWidget.h  -  paint a ratio
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "RatioWidget.h"

/* these includes are needed for Qt support */
#include <QPainter>
#include <QPaintEvent>

#ifdef DEBUG
#include <QDebug>
#endif

RatioWidget::RatioWidget(QWidget * parent,
                         const Ratio &para_ratio) :
    FractionBaseWidget(parent), m_ratio(para_ratio)
{
#ifdef DEBUG
    qDebug() << "constructor RatioWidget";
#endif
}

RatioWidget::~RatioWidget()
{
#ifdef DEBUG
    qDebug() << "destructor RatioWidget";
#endif
}

void RatioWidget::setRatio(const Ratio &para_ratio)
{
    m_ratio = para_ratio;
    update();
}

void RatioWidget::setQuestionMixed(bool value)
{
    m_questionMixed = value;
}

void RatioWidget::paintEvent(QPaintEvent* /* p_paintEvent */)
{
    // our x position, we paint from left to right;
    // we don't want to start directly on the border, so add the margin
    int x_pos = _MARGIN_X;
    int y_pos = 0;

    // start the painter
    QPainter paint(this);

    // ratios and operation signs are painted with the same font
    paint.setFont(m_font);

    // set the pen for painting
    QPen pen(Qt::SolidLine);
    pen.setWidth(0);
    paint.setPen(pen);

    // get the font height; the font height doesn't change while painting
    QFontMetrics fm(paint.fontMetrics());

    // now we can correctly set the height of the widget
    setMinimumHeight(2 * fm.lineSpacing() + 10);
    setMaximumHeight(2 * fm.lineSpacing() + 10);

    // result as normal ratio
    paintRatio(paint, m_ratio, x_pos, y_pos, fm, m_questionMixed);

    // stop the painter
    paint.end();

    // the space we needed for painting is the minimum width of the widget
    setMinimumWidth(x_pos);

    return;
}
