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

#include "PrimeFactorsLineEdit.h"
#include "PrimeFactorsLineEdit.moc"

/* constructor */
PrimeFactorsLineEdit::PrimeFactorsLineEdit(QWidget * parent)
    : KLineEdit(parent)
{
#ifdef DEBUG
    kDebug() << "constructor PrimeFactorsLineEdit";
#endif
    m_allowedChars = "0123456789x";
}

/* destructor */
PrimeFactorsLineEdit::~PrimeFactorsLineEdit()
{
#ifdef DEBUG
    kDebug() << "destructor PrimeFactorsLineEdit";
#endif
}

/** ignore all invalid characters */
void PrimeFactorsLineEdit::keyPressEvent(QKeyEvent * e)
{
    // forward special chars to KLineEdit
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return || e->key() ==
            Qt::Key_Delete || e->key() == Qt::Key_Backspace || (e->modifiers()
                & (Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier |
                    Qt::GroupSwitchModifier)))
    {
        KLineEdit::keyPressEvent(e);
        return;
    }

    // first ignore all chars, which are not numeric or multiply sign "*"
    if (! m_allowedChars.contains(e->text())) {
        return;
    }

    // don't allow 2 multiply signs next to each other
    const QString currentText = text();
    if (e->text() == "x" && currentText.right(1) == "x") {
        return;
    } 

    // valid char, process it as usual
    KLineEdit::keyPressEvent(e);
}

/** react on change input method events */
void PrimeFactorsLineEdit::inputMethodEvent(QInputMethodEvent * e)
{
    const QString str = e->commitString();
    
    Q_FOREACH(QChar ch, str) {
        if (! m_allowedChars.contains(ch)) {
            return;
        }
    }

    KLineEdit::inputMethodEvent(e);
}
