/***************************************************************************
                          CPasswdDialog.cpp  -  description
                             -------------------
    begin                : Wed Jan 14 2004
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

#include "CPasswdDialog.h"

#include <qlineedit.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qpushbutton.h>

#include <iostream>

// -------------------------------------------------------------------------------
CPasswdDialog::CPasswdDialog( QWidget* pParent )
 : QDialog( pParent )
 , msPasswd( "" )
// -------------------------------------------------------------------------------
{
   ui.setupUi( this );
   setModal( TRUE );

   connect( ui.mpOkButton, SIGNAL(released()), this, SLOT(slotVerifyPasswd()) );
}

// -------------------------------------------------------------------------------
void CPasswdDialog::setUp( const QString& sIEDescription )
// -------------------------------------------------------------------------------
{
   if ( (NULLPTR == ui.mpIEDescription) || (NULLPTR == ui.mpPasswdLineOne)
        || (NULLPTR == ui.mpPasswdLineTwo) )
      return;

   ui.mpIEDescription->setText("'"+sIEDescription+"'");
   startPasswdInput();
   exec();        // Block application and call the dialog in modal mode.
}



// -------------------------------------------------------------------------------
void CPasswdDialog::slotVerifyPasswd()
// -------------------------------------------------------------------------------
{
   if ( (NULLPTR == ui.mpPasswdLineOne) || (NULLPTR == ui.mpPasswdLineTwo) )
      return;

   if ( ui.mpPasswdLineOne->text().trimmed().isEmpty() )
   {
      (void) QMessageBox::warning( this, "TuxCards", "No password is entered.",
                                         "Change it." );
      //std::cout<<"no passwd"<<std::endl;
      startPasswdInput();
      return;
   }

   if ( 0 != ui.mpPasswdLineOne->text().trimmed().compare(
                        ui.mpPasswdLineTwo->text().trimmed())
      )
   {
      (void) QMessageBox::warning( this, "TuxCards", "Passwords do not match.",
                                         "Change it." );
      //std::cout<<"passwds do not match"<<std::endl;
      startPasswdInput();
      return;
   }

   msPasswd = ui.mpPasswdLineOne->text().trimmed();
   //std::cout<<"CPasswdDialog::done() passwd="<<msPasswd<<std::endl;
   accept();
}


// -------------------------------------------------------------------------------
void CPasswdDialog::startPasswdInput()
// -------------------------------------------------------------------------------
{
   ui.mpPasswdLineOne->setText("");
   ui.mpPasswdLineTwo->setText("");
   msPasswd = "";

   ui.mpPasswdLineOne->setFocus();
   show();
}


// -------------------------------------------------------------------------------
QString CPasswdDialog::getPasswd()
// -------------------------------------------------------------------------------
{ return msPasswd; }
