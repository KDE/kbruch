/*
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
