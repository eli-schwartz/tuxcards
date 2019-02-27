/***************************************************************************
                          IView.h  -  description
                             -------------------
    begin                : Mon Dec 02 2003
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
#ifndef IVIEW_H
#define IVIEW_H

#include "IRemoveElementListener.h"


class IView : public IRemoveElementListener{
public:

   // ****************** IRemoveElementListener *********************
   /**
    * This method is called just before the information element 'pIE'
    * is removed from the data collection.
    */
   virtual void aboutToRemoveElement( CInformationElement* pIE ) = 0;
};
#endif

