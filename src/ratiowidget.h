/***************************************************************************
                          ratiowidget.h  -  paint a ratio
                             -------------------
    begin                : 2004/06/03
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

#ifndef RATIOWIDGET_H
#define RATIOWIDGET_H

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "fractionbasewidget.h"
#include "ratio.h"

#include <qwidget.h>

/*! class to paint the ratio
 *
 *  \author Sebastian Stein */
class RatioWidget : public FractionBaseWidget
{
	Q_OBJECT

public:
	/** constructor */
	RatioWidget(QWidget * parent, const char * name, const ratio para_ratio);

	/** destructor */
	~RatioWidget();

	/** set the task to be displayed */
	void setRatio(const ratio para_ratio);

private:
	
	/** the ratio to be displayed */
	ratio m_ratio;

	/** overrideing the paint event of FractionBaseWidget */
	void paintEvent(QPaintEvent*);
};

#endif
