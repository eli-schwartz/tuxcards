/***************************************************************************
                          integers.cpp  -  description
                             -------------------
    begin                : Tue Jul 23 2002
    copyright            : (C) 2002 by Alexander Theel
    email                : alex.theel@gmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "integers.h"


/**
 * auxiliary function; otherwise some systems get an undefined reference error
 * while compiling
 */
int Integers::abs(int i){ return ((i>=0) ? i : -i); }
