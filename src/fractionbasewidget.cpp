/***************************************************************************
                          fractionbasewidget.cpp  -  base fraction painting
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

#include "fractionbasewidget.h"
#include "fractionbasewidget.moc"

/* these includes are needed for KDE support */
#include <kglobalsettings.h>

/* these includes are needed for Qt support */
#include <qpainter.h>

#include "settingsclass.h"

FractionBaseWidget::FractionBaseWidget(QWidget * parent = 0, const char * name = 0) :
			QWidget(parent, name)
{
#ifdef DEBUG
	kdDebug() << "constructor FractionBaseWidget" << endl;
#endif

	// set colors and font used for task displaying
	setColorAndFont();
}

FractionBaseWidget::~FractionBaseWidget()
{
#ifdef DEBUG
	kdDebug() << "destructor FractionBaseWidget" << endl;
#endif
}

void FractionBaseWidget::updateAndRepaint()
{
	setColorAndFont();
	update();
}

void FractionBaseWidget::paintRatio(QPainter & paint, ratio tmp_ratio, int & x_pos, QFontMetrics & fm, bool show_mixed, bool addMargin)
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
	if (show_mixed == true && QABS(int_numerator) >= QABS(int_denominator))
	{
		// calculate the mixed number
		int_mixed = int(int_numerator / int_denominator);

		// the negative sign is in front of the mixed number
		int_numerator = QABS(int_numerator);
		int_denominator = QABS(int_denominator);

		// we have to reduce the numerator by the mixed number * denominator
		int_numerator = int_numerator % int_denominator;

		// now we can convert the numbers into strings for painting
		str_mixed.setNum(int_mixed);
		str_numerator.setNum(int_numerator);
		str_denominator.setNum(int_denominator);

		// paint the front part of the mixed number
		paintMiddle(paint, str_mixed, x_pos, fm, m_colorNumber);
	} else {

		// don't show the ratio as mixed number
		str_numerator.setNum(int_numerator);
		str_denominator.setNum(int_denominator);
	} // if (show_mixed == true && QABS(int_numerator) > QABS(int_denominator))

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
	if (! (int_numerator == 0 && show_mixed == true) )
	{
		// paint the numerator
		paint.drawText(x_pos, 0, fontWidth, fontHeight, AlignCenter, str_numerator);
	
		// paint the fraction line between numerator and denominator
		paint.fillRect(x_pos, fontHeight + 4, fontWidth, 2, m_colorLine);
	
		// paint the denominator
		paint.drawText(x_pos, fontHeight + 10, fontWidth, fontHeight, AlignCenter, str_denominator);
	
		// move the x position to the right by adding the width used for painting
		// the ratio and a margin
		x_pos += fontWidth;

		if (addMargin == true)
			x_pos += _MARGIN_X;
	}

	return;
}

void FractionBaseWidget::paintMiddle(QPainter & paint, const QString& paint_str, int & x_pos, QFontMetrics & fm, QColor color, bool addMargin)
{
	// get the pen, font height and font width
	QPen pen = paint.pen();
	int fontHeight = fm.lineSpacing();
	int fontWidth = fm.width(paint_str);

	// paint the string
	pen.setColor(color);
	paint.setPen(pen);
	paint.drawText(x_pos, fontHeight + 5 - fontHeight / 2, fontWidth, fontHeight, AlignCenter, paint_str);

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
