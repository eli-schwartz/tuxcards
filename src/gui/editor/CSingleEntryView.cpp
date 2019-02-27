/***************************************************************************
                          CSingleEntryView.cpp  -  description
                             -------------------
    begin                : Fri Jan 09 2004
    copyright            : (C) 2004 by Alexander Theel
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

#include "CSingleEntryView.h"
#include <iostream>

#include "./CEncryptedEntryView.h"

// -------------------------------------------------------------------------------
CSingleEntryView::CSingleEntryView( QWidget* pParent )
  : QStackedWidget( pParent )
  , mpActiveElement( NULLPTR )
  , mpEditor( NULLPTR )
  , mpPasswdView( NULLPTR )
//  , mpInformationElement( NULLPTR )
// -------------------------------------------------------------------------------
{
   mpEditor = new CEditor( this );
   mpPasswdView = new CEncryptedEntryView( this );

   Q_ASSERT( NULLPTR != mpEditor );
   if ( !mpEditor || !mpPasswdView )
      return;

   insertWidget( EDITOR, mpEditor );
   insertWidget( PASSWD, mpPasswdView );
   setCurrentIndex( PASSWD );

   connect( mpPasswdView, SIGNAL(signalEntryDecrypted()), this, SLOT(slotEntryDecrypted()) );
}


// -------------------------------------------------------------------------------
CSingleEntryView::~CSingleEntryView()
// -------------------------------------------------------------------------------
{
   mpActiveElement = NULLPTR;
}

// ************** IView *********************************************************
// -------------------------------------------------------------------------------
void CSingleEntryView::aboutToRemoveElement( CInformationElement* pIE )
// -------------------------------------------------------------------------------
{
   if ( mpActiveElement == pIE )
   {
      mpActiveElement = NULLPTR;
   }
   mpEditor->aboutToRemoveElement( pIE );
   mpPasswdView->aboutToRemoveElement( pIE );
}
// ************** IView - End ****************************************************


// -------------------------------------------------------------------------------
CEditor* CSingleEntryView::getEditor()
// -------------------------------------------------------------------------------
{
   return mpEditor;
}


/**
 * This slot is called as soon as the encrypted entry is decrypted.
 */
// -------------------------------------------------------------------------------
void CSingleEntryView::slotEntryDecrypted()
// -------------------------------------------------------------------------------
{
   activeInformationElementChanged( mpActiveElement );
}


// -------------------------------------------------------------------------------
QString CSingleEntryView::getText()
// -------------------------------------------------------------------------------
{
   return (NULLPTR != mpEditor) ? mpEditor->getText() : QString("");
}

// -------------------------------------------------------------------------------
void CSingleEntryView::setText( const QString& sText )
// -------------------------------------------------------------------------------
{
   if ( mpEditor )
      mpEditor->setText( sText );
}


// -------------------------------------------------------------------------------
void CSingleEntryView::writeCurrentTextToActiveInformationElement( void )
// -------------------------------------------------------------------------------
{
   if (NULLPTR != mpEditor)
      mpEditor->writeCurrentTextToActiveInformationElement();
}

// // -------------------------------------------------------------------------------
// int CSingleEntryView::countBRs( void )
// // -------------------------------------------------------------------------------
// {
//    return (NULLPTR != mpEditor) ? mpEditor->countBRs() : 0;
// }

// -------------------------------------------------------------------------------
void CSingleEntryView::activeInformationElementChanged( CInformationElement* pIE )
// -------------------------------------------------------------------------------
{
   //std::cout<<"CSingleEntryView::activeInformationElementChanged()"<<std::endl;

   if ( !pIE || !mpPasswdView || !mpEditor )
      return;

   mpActiveElement = pIE;

   if ( mpActiveElement->isCurrentlyEncrypted() )
   {
      //std::cout<<"CSingleEntryView - entry is encrypted"<<std::endl;
      mpPasswdView->activeInformationElementChanged( mpActiveElement );
      setCurrentIndex( PASSWD );
   }
   else
   {
      //std::cout<<"CSingleEntryView - entry is not encrypted"<<std::endl;
      mpEditor->activeInformationElementChanged( mpActiveElement );
      setCurrentIndex( EDITOR );
      emit signalEntryDecrypted();
   }
}
