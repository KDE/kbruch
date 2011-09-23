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
    m_usedFactors << "2" << "3" << "5" << "7" << "11" << "13" << "17" << "19";
    // I need this so dead keys go trough keyPressEvent
    setAttribute(Qt::WA_InputMethodEnabled, false);
    connect(this, SIGNAL(textEdited(QString)), this, SLOT(textHasChanged(QString)));
}

/* destructor */
PrimeFactorsLineEdit::~PrimeFactorsLineEdit()
{
#ifdef DEBUG
    kDebug() << "destructor PrimeFactorsLineEdit";
#endif
}

void PrimeFactorsLineEdit::textHasChanged(QString text)
{
    QPalette palette;

    if(!checkCorrectness(text)) {
        palette.setColor(QPalette::Base, Qt::red);
        setPalette(palette);
        emit contentIsRight(false);
    } else {
        palette.setColor(QPalette::Base, Qt::white);
        setPalette(palette);
        emit contentIsRight(true);
    }
}

bool PrimeFactorsLineEdit::checkCorrectness(const QString& theText)
{
    QString auxStr = theText;
    QString noSpaces = auxStr.remove(' ');
    QStringList terms = noSpaces.split('x');

    return (areFactors(terms) || text().isEmpty());
}

bool PrimeFactorsLineEdit::areFactors(const QStringList& factors)
{
    m_theFactors.clear();
    foreach(QString str, factors) {
        bool found = false;
        foreach(QString aux, m_usedFactors) {
            if(str.compare(str, aux) == 0) {
                found = true;
                m_theFactors.append(str);
                break;
            }
        }
        if(!found) {
            return false;
        }
    }

    return true;
}

QStringList PrimeFactorsLineEdit::getFactors() const
{
    return m_theFactors;
}

void PrimeFactorsLineEdit::keyPressEvent(QKeyEvent * event)
{
    QString allowedChars = "123579xX*";
    QString allowedDigits = "123579";
    QString symbols = "xX*";
    bool backspaceKey = (event->key() == Qt::Key_Backspace);
    bool returnKey = (event->key() == Qt::Key_Return
                      || event->key() == Qt::Key_Enter);

    setCursorPosition(text().length());

    if(allowedChars.contains(event->text()) || backspaceKey || returnKey) {
        QString lastFactor;
        QChar ch = '#';
        QString factor = "";
        if(!event->text().isEmpty()) {
            ch = event->text().at(0);
        }
        if(!text().isEmpty()) {
            lastFactor = text().section('x', -1);
            factor = lastFactor + ch;
        } else {
            factor = ch;
        }

        if(allowedChars.contains(ch) || backspaceKey || returnKey) {
            // turns '*' and 'X' into 'x' to avoid mixed symbols
            // and to make easier to split the text later
            if(!event->text().isEmpty() && (symbols.indexOf(ch) != -1) && (factor.compare("x") != 0) && !lastFactor.isEmpty() && lastFactor.compare("1") != 0) {
                QKeyEvent myKeyEvent(QKeyEvent::KeyPress, Qt::Key_X, Qt::NoModifier, "x", false, 0);
                KLineEdit::keyPressEvent(&myKeyEvent);
            }

            if((m_usedFactors.indexOf(factor) == -1)
                    && (factor.compare("1") != 0) && !backspaceKey &&
                    !returnKey) {
                return;
            }

            KLineEdit::keyPressEvent(event);
        }
    }
}
