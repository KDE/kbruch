/*
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tiago Porangaba <tiago.porangaba@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef FRACTIONRINGWIDGET_H
#define FRACTIONRINGWIDGET_H

#include <KXmlGuiWindow>

#include "AppMenuWidget.h"
#include "Ratio.h"
#include "FractionPainter.h"

class QAction;
class QGridLayout;
class AppMenuWidget;
class QColor;
class QLabel;
class FractionPainter;
class QSpinBox;
class QPushButton;
class QLabel;
class QTextEdit;
class QAction;
class QWidgetAction;


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
    void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;

    void resetFraction(bool update);
    bool checkTask();
    int MCM(int a, int b);
    int GCF(int a, int b);

private:
    QWidget * baseWidget;
    QWidget * interfaceWidget;

    QGridLayout * layout1;
    QGridLayout * gridLayout;
    QSpinBox * leftSpinBox;
    QSpinBox * rightSpinBox;
    QPushButton * resetButton;
    QTextEdit * textMsg;
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

    QWidgetAction * m_NewTaskAction;
    QWidgetAction * m_BackAction;
    QAction * m_HintAction;

    Ratio rLeft;
    Ratio rRight;

    int numLeft;
    int denLeft;
    int multLeft;

    int numRight;
    int denRight;
    int multRight;

    void setupActions();

private Q_SLOTS:
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
