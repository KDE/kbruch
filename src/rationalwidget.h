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

#ifndef RATIONALWIDGET_H
#define RATIONALWIDGET_H

#define DEBUG

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "fractionbasewidget.h"

class QString;

/*! class to paint a rational number
 *
 *  \author Sebastian Stein */
class RationalWidget : public FractionBaseWidget
{
	Q_OBJECT

public:
	/** constructor */
	RationalWidget(QWidget * parent, const char * name, const QString pnumber, const uint pperiodStart = 1, const uint pperiodLength = 0);

	/** destructor */
	~RationalWidget();

	/** set the task to be displayed */
	void setRational(const QString pnumber, const uint pperiodStart = 1, const uint pperiodLength = 0);

private:
	
	/** the rational number to be displayed */
	QString m_number;

	/** starting of the period */
	uint m_periodStart;
	
	/** length of the period */
	uint m_periodLength;

	/** overrideing the paint event of FractionBaseWidget */
	void paintEvent(QPaintEvent*);
};

#endif
