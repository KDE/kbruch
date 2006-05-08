/***************************************************************************
                          resultwidget.h  -  paint the result
                             -------------------
    begin                : 2004/05/30
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

#include "resultwidget.h"
#include "resultwidget.moc"

/* these includes are needed for Qt support */
#include <qpainter.h>

#include "settingsclass.h"

ResultWidget::ResultWidget(QWidget * parent = 0, const char * name = 0,
													const ratio para_result = *new ratio()) :
			FractionBaseWidget(parent, name), m_result(para_result)
{
#ifdef DEBUG
	kdDebug() << "constructor ResultWidget" << endl;
#endif
}

ResultWidget::~ResultWidget()
{
#ifdef DEBUG
	kdDebug() << "destructor ResultWidget" << endl;
#endif
}

void ResultWidget::setResult(const ratio para_result)
{
	m_result = para_result;
	update();
}

void ResultWidget::paintEvent(QPaintEvent* /* p_paintEvent */)
{
	// our x position, we paint from left to right;
	// we don't want to start directly on the border, so add the margin
	int old_x = _MARGIN_X;

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
	paintMiddle(paint, QString("="), old_x, fm, m_colorOperation);
	paintRatio(paint, m_result, old_x, fm, false);

	if (SettingsClass::showSpecialRatioNotation() == true && QABS(m_result.numerator()) >= QABS(m_result.denominator()))
	{
		// result as mixed number
		paintMiddle(paint, QString("="), old_x, fm, m_colorOperation);
		paintRatio(paint, m_result, old_x, fm, true);
	}

	// stop the painter
	paint.end();

	// the space we needed for painting is the minimum width of the widget
	setMinimumWidth(old_x);

	return;
}
