/***************************************************************************
                          tool.cpp  -  a small tool libary
                             -------------------
    begin                : Tue Feb 07 21:01:00 CET 2002
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

#include "tool.h"

/* computes basis high exponent: base^exponent = result; */
unsigned int high(unsigned short base, unsigned short exponent)
{
	unsigned int result = 1;
	
	for (unsigned short counter = 0; counter < exponent; counter++)
		result *= base;

	return result;
}
