/***************************************************************************
                          CInformationElementHistory.h  -  description
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
#ifndef CINFORMATION_ELEMENT_HISTORY_H
#define CINFORMATION_ELEMENT_HISTORY_H

#include "CInformationElement.h"
#include "IView.h"
#include <QList>
#include "IHistoryListener.h"


class CInformationElementHistory : public IView
{
public:
   CInformationElementHistory( void );
   virtual ~CInformationElementHistory( void );

   void                  setListener( IHistoryListener* pListener );

   void                  addElement( CInformationElement& ie );

   CInformationElement*  getPrevious( void );
   CInformationElement*  getNext( void );

   void                  dump( void );

   // ************** IView *************************************
   virtual void aboutToRemoveElement( CInformationElement* pIE );
   // ************** IView - End *******************************

private:
   #define HISTORY_SIZE 8
   #define LAST_POSITION (HISTORY_SIZE - 1)
   QList<CInformationElement*> mList;

   #define INVALID_INDEX -1
   int                           miCurrentIndex;

   IHistoryListener*             mpListener;
   void                          notifyListener( void );
};
#endif

