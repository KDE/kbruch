/***************************************************************************
                          rationalwidget.h  -  paint a rational number
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

#include "rationalwidget.h"
#include "rationalwidget.moc"

/* these includes are needed for Qt support */
#include <qpainter.h>
#include <qstring.h>

RationalWidget::RationalWidget(QWidget * parent, const char * name, const QString pnumber, const uint pperiodStart, const uint pperiodLength):
			FractionBaseWidget(parent, name), m_number(pnumber),
				m_periodStart(pperiodStart), m_periodLength(pperiodLength)
{
#ifdef DEBUG
	kdDebug() << "constructor RationalWidget" << endl;
#endif
}

RationalWidget::~RationalWidget()
{
#ifdef DEBUG
	kdDebug() << "destructor RationalWidget" << endl;
#endif
}

void RationalWidget::setRational(const QString pnumber, const uint pperiodStart, const uint pperiodLength)
{
	m_number = pnumber;
	m_periodStart = pperiodStart;
	m_periodLength = pperiodLength;

	update();

	return;
}

void RationalWidget::paintEvent(QPaintEvent* /* p_paintEvent */)
{
	// our x position, we paint from left to right;
	// we don't want to start directly on the border, so add the margin
	int x_pos = _MARGIN_X;
	int x_startPos = _MARGIN_X;
	bool tmp_painting = false;

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
	int fontHeight = fm.lineSpacing(); // get the font height

	// now we can correctly set the height of the widget
	setMinimumHeight(2 * fontHeight + 10);
	setMaximumHeight(2 * fontHeight + 10);

	// paint each char one by one
	for (uint stringPos = 0; stringPos < m_number.length(); stringPos++)
	{
		// check if the period line starts over the current number
		if (m_periodLength > 0 && stringPos == m_periodStart && tmp_painting == false)
		{
			x_startPos = x_pos;
			tmp_painting = true;
		}

		// paint the current number (or comma)
		paintMiddle(paint, QString(m_number[stringPos]), x_pos, fm, m_colorNumber, false);

		// check if the period line ends over the current number; in this case
		// draw the period line
		if (tmp_painting == true && m_periodStart + m_periodLength - 1 == stringPos)
		{
			tmp_painting = false;

			// paint the period line above the numbers
			paint.fillRect(x_startPos, fontHeight - 3, x_pos - x_startPos, 1, m_colorNumber);
		}
	}

	// paint a = at the end
	x_pos += _MARGIN_X;
	paintMiddle(paint, "=", x_pos, fm, m_colorOperation);

	// stop the painter
	paint.end();

	// the space we needed for painting is the minimum width of the widget
	setMinimumWidth(x_pos);

	return;
}
