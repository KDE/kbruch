/***************************************************************************
                          fractionbasewidget.h  -  base fraction painting
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

#ifndef FRACTIONBASEWIDGET_H
#define FRACTIONBASEWIDGET_H

/** the space between a ratio and an operation */
#define _MARGIN_X 5

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "ratio.h"

#include <qwidget.h>


/*! base class for painting fractions
 *
 *  \author Sebastian Stein */
class FractionBaseWidget : public QWidget
{
	Q_OBJECT

public:
	/** constructor */
	FractionBaseWidget(QWidget * parent, const char * name);

	/** destructor */
	~FractionBaseWidget();

	/** updates the widget by first getting the settings and then repainting */
	void updateAndRepaint();	

protected:
	
	/* store the different colors */
	QColor m_colorNumber;
	QColor m_colorLine;
	QColor m_colorOperation;

	/* the font to paint with */
	QFont m_font;

	/** overrideing the paint event of QWidget */
	virtual void paintEvent(QPaintEvent*) = 0;

	/** paints a ratio at the given position */
	void paintRatio(QPainter & paint, ratio tmp_ratio, int & x_pos, QFontMetrics & fm, bool show_mixed, bool addMargin = true);

	/** paints a string in the vertical middle (aligned to the operation signs) */
	void paintMiddle(QPainter & paint, const QString& paint_str, int & x_pos, QFontMetrics & fm, QColor color, bool addMargin = true);

private:
	/** sets the font and color; values taken from settingsclass */
	void setColorAndFont();
};

#endif
