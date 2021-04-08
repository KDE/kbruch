/*
    appmenuwidget.h  -  The main Qt/KDE window

    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef APPMENUWIDGET_H
#define APPMENUWIDGET_H

#include <KXmlGuiWindow>

#include <QApplication>
#include <QHBoxLayout>

#include "MainQtWidget.h"
#include "FractionRingWidget.h"

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
