/***************************************************************************
                          ratio.h  -  class ratio
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

#ifndef RATIO_H
#define RATIO_H

#include <qtextstream.h>

/** Represents a ratio
 *  This class represents 1 ratio. There are several functions provided to
 *  modify the numerator and denominator. It is also possible to calculate with
 *  objects of the class ratio. Overloaded operation functions are provided for
 *  this task.
 *  \author Sebastian Stein */
class ratio
{
public:
	/** constructor */
	ratio(int pzaehler = 0, int pnenner = 1);

	/** destructor */
	~ratio();

	/** returns the ratio as QTextStream object */
	QTextStream & display(QTextStream & str) const;

	/** returns the numerator */
	int get_zaehler();

	/** returns the denominator */
	int get_nenner();

	/** set numerator and reduce the ratio */
	void set_zaehler(int pzaehler = 0);

	/** set denominator and reduce the ratio */
	void set_nenner(int pnenner = 1);

	/** set numerator without reducing the ratio */
	void set_zaehler_ohne_k(int pzaehler = 0);

	/** set denominator without reducing the ratio */
	void set_nenner_ohne_k(int pnenner = 1);

	/** operator overloading for: c = object + summand */
	ratio operator+(ratio summand);

	/** operator overloading for: c = object - subtrahend */
	ratio operator-(ratio subtrahend);

	/** operator overloading for: c = object * factor */
	ratio operator*(ratio factor);

	/** operator overloading for: c = object / divisor */
	ratio operator/(ratio divisor);

	/** set numerator with dummy and denominator = 1 */
	ratio operator=(int dummy);

	/** compares the current ratio with a given one */
	bool operator==(ratio right);

	/** exchange numerator and denominator */
	void reziproc();

	/** reduce the ratio */
	void reduce();
private:
	/** numerator */
	int zaehler;

	/** denominator */
	int nenner;

	/** change sign of the ratio */
	void change_sign();
}
;

/* ------ some prototyps of non class functions ------ */

/** it is possible to code: cout << ratio_object << endl; */
QTextStream & operator<<(QTextStream & str, const ratio & pratio);

#endif
