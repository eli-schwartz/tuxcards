/***************************************************************************
                          IParent.h  -  description
                             -------------------
    begin                : Thu Dec 04 2003
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
#ifndef IPARENT_H
#define IPARENT_H

#include "IRemoveElementListener.h"
#include <qstring.h>

class CInformationElement;


class IParent : public IRemoveElementListener
{
public:
   // Use a 'IParent'-ptr within the constructor; if we have a better
   // idea we might remove these method from the interface.
   virtual QString  getDescription( void ) const = 0;
   virtual IParent* getParent( void ) const = 0;

   /**
    * This method is called from a child in order to cause its
    * deletion.
    */
   virtual void removeChild( CInformationElement* pIE ) = 0;


   // ****************** IRemoveElementListener *********************
   /**
    * This method is called just before the information element 'pIE'
    * is removed.
    */
   virtual void aboutToRemoveElement( CInformationElement* pIE ) = 0;
};
#endif

