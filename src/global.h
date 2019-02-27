/***************************************************************************
                          global.h  -  description
                             -------------------
    begin                : Wed Sep 24 2003
    copyright            : (C) 2003 by Alexander Theel
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

#ifndef GLOBAL_H
#define GLOBAL_H

   #ifdef NULL
      #undef  NULL
   #endif
   #define NULL    0


   #define NULLPTR 0
   #define DELETE(x)        delete (x); (x) = NULLPTR

   #define ARRAYLEN(x)      (sizeof(x)/sizeof(x[0]))

   #define TUX_CONFIG_FILE    ".tuxcards"
   #define TUX_GREETING_FILE  "tuxcards_greeting.tux"

   #define INFORMATION_COLLECTION_DESC "##INFORMATION##COLLECTION##DESCRIPTION##"

   #define TUXCARDS_DRAGDROP_TYPE "application/x-tuxcards_entry.xml"

#endif
