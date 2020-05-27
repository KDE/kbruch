/***************************************************************************
                          appmenuwidget.h  -  The main Qt/KDE window
                             -------------------
    begin                : Tue Sep 16 00:00:00 CET 2008
    copyright            : (C) 2008 by Danilo Balzaque
    email                : danilo.balzaque@ltia.fc.unesp.br

    copyright            : (C) 2008 by Tadeu Araujo, tadeu.araujo@ltia.fc.unesp.br
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef APPMENUWIDGET_H
#define APPMENUWIDGET_H

#include <KXmlGuiWindow>

#include <QApplication>
#include <QHBoxLayout>

#include "MainQtWidget.h"
#include "FractionRingWidget.h"

class QHBoxLayout;
class QPushButton;
class MainQtWidget;
class FractionRingWidget;
class QAction;

/** Constructs the main window and presents the user interface.
 *  The window give the user the ability to choose what application
 *  he want to open first.
 *  \author Danilo Balzaque
 **/
class AppMenuWidget : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /** constructor */
    AppMenuWidget();

    /** destructor */
    ~AppMenuWidget();

private:
    QWidget * baseWidget;
    QWidget * interfaceWidget;

    QHBoxLayout * layout1;
    QGridLayout * gridLayout;

    QPushButton * m_Freestyle;
    QLabel * labelFreestyle;
    QPushButton * m_Learning;
    QLabel * labelLearning;
    QLabel * labelInfo;

    MainQtWidget * kbruchApp;
    FractionRingWidget * fractionRing;

    void setupActions();

    void slotFreestyleClicked();
    void slotLearningClicked();
    /**
    * makes sure, all parts of the UI update to new settings
    */
    void slotApplySettings();
};

#endif // AppMenuWidget
