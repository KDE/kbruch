/***************************************************************************
                          kbruch.h  -  Header File
                             -------------------
    begin                : Tue Nov 27 16:40:42 CET 2001
    copyright            : (C) 2001 by Sebastian Stein
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

/*#define DEBUG*/

#ifndef BRUCH_H
#define BRUCH_H

#include <config.h>

/* for the I18N_NOOP macro */
#include <klocale.h>

#include "task.h"
#include "mainqtwidget.h"

static const char description[] =
	I18N_NOOP("KBruch");

class MainQtWidget;
class task;
class ratio;
class primzahl;

#endif /* ifndef BRUCH_H */
