/***************************************************************************
                          ratio.cpp  -  source code of class ratio
                             -------------------
    begin                : Tue Nov 27 16:40:42 CET 2001
    copyright            : (C) 2001 by Sebastian Stein
    email                : seb.stein@hpfsc.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kdebug.h>

#include "ratio.h"
#include "primzahl.h"

/* ----- public member functions ----- */

/* constructor */
ratio::ratio(int pzaehler, int pnenner):zaehler(pzaehler), nenner(pnenner)
{
	/* denominator is never allowed to be 0 */
	if (!nenner)
		nenner = 1;
	reduce(); // kuerzen
#ifdef DEBUG
	kdDebug() << "constructor ratio" << endl;
#endif
}

/* destructor */
ratio::~ratio()
{
#ifdef DEBUG
	kdDebug() << "destructor ratio" << endl;
#endif
}

/* displays the ratio on stdout; just for debugging */
QTextStream & ratio::display(QTextStream & str) const
{
	int weite = str.width();
	str << qSetW(5) << " ";
	str << qSetW(5) << zaehler << endl;
	str << qSetW(weite) << " ";
	str << " ----- " << endl;
	str << qSetW(weite) << " ";
	return str << qSetW(5) << nenner;
}

/* return the numerator */
int ratio::get_zaehler()
{
	return zaehler;
}

/* return the denominator */
int ratio::get_nenner()
{
	return nenner;
}

/* set the numerator */
void ratio::set_zaehler(int pzaehler)
{
	zaehler = pzaehler;
	reduce();
}

/* set the denominator */
void ratio::set_nenner(int pnenner)
{
	/* denominator is not allowed to be 0 */
	if (!pnenner)
		pnenner = 1;

	nenner = pnenner;
	reduce();
}

/* set the numerator without reducing */
void ratio::set_zaehler_ohne_k(int pzaehler)
{
	zaehler = pzaehler;
}

/* set the denominator without reducing */
void ratio::set_nenner_ohne_k(int pnenner)
{
	/* denominator is not allowed to be 0 */
	if (!pnenner)
		pnenner = 1;

	nenner = pnenner;
}

/* add a ratio to a ratio like c = a + b */
ratio ratio::operator+(ratio summand)
{
	/* this object will be returned as the sum */
	ratio summe(0, 0);

	/* calculate the numerator */
	summe.set_zaehler(zaehler * summand.get_nenner()
	                  + summand.get_zaehler() * nenner);

	/* calculate the denominator */
	summe.set_nenner(nenner * summand.get_nenner());

	return summe;
}

/* sub a ratio from a ratio like c = a - b */
ratio ratio::operator-(ratio subtrahend)
{
	/* this object will be returned as the difference */
	ratio diff(0, 1);

	/* change the sign of the subtrahend, so we can handle it as an addition */
	subtrahend.change_sign();
	diff = operator+(subtrahend);

	/* we have to change the sign back, so everything will be as before */
	subtrahend.change_sign();

	/* return the difference */
	return diff;
}

/* mul a ratio with a ratio like c = a * b */
ratio ratio::operator*(ratio factor)
{
	/* this object will be returned as the product */
	ratio product(0, 1);

	/* calculate numerator and denominator */
	product.set_zaehler(zaehler * factor.get_zaehler());
	product.set_nenner(nenner * factor.get_nenner());

	return product;
}

/* div a ratio with a ratio like c = a / b */
ratio ratio::operator/(ratio divisor)
{
	/* this object will be returned as the quotient */
	ratio quotient(0, 1);

	/* exchange numerator and denominator so we can handle as multiplication */
	divisor.reziproc();
	quotient = operator*(divisor);

	/* go back to the original state */
	divisor.reziproc();

	return quotient;
}

/* we need this for initialization during a function prototyp;
 * ratio bruch = 0 */
ratio ratio::operator=(int dummy)
{
	zaehler = dummy;
	nenner = 1;
	return *this;
}

/* check, if the ratios are equivalent; -1/2 == 1/-2 -> TRUE */
bool ratio::operator==(ratio right)
{
	signed short orig_sign = 1, right_sign = 1;

	/* we do not the presign at this point */
	if (abs(zaehler) != abs(right.get_zaehler()))
		return false;
	if (abs(nenner) != abs(right.get_nenner()))
		return false;

	/* check if the signs of the ratios are equivalent */
	if (zaehler < 0)
		orig_sign = -1;
	if (nenner < 0)
		orig_sign *= -1;
	if (right.get_zaehler() < 0)
		right_sign = -1;
	if (right.get_nenner() < 0)
		right_sign *= -1;

	if (orig_sign != right_sign)
		return false;

	return true;
}

/* ----- private member functions ----- */

/* reduce the ratio */
void ratio::reduce()
{
	/* we try prime numbers as divisors; I think it is the fastet way to do */
	primzahl number;
	short sign_zaehler = 0, sign_nenner = 0;

	/* make the whole ratio positive; save the signs; it is easier to reduce
	 * the ratio, if it is positive */
	if (zaehler < 0) // save numerator sign
	{
		sign_zaehler = 1;
		zaehler *= -1;
	}
	if (nenner < 0) // save denominator sign
	{
		sign_nenner = 1;
		nenner *= -1;
	}

	for (int divisor = number.get_first();
	        divisor <= zaehler && divisor <= nenner; divisor = number.get_next())
	{
		if (divisor == 0)
		{
#ifdef DEBUG
			kdDebug() << "ratio::reduce() -> divisor == 0 !!!" << endl;
			kdDebug() << "zaehler: " << zaehler << endl;
			kdDebug() << "nenner: " << nenner << endl;
			// cin.get();
#endif
			/* so that the application does not crash with a floating
			 * point exception; the error should not appear, but in some
			 * cases it does and I do not know why */
			continue;
		}

		/* is the prime number a divisor of numerator and denominator? */
		if ((zaehler % divisor == 0) && (nenner % divisor == 0))
		{
			/* reduce the ratio by the divisor */
			zaehler /= divisor;
			nenner /= divisor;
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
	if (sign_zaehler)
		zaehler *= -1;
	if (sign_nenner)
		nenner *= -1;
	if (zaehler == 0)
		nenner = 1;
}

/* change the sign of the ratio; ratio = ratio * -1 */
void ratio::change_sign()
{
	/* this would be enough to change the sign of the ratio */
	zaehler *= -1;

	/* if numerator and denominator both are negative, make them positive;
	 * if denominator is negative and numerator positive, exchange the sign */
	if ((zaehler < 0 && nenner < 0) || (zaehler > 0 && nenner < 0))
	{
		zaehler *= -1;
		nenner *= -1;
	}
}

/* exchange numerator and denominator */
void ratio::reziproc()
{
	int temp = zaehler;
	zaehler = nenner;
	nenner = temp;
}


/* ------ some prototyps of non class functions ------ */

// it is possible to stram ratio_object
QTextStream & operator<<(QTextStream & str, const ratio & pratio)
{
	return pratio.display(str);
}
