/***************************************************************************
                          cactustools.cpp  -  description
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

#include "cactustools.h"
#include <stdlib.h>

CactusTools::CactusTools(){ }

CactusTools::~CactusTools(){ }

/**
* Jeweils 50% Wahrscheinlichkeit für true oder false.
*/
bool CactusTools::fifty() {
  return (rand() % 100 < 50);
}

/**
* it x% Wahrscheinlichkeit true.
*/
bool CactusTools::ranPercent(int percent) {
  return (rand() % 100 < percent);
}

/**
* Zufallszahl innerhalb [min...max] gleichverteilt.
*/
int CactusTools::ran(int min,int max) {
  if (max<min) return -1;
  if (min==max) return min;
  return (min + rand() % (max+1-min));
}

/**
* Zufallszahl innerhalb [min...max], wobei die Wahrscheinlichkeit des auftretens
* dervon Zahlen von max nach min quadratisch ansteigt.
*/
int CactusTools::ranLower(int min,int max) {
  if (max<min) return -1;
  if (min==max) return min;
  int r=rand();                             // <- this is probably wrong
  return (min+  r*r % (max+1-min));         // <- this is probably wrong
}

/**
* Zufallszahl innerhalb [min...max], wobei die Wahrscheinlichkeit des auftretens
* dervon Zahlen von max nach min quadratisch ansteigt.
*/
int CactusTools::ranUpper(int min,int max) {
  if (max<min) return -1;
  if (min==max) return min;
  return ran(min, max);
//  return (int)( (1.0-Math.pow(Math.random(),2))*(max+1-min)+min);
}

/**
* Minimum von x und y.
*/
int CactusTools::min(int x,int y){
  return (x<y) ? x : y;
}

/**
* Maximum von x und y.
*/
int CactusTools::max(int x,int y){
  return (x>y) ? x : y;
}
