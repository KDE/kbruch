/***************************************************************************
                                exercisebase.h
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

#ifndef EXERCISEBASE_H
#define EXERCISEBASE_H

#define _CHECK_TASK	0
#define _NEXT_TASK	1

#ifdef DEBUG
#include <kdebug.h>
#endif

#include <qwidget.h>

/*! Constructs a QWidget.
 *
 * It is the base class for showing the different exercises.
 *
 *  \author Sebastian Stein
 * */
class ExerciseBase : public QWidget
{
	Q_OBJECT

public:
	/** constructor */
	ExerciseBase(QWidget * parent = 0, const char * name = 0);

	/** destructor */
	~ExerciseBase();

	/** force the creation of a new task */
	virtual void forceNewTask() = 0;
};

#endif
