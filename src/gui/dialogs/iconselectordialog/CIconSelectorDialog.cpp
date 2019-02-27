/***************************************************************************
                          CIconSelectorDialog.cpp  -  description
                             -------------------
    begin                : Sun Jun 10 2007
    copyright            : (C) 2007 by Alexander Theel
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


#include "CIconSelectorDialog.h"

#include "../../../global.h"
#include "CIconSelectorItem.h"


CIconSelectorDialog* CIconSelectorDialog::static_pInstance = NULLPTR;

// -------------------------------------------------------------------------------
CIconSelectorDialog::CIconSelectorDialog( QWidget* pParent )
 : QDialog ( pParent )
// ------------------------------------------------------------------------------- 
{
   ui.setupUi( this );
   ui.pDirectoryUpButton->setIcon( QApplication::style()->standardIcon( QStyle::QStyle::SP_FileDialogToParent ) );

   connect( ui.pDirectoryUpButton, SIGNAL( clicked() ), this, SLOT( cdUp() ) );
   connect( ui.pList, SIGNAL( itemDoubleClicked(QListWidgetItem*) ), this, SLOT( itemDoubleClicked(QListWidgetItem*) ) );
   connect( ui.pList, SIGNAL( itemClicked(QListWidgetItem*) ), this, SLOT( fileSelected(QListWidgetItem*) ) );
   
   readDirectory( QDir::current().absolutePath() );
}

// -------------------------------------------------------------------------------
CIconSelectorDialog* CIconSelectorDialog::getInstance( QWidget* pParent )
// -------------------------------------------------------------------------------
{
   if ( !static_pInstance )
      static_pInstance = new CIconSelectorDialog( pParent );

   return static_pInstance;
}


// -------------------------------------------------------------------------------
void CIconSelectorDialog::cdUp()
// -------------------------------------------------------------------------------
{
   mCurrentDir.cdUp();
   readDirectory( mCurrentDir );
}



// -------------------------------------------------------------------------------
void CIconSelectorDialog::directoryChanged( const QString& sDir )
// -------------------------------------------------------------------------------
{
   mCurrentDir = QDir( sDir );
   setPathForComboBox();
}


// -------------------------------------------------------------------------------
QString CIconSelectorDialog::getIconFileName() const
// -------------------------------------------------------------------------------
{
   if( ui.pIconNameLineEdit->text().isEmpty() )       // i.e. no file was selected
      return "";
   else
      return mCurrentDir.absolutePath() +"/"+ ui.pIconNameLineEdit->text();
}


// -------------------------------------------------------------------------------
void CIconSelectorDialog::fileSelected( QListWidgetItem* pItem )
// -------------------------------------------------------------------------------
{
   CIconSelectorItem* pISItem = dynamic_cast<CIconSelectorItem*>( pItem );
   
   Q_ASSERT( (NULLPTR != pISItem) && (NULLPTR != ui.pIconNameLineEdit) );
   if ( !pISItem || !ui.pIconNameLineEdit )
      return;
      
   if ( pISItem->isDir() || pISItem->isLink() )
      return;
      
   ui.pIconNameLineEdit->setText( pISItem->text() );
}

// -------------------------------------------------------------------------------
void CIconSelectorDialog::setPathForComboBox()
// -------------------------------------------------------------------------------
{
   QString sDir = mCurrentDir.absolutePath();
   int iIndex = ui.pDirectoryComboBox->findText( sDir );
   
   if ( -1 == iIndex )
   {
      ui.pDirectoryComboBox->addItem( sDir );
      iIndex = ui.pDirectoryComboBox->count() - 1;;
   }

   ui.pDirectoryComboBox->setCurrentIndex( iIndex );
}


// -------------------------------------------------------------------------------
void CIconSelectorDialog::itemDoubleClicked( QListWidgetItem* pItem )
// -------------------------------------------------------------------------------
{
   CIconSelectorItem* pISItem = dynamic_cast<CIconSelectorItem*>( pItem );
   
   Q_ASSERT( NULLPTR != pISItem );
   if ( !pISItem )
      return;

   if ( pISItem->isDir() )
   {
      readDirectory( QDir( pISItem->filename() ) );
   }
   else if ( pISItem->isLink()
             && QFileInfo( QFileInfo( pISItem->filename() ).readLink() ).isDir() )
   {
      readDirectory( QDir( QFileInfo( pISItem->filename() ).readLink() ) );
   }
}


// -------------------------------------------------------------------------------
void CIconSelectorDialog::progressBar_startReadDir( int iDirCount )
// -------------------------------------------------------------------------------
{
   if ( !ui.pProgressBar )
      return;
      
   ui.pProgressBar->reset();
   ui.pProgressBar->setMaximum( iDirCount );
   ui.pProgressBar->setVisible( TRUE );
}

// -------------------------------------------------------------------------------
void CIconSelectorDialog::progressBar_readNextDir()
// -------------------------------------------------------------------------------
{
   if ( !ui.pProgressBar )
      return;
 
   int i = ui.pProgressBar->value();
   ui.pProgressBar->setValue( ++i );
}

// -------------------------------------------------------------------------------
void CIconSelectorDialog::progressBar_readDirDone()
// -------------------------------------------------------------------------------
{
   if ( !ui.pProgressBar )
      return;
      
   ui.pProgressBar->setValue( ui.pProgressBar->maximum() );
   ui.pProgressBar->setVisible( FALSE );
}




/**
 * Reads the content of the given directory and displays it.
 */
// -------------------------------------------------------------------------------
void CIconSelectorDialog::readDirectory( const QDir& dir )
// -------------------------------------------------------------------------------
{
   if( !dir.isReadable() )
      return;

   if( dir.isRoot() )
      ui.pDirectoryUpButton->setEnabled( FALSE );
   else
      ui.pDirectoryUpButton->setEnabled( TRUE );

   ui.pList->clear();
   directoryChanged( dir.absolutePath() );
   
   const QFileInfoList& filist = dir.entryInfoList( QDir::NoFilter, QDir::DirsFirst | QDir::Name );
   
   progressBar_startReadDir( filist.count() );
   
   QListIterator<QFileInfo> it( filist );
   
   QFileInfo fi;
   while( it.hasNext() )
   {
         fi = it.next();
         if( fi.fileName() == ".." && ( fi.absolutePath() == "/" || fi.absolutePath().isEmpty() ) )
            continue;
      
         progressBar_readNextDir();
         (void) new CIconSelectorItem( ui.pList, new QFileInfo( fi ) );
      }//while
   
   progressBar_readDirDone();
}
