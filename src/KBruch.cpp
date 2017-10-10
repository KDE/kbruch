/***************************************************************************
                          KBruch.cpp  -  Main File of KBruch
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

#include "KBruch.h"

#include "KBruchVersion.h"

/* standard C++ library includes */
#include <ctime>

/* these includes are needed for KDE support */
#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>

/* these includes are needed for Qt support */
#include <Kdelibs4ConfigMigrator>
#include <QApplication>
#include <QCommandLineParser>
#include <QObject>

/* the main program */
int main(int argc, char * argv[])
{
    // init random generator
    srand(time(NULL));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    KLocalizedString::setApplicationDomain("kbruch");

    Kdelibs4ConfigMigrator migrate(QStringLiteral("kbruch"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kbruchrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("AppMenuWidgetui.rc") <<  QStringLiteral("FractionRingWidgetui.rc") << QStringLiteral("kbruchui.rc"));
    migrate.migrate();


    /* fill the about data; the common KDE about dialog will show it to the
     * user */
    KAboutData aboutData(QStringLiteral("kbruch"), i18n("KBruch"),
                         QStringLiteral(KBRUCH_VERSION),
                         i18n("Learn calculating with fractions"),
                         KAboutLicense::GPL,
                         i18n("© 2002-2011, Sebastian Stein"));
    aboutData.addAuthor(i18n("Sebastian Stein"), i18n("Current maintainer, original author"), QStringLiteral("seb.kde@hpfsc.de"));
    aboutData.addAuthor(i18n("Paulo Cattai"), i18n("New interface design and usability improvements"), QStringLiteral("paulo.cattai@ltia.fc.unesp.br"));
    aboutData.addAuthor(i18n("Danilo Balzaque"), i18n("New interface design and usability improvements"), QStringLiteral("danilo.balzaque@ltia.fc.unesp.br"));
    aboutData.addAuthor(i18n("Roberto Cunha"), i18n("New interface design and usability improvements"), QStringLiteral("roberto.cunha@ltia.fc.unesp.br"));
    aboutData.addAuthor(i18n("Tadeu Araujo"), i18n("New interface design and usability improvements"), QStringLiteral("tadeu.araujo@ltia.fc.unesp.br"));
    aboutData.addAuthor(i18n("Tiago Porangaba"), i18n("New interface design and usability improvements"), QStringLiteral("tiago.porangaba@ltia.fc.unesp.br"));
    aboutData.setHomepage(QStringLiteral("http://edu.kde.org/kbruch/"));

    KCrash::initialize();

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    QObject::connect(&app, &QApplication::lastWindowClosed, &app, &QApplication::quit);

    AppMenuWidget * kbruchApp = new AppMenuWidget();
    kbruchApp->show();

    /* lets execute the Qt GUI; it will handle all cmd options which are left */
    return app.exec();
}
