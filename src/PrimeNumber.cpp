/*
    PrimeNumber.cpp  -  source code of class PrimeNumber
    SPDX-FileCopyrightText: 2001 Sebastian Stein <seb.kde@hpfsc.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "PrimeNumber.h"

#include <QDebug>


/* ----- the global prime number vector ----- */
UnsignedIntArray PrimeNumber::prim_vector;

/* ----- public member functions ----- */

/*  constructor for class primenumber */
PrimeNumber::PrimeNumber()
{
    /* if the vector is empty, we will add the first 2 prime numbers */
    if (prim_vector.empty()) {
#ifdef DEBUG
        qDebug() << "prim_vector is still empty";
#endif

        prim_vector.push_back(2);
        prim_vector.push_back(3);
    }
    current_pos = prim_vector.begin();
#ifdef DEBUG
    qDebug() << "constructor primenumber";
#endif
}

/* destructor for class primenumber */
PrimeNumber::~PrimeNumber()
{
#ifdef DEBUG
    qDebug() << "destructor primenumber";
#endif
}

/* check, if the given number is a prime number;
 * return 0 if no, 1 if yes */
short PrimeNumber::isPrimeNumber(uint number)
{
#ifdef DEBUG
    qDebug() << "primenumber::isPrimeNumber(" << number << ")";
#endif
    /* 0 is not a prime number */
    if (number == 0)
        return 0;

    /* jump to the start of the vector */
    move_first();

    /* check, if we can find a divisor */
    for (unsigned int dummy = get_first(); dummy < number; dummy = get_next()) {
        if ((number % dummy == 0) && (dummy != number))
            return 0; // the number is not a prime number

        /* we found a prime number, because we only have to test the given
         * number against all known prime numbers smaller square root of the
         * number */
        if (dummy * dummy > number)
            return 1;
    }

    return 1; // the given number is a prime number
}

/* returns next prime number */
unsigned int PrimeNumber::get_next()
{
    /* if we do not know the next number, we have to find it first */
    if (current_pos == prim_vector.end() ||
            ++current_pos == prim_vector.end()) {
        /* we do not know the next prime number, so we have to find it */
        find_next();
        move_last();
        return get_last(); /* return it */
    } else {
        /* we know the next prime number, set the pointer on it */
        return *current_pos; /* return it */
    }
}

/* returns the first prime number of the vector */
unsigned int PrimeNumber::get_first() const
{
    return *prim_vector.begin();
}

/* returns the last prime number in the vector */
unsigned int PrimeNumber::get_last() const
{
    return * (prim_vector.end() - 1);
}

/* returns currently selected prime number */
unsigned int PrimeNumber::get_current() const
{
    if (current_pos == prim_vector.end() + 1)
        return get_last();
    return *current_pos;
}

/* set current_pos to the first prime number */
void PrimeNumber::move_first()
{
    current_pos = prim_vector.begin();
}

/* set current_pos to the last prime number */
void PrimeNumber::move_last()
{
    current_pos = prim_vector.end() - 1;
}

/* move to the next prime number */
void PrimeNumber::move_forward()
{
    /* if we are at the end of prim_vector, we have to find a new number */
    if (current_pos == prim_vector.end() ||
            ++current_pos == prim_vector.end()) {
        find_next();
        move_last();
    }
}

/* move one prime number back */
void PrimeNumber::move_back()
{
    /* current_pos must be at least pointing to the first element
     * of our vector after this function */
    if (current_pos != prim_vector.begin())
        --current_pos;
}

/* displays the whole prim_vector on stdout; just for debugging */
void PrimeNumber::display_all()
{
    unsigned int dummy = 0; // count the numbers

    /* looping through the complete vector */
    for (current_pos = prim_vector.begin(); current_pos != prim_vector.end();
            current_pos++, dummy++)
        qDebug() << dummy << ": " << *current_pos;

    current_pos = prim_vector.end() - 1;
}

/* ----- private member functions ----- */

/* finds next prime number and adds it to the vector */
void PrimeNumber::find_next()
{
    /* our new prime number, must be bigger than the last one */
    unsigned int new_prim = * (prim_vector.end() - 1);

    do {
        /* new prime number must be bigger as biggest known one */
        new_prim += 2;
        /* loop as long as we find a divisor for the new number */
        for (current_pos = prim_vector.begin(); current_pos != prim_vector.end();
                ++current_pos)
            if ((new_prim % *current_pos == 0) || (new_prim < *current_pos))
                break;

        /* if we tried all known numbers and found no divisor, well,
         * we are happy to have found a new prime number
         *
         * we found a prime number, because we only have to test the given
         * number against all known prime numbers smaller square root of the
         * number */
        if ((current_pos == prim_vector.end())
                || (*current_pos * *current_pos > new_prim))
            break;
    } while (1);

    /* add the new prime number to the vector */
    prim_vector.push_back(new_prim);

    current_pos = prim_vector.end() - 1;
}
