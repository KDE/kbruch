/*
    PrimeNumber.cpp  -  source code of class PrimeNumber
    SPDX-FileCopyrightText: 2001 Sebastian Stein <seb.kde@hpfsc.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef PRIMENUMBER_H
#define PRIMENUMBER_H

#include <QVector>

typedef QVector<uint> UnsignedIntArray;

/** Class to handle prime numbers.
 *  The prime numbers are stored in a static vector, so that different instances
 *  can use them. Each time a higher prime number is needed, the new found prime
 *  number is stored in the vector. To check if a given number is a prime number
 *  this vector is scanned and if needed new prime numbers are generated. This
 *  algorithm should reduce calculation time and speed up every program using
 *  this prime number class.
 *  \author Sebastian Stein */
class PrimeNumber
{
public:
    /** constructor */
    PrimeNumber();

    /** destructor */
    ~PrimeNumber();

    /** returns whether the given number is a prime number */
    short isPrimeNumber(uint number);

    /** returns the next prime number */
    unsigned int get_next();

    /** returns the first prime number */
    unsigned int get_first() const;

    /** return the last known prime number */
    unsigned int get_last() const;

    /** returns the current prime number */
    unsigned int get_current() const;

    /** moves the internal pointer to the first prime number */
    void move_first();

    /** moves the internal pointer to the last prime number */
    void move_last();

    /** moves the internal pointer to the next prime number */
    void move_forward();

    /** moves the internal pointer to the previous prime number */
    void move_back();

    /** Displays all known prime numbers, mainly used for debugging. */
    void display_all();
private:
    /** a vector storing all known prime numbers, access for all objects;
     * we are using the vector<T> template; so we do not have to think
     * about dynamic mem manipulation */
    static UnsignedIntArray prim_vector;

    /** current selected prime number */
    UnsignedIntArray::iterator current_pos;

    /** finds next prime number and adds it to the vector */
    void find_next();
};
#endif
