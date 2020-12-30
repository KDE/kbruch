/*
    KBruch - line edit used for entering prime factors
    SPDX-FileCopyrightText: 2011 Sebastian Stein <seb.kde@hpfsc.de>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    explicit PrimeFactorsLineEdit(QWidget* parent = nullptr);

    /** destructor */
    ~PrimeFactorsLineEdit();

    /** return the factors entered */
    QStringList getFactors() const;

    /** called when the text in QLineEdit's content changes */
    void textHasChanged(const QString &text);

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
    void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
};

#endif // PRIMEFACTORSLINEEDIT_H
