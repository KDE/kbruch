/***************************************************************************
                          primzahl.h  -  class primzahl
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

#ifndef PRIMZAHL_H
#define PRIMZAHL_H

/*#define DEBUG*/

#include <iostream.h>
#include <vector>

typedef std::vector<unsigned int> UnsignedIntArray;

/* class primzahl, handling prime numbers */
class primzahl
{
public:
	primzahl();
	~primzahl();
	short isPrimeNumber(unsigned int number);
	unsigned int get_next();
	unsigned int get_first();
	unsigned int get_last();
	unsigned int get_current();
	void move_first();
	void move_last();
	void move_forward();
	void move_back();
	void display_all();
private:
	/* a vector storing all known prime numbers, access for all objects;
	 * we are using the vector<T> template; so we do not have to think
	 * about dynamic mem manipulation */
	static UnsignedIntArray prim_vector;

	/* current selected prime number */
	UnsignedIntArray::iterator current_pos;

	/* finds next prime number and adds it to the vector */
	void find_next();
};
#endif
