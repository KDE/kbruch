/***************************************************************************
                              fractionringwidget.cpp
                             -------------------
    begin                : 2009/08/10
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

#ifndef TASKCOLORS_H
#define TASKCOLORS_H

#include "ui_taskcolorsbase.h"

class TaskColors : public QWidget, public Ui::TaskColorsBase
{
public:
    TaskColors (QWidget * parent) : QWidget (parent) {
        setupUi (this);
    }
};

#endif
