/***************************************************************************
                                ExerciseBase.cpp
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

#include "ExerciseBase.h"

/* these includes are needed for Qt support */
#ifdef DEBUG
#include <QDebug>
#endif

/* ----- public member functions ----- */

/* constructor */
ExerciseBase::ExerciseBase(QWidget * parent) :
    QWidget(parent)
{
#ifdef DEBUG
    qDebug() << "constructor ExerciseBase()";
#endif
}

/* destructor */
ExerciseBase::~ExerciseBase()
{
#ifdef DEBUG
    qDebug() << "destructor ExerciseBase()";
#endif
}
