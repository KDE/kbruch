/***************************************************************************
                              FractionPainter.h
                             -------------------
    begin                : 2008/09/18
    copyright            : (C) 2008 by Danilo Balzaque
    email                : danilo.balzaque@ltia.fc.unesp.br
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FRACTIONPAINTER_H
#define FRACTIONPAINTER_H

#define DEBUG

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "ratio.h"
#include "ratiowidget.h"

#include <qwidget.h>
//Added by qt3to4:
#include <QPaintEvent>
#include <QPainter>

class QPainter;

/*! Constructs a QWidget bla bla bla
 *  \author Danilo Balzaque
 * */
class FractionPainter : public FractionBaseWidget
{
	Q_OBJECT

public:
	/** constructor */
	FractionPainter(QWidget * parent);

	/** destructor */
	~FractionPainter();

	void paintFraction(const QString & str_operation, ratio & leftRatio, int leftMult,
		ratio & rightRatio, int rightMult);
protected:
	/** Function is called every time the screen need to be painted.
	 **/
	void paintEvent(QPaintEvent * event);
	void paintWidget(QPainter & paint);

private:
	QString str_operation;
	ratio leftRatio;
	int leftMult;
	ratio rightRatio;
	int rightMult;

public slots:
	void update();
};

#endif
