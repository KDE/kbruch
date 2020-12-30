/*
    Ratio.cpp  -  source code of class Ratio
    SPDX-FileCopyrightText: 2001-2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "Ratio.h"

#ifdef DEBUG
#include <QDebug>
#endif

#include "PrimeNumber.h"


/* ----- public member functions ----- */

/* constructor */
Ratio::Ratio(int pnumerator, int pdenominator) : m_numerator(pnumerator), m_denominator(pdenominator)
{
#ifdef DEBUG
    qDebug() << "constructor ratio";
#endif

    // denominator is never allowed to be 0
    if (!m_denominator)
        m_denominator = 1;

    // reduce the new ratio
    reduce();
}

Ratio::Ratio(int pnumerator, int pdenominator, bool reduce_fraction) : m_numerator(pnumerator), m_denominator(pdenominator)
{
#ifdef DEBUG
    qDebug() << "constructor ratio";
#endif

    // denominator is never allowed to be 0
    if (!m_denominator)
        m_denominator = 1;

    // reduce the new ratio
    if (reduce_fraction)
        reduce();
}

/* copy constructor */
Ratio::Ratio(const Ratio & copy_ratio)
{
#ifdef DEBUG
    qDebug() << "copy constructor ratio";
#endif
    setNumerator(copy_ratio.numerator(), false);
    setDenominator(copy_ratio.denominator(), false);
}

/* destructor */
Ratio::~Ratio()
{
#ifdef DEBUG
    qDebug() << "destructor ratio";
#endif
}

/* displays the ratio on stdout; just for debugging */
QTextStream & Ratio::display(QTextStream & str) const
{
    int tmp_width = str.fieldWidth();
    str << qSetFieldWidth(5) << " ";
    str << qSetFieldWidth(5) << m_numerator << QLatin1Char('\n');
    str << qSetFieldWidth(tmp_width) << " ";
    str << " ----- " << QLatin1Char('\n');
    str << qSetFieldWidth(tmp_width) << " ";
    str << qSetFieldWidth(5) << m_denominator;
    str.flush();
    return str;
}

/* return the numerator */
int Ratio::numerator() const
{
    return m_numerator;
}

/* return the denominator */
int Ratio::denominator() const
{
    return m_denominator;
}

/* set the numerator */
void Ratio::setNumerator(int pnumerator, bool reduce_it)
{
    m_numerator = pnumerator;

    // check, if we have to reduce the ratio
    if (reduce_it == true)
        reduce();

    return;
}

/* set the denominator */
void Ratio::setDenominator(int pdenominator, bool reduce_it)
{
    /* denominator is not allowed to be 0 */
    if (!pdenominator)
        pdenominator = 1;

    m_denominator = pdenominator;

    // check, if we have to reduce the ratio
    if (reduce_it == true)
        reduce();

    return;
}

/* set completely new ratio */
void Ratio::setRatio(int pnumerator, int pdenominator, bool reduce_it)
{
    setNumerator(pnumerator, false);
    setDenominator(pdenominator, false);

    // check, if we have to reduce the ratio
    if (reduce_it == true) {
        reduce();
    }

    return;
}

/* set completely new ratio using mixed numbers */
void Ratio::setRatio(int pinteger, int pnumerator, int pdenominator, bool reduce_it)
{
    // calculate new Numerator, but ignore negative values
    int newNumerator = qAbs(pinteger * pdenominator) + qAbs(pnumerator);

    // restore negative values
    if ((pinteger < 0 || pnumerator < 0) && !(pinteger < 0 && pnumerator < 0))
        newNumerator *= -1;

    setRatio(newNumerator, pdenominator, reduce_it);

    return;
}


/* add a ratio to a ratio like c = a + b */
Ratio Ratio::operator+ (const Ratio &addend)
{
    // this object will be returned as the sum
    Ratio sum(0, 1);

    // calculate and set the numerator without reducing
    sum.setNumerator(m_numerator * addend.denominator()
                     + addend.numerator() * m_denominator, false);

    // calculate and set the denominator without reducing
    sum.setDenominator(m_denominator * addend.denominator(), false);

    // reduce the sum
    sum.reduce();

    return sum;
}

/* sub a ratio from a ratio like c = a - b */
Ratio Ratio::operator- (Ratio subtrahend)
{
    /* this object will be returned as the difference */
    Ratio diff(0, 1);

    /* change the sign of the subtrahend, so we can handle it as an addition */
    subtrahend.change_sign();
    diff = operator+ (subtrahend);

    /* we have to change the sign back, so everything will be as before */
    subtrahend.change_sign();

    /* return the difference */
    return diff;
}

/* mul a ratio with a ratio like c = a * b */
Ratio Ratio::operator*(const Ratio &factor)
{
    // this object will be returned as the product
    Ratio product(0, 1);

    // calculate and set numerator and denominator without reducing
    product.setNumerator(m_numerator * factor.numerator(), false);
    product.setDenominator(m_denominator * factor.denominator(), false);

    // reduce the product
    product.reduce();

    return product;
}

/* div a ratio with a ratio like c = a / b */
Ratio Ratio::operator/ (Ratio divisor)
{
    /* this object will be returned as the quotient */
    Ratio quotient(0, 1);

    /* exchange numerator and denominator so we can handle as multiplication */
    divisor.reziproc();
    quotient = operator* (divisor);

    /* go back to the original state */
    divisor.reziproc();

    return quotient;
}

/* we need this for initialization during a function prototyp;
 * ratio fraction = 0 */
Ratio Ratio::operator= (int dummy)
{
    m_numerator = dummy;
    m_denominator = 1;

    return *this;
}

/* check, if the ratios are equivalent; -1/2 == 1/-2 -> true */
bool Ratio::operator== (const Ratio &right)
{
    signed short orig_sign = 1, right_sign = 1;

    /* we do not check the presign at this point */
    if (qAbs(m_numerator) != qAbs(right.numerator()))
        return false;
    if (qAbs(m_denominator) != qAbs(right.denominator()))
        return false;

    /* check if the signs of the ratios are equivalent */
    if (m_numerator < 0)
        orig_sign = -1;
    if (m_denominator < 0)
        orig_sign *= -1;
    if (right.numerator() < 0)
        right_sign = -1;
    if (right.denominator() < 0)
        right_sign *= -1;

    if (orig_sign != right_sign)
        return false;

    return true;
}

bool Ratio::operator< (const Ratio &right)
{
    signed short sign = 1;
    Ratio tmp_ratio = Ratio(m_numerator, m_denominator) - right;

    // check for this == right
    if (tmp_ratio == Ratio(0, 1))
        return false;

    // get the presign of the diff
    if (tmp_ratio.numerator() < 0)
        sign = -1;
    if (tmp_ratio.denominator() < 0)
        sign *= -1;

    // if the diff is negative, this is smaller than right
    if (sign > 0) {
        return false;
    } else {
        return true;
    }
}

bool Ratio::operator> (const Ratio &right)
{
    signed short sign = 1;
    Ratio tmp_ratio = Ratio(m_numerator, m_denominator) - right;

    // check for this == right
    if (tmp_ratio == Ratio(0, 1))
        return false;

    // get the presign of the diff
    if (tmp_ratio.numerator() < 0)
        sign = -1;
    if (tmp_ratio.denominator() < 0)
        sign *= -1;

    // if the diff is positive, this is smaller than right
    if (sign < 0) {
        return false;
    } else {
        return true;
    }
}

/* ----- private member functions ----- */

/* reduce the ratio */
void Ratio::reduce()
{
    /* we try prime numbers as divisors; I think it is the fastest way to do */
    PrimeNumber number;
    short sign_numerator = 0, sign_denominator = 0;

    /* make the whole ratio positive; save the signs; it is easier to reduce
     * the ratio, if it is positive */
    if (m_numerator < 0) { // save numerator sign
        sign_numerator = 1;
        m_numerator *= -1;
    }
    if (m_denominator < 0) { // save denominator sign
        sign_denominator = 1;
        m_denominator *= -1;
    }

    for (int divisor = number.get_first();
            divisor <= m_numerator && divisor <= m_denominator; divisor = number.get_next()) {
        if (divisor == 0) {
#ifdef DEBUG
            qDebug() << "ratio::reduce() -> divisor == 0 !!!";
            qDebug() << "m_numerator: " << m_numerator;
            qDebug() << "m_denominator: " << m_denominator;
            // cin.get();
#endif
            /* so that the application does not crash with a floating
             * point exception; the error should not appear, but in some
             * cases it does and I do not know why */
            continue;
        }

        /* is the prime number a divisor of numerator and denominator? */
        if ((m_numerator % divisor == 0) && (m_denominator % divisor == 0)) {
            /* reduce the ratio by the divisor */
            m_numerator /= divisor;
            m_denominator /= divisor;

            /* we have to go recursive, if the 2 is a divisor, because there
             * is no way to step one number before 2 -> there is no prime
             * number smaller than 2 */
            if (divisor == 2)
                reduce();
            else
                number.move_back(); // the prime number could be a divisor again
        } // if ((zaehler % divisor == 0) && (nenner % divisor == 0))
    } // for (unsigned int divisor = number.get_first(); ...

    /* restore the correct signs */
    if (sign_numerator)
        m_numerator *= -1;
    if (sign_denominator)
        m_denominator *= -1;
    if (m_numerator == 0)
        m_denominator = 1;

    return;
}

/* exchange numerator and denominator */
void Ratio::reziproc()
{
    int temp = m_numerator;
    m_numerator = m_denominator;
    m_denominator = temp;

    return;
}

/* ------ private member functions ------ */

/* change the sign of the ratio; ratio = ratio * -1 */
void Ratio::change_sign()
{
    /* this would be enough to change the sign of the ratio */
    m_numerator *= -1;

    /* if numerator and denominator both are negative, make them positive;
     * if denominator is negative and numerator positive, exchange the sign */
    if ((m_numerator < 0 && m_denominator < 0) || (m_numerator > 0 && m_denominator < 0)) {
        m_numerator *= -1;
        m_denominator *= -1;
    }

    return;
}


/* ------ some prototyps of non class functions ------ */

// it is possible to stram ratio_object
QTextStream & operator<< (QTextStream & str, const Ratio & pratio)
{
    return pratio.display(str);
}
