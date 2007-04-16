/***************************************************************************
                          kbruch.cpp  -  Main File of KBruch
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

#include "kbruch.h"

#include "version.h"

/* standard C++ library includes */
#include <time.h>

/* these includes are needed for Qt support */
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kapplication.h>

/* the main program */
int main(int argc, char * argv[])
{
	// init random generator
	srand(time(NULL));

	/* fill the about data; the common KDE about dialog will show it to the
	 * user */
	KAboutData aboutData( "kbruch",
		description,
		KBRUCH_VERSION,
		I18N_NOOP("Learn calculating with fractions"),
		KAboutData::License_GPL,
    	"(c) 2002-2004, Sebastian Stein", 0, "http://edu.kde.org/kbruch/",
		"seb.kde@hpfsc.de");
	aboutData.addAuthor("Sebastian Stein",0, "seb.kde@hpfsc.de");
 	KCmdLineArgs::init( argc, argv, &aboutData );

	KApplication kbruch;
	QObject::connect(kapp, SIGNAL(lastWindowClosed()), kapp, SLOT(quit()));

	MainQtWidget * kbruchApp = new MainQtWidget();
	kbruchApp->show();

	/* lets execute the Qt GUI; it will handle all cmd options which are left */
	return kbruch.exec();
}
