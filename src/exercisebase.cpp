/***************************************************************************
                                exercisebase.cpp
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

#include "exercisebase.h"
#include "exercisebase.moc"

/* these includes are needed for KDE support */
#include <kapplication.h>
#include <klocale.h>

/* ----- public member functions ----- */

/* constructor */
ExerciseBase::ExerciseBase(QWidget * parent, const char * name):
		QWidget(parent, name)
{
#ifdef DEBUG
	kdDebug() << "constructor ExerciseBase()" << endl;
#endif
}

/* destructor */
ExerciseBase::~ExerciseBase()
{
#ifdef DEBUG
	kdDebug() << "destructor ExerciseBase()" << endl;
#endif
}
