/***************************************************************************
                          CSearchposition.h  -  description
                             -------------------
    begin                : Sun Mar 26 2000
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
#ifndef CSEARCHPOSITION_H
#define CSEARCHPOSITION_H

#include <QString>
#include "../../CTreeElement.h"

//TODO: Refactoring necessary.

class CSearchPosition {
public:
   CSearchPosition( Path* path, int location,
                    int pos, int len, QString s );

   CTreeElement* getReferenceElement();
   const Path* getPath() const;

   int getPos() const;
   int getLen() const;
   QString getName();
   QString getString();

   // states where the string that was searched is found
   enum SP_LOCATION
   {
      SP_NONE = -1,
      SP_NAME,
      SP_INFORMATION
   };

   /*SP_LOCATION*/int getLocation() const;


private:
   CTreeElement/*QListViewItem*/* referenceElement;     // actually a 'CTreeElement' -> kreuzweise Einbindung d. Headerdateien
   Path* path;
   int searchLocation;                                  // states where the string that was searched is found

   int pos;
   int len;
   QString s;
};

#endif
