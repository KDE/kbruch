/***************************************************************************
                          task.h  -  class task
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

#ifndef TASK_H
#define TASK_H

/*#define DEBUG*/

#include "ratio.h"
#include "primzahl.h"

#include <iostream.h>
#include <iomanip.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

/* important for add_sub and mul_div */
#define YES 1
#define NO  0
#define TRUE 1
#define FALSE 0

/* important for op_vector */
#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3

/* to mark a prime factor as used or unused */
#define UNUSED 0
#define USED  	1

/* we need a structure to store a prime factor and a flag to mark the
 * factor as used or unused in the given context */
typedef struct PRIME_FACTOR
{
	int factor;
	short flag;
} Tprime_factor;

/* we use the vector template class to create 3 dynamic types */
typedef std::vector<ratio> RatioArray;
typedef std::vector<short> ShortArray;
typedef std::vector<Tprime_factor> PrimeFactorArray;

/* class to handle mathematical tasks with ratios */
class task
{
	public:
		task();
		~task();
		void create_task(unsigned int pmax_md = 10, short pnr_ratios = 2,
							 	short padd_sub = YES, short pmul_div = NO);
		void set_ratio_n(unsigned short number = 0, int zaehler = 0,
							 	int nenner = 1);
		void set_ratio_n(unsigned short number = 0, ratio bruch = 0);
		ratio get_ratio_n(unsigned short number = 0);
		void set_op_n(unsigned short number = 0, short operation = ADD);
		short get_op_n(unsigned short number = 0);
		void add_ratio(ratio new_ratio = 0);
		void add_ratio(int zaehler = 0, int nenner = 1);
		void add_operation(short operation = ADD);
		ostream & display(ostream & str);
		ratio solve();
	private:
		/* max. size of main denominator */
		int max_md;

		/* how many ratios should the task have */
		short nr_ratios;

		/* are add/sub operations allowed */
		short add_sub;

		/* are mul/div operations allowed */
		short mul_div;

		/* the ratio vector */
		RatioArray ratio_vector;

		/* the operation vector, smaller by one then ratio_vector */
		ShortArray op_vector;

		/* the prime factor vector is used to store alle prime factors of the
		 * main denominator */
		PrimeFactorArray prim_fac_vector;

		/* this function is needed by solve() */
		ratio product(RatioArray::iterator & ratio_pointer,
			 ShortArray::iterator & op_pointer);

		/* computes basis high exponent */
		unsigned int high(unsigned short base, unsigned short exponent);

		/* generate the operations randomly; return how many mul or div
		 * are in one block */
		unsigned short make_operation(short padd_sub, short pmul_div,
							 short pnr_ratios);

		/* find a denominator for the task */
		int make_main_dn(unsigned int pmax_md, unsigned short max_product_length);

		/* returns the count number's prime factors  */
		unsigned short prim_factor_nr(int number = 1);
		
		/* set the numerators randomly */
		void make_numerators(int main_denominator, short pnr_ratios);

		/* create the ratios' denominators */
		void make_denominators(int main_denominator, short pmax_md,
							 			short pmul_div);
};


/* ------ some prototyps of non class functions ------ */

/* it is possible to code: cout << task_object << endl; */
ostream & operator<<(ostream & str, task & ptask);

#endif
