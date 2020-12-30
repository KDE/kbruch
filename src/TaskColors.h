/*
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tiago Porangaba <tiago.porangaba@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TASKCOLORS_H
#define TASKCOLORS_H

#include "ui_taskcolorsbase.h"

class TaskColors : public QWidget, public Ui::TaskColorsBase
{
public:
    explicit TaskColors(QWidget * parent) : QWidget(parent) {
        setupUi(this);
    }
};

#endif
