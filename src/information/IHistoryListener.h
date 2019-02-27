/***************************************************************************
                          IHistoryListener.h  -  description
                             -------------------
    begin                : Mon Dec 01 2003
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
#ifndef IHISTORY_LISTENER_H
#define IHISTORY_LISTENER_H


class IHistoryListener{
public:
   virtual ~IHistoryListener(){};
   


   /**
    * This method is called when ever the status of the current
    * element within the history has changed.
    *
    * The parameter 'bHasPreviousElement' states whether the current
    * "history-element" does have a previous element.
    * The parameter 'bHasNextElement' states whether the current
    * "history-element" does have a next element.
    */
   virtual void historyStatusChanged( bool bHasPreviousElement, bool bHasNextElement ) = 0;
};
#endif

