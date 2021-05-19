/*
    KBruch - line edit used for entering prime factors
    SPDX-FileCopyrightText: 2011 Sebastian Stein <seb.kde@hpfsc.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "PrimeFactorsLineEdit.h"

#ifdef DEBUG
#include <QDebug>
#endif

/* constructor */
PrimeFactorsLineEdit::PrimeFactorsLineEdit(QWidget * parent)
    : QLineEdit(parent)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor PrimeFactorsLineEdit");
#endif
    m_usedFactors << QStringLiteral("2") << QStringLiteral("3") << QStringLiteral("5") << QStringLiteral("7") << QStringLiteral("11") << QStringLiteral("13") << QStringLiteral("17") << QStringLiteral("19");
    // I need this so dead keys go trough keyPressEvent
    setAttribute(Qt::WA_InputMethodEnabled, false);
    connect(this, &PrimeFactorsLineEdit::textEdited, this, &PrimeFactorsLineEdit::textHasChanged);
}

/* destructor */
PrimeFactorsLineEdit::~PrimeFactorsLineEdit()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("destructor PrimeFactorsLineEdit");
#endif
}

void PrimeFactorsLineEdit::textHasChanged(const QString &text)
{
    QPalette palette;

    if (!checkCorrectness(text)) {
        palette.setColor(QPalette::Base, Qt::red);
        setPalette(palette);
        Q_EMIT contentIsRight(false);
    } else {
        palette.setColor(QPalette::Base, Qt::white);
        setPalette(palette);
        Q_EMIT contentIsRight(true);
    }
}

bool PrimeFactorsLineEdit::checkCorrectness(const QString& theText)
{
    QString auxStr = theText;
    QString noSpaces = auxStr.remove(QStringLiteral(" "));
    QStringList terms = noSpaces.split(QLatin1Char('x'));

    return (areFactors(terms) || text().isEmpty());
}

bool PrimeFactorsLineEdit::areFactors(const QStringList& factors)
{
    m_theFactors.clear();
    for (const QString & str : factors) {
        bool found = false;
        for (const QString & aux : qAsConst(m_usedFactors)) {
            if (str.compare(str, aux) == 0) {
                found = true;
                m_theFactors.append(str);
                break;
            }
        }
        if (!found) {
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
    QString allowedChars = QStringLiteral("123579xX*");
    QString allowedDigits = QStringLiteral("123579");
    QString symbols = QStringLiteral("xX*");
    bool backspaceKey = (event->key() == Qt::Key_Backspace);
    bool returnKey = (event->key() == Qt::Key_Return
                      || event->key() == Qt::Key_Enter);

    setCursorPosition(text().length());

    if (allowedChars.contains(event->text()) || backspaceKey || returnKey) {
        QString lastFactor;
        QString ch = QStringLiteral("#");
        QString factor;
        if (!event->text().isEmpty()) {
            ch = event->text().at(0);
        }
        if (!text().isEmpty()) {
            lastFactor = text().section(QLatin1Char('x'), -1);
            factor = lastFactor + ch;
        } else {
            factor = ch;
        }

        if (allowedChars.contains(ch) || backspaceKey || returnKey) {
            // turns '*' and 'X' into 'x' to avoid mixed symbols
            // and to make easier to split the text later
            if (!event->text().isEmpty() && (symbols.indexOf(ch) != -1) && (factor.compare(QLatin1String("x")) != 0) && !lastFactor.isEmpty() && lastFactor.compare(QLatin1String("1")) != 0) {
                QKeyEvent myKeyEvent(QKeyEvent::KeyPress, Qt::Key_X, Qt::NoModifier, QStringLiteral("x"), false, 0);
                QLineEdit::keyPressEvent(&myKeyEvent);
            }

            if ((m_usedFactors.indexOf(factor) == -1)
                    && (factor.compare(QStringLiteral("1")) != 0) && !backspaceKey &&
                    !returnKey) {
                return;
            }

            QLineEdit::keyPressEvent(event);
        }
    }
}
