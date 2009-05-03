/***************************************************************************
                          task.h  -  class task
                             -------------------
    begin                : Tue Nov 27 16:40:42 CET 2001
    copyright            : (C) 2001 by Sebastian Stein
    email                : seb.kde@hpfsc.de

    copyright            : (C) 2008 by Tiago Porangaba, tiago.porangaba@ltia.fc.unesp.br
    copyright            : (C) 2008 by Tadeu Araujo, tadeu.araujo@ltia.fc.unesp.br
    copyright            : (C) 2008 by Danilo Balzaque, danilo.balzaque@ltia.fc.unesp.br
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

#include "ratio.h"
#include "primenumber.h"

#include <vector>
//Added by qt3to4:
#include <QTextStream>

/** important for add_sub and mul_div */
#define YES 1
#define NO  0

/** important for op_vector */
#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3

/** to mark a prime factor as used or unused */
#define UNUSED 0
#define USED  	1

/** Structure represents a prime factor.
 *  Structure stores a prime factor and its usage status. The factor is marked
 *  as used or unused.
 **/
typedef struct PRIME_FACTOR
{
	/** the prime factor */
	int factor;

	/** the status of the prime factor (used or unused) */
	short flag;
}
Tprime_factor;

/** we use the vector template class to create 3 dynamic types */
typedef QVector<ratio> RatioArray;
typedef QVector<short> ShortArray;
typedef QVector<Tprime_factor> PrimeFactorArray;

/*! class to handle mathematical tasks with ratios
 *  naming:
 *  - a task has at least 2 ratios
 *  - a task has at least 1 operation
 *
 *  \author Sebastian Stein */
class task
{
public:
	/** constructor */
	task();

	/** destructor */
	~task();

	/** automatically generate a new task with the given parameters */
	void create_task(unsigned int pmax_md = 10, short pnr_ratios = 2,
	                 short padd_add = YES, short padd_div = NO, short padd_mult = NO, short padd_sub = NO);

	/** set ratio n */
	void set_ratio_n(unsigned short number = 0, int numerator = 0,
	                 int denominator = 1);

	/** set ratio n */
	void set_ratio_n(unsigned short number, ratio fraction);

	/** returns ration n */
	ratio get_ratio_n(unsigned short number = 0) const;

	/** set operation n */
	void set_op_n(unsigned short number = 0, short operation = ADD);

	/** return operation n */
	short get_op_n(unsigned short number = 0) const;

	/** add a ratio to the end of the task */
	void add_ratio(ratio new_ratio);

	/** add a ratio to the end of the task */
	void add_ratio(int numerator = 0, int denominator = 1);

	/** add an operation at the end of the task */
	void add_operation(short operation = ADD);

	/** display the whole task, mainly for debug */
	QTextStream & display(QTextStream & str);

	/** solves the task and returns the result as ratio */
	ratio solve();

	/** returns the number of ratios in the vector */
	int getNumberOfRatios() const;

	/** returns the number of operations in the vector */
	int getNumberOfOperations() const;

	/** removes all ratios and operations from the given task */
	void clean();

private:
	/** max. size of main denominator */
	int max_md;

	/** how many ratios should the task have */
	short nr_ratios;

	short add_sub;
	short add_add;
	short add_mult;
	short add_div;		

	/** the ratio vector */
	RatioArray ratio_vector;

	/** the operation vector, smaller by one than ratio_vector */
	ShortArray op_vector;

	/** the prime factor vector is used to store all prime factors of the
	 * main denominator */
	PrimeFactorArray prim_fac_vector;

	/** this function is needed by solve() */
	ratio product(RatioArray::iterator & ratio_pointer,
	              ShortArray::iterator & op_pointer);

	/** generate the operations randomly; return how many mul or div
	 * are in one block */
	unsigned short make_operation(short padd_add = YES, short padd_div = NO, 
				      short padd_mult = NO, short padd_sub = NO,
	                              short pnr_ratios = 0);

	/** find a denominator for the task */
	int make_main_dn(unsigned int pmax_md, unsigned short max_product_length);

	/** returns the count number's prime factors  */
	unsigned short prim_factor_nr(int number = 1);

	/** set the numerators randomly */
	void make_numerators(int main_denominator, short pnr_ratios);

	/** create the ratios' denominators */
	void make_denominators(int main_denominator = 0, short pmax_md = 0, 
			       short padd_div = NO, short padd_mult = NO);
};


/* ------ some prototypes of non class functions ------ */

/** it is possible to code: cout << task_object << endl; */
QTextStream & operator<<(QTextStream & str, task & ptask);

#endif
