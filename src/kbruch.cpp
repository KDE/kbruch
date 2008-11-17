/***************************************************************************
                          kbruch.cpp  -  Main File of KBruch
                             -------------------
    begin                : Tue Nov 27 16:40:42 CET 2001
    copyright            : (C) 2001 by Sebastian Stein
    email                : seb.kde@hpfsc.de

    copyright            : (C) 2008 by Tiago Porangaba, tiago.porangaba@ltia.fc.unesp.br
    copyright            : (C) 2008 by Tadeu Araujo, tadeu.araujo@ltia.fc.unesp.br
    copyright            : (C) 2008 by Danilo Balzaque, danilo.balzaque@ltia.fc.unesp.br
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

#include "kbruch_version.h"

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
	KAboutData aboutData( "kbruch", 0,
		ki18n(description),
		KBRUCH_VERSION,
		ki18n("Learn calculating with fractions"),
		KAboutData::License_GPL,
    	ki18n("(c) 2002-2004, Sebastian Stein"), KLocalizedString(), "http://edu.kde.org/kbruch/",
		"seb.kde@hpfsc.de");
	aboutData.addAuthor(ki18n("Sebastian Stein"),ki18n("Original Author"),"seb.kde@hpfsc.de");
	aboutData.addAuthor(ki18n("Paulo Cattai"),ki18n("New interface design and usability improvements"), "paulo.cattai@ltia.fc.unesp.br");
	aboutData.addAuthor(ki18n("Danilo Balzaque"),ki18n("New interface design and usability improvements"), "danilo.balzaque@ltia.fc.unesp.br");
	aboutData.addAuthor(ki18n("Roberto Cunha"),ki18n("New interface design and usability improvements"), "roberto.cunha@ltia.fc.unesp.br");
	aboutData.addAuthor(ki18n("Tadeu Araujo"),ki18n("New interface design and usability improvements"), "tadeu.araujo@ltia.fc.unesp.br");
	aboutData.addAuthor(ki18n("Tiago Porangaba"),ki18n("New interface design and usability improvements"), "tiago.porangaba@ltia.fc.unesp.br");

 	KCmdLineArgs::init( argc, argv, &aboutData );

	KApplication kbruch;
	QObject::connect(kapp, SIGNAL(lastWindowClosed()), kapp, SLOT(quit()));

	AppMenuWidget * kbruchApp = new AppMenuWidget();
	kbruchApp->show();

	/* lets execute the Qt GUI; it will handle all cmd options which are left */
	return kbruch.exec();
}
