/***************************************************************************
                          kbruch.h  -  Header File
                             -------------------
    begin                : Tue Nov 27 16:40:42 CET 2001
    copyright            : (C) 2001 by Sebastian Stein
    email                : seb.stein@hpfsc.de
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#define VERSION "0.19"
#endif

#define MAINTAINER "Sebastian Stein"
#define MAINTAINER_EMAIL "bruch@hpfsc.de"
#define HOMEPAGE "http://www.hpfsc.de/bruch/"
#define DATE "[2002-05-01]"

#include "primzahl.h"
#include "ratio.h"
#include "task.h"
#include "gui_qt.h"
#include "kbruch.h"

#include <stdlib.h>
#include <iostream.h>
#include <iomanip.h>
#include <limits.h>

/* this includes are needed for Qt support */
#include <kapp.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>
#include <qfont.h>

static const char *description =
	I18N_NOOP("KBruch");
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE
	
	
static KCmdLineOptions options[] =
{
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

/* activate this macro, if you want to build a text gui for MS DOS */
/*#define MSDOS*/

/* we need this only for MS DOS because of clrscr()*/
/* under Borland C 5 main menu bar Optionen|Project|Compiler|Definitions
 *	put "MSDOS" into the input box */
#ifdef MSDOS
#include <conio.h>
#define CLEAR clrscr();
#else

/* and on any other system (Unix) */
#define CLEAR system("clear");
#endif /* ifdef MSDOS */

/* for Microsoft Windows 9X/2000/XP/... (WIN32) */
#ifdef WIN32
#define CLEAR system("cls");
#endif

class gui_qt;
class task;
class ratio;
class primzahl;

/* ------ function prototypes ------ */

/* show the main menu for a text gui */
char show_main_txt();

/* ask for the other parameters needed to generate a task */
void ask_parameters_txt(short & nr_ratios, unsigned int & max_md);

/* generates the next task, asks the user for the result */
short run_task_txt(short add_sub, short mul_div,
						short nr_ratios, unsigned int max_md);

/* show some more information about the program */
void show_about_txt(void);

/* to show a very simple text interface for console users */
int gui_konsole(void);

/* to show a Qt GUI */
int gui_qt_ini(int argc, char * argv[]);

#endif /* ifndef BRUCH_H */
