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

#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "fractionbasewidget.h"
#include "ratio.h"

#include <qwidget.h>

/*! class to paint the result
 *
 *  \author Sebastian Stein */
class ResultWidget : public FractionBaseWidget
{
	Q_OBJECT

public:
	/** constructor */
	ResultWidget(QWidget * parent, const char * name, const ratio para_result);

	/** destructor */
	~ResultWidget();

	/** set the task to be displayed */
	void setResult(const ratio para_result);

private:
	
	/** the ratio to be displayed */
	ratio m_result;

	/** overrideing the paint event of FractionBaseWidget */
	void paintEvent(QPaintEvent*);
};

#endif
