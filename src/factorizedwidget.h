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

#ifndef FACTORIZEDWIDGET_H
#define FACTORIZEDWIDGET_H

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "fractionbasewidget.h"

#include <qwidget.h>

// a list containing uints
typedef QValueList<uint> uintList;

/*! class to paint the factorization of a number
 *
 *  \author Sebastian Stein */
class FactorizedWidget : public FractionBaseWidget
{
	Q_OBJECT

public:
	/** constructor */
	FactorizedWidget(QWidget * parent, const char * name, const uintList para_factors);

	/** destructor */
	~FactorizedWidget();

	/** set the task to be displayed */
	void setFactors(const uintList para_factors);

private:

	/** the prime factors of the number */
	uintList m_factors;

	/** overrideing the paint event of FractionBaseWidget */
	void paintEvent(QPaintEvent*);
};

#endif
