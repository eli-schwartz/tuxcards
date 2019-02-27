/***************************************************************************
                          CPropertyDialog.cpp  -  description
                             -------------------
    begin                : Tue Mar 28 2000
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
#include "../../../icons/blank.xpm"


#include "CPropertyDialog.h"
#include "../../../global.h"

#include "../../../information/CTreeInformationElement.h"
#include "../../../CTuxCardsConfiguration.h"
#include "../dateInputDialog/CDateInputDialog.h"
#include "../iconselectordialog/CIconSelectorDialog.h"

#include <qmessagebox.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <QPixmap>
#include <QColorDialog>
#include <QPainter>

CPropertyDialog* CPropertyDialog::static_pInstance = NULLPTR;

// -------------------------------------------------------------------------------
CPropertyDialog::CPropertyDialog( QWidget* pParent/*,
                                  CTuxCardsConfiguration& refTuxConfiguration*/ )
 : QDialog( pParent )
 , mExpiryDate()
 , mpDateInputDialog( NULLPTR )
 , mBlankIcon( blank_xpm )
 , mpIconSelectorDialog( NULLPTR )
 , miMode( MODE_NONE )
 , miChoice( 0 )
 , mpEditingElement( NULLPTR )
 , mDescriptionColor( 0,0,0 )
// , mrefTuxConfiguration( refTuxConfiguration )
// -------------------------------------------------------------------------------
{
   ui.setupUi( this );
   setModal( TRUE );
   
   connect( ui.mpChangeIconButton, SIGNAL(clicked()), this, SLOT(chooseIcon()) );
   connect( ui.mpChangeExpiryDateButton, SIGNAL(clicked()), this, SLOT(openDateInputDialog()) );
   connect( ui.mpDescriptionColorButton, SIGNAL(clicked()), this, SLOT(slotChooseDescriptionColor()) );
   connect( this, SIGNAL(accepted()), this, SLOT(changeProperties()) );
   
//   connect( ui.mpEncryptionPanel, SIGNAL(clicked()), this, SLOT(checkEncryption()) );

   // initialize the date input dialog
   CDateInputDialog::createInstance( this );
   mpDateInputDialog = CDateInputDialog::getInstance();
   Q_ASSERT( NULLPTR != mpDateInputDialog );
   connect( mpDateInputDialog, SIGNAL(accepted()), this, SLOT(changeExpiryDate()) );
}


// -------------------------------------------------------------------------------
CPropertyDialog::~CPropertyDialog( void )
// -------------------------------------------------------------------------------
{
   mpEditingElement = NULLPTR;                 // do not kill this pointer
}


/**
 * Creates a property dialog instance with the given widget as parent.
 * This method should be called only once for the lifetime of the application.
 * Further calls are ignored.
 */
// -------------------------------------------------------------------------------
void CPropertyDialog::createInstance( QWidget* pParent )
// -------------------------------------------------------------------------------
{
   if ( !static_pInstance )
   {
      static_pInstance = new CPropertyDialog( pParent );
   }
}


/**
 * Returns the static instance.
 * If no property dialog was created with <code>createInstance()</code>,
 * then a <code>NULLPTR</code> is returned.
 */
// -------------------------------------------------------------------------------
CPropertyDialog* CPropertyDialog::getInstance()
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != static_pInstance );
   return static_pInstance;
}

// -------------------------------------------------------------------------------
int CPropertyDialog::getMode() const
// -------------------------------------------------------------------------------
{
   return miMode;
}


// -------------------------------------------------------------------------------
void CPropertyDialog::setUp( CInformationElement* pElement, int iMode )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == pElement )
      return;

   if ( MODE_CHANGE_PROPERTIES == iMode )
   {
      setWindowTitle( tr("Change Properties of existing Entry") );
      
      ui.mpParentLabel->setText( "" );      
      setAttributes( pElement->getDescription(), pElement->getIconFileName(),
                     pElement->expires(), pElement->getExpiryDate(),
                     pElement->getDescriptionColor()/*,
                     pElement->isEncryptionEnabled(),
                     pElement->isCurrentlyEncrypted()*/ );
      ui.mpTextFormatChoser->setEnabled( FALSE );
   }
   else if ( MODE_CREATE_NEW_ELEMENT == iMode )
   {
      setWindowTitle( tr("Add new Entry") );

      ui.mpParentLabel->setText( tr("Child from") + " '" + pElement->getDescription() + "'" );
      setAttributes( "", CInformationElement_NO_ICON );
      ui.mpTextFormatChoser->setEnabled( TRUE );
   }
   else
   {
      return;
   }


   miMode = iMode;
   mpEditingElement = pElement;

   show();

//   exec();
}


// -------------------------------------------------------------------------------
void CPropertyDialog::setAttributes( QString sDescription, QString sIconFilename,
                                     bool bExpires, QDate expiryDate,
                                     const QColor& desColor/*,
                                     bool bIsEncryptionEnabled,
                                     bool isCurrentlyEncrypted*/ )
// -------------------------------------------------------------------------------
{
   if ( sIconFilename == CInformationElement_NO_ICON )
   {
      // entry without icon
      ui.mpUseIconGroupBox->setChecked( FALSE );
      ui.mpIconLabel->setPixmap( mBlankIcon );
   }
   else
   {
      // entry with icon
      ui.mpUseIconGroupBox->setChecked( TRUE );
      ui.mpIconLabel->setPixmap( QPixmap::fromImage( QImage(sIconFilename) ) );
   }
   ui.mpLocationLabel->setText( sIconFilename );
   
   ui.mpNameLine->setText(sDescription);

   setDescriptionColor( desColor );
   
   ui.mpExpiryDateGroupBox->setChecked( bExpires );
   mExpiryDate = expiryDate;
   ui.mpExpiryDateLabel->setText( mExpiryDate.toString("dddd, d MMMM yyyy") );

/*   if ( bIsEncryptionEnabled )
   {
      ui.mpEncryptionPanel->setChecked( true );
      ui.mpEncryptionPanel->setEnabled( !isCurrentlyEncrypted );
//      mpPasswdLineOne->setText( "*******" );
//      mpPasswdLineTwo->setText( "*******" );

      mpPasswdLineOne->setEnabled( false );
      mpPasswdLineTwo->setEnabled( false );
   }
   else
   {
      ui.mpEncryptionPanel->setChecked( false );
   }
*/
}



/**
 * Open an icon slector dialog to let user select his icon.
 * If the dialog is not created it will be initialized.
 */
// -------------------------------------------------------------------------------
void CPropertyDialog::chooseIcon()
// -------------------------------------------------------------------------------
{
   if ( !mpIconSelectorDialog )
   {
      // initialize the icon selector dialog
      mpIconSelectorDialog = /*new CIconSelectorDialog( this ); //*/CIconSelectorDialog::getInstance( this );
      Q_ASSERT( NULLPTR != mpIconSelectorDialog );
      if ( !mpIconSelectorDialog )
         return;
         
      connect( mpIconSelectorDialog, SIGNAL(accepted()), this, SLOT(iconSelected()) );
   }

   mpIconSelectorDialog->show();
}

// -------------------------------------------------------------------------------
void CPropertyDialog::iconSelected()
// -------------------------------------------------------------------------------
{
   if ( !mpIconSelectorDialog )
      return;
      
      
   QString sIconFileName = mpIconSelectorDialog->getIconFileName();

   if( sIconFileName.isEmpty() )
      return;
      
   // if 'sIconFileName' is a valid fileName, test whether it is a valid Pixmap
   QPixmap pix( sIconFileName );
   if ( ! pix.isNull() )
   {
      ui.mpIconLabel->setPixmap( pix );
      ui.mpLocationLabel->setText( sIconFileName );
   }
}

// -------------------------------------------------------------------------------
void CPropertyDialog::slotChooseDescriptionColor()
// -------------------------------------------------------------------------------
{
   QColor c = QColorDialog::getColor( getDescriptionColor() );
   if( !c.isValid() ) return;

   setDescriptionColor( c );
}

// -------------------------------------------------------------------------------
void CPropertyDialog::setDescriptionColor( const QColor& c )
// -------------------------------------------------------------------------------
{
   mDescriptionColor = c;
   
   if ( !ui.mpDescriptionColorButton )
      return;
      
   int iWidth  = 24;
   int iHeight = 24;
   QPixmap pixmap( iWidth, iHeight );
   
   QPainter p;
   p.begin( &pixmap );
   p.fillRect( 0,0, iWidth,iHeight, mDescriptionColor );
   p.end();
   
   ui.mpDescriptionColorButton->setIcon( pixmap );

//    QPalette p = ui.mpDescriptionColorButton->palette();
//    p.setColor( QPalette::ButtonText, mDescriptionColor );
//    ui.mpDescriptionColorButton->setPalette( p );
}

// -------------------------------------------------------------------------------
QString CPropertyDialog::getName() const
// -------------------------------------------------------------------------------
{ return ui.mpNameLine->text(); }


// -------------------------------------------------------------------------------
QString CPropertyDialog::getIconFileName() const
// -------------------------------------------------------------------------------
{
   if ( ui.mpUseIconGroupBox->isChecked() )
      return ui.mpLocationLabel->text();
   else
      return CInformationElement_NO_ICON;
}


// -------------------------------------------------------------------------------
InformationFormat* CPropertyDialog::getInformationFormat() const
// -------------------------------------------------------------------------------
{
   return InformationFormat::getByString( ui.mpTextFormatChoser->currentText() );
}

// -------------------------------------------------------------------------------
QColor CPropertyDialog::getDescriptionColor() const
// -------------------------------------------------------------------------------
{
   return mDescriptionColor; //ui.mpDescriptionColorButton->palette().color( QPalette::ButtonText );
}

// -------------------------------------------------------------------------------
bool CPropertyDialog::hasExpiryDateSet() const
// -------------------------------------------------------------------------------
{
   return ui.mpExpiryDateGroupBox->isChecked();
}

// -------------------------------------------------------------------------------
QDate CPropertyDialog::getExpiryDate() const
// -------------------------------------------------------------------------------
{
   return mExpiryDate;
}


/**
 *  This slot is called when the apply-Button is pressed.
 *  The set attributes are applied to the currently edited informationElement.
 */
// -------------------------------------------------------------------------------
void CPropertyDialog::changeProperties()
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == mpEditingElement )
      return;

//   if ( (NULLPTR == mpPasswdLineOne) || (NULLPTR == mpPasswdLineTwo) )
//      return;

   if ( getName().trimmed().isEmpty() )
   {
      int iAnswer = QMessageBox::warning( this, "TuxCards", tr("The name of your entry is empty.\n"
                                          "Do you want to change this?"),
                                          QMessageBox::Yes, QMessageBox::No );
      if ( QMessageBox::Yes == iAnswer )
      {
         show();
         return;
      }
   }

/*
   // is set to 'true' if the encryption mode is changed
   bool bEncryptionModeChanged = false;
   // is set to 'true' if entry should be encrypted;
   // this is only important if 'bEncryptionModeChanged' is 'true'
   bool bEncrypt = false;
   if ( NULLPTR != ui.mpEncryptionPanel )
   {
      if ( mpEditingElement->isEncryptionEnabled() != ui.mpEncryptionPanel->isChecked() )
      {
         bEncryptionModeChanged = true;

         if ( ui.mpEncryptionPanel->isChecked() )
         {
            if ( mpPasswdLineOne->text().stripWhiteSpace().isEmpty() )
            {
               (void) QMessageBox::warning( this, "TuxCards", "Encryption is turned on, but\n"
                                                "no password is entered.",
                                                "Change it." );
               show();
               return;
            }

            if ( 0 != mpPasswdLineOne->text().stripWhiteSpace().compare(
                                 mpPasswdLineTwo->text().stripWhiteSpace())
               )
            {
               (void) QMessageBox::warning( this, "TuxCards", "Passwords do not match.",
                                                "Change it." );
               mpPasswdLineOne->setText("");
               mpPasswdLineTwo->setText("");
               show();
               return;
            }

            bEncrypt = true;
         }
         else
         {
//             if ( tmpPasswd == "" )
//             {
//                QMessageBox::information("Geht nicht, "
//                "encryption can only be removed if entry is decrypted.");
//
//                ui.mpEncryptionPanel->setChecked( false );
//                bEncrypt = false;
//             }

         }
      }
   }
*/


   if ( miMode == MODE_CHANGE_PROPERTIES )
   {
      // change properties: name & icon
      mpEditingElement->setBatched( TRUE );
      mpEditingElement->setDescription( getName() );
      mpEditingElement->setIconFileName( getIconFileName() );
      mpEditingElement->setDescriptionColor( getDescriptionColor() );
      mpEditingElement->setExpiryDate( ui.mpExpiryDateGroupBox->isChecked(), mExpiryDate );
      mpEditingElement->setBatched( FALSE );

      // only do something if encryption mode has changed
//      if ( bEncryptionModeChanged )
//         mpEditingElement->enableEncryption( bEncrypt, mpPasswdLineOne->text().stripWhiteSpace() );
   }
   else if ( miMode == MODE_CREATE_NEW_ELEMENT )
   {
      emit( dataSuccessfullyEntered() );

      // only do something if encryption mode has changed
//   if ( bEncryptionModeChanged )
//       pNewElement->enableEncryption( bEncrypt, mpPasswdLineOne->text().stripWhiteSpace() );
   }

   close();
}


// -------------------------------------------------------------------------------
void CPropertyDialog::openDateInputDialog()
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpDateInputDialog );
   if ( !mpDateInputDialog )
      return;
   
   mpDateInputDialog->setUp( mExpiryDate );
}

// -------------------------------------------------------------------------------
void CPropertyDialog::changeExpiryDate()
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpDateInputDialog );
   if ( !mpDateInputDialog )
      return;
   
   mExpiryDate = mpDateInputDialog->getSelectedDate();
   ui.mpExpiryDateLabel->setText( mExpiryDate.toString("dddd, d MMMM yyyy") );
}


/*
// -------------------------------------------------------------------------------
void CPropertyDialog::checkEncryption()
// -------------------------------------------------------------------------------
{
   if ( (!ui.mpEncryptionPanel) || (!ui.mpEncryptionPanel->isEnabled()) )
      return;

   if ( !mrefTuxConfiguration.askForUsingEncryption() )
   {
      ui.mpEncryptionPanel->setChecked( false );
   }
}
*/
