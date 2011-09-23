/***************************************************************************
                              fractionringwidget.h
                             -------------------
    begin                : 2008/09/16
    copyright            : (C) 2008 by Danilo Balzaque
    email                : danilo.balzaque@ltia.fc.unesp.br

    copyright            : (C) 2008 by Tadeu Araujo, tadeu.araujo@ltia.fc.unesp.br
    copyright            : (C) 2008 by Tiago Porangaba, tiago.porangaba@ltia.fc.unesp.br
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FRACTIONRINGWIDGET_H
#define FRACTIONRINGWIDGET_H

#define DEBUG

#ifdef DEBUG
#include <kdebug.h>
#endif

#include <kxmlguiwindow.h>
#include <kapplication.h>
#include <KTextEdit>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QColor>
#include <QPushButton>
#include <QLabel>

#include "AppMenuWidget.h"
#include "ratio.h"
#include "FractionPainter.h"

class KAction;
class QGridLayout;
class QHBoxLayout;
class AppMenuWidget;
class QColor;
class QLabel;
class FractionPainter;
class KIntSpinBox;
class QPushButton;
class QLabel;
class KTextEdit;
class KAction;


/*! Constructs a QWidget bla bla bla
 *  \author Danilo Balzaque
 * */
class FractionRingWidget : public KXmlGuiWindow
{
    Q_OBJECT

public:
    /** constructor */
    FractionRingWidget();

    /** destructor */
    ~FractionRingWidget();

protected:
    /** Function is called every time the screen need to be painted.
     **/
    void paintEvent(QPaintEvent * event);

    void resetFraction(bool update);
    bool checkTask();
    int MCM(int a, int b);
    int GCF(int a, int b);

private:
    QWidget * baseWidget;
    QWidget * interfaceWidget;

    QGridLayout * layout1;
    QGridLayout * gridLayout;
    KIntSpinBox * leftSpinBox;
    KIntSpinBox * rightSpinBox;
    QPushButton * resetButton;
    KTextEdit * textMsg;
    QLabel * leftInfoLabel;
    QLabel * rightInfoLabel;

    FractionPainter * fractionWidget;

    AppMenuWidget * kbruchApp;

    QColor bgOutsideRing;
    QColor bgOutsideMold;
    QColor bgInsideRing;
    QColor bgInsideMold;
    QColor bgColor;
    QColor colorListLeft[5];
    QColor colorListRight[5];

    KAction * m_NewTaskAction;
    KAction * m_BackAction;
    KAction * m_HintAction;

    ratio rLeft;
    ratio rRight;

    int numLeft;
    int denLeft;
    int multLeft;

    int numRight;
    int denRight;
    int multRight;

    void setupActions();

private slots:
    void slotLeftSpinBoxValueChanged(int value);
    void slotRightSpinBoxValueChanged(int value);
    void NewTask();
    void Hint();
    void GoBack();
    /**
    * calls the settings dialog
    */
    void slotPrefs();
    /**
    * makes sure, all parts of the UI update to new settings
    */
    void slotApplySettings();
};

#endif
