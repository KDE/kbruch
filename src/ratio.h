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

/* handling ratios */
class ratio
{
public:
	ratio(int pzaehler = 0, int pnenner = 1);
	~ratio();
	QTextStream & display(QTextStream & str) const;
	int get_zaehler(); // return numerator
	int get_nenner(); // return denominator
	void set_zaehler(int pzaehler = 0); // set numerator
	void set_nenner(int pnenner = 1); // set denominator
	void set_zaehler_ohne_k(int pzaehler = 0); // set numerator without reduce
	void set_nenner_ohne_k(int pnenner = 1); // set denominator without reduce
	ratio operator+(ratio summand); // c = object + summand
	ratio operator-(ratio subtrahend); // c = object - subtrahend
	ratio operator*(ratio factor); // c = object * factor
	ratio operator/(ratio divisor); // c = object / divisor
	ratio operator=(int dummy);
	bool operator==(ratio right);
	void reziproc(); // Reziproke bilden
	void reduce(); // kuerzen
private:
	int zaehler; // numerator
	int nenner; // denominator
	void change_sign(); // Vorzeichen tauschen
}
;

/* ------ some prototyps of non class functions ------ */

/* it is possible to code: cout << ratio_object << endl; */
QTextStream & operator<<(QTextStream & str, const ratio & pratio);

#endif
