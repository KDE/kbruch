/*
    Ratio.h  -  source code of class Ratio
    SPDX-FileCopyrightText: 2001-2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef RATIO_H
#define RATIO_H

#include <QTextStream>

/** Represents a ratio
 *  This class represents 1 ratio. There are several functions provided to
 *  modify the numerator and denominator. It is also possible to calculate with
 *  objects of the class ratio. Overloaded operation functions are provided for
 *  this task.
 *  \author Sebastian Stein */
class Ratio
{
public:
    /** constructor */
    explicit Ratio(int pnumerator = 0, int pdenominator = 1);

    /** constructor with reduce option */
    Ratio(int pnumerator, int pdenominator, bool reduce_fraction);

    /** copy constructor */
    Ratio(const Ratio & copy_ratio);

    /** destructor */
    ~Ratio();

    /** returns the ratio as QTextStream object */
    QTextStream & display(QTextStream & str) const;

    /** returns the numerator */
    int numerator() const;

    /** returns the denominator */
    int denominator() const;

    /** set numerator and reduce the ratio */
    void setNumerator(int pnumerator = 0, bool reduce = true);

    /** set denominator and reduce the ratio */
    void setDenominator(int pdenominator = 1, bool reduce = true);

    /** set both numerator and denominator;
     *  reducing will be done after numerator and denominator were set!
     */
    void setRatio(int pnumerator, int pdenominator, bool reduce = true);

    /** set new ratio using mixed numbers;
     *  reducing will be done after numerator and denominator were set!
     */
    void setRatio(int pinteger, int pnumerator, int pdenominator, bool reduce = true);

    /** operator overloading for: c = object + summand */
    Ratio operator+ (const Ratio &addend);

    /** operator overloading for: c = object - subtrahend */
    Ratio operator- (Ratio subtrahend);

    /** operator overloading for: c = object * factor */
    Ratio operator*(const Ratio &factor);

    /** operator overloading for: c = object / divisor */
    Ratio operator/ (Ratio divisor);

    /** set numerator with dummy and denominator = 1 */
    Ratio operator= (int dummy);

    /** compares the current ratio with a given one */
    bool operator== (const Ratio &right);

    /** compares the current ratio with a given one */
    bool operator< (const Ratio &right);

    /** compares the current ratio with a given one */
    bool operator> (const Ratio &right);

    /** exchange numerator and denominator */
    void reziproc();

    /** reduce the ratio */
    void reduce();

private:
    /** numerator */
    int m_numerator;

    /** denominator */
    int m_denominator;

    /** change sign of the ratio */
    void change_sign();
}
;

/* ------ some prototyps of non class functions ------ */

/** it is possible to code: cout << ratio_object << endl; */
QTextStream & operator<< (QTextStream & str, const Ratio & pratio);

#endif
