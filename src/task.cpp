/***************************************************************************
                          task.cpp  -  source code of class task
                             -------------------
    begin                : Tue Nov 27 16:40:42 CET 2001
    copyright            : (C) 2001 by Sebastian Stein
    email                : seb.kde@hpfsc.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <math.h>
#include <stdlib.h>

#include <kdebug.h>

#include <time.h>

#include "task.h"

/** constructor of class task */
task::task()
{
	srand(time(NULL));
#ifdef DEBUG
	kdDebug() << "constructor task" << endl;
#endif
}

/** destructor of class task */
task::~task()
{
#ifdef DEBUG
	kdDebug() << "destructor task" << endl;
#endif
}

/** create a task with random ratios and operations; the generated task
 * can be customized by the given parameters:
 * pmax_md: maximum main denominator
 * pnr_ratios: number of ratios -> pnr_ratios - 1 operations
 * padd_sub: if TRUE + and - are allowed operations
 * pmul_div: if TRUE * and / are allowed operations */
void task::create_task(unsigned int pmax_md, short pnr_ratios,
                       short padd_sub, short pmul_div)
{
	unsigned short max_product_length = 0;
	int main_denominator = 1;

	/* we say that if add/sub and mul/div are not allowed we want a task
	 * for add/sub only */
	if (padd_sub == NO && pmul_div == NO)
		padd_sub = YES;

	do
	{
		/* delete a maybe given task */
		ratio_vector.clear();

		/* generate the operations and count the max. mul/div in one block */
		max_product_length = make_operation(padd_sub, pmul_div, pnr_ratios);

#ifdef DEBUG
		kdDebug() << "1: max_product_length: " << max_product_length << endl;
#endif
		/* later we must be able to find a main denominator;
		 * so 2 ^ max_product_length couldn't be bigger than the max. denominator */
	}
	while ((unsigned int) pow(2, max_product_length) > pmax_md);

#ifdef DEBUG
	kdDebug() << "2: max_product_length: " << max_product_length << endl;
#endif

	/* find a main denominator */
	main_denominator = make_main_dn(pmax_md, max_product_length);

#ifdef DEBUG
	kdDebug() << "after make_main_dn()" << endl;
#endif

	/* create the ratios' numerators */
	make_numerators(main_denominator, pnr_ratios);

#ifdef DEBUG
	kdDebug() << "after make_numerators()" << endl;
#endif

	/* create the ratios' denominators */
	make_denominators(main_denominator, pmax_md, pmul_div);

#ifdef DEBUG
	kdDebug() << "main deno: " << main_denominator << endl;
	kdDebug() << "prim fakt: " << prim_fac_vector.size() << endl;
#endif

	return;
}

/** set ratio n in the ratio_vector */
void task::set_ratio_n(unsigned short number, int numerator, int denominator)
{
	/* do not set something outside our vector */
	if (number > ratio_vector.size() - 1)
		number = 0;
	ratio_vector[number].setNumerator(numerator); // set numerator
	ratio_vector[number].setDenominator(denominator); // set denominator
	return;
}

/** set ratio n in the ratio_vector */
void task::set_ratio_n(unsigned short number, ratio fraction)
{
	/* do not set something outside our vector */
	if (number > ratio_vector.size() - 1)
		number = 0;
	ratio_vector[number].setNumerator(fraction.numerator()); // set numerator
	ratio_vector[number].setDenominator(fraction.denominator()); // set denominator
	return;
}

/** returns the ratio given by number from the ratio_vector */
ratio task::get_ratio_n(unsigned short number) const
{
	/* do not set something outside our vector */
	if (number > ratio_vector.size() - 1)
		number = 0;
	return ratio_vector[number];
}

/** set operation given by the number in the op_vector */
void task::set_op_n(unsigned short number, short operation)
{
	/* do not set something outside our vector */
	if (number > op_vector.size() - 1)
		number = 0;
	op_vector[number] = operation;
	return;
}

/** returns the operation given by number from the op_vector */
short task::get_op_n(unsigned short number) const
{
	/* do not set something outside our vector */
	if (number > op_vector.size() - 1)
		number = 0;
	return op_vector[number];
}

/** add a new ratio at the end of the ratio vector */
void task::add_ratio(ratio new_ratio)
{
	ratio_vector.push_back(new_ratio);
	return;
}

/** add a new ratio at the end of the ratio vector */
void task::add_ratio(int numerator, int denominator)
{
	ratio new_ratio(numerator, denominator);
	ratio_vector.push_back(new_ratio);
	return;
}

/** add a new operation at the end of the operation vector */
void task::add_operation(short operation)
{
	op_vector.push_back(operation);
	return;
}

/** just outputs the whole given task to stdout; for debugging */
QTextStream & task::display(QTextStream & str)
{
	/* this is our pointer on the ratio_vector, set it to the beginning */
	RatioArray::iterator ratio_pointer = ratio_vector.begin();

	/* this is our pointer on the op_vector, set it to the beginning */
	ShortArray::iterator op_pointer = op_vector.begin();

	/* we need this array to look up the fitting chars for the operations */
	const char a[] = "+-*/";

	/* check, if a qSetW() was given to the stream */
	int weite = str.width();
	int pweite = weite;
	str << qSetW(0);

	/* check, if ratio number and operation number fit together */
	if (ratio_vector.size() != op_vector.size() + 1)
	{
		kdDebug() << "Number of ratios and operations do not fit." << endl;
		return str;
	}

	while (pweite-- > 0)
		str << " ";

	/* display all numerators */
	for (ratio_pointer = ratio_vector.begin();
	        ratio_pointer != ratio_vector.end(); ratio_pointer++)
	{
		str << qSetW(5) << ratio_pointer->numerator() << "   ";
	}
	str << endl;

	pweite = weite;
	while (pweite-- > 0)
		str << " ";

	/* display all operations */
	for (op_pointer = op_vector.begin();
	        op_pointer != op_vector.end(); op_pointer++)
	{
		str << " ----- " << a[*op_pointer];
	}
	str << " ----- = " << endl;

	pweite = weite;
	while (pweite-- > 0)
		str << " ";

	/* display all denominators */
	for (ratio_pointer = ratio_vector.begin();
	        ratio_pointer != ratio_vector.end(); ratio_pointer++)
	{
		if (ratio_pointer == ratio_vector.end() - 1)
			return str << qSetW(5) << ratio_pointer->denominator() << "   ";
		str << qSetW(5) << ratio_pointer->denominator() << "   ";
	}
	return str;
}

/** solves the given task and returns the result as a ratio */
ratio task::solve()
{
	ratio ergebnis(0, 1); /* that is the starting point */

	/* this is our pointer on the ratio_vector, set it to the beginning */
	RatioArray::iterator ratio_pointer = ratio_vector.begin();

	/* add a temp operation at the beginning */
	op_vector.insert(op_vector.begin(), ADD);

	/* this is our pointer on the op_vector, set it to the beginning */
	ShortArray::iterator op_pointer = op_vector.begin() + 1;

	/* check, if ratio number and operation number fit together */
	if (ratio_vector.size() != op_vector.size())
	{
		kdDebug() << "Number of ratios and operations do not fit." << endl;
		return ergebnis;
	}

	do
	{
		/* we have to decide our next action by the given operation */
		switch (*op_pointer)
		{
		case ADD :
		case SUB :
			switch(*(op_pointer - 1))
			{
				/* we only have to add/sub the next ratio */
			case ADD :
				ergebnis = ergebnis + *ratio_pointer++;
				break;
			case SUB :
				ergebnis = ergebnis - *ratio_pointer++;
				break;
			}
			break;
		case MUL :
		case DIV :
			switch (*(op_pointer - 1))
			{
				/* the next ratio is a product, so we have to
				 * compute this product first and than add/sub it */
			case ADD :
				ergebnis = ergebnis +
				           product(ratio_pointer, op_pointer);
				break;
			case SUB :
				ergebnis = ergebnis -
				           product(ratio_pointer, op_pointer);
				break;
			}
			break;
		}
		/* check if we reached the and of the task */
		if (ratio_pointer == ratio_vector.end())
			break;

#ifdef DEBUG
		kdDebug() << "Schleifenende" << endl;
#endif

	}
	while (++op_pointer != op_vector.end());

#ifdef DEBUG

	kdDebug() << "after do while in solve()" << endl;
#endif

	/* if the last operation was an add/sub we haven't add/subed it until now */
	--op_pointer;
	switch (*op_pointer)
	{
	case ADD :
		ergebnis = ergebnis + *ratio_pointer;
		break;
	case SUB :
		ergebnis = ergebnis - *ratio_pointer;
		break;
	}

	/* erase the temp operation */
	op_vector.erase(op_vector.begin());

	/* before we return the result we have to reduce it */
	ergebnis.reduce();

	return ergebnis; /* return the solution */
}

/* returns the number of ratios in the vector */
int task::getNumberOfRatios() const
{
	return ratio_vector.count();
}

/* returns the number of operations in the vector */
int task::getNumberOfOperations() const
{
	return op_vector.count();
}

/** this function is called by the solving function to compute a given
 * product (or div) and return the solution */
ratio task::product(RatioArray::iterator & ratio_pointer,
                    ShortArray::iterator & op_pointer)
{
	/* the function's parameters are pointing to the next ratio;
	 * to the starting point of the product */
	ratio product(ratio_pointer->numerator(), ratio_pointer->denominator());

#ifdef DEBUG

	kdDebug() << "in product()" << endl;
#endif

	++ratio_pointer;
	do
	{
		switch (*op_pointer)
		{
		case ADD :
		case SUB :
			return product; /* finished */

			/* compute the next step of the product (or div) */
		case MUL :
			product = product * *ratio_pointer++;
			++op_pointer;
			break;
		case DIV :
			product = product / *ratio_pointer++;
			++op_pointer;
			break;
		}
	}
	while (op_pointer != op_vector.end());

	/* we get here if the product consists of the whole given task starting
	 * at the point given by the function's parameters */
	return product;
}

/** generate the operations randomly; return how many mul or div
 * are in one block */
unsigned short task::make_operation(short padd_sub, short pmul_div,
                                    short pnr_ratios)
{
	unsigned short max_product_length = 0;
	unsigned short operations = 0;

	/* this is our pointer on the op_vector, set it to the beginning */
	ShortArray::iterator op_pointer;

	/* we need this to generate the fitting operations */
	if (padd_sub == YES)
		operations += 2;
	if (pmul_div == YES)
		operations += 2;

	/* clear the old operations */
	op_vector.clear();

	/* generate the operations */
	for (short counter = 0; counter < pnr_ratios - 1; counter++)
		op_vector.push_back(short((double(rand()) / RAND_MAX) * operations));

	/* if we only wanted mul/div, operations was 2; but we want values
	 * for the operations with 2 and 3 so we have to add 2 */
	if (padd_sub == NO && pmul_div == YES)
	{
		/* loop through all operations and add 2, so that the operations
		 * are interpreted as mul/div and not add/sub */
		for (op_pointer = op_vector.begin();
		        op_pointer != op_vector.end(); op_pointer++)
			*op_pointer += 2;
	}

	if (pmul_div == YES)
	{
		short flag_counter = 0;

		/* loop through all operations */
		for (op_pointer = op_vector.begin();
		        op_pointer != op_vector.end(); op_pointer++)
		{
			/* look if we got a mul/div or add/sub */
			if (*op_pointer == DIV || *op_pointer == MUL)
			{
				flag_counter++;
			}
			else
			{
				/* we have to decide, if this was the end of a mul/div block or
				 * just another add/sub */
				if (flag_counter > 0)
				{
					/* it was the end of a mul/div block; lets look if it was
					 * longer than the blocks before and save it; than restart */
					if (flag_counter > max_product_length)
						max_product_length = flag_counter;
					flag_counter = 0;
				} /* if (flag_counter > 0) */
			} /* if (*op_pointer == DIV || *op_pointer == MUL) */
		} /* for (op_pointer = op_vector.begin(); ...) */

		/* just to correct the things a little bit if the last operation was a
		 * mul/div as well */
		if (flag_counter > max_product_length)
			max_product_length = flag_counter;
		max_product_length++;
	}
	else
	{ /* if (pmul_div == YES) */
		/* a task is given only with add/sub ops; so we want a max.
		 * of pnr_ratios / 2 + 1 prime factors, but at least  */
		max_product_length = (unsigned short) (float(pnr_ratios) / 2) + 1;
		if (max_product_length < 2)
			max_product_length = 2;
	} /* if (pmul_div == YES) */

	return max_product_length;
}

/** find a denominator for the task */
int task::make_main_dn(unsigned int pmax_md, unsigned short max_product_length)
{
	int denominator;

	/* find a main denominator in the given limits by pmax_md and check
	 * if the main denominator has enough prime factors */
	do
	{
		denominator = int(((double(rand()) / RAND_MAX) * pmax_md) + 1);
	}
	while ((pmax_md < 1) ||
	        (prim_factor_nr(denominator) < max_product_length));

	return denominator;
}

/** returns the count number's prime factors and stores the prime factors
 * in the prim_fac_vektor vektor */
unsigned short task::prim_factor_nr(int number)
{
	unsigned int tmp_number = number;
	primenumber primenumber;
	Tprime_factor prim_fac_struct;

	/* delete all the prime factors of the old main denominator */
	prim_fac_vector.clear();

	/* test if we can find prime factors */
	for (primenumber.move_first(); primenumber.get_current() <= tmp_number; )
	{
		/* if the current selected prime number is a divisor */
		if (tmp_number % primenumber.get_current() != 0)
		{
			primenumber.move_forward(); /* no, test next one */
		}
		else
		{
			/* yes, we found a new prime factor; so first we use the divisor */
			tmp_number = int(tmp_number / primenumber.get_current());

			/* now we add the prime factor to our prime factor vector */
			prim_fac_struct.factor = primenumber.get_current();
			prim_fac_struct.flag = UNUSED;
			prim_fac_vector.push_back(prim_fac_struct);
		}
	}
#ifdef DEBUG
	PrimeFactorArray::iterator prim_fac_pointer = prim_fac_vector.begin();
	kdDebug() << "Primfaktoren von: " << number << endl;
	for (prim_fac_pointer = prim_fac_vector.begin();
	        prim_fac_pointer != prim_fac_vector.end();
	        prim_fac_pointer++)
		kdDebug() << (*prim_fac_pointer).factor << endl;
	kdDebug() << "Anzahl: " << prim_fac_vector.size() << endl;
#endif

	return prim_fac_vector.size();
}

/** set the numerators randomly */
void task::make_numerators(int main_denominator, short pnr_ratios)
{
	/* I think it is to easy to deal with ratios like 1/1 or 4/4; so
	 * I limit the maximum of a numerator */
	int max_numerator = int(main_denominator * float(0.7));

	/* add a new ratio to the task and compute the numerator randomly */
	for (short tmpcounter = 0; tmpcounter < pnr_ratios; tmpcounter++)
	{
		(*this).add_ratio(int((double(rand()) / RAND_MAX)
		                      * max_numerator) + 1, 1);
	}
	return;
}

/** create the ratios' denominators */
void task::make_denominators(int main_denominator, short pmax_md,
                             short pmul_div)
{
	/* this is our pointer on the ratio_vector, set it to the beginning */
	RatioArray::iterator ratio_pointer = ratio_vector.begin();

	/* this is our pointer on the op_vector, set it to the beginning */
	ShortArray::iterator op_pointer = op_vector.begin() + 1;

	/* this is a pointer on the array with the prime factors of the main
	 * denominator */
	PrimeFactorArray::iterator prim_fac_pointer;

	unsigned short unused_fac = prim_fac_vector.size();
	unsigned short next_fac;
	unsigned short tmp_counter;
	int tmp_deno;

	/* check, if ratio number and operation number fit together */
	if (ratio_vector.size() != op_vector.size() + 1)
	{
		kdDebug() << "Number of ratios and operations do not fit." << endl;
		return;
	}

	/* first make all denominators */
	for (ratio_pointer = ratio_vector.begin();
	        ratio_pointer != ratio_vector.end(); ratio_pointer++)
	{
		do
		{
			tmp_deno = int((double(rand()) / RAND_MAX) * pmax_md) + 1;
		}
		while (main_denominator % tmp_deno != 0);
		(*ratio_pointer).setDenominator(tmp_deno);
	}

	/* if the ratio is connected to a mul or div operation, we have to do some
	 * extra work and regenerate the denominators */
	if (pmul_div == YES)
	{
		/* lets loop through all ratios and check, if there is a mul/div
		 * after the ratio */
		ratio_pointer = ratio_vector.begin();
		op_pointer = op_vector.begin();
		do
		{
			if (*op_pointer == MUL || *op_pointer == DIV)
			{
				/* yes, there is a mul/div after the ratio;
				 * reset the prime number structure */
				for (prim_fac_pointer = prim_fac_vector.begin();
				        prim_fac_pointer != prim_fac_vector.end();
				        prim_fac_pointer++)
					(*prim_fac_pointer).flag = UNUSED;

				/* how many prime factors are avaible? */
				unused_fac = prim_fac_vector.size() - 1;

				/* now loop through this mul/div section until we find a add/sub */
				do
				{
					/* the prim_fac_vector is sorted, but we do not want the
					 * factors in this sorted way as our denominators;
					 * so we choose one randomly */
					next_fac = (unsigned short)((double(rand()) / RAND_MAX)
					                            * unused_fac);
					tmp_counter = 0;

					/* check the prime factors, if they are unused */
					for (prim_fac_pointer = prim_fac_vector.begin();
					        prim_fac_pointer != prim_fac_vector.end();
					        prim_fac_pointer++)
					{
						if ((*prim_fac_pointer).flag == UNUSED)
						{
							tmp_counter++; /* we found a unused factor */
						}
						/* we found the factor, which we have chosen randomly */
						if (tmp_counter > next_fac)
							break;
					}
					/* mark the factor as used, so we can not use it again in
					 * this mul/div section */
					(*prim_fac_pointer).flag = USED;

					/* store the factor as our new denominator for this ratio */
					(*ratio_pointer).setDenominator((*prim_fac_pointer).factor, false);
					unused_fac--; /* now there is one factor less avaible */

					/* move to the next ratio */
					ratio_pointer++;
					op_pointer++;
				}
				while ((op_pointer != op_vector.end()) &&
							(*op_pointer == MUL || *op_pointer == DIV));

				/* we always miss to set the last ratio in a mul/div section;
				 * so we have to fix this here */
				if (ratio_pointer != ratio_vector.end())
				{
					/* the prim_fac_vector is sorted, but we do not want the
					 * factors in this sorted way as our denominators;
					 * so we choose one randomly */
					next_fac = (unsigned short)((double(rand()) / RAND_MAX)
					                            * unused_fac);
					tmp_counter = 0;

					/* check the prime factors, if they are unused */
					for (prim_fac_pointer = prim_fac_vector.begin();
					        prim_fac_pointer != prim_fac_vector.end();
					        prim_fac_pointer++)
					{
						if ((*prim_fac_pointer).flag == UNUSED)
						{
							tmp_counter++; /* we found a unused factor */
						}
						/* we found the factor, which we have chosen randomly */
						if (tmp_counter > next_fac)
							break;
					}
					/* mark the factor as used, so we can not use it again in
					 * this mul/div section */
					(*prim_fac_pointer).flag = USED;

					/* store the factor as our new denominator for this ratio */
					(*ratio_pointer).setDenominator((*prim_fac_pointer).factor, false);
					unused_fac--; /* now there is one factor less avaible */

					/* move to the next ratio */
					ratio_pointer++;
					op_pointer++;
				}
			}
			else
			{ /* if (*op_pointer == MUL || ...) */
				ratio_pointer++;
				op_pointer++;
			}
		}
		while (ratio_pointer != ratio_vector.end() &&
		        op_pointer != op_vector.end());

		/* now we will swap all ratios, if there is a div in front of */
		ratio_pointer = ratio_vector.begin();
		ratio_pointer++;

		for (op_pointer = op_vector.begin(); op_pointer != op_vector.end();
		        op_pointer++)
		{
			if (*op_pointer == DIV)
			{
				(*ratio_pointer).reziproc();
			}
			ratio_pointer++;
		}
	} /* if (pmul_div == YES) */

	return;
}


/* ------ some prototyps of non class functions ------ */

/** it is possible to code: cout << task_object << endl; */
QTextStream & operator<<(QTextStream & str, task & ptask)
{
	return ptask.display(str);
}
