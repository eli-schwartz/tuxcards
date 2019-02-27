/***************************************************************************
                          CEncryptedEntryView.cpp  -  description
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

#include "../../global.h"

#include "CEncryptedEntryView.h"
#include "./../../information/CInformationElement.h"
#include <iostream>

#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <QVBoxLayout>
#include <QHBoxLayout>

// -------------------------------------------------------------------------------
CEncryptedEntryView::CEncryptedEntryView( QWidget* pParent )
  : QWidget( pParent )
  , mpPasswdLine( NULLPTR )
  , mpResultLabel( NULLPTR )
  , mpActiveElement( NULLPTR )
// -------------------------------------------------------------------------------
{
   QVBoxLayout* pLayout = new QVBoxLayout( this );
   pLayout->setMargin( 10 );
   pLayout->setSpacing( 5 );
   
   QLabel* pLabel = new QLabel( this );
   mpPasswdLine  = new QLineEdit( this );
   mpResultLabel  = new QLabel( this );

   QHBoxLayout* pMiniLayout = new QHBoxLayout( );

   if ( (NULLPTR == pLayout) || (NULLPTR == pLabel)
        || (NULLPTR == mpPasswdLine) || (NULLPTR == mpResultLabel)
        || (NULLPTR == pMiniLayout) )
   {
      std::cout<<"Constructor 'CEncryptedEntryView': ERROR not enough memory "
               <<" to create objects!!!"<<std::endl;
      return;
   }


   pMiniLayout->addWidget( mpPasswdLine );

   pLayout->addStretch();
   pLayout->addWidget( pLabel );
   pLayout->addLayout( pMiniLayout );
   pLayout->addWidget( mpResultLabel );
   pLayout->addStretch();
   pLayout->addStretch();
   pLayout->addStretch();

   pLabel->setText( "<center>"
                    "<font size=+1><b>" + tr("Encrypted Entry") + "</b></font><br>"
                    "<i>" + tr("To view entry content enter password below and "
                    "press RETURN.") + "</i>"
                    "</center>");
   mpPasswdLine->setEchoMode( QLineEdit::Password );
   mpPasswdLine->setMaximumWidth( 200 );
   mpResultLabel->setText(" ");

   connect( mpPasswdLine, SIGNAL(returnPressed()), this, SLOT(slotPasswdEntered()) );
}


// -------------------------------------------------------------------------------
CEncryptedEntryView::~CEncryptedEntryView()
// -------------------------------------------------------------------------------
{
//   mpInformationElement = NULLPTR;
}


// ************** IView *********************************************************
// -------------------------------------------------------------------------------
void CEncryptedEntryView::aboutToRemoveElement( CInformationElement* pIE )
// -------------------------------------------------------------------------------
{
   if ( mpActiveElement == pIE )
   {
      mpActiveElement = NULLPTR;
   }
}
// ************** IView - End ****************************************************

// -------------------------------------------------------------------------------
void CEncryptedEntryView::activeInformationElementChanged( CInformationElement* pIE )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == pIE )
      return;

   if ( (NULLPTR == mpPasswdLine) || (NULLPTR == mpResultLabel) )
      return;

   mpPasswdLine->setText("");
   mpPasswdLine->setFocus();
   mpResultLabel->setText("");

   mpActiveElement = pIE;
}

// -------------------------------------------------------------------------------
void CEncryptedEntryView::slotPasswdEntered()
// -------------------------------------------------------------------------------
{
   if ( (NULLPTR == mpPasswdLine) || (NULLPTR == mpResultLabel) )
      return;

   if ( NULLPTR == mpActiveElement )
      return;

   // testing passwd
   if ( 0 == mpPasswdLine->text().trimmed().length() )
      return;

   bool bCorrectPasswd = mpActiveElement->decrypt( mpPasswdLine->text().trimmed() );

   if ( !bCorrectPasswd )
   {
      mpPasswdLine->setText( "" );
      mpResultLabel->setText( "<center><b>" + tr("Wrong Passwd. Try again.") + "</b></center>" );
   }
   else
   {
      // every thing is alright
      mpResultLabel->setText( " " );
      emit signalEntryDecrypted();
   }
}
