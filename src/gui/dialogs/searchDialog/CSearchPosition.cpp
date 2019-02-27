/***************************************************************************
                          CSearchposition.cpp  -  description
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
#include "CSearchPosition.h"

/** 
 * 'CTreeElement' should be used instead of 'QListViewItem' in future
 * 'x' is actually a 'CTreeElement', the 'CTreeElement' where we have found s.th.
 * 'pos':  the position within the document
 * 'len':  length of searchString; needed to draw this item (especially for
 *         regular expressions)
 */
// -------------------------------------------------------------------------------
CSearchPosition::CSearchPosition( Path/*CTreeElement*//*QListViewItem*/* path, int location,
                                  int iPos, int iLen,
                                  QString s )
// -------------------------------------------------------------------------------
{
   //referenceElement = x;
   this->path     = path;
   this->searchLocation = location;
   this->pos      = iPos;
   this->len      = iLen;
   this->s        = s;
}

//TODO: Destructor is missing. All local pointers must be taken care of.


// -------------------------------------------------------------------------------
CTreeElement* CSearchPosition::getReferenceElement()
// -------------------------------------------------------------------------------
{
   return referenceElement;
}

// -------------------------------------------------------------------------------
const Path* CSearchPosition::getPath() const
// -------------------------------------------------------------------------------
{
   return path;
}

int CSearchPosition::getPos() const { return pos; }
int CSearchPosition::getLen() const { return len; }

// really dirty, calling x->text(0), althought we are using 'Knoten'
// -> da kreuzweise verschachtelung der Headerfiles 'knoten.h' & 'searchpos.h'
QString CSearchPosition::getName(){ return referenceElement->text(0); }
QString CSearchPosition::getString(){ return s; }


/*SP_LOCATION SearchPosition::getLocation()*/

// -------------------------------------------------------------------------------
int CSearchPosition::getLocation() const
// -------------------------------------------------------------------------------
{
   return searchLocation;
}
