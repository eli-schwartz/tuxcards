/***************************************************************************
                          cactustools.h  -  description
                             -------------------
    begin                : Mon Apr 10 2000
    copyright            : (C) 2000 by Alexander Theel
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

#ifndef CACTUSTOOLS_H
#define CACTUSTOOLS_H


class CactusTools {
public:
	~CactusTools();
	
   static bool fifty();
   static bool ranPercent(int percent);
   static int  ran(int min,int max);
   static int  ranLower(int min,int max);
   static int  ranUpper(int min,int max);
   static int  min(int x,int y);
   static int  max(int x,int y);
	
private:
	CactusTools();

};

#endif
