/***************************************************************************
                          factorizedwidget.h  -  paint a factorization
                             -------------------
    begin                : 2004/07/11
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

#include "factorizedwidget.h"
#include "factorizedwidget.moc"

/* these includes are needed for Qt support */
#include <qpainter.h>

FactorizedWidget::FactorizedWidget(QWidget * parent, const char * name, const uintList para_factors) :
			FractionBaseWidget(parent, name), m_factors(para_factors)
{
#ifdef DEBUG
	kdDebug() << "constructor FactorizedWidget" << endl;
#endif
}

FactorizedWidget::~FactorizedWidget()
{
#ifdef DEBUG
	kdDebug() << "destructor FactorizedWidget" << endl;
#endif
}

void FactorizedWidget::setFactors(const uintList para_factors)
{
	m_factors = para_factors;
	update();
}

void FactorizedWidget::paintEvent(QPaintEvent* /* p_paintEvent */)
{
	// our x position, we paint from left to right;
	// we don't want to start directly on the border, so add the margin
	int x_pos = _MARGIN_X;

	int fontWidth; // to store the width of the last thing painted

	// start the painter
	QPainter paint(this);

	// ratios and operation signs are painted with the same font
	paint.setFont(m_font);

	// set the pen for painting
	QPen pen(Qt::SolidLine);
	pen.setWidth(0);
	paint.setPen(pen);

	// get the font height; the font height doesn't change while painting
	QFontMetrics  fm(paint.fontMetrics());

	// now we can correctly set the height of the widget
	setMinimumHeight(fm.lineSpacing());
	setMaximumHeight(fm.lineSpacing());

	QString tmpStr;
	int fontHeight = fm.lineSpacing(); // get the font height

	for (uint tmpInt = 0; tmpInt < m_factors.count(); tmpInt++)
	{
		// set color for operation sign
		pen.setColor(m_colorOperation);
		paint.setPen(pen);

		if (tmpInt == 0)
		{
			fontWidth = fm.width("=");
			paint.drawText(x_pos, 0, fontWidth, fontHeight, AlignCenter, "=");
			x_pos += fontWidth;
			x_pos += _MARGIN_X;
		} else {
			fontWidth = fm.width("*");
			paint.drawText(x_pos, 0, fontWidth, fontHeight, AlignCenter, "*");
			x_pos += fontWidth;
			x_pos += _MARGIN_X;
		}

		// set color for number
		pen.setColor(m_colorNumber);
		paint.setPen(pen);

		tmpStr.setNum(m_factors[tmpInt]);

		fontWidth = fm.width(tmpStr);
		paint.drawText(x_pos, 0, fontWidth, fontHeight, AlignCenter, tmpStr);
		x_pos += fontWidth;
		x_pos += _MARGIN_X;
	}

	// stop the painter
	paint.end();

	// the space we needed for painting is the minimum width of the widget
	setMinimumWidth(x_pos);

	return;
}
