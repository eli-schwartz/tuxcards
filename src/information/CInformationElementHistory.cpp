/***************************************************************************
                          CInformationElementHistory.cpp  -  description
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

#include "../global.h"
#include "CInformationElementHistory.h"
#include <iostream>

// -------------------------------------------------------------------------------
CInformationElementHistory::CInformationElementHistory( void )
 : mList()
 , miCurrentIndex( INVALID_INDEX )
 , mpListener( NULLPTR )
// -------------------------------------------------------------------------------
{
}


// -------------------------------------------------------------------------------
CInformationElementHistory::~CInformationElementHistory( void )
// -------------------------------------------------------------------------------
{
   mList.clear();
   mpListener = NULLPTR;
}


// ************** IView *********************************************************
// -------------------------------------------------------------------------------
void CInformationElementHistory::aboutToRemoveElement( CInformationElement* pIE )
// -------------------------------------------------------------------------------
{
   while ( 0 < mList.contains( pIE ) )
   {
      int iIndex = mList.indexOf( pIE );
      if ( iIndex < miCurrentIndex )
      {
         miCurrentIndex--;
      }
      else
      {
         // There are the cases:
         //   a) iIndex > miCurrentIndex
         //   b) iIndex == miCurrentIndex
         // do nothing in both cases.
      }
      (void)mList.removeAt(iIndex);
   }

   if ( 0 == mList.count() )
   {
      miCurrentIndex = INVALID_INDEX;
   }
}
// ************** IView - End ****************************************************

// -------------------------------------------------------------------------------
void CInformationElementHistory::setListener( IHistoryListener* pListener )
// -------------------------------------------------------------------------------
{
   mpListener = pListener;
}


// -------------------------------------------------------------------------------
void CInformationElementHistory::addElement( CInformationElement& ie )
// -------------------------------------------------------------------------------
{
   // return if same element is added twice (one after the other)
   if ( INVALID_INDEX != miCurrentIndex )
   {
      CInformationElement* pIE = mList.at(miCurrentIndex);
      if ( (NULLPTR != pIE) && (pIE == &ie) )
         return;
   }


   if ( HISTORY_SIZE == mList.count() )
   {
      if (  0 == miCurrentIndex )
      {
         // if list is full and the current element is the first one, remove
         // the last element
         mList.removeLast();
      }
   }

   if ( 0 < miCurrentIndex )
   {
      for ( int i = 0; i < miCurrentIndex; i++ )
      {
         mList.removeFirst();
      }
   }

   mList.prepend( &ie );
   miCurrentIndex = 0;

   notifyListener();
}



// -------------------------------------------------------------------------------
CInformationElement* CInformationElementHistory::getPrevious( void )
// -------------------------------------------------------------------------------
{
   if ( (int)(mList.count()-1) == miCurrentIndex )
      return NULLPTR;

   miCurrentIndex++;
   CInformationElement* pIE = mList.at(miCurrentIndex);

   notifyListener();
   return pIE;
}

// -------------------------------------------------------------------------------
CInformationElement* CInformationElementHistory::getNext( void )
// -------------------------------------------------------------------------------
{
   if ( 0 == miCurrentIndex )
      return NULLPTR;

   miCurrentIndex--;
   CInformationElement* pIE = mList.at(miCurrentIndex);

   notifyListener();
   return pIE;
}

// -------------------------------------------------------------------------------
void CInformationElementHistory::dump( void )
// -------------------------------------------------------------------------------
{
   std::cout<<"--History"<<std::endl;
   QString sSeparator;
   QString sDesc;

   for ( int i = 0; i < (int)mList.count(); i++ )
   {
      sSeparator = (i == miCurrentIndex ) ? " *" : "  ";

      CInformationElement* pIE = mList.at(i);
      std::cout<<i<<sSeparator.toStdString()<<pIE<<"\t";
      sDesc =  (NULLPTR != pIE) ? pIE->getDescription() : "NULLPTR";
      std::cout<<sDesc.toStdString()<<std::endl;
   }

   std::cout<<"--History - End"<<std::endl;
}


// -------------------------------------------------------------------------------
void CInformationElementHistory::notifyListener( void )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR != mpListener )
      mpListener->historyStatusChanged( (int)(mList.count()-1) > miCurrentIndex,
                                        0 < miCurrentIndex );
}
