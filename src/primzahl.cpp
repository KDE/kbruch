/***************************************************************************
                          primzahl.cpp  -  source code of class primzahl
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

#include "primzahl.h"

#include <iostream>
using std::cout;
using std::endl;

/* ----- the global prime number vector ----- */
UnsignedIntArray primzahl::prim_vector;

/* ----- public member functions ----- */

/*  constructor for class primzahl */
primzahl::primzahl()
{
	/* if the vector is empty, we will add the first 2 prime numbers */
	if (prim_vector.empty())
	{
#ifdef DEBUG
		cout << "prim_vector is still empty" << endl;
#endif

		prim_vector.push_back(2);
		prim_vector.push_back(3);
	}
	current_pos = prim_vector.begin();
#ifdef DEBUG

	cout << "constructor primzahl" << endl;
#endif
}

/* destructor for class primzahl */
primzahl::~primzahl()
{
#ifdef DEBUG
	cout << "destructor primzahl" << endl;
#endif
}

/* check, if the given number is a prime number;
 * return 0 if no, 1 if yes */
short primzahl::isPrimeNumber(unsigned int number)
{
	/* check, if we can find a divisor */
	for (unsigned int dummy = get_first(); dummy < number; dummy = get_next())
		if ((number % dummy == 0) && (dummy != number))
			return 0; // we found one

	return 1; // we found no one
}

/* returns next prime number */
unsigned int primzahl::get_next()
{
	/* if we do not know the next number, we have to find it first */
	if (current_pos == prim_vector.end() ||
	        ++current_pos == prim_vector.end())
	{
		/* we do not know the next prime number, so we have to find it */
		find_next();
		move_last();
		return get_last(); /* return it */
	}
	else
	{
		/* we know the next prime number, set the pointer on it */
		return *current_pos; /* return it */
	}
}

/* returns the first prime number of the vector */
unsigned int primzahl::get_first()
{
	return *prim_vector.begin();
}

/* returns the last prime number in the vector */
unsigned int primzahl::get_last()
{
	return *(prim_vector.end() - 1);
}

/* returns currently selected prime number */
unsigned int primzahl::get_current()
{
	if (current_pos == prim_vector.end() + 1)
		return get_last();
	return *current_pos;
}

/* set current_pos to the first prime number */
void primzahl::move_first()
{
	current_pos = prim_vector.begin();
	return;
}

/* set current_pos to the last prime number */
void primzahl::move_last()
{
	current_pos = prim_vector.end() - 1;
	return;
}

/* move to the next prime number */
void primzahl::move_forward()
{
	/* if we are at the end of prim_vector, we have to find a new number */
	if (current_pos == prim_vector.end() ||
	        ++current_pos == prim_vector.end())
	{
		find_next();
		move_last();
	}
	return;
}

/* move one prime number back */
void primzahl::move_back()
{
	/* current_pos must be at least pointing to the first element
	 * of our vector after this function */
	if (current_pos != prim_vector.begin())
		--current_pos;
	return;
}

/* displays the whole prim_vector on stdout; just for debugging */
void primzahl::display_all()
{
	unsigned int dummy = 0; // count the numbers

	/* looping through the complete vector */
	for (current_pos = prim_vector.begin(); current_pos != prim_vector.end();
	        current_pos++, dummy++)
		cout << dummy << ": " << *current_pos << endl;

	current_pos = prim_vector.end() - 1;
	return;
}

/* ----- private member functions ----- */

/* finds next prime number and adds it to the vector */
void primzahl::find_next()
{
	/* our new prime number, must be bigger then the last one */
	unsigned int new_prim = *(prim_vector.end() - 1);

	do
	{
		/* new prime number must be bigger as biggest known one */
		new_prim += 2;
		/* loop as long as we find a divisor for the new number */
		for (current_pos = prim_vector.begin(); current_pos != prim_vector.end();
		        current_pos++)
			if ((new_prim % *current_pos == 0) || (new_prim < *current_pos))
				break;

		/* if we tried all known numbers and found no divisor, well,
		 * we are happy to have found a new prime number */
		if (current_pos == prim_vector.end())
			break;
	}
	while(1);

	/* add the new prime number to the vector */
	prim_vector.push_back(new_prim);

	current_pos = prim_vector.end() - 1;
	return;
}
