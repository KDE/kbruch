/*
    KBruch - line edit used for entering prime factors
    Copyright (C) 2011 - Sebastian Stein

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef PRIMEFACTORSLINEEDIT_H
#define PRIMEFACTORSLINEEDIT_H

#ifdef DEBUG
#include <kdebug.h>
#endif

#include "klineedit.h"

#include <QKeyEvent>

class QString;

/**
 * line edit used for entering prime factors
 * based on KRestrictedLine class
 *
 * \author Sebastian Stein
 */
class PrimeFactorsLineEdit : public KLineEdit
{
    Q_OBJECT

public:
    /** constructor */
    explicit PrimeFactorsLineEdit (QWidget* parent = 0);

    /** destructor */
    ~PrimeFactorsLineEdit();

protected:
    /** override key press event */
    void keyPressEvent (QKeyEvent * e);

    /** override input method change event */
    void inputMethodEvent (QInputMethodEvent * e);

private:
    QString m_allowedChars;
};

#endif // PRIMEFACTORSLINEEDIT_H
