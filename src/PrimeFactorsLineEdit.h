/*
    KBruch - line edit used for entering prime factors
    Copyright 2011 - Sebastian Stein <seb.kde@hpfsc.de>

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

#include <QLineEdit>
#include <QKeyEvent>

class QString;

/**
 * line edit used for entering prime factors
 * based on KRestrictedLine class
 *
 * \author Sebastian Stein
 */
class PrimeFactorsLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    /** constructor */
    explicit PrimeFactorsLineEdit(QWidget* parent = 0);

    /** destructor */
    ~PrimeFactorsLineEdit();

    /** return the factors entered */
    QStringList getFactors() const;

public Q_SLOTS:
    /** called when the text in QLineEdit's content changes */
    void textHasChanged(QString text);

Q_SIGNALS:
    /** emitted when QLineEdit's content changes and it's correct
        itIs is true if the content is correct otherwise.
     */
    void contentIsRight(bool itIs);

private:
    /** will content the factor the application will used:
      * 2, 3, 5, 7, 11, 13, 17, 19 */
    QStringList m_usedFactors;
    /** the factors entered in the QLineEdit */
    QStringList m_theFactors;

    /** will check if the QLineEdit's content is right */
    bool checkCorrectness(const QString &text);
    /** will check if every factors' element is actually a factor */
    bool areFactors(const QStringList &factors);

    /** reimplement keyPressEvent inherited from QWidget */
    void keyPressEvent(QKeyEvent * event);
};

#endif // PRIMEFACTORSLINEEDIT_H
