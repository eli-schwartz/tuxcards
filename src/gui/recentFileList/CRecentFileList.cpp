/***************************************************************************
                          CRecentFileList.cpp  -  description
                             -------------------
    begin                : Sun Feb 09 2003
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

#include "CRecentFileList.h"
#include "../../global.h"

//#include <iostream>
#include <qfile.h>
#include <qmessagebox.h>

#include <qcombobox.h>
#include <QMenu>

const QString CRecentFileList::SEPARATOR         = ",";
const uint    CRecentFileList::MAX_ELEMENT_COUNT =   5;

/**
 * Constructor
 */
// -------------------------------------------------------------------------------
CRecentFileList::CRecentFileList( QWidget* pParent, QMenu* pParentMenu,
                                  const QString& sFiles )
 : QObject()
 , mpParentMenu( 0 )
 , mpRecentlyFilesMenu( 0 )
 , mpComboBox( 0 )
// -------------------------------------------------------------------------------
{
   mpRecentlyFilesMenu = new QMenu( tr("Recently Used Files") );
   pParentMenu->addMenu( mpRecentlyFilesMenu );

   createActions( pParent );

   setList( sFiles );
   update();
}

// -------------------------------------------------------------------------------
void CRecentFileList::createActions( QWidget* pParent )
// -------------------------------------------------------------------------------
{
   QAction* pAction;
   for ( uint i = 0; i < MAX_ELEMENT_COUNT; i++ )
   {
        pAction = new QAction( pParent );
        pAction->setVisible( false );
        connect( pAction, SIGNAL(triggered()), this, SLOT(slotOpenRecentFile()) );
        
        mpRecentlyFilesMenu->addAction( pAction );
        mRecentFileActs.append( pAction );
   }
}

/**
 * Sets the given string on top of the list. If the string
 * is already within the list, it is moved to the first position.
 */
// -------------------------------------------------------------------------------
void CRecentFileList::setOnTop( const QString& sAbsPath )
// -------------------------------------------------------------------------------
{
   remove( sAbsPath );
   mFileList.prepend( sAbsPath );
   
   update();
}




/**
 * Updates the corresponding recent file list.
 *
 */
// -------------------------------------------------------------------------------
void CRecentFileList::setList( const QString& sFiles )
// -------------------------------------------------------------------------------
{
   if ( sFiles.isEmpty() )
   {
      mFileList.clear();
      return;
   }

   mFileList = sFiles.split( SEPARATOR );
}



// -------------------------------------------------------------------------------
void CRecentFileList::update()
// -------------------------------------------------------------------------------
{
   checkSize();
   updateMenu();
   updateComboBox();
}



/**
 * Makes sure that the file list is limited to MAX_ELEMENT_COUNT.
 * Any additional elements will be removed from the list.
 *
 */
// -------------------------------------------------------------------------------
void CRecentFileList::checkSize()
// -------------------------------------------------------------------------------
{
   while ((uint)mFileList.count() > MAX_ELEMENT_COUNT)
   {
      mFileList.removeLast();
   }
}





/**
 * Updates the corresponding recent menu.
 *
 */
// -------------------------------------------------------------------------------
void CRecentFileList::updateMenu()
// -------------------------------------------------------------------------------
{
   // set the recent files as visible
   for ( int i=0; i < mFileList.count(); i++ )
   {
      QString sText = QString("&%1  %2").arg(i + 1).arg( mFileList[i] );
      mRecentFileActs[i]->setText( sText );
      mRecentFileActs[i]->setData( mFileList[i] );
      mRecentFileActs[i]->setVisible( true );
   }

   // set the remaining items as not visible
   for ( uint j = mFileList.count(); j < MAX_ELEMENT_COUNT; ++j )
      mRecentFileActs[j]->setVisible( false );
}


// -------------------------------------------------------------------------------
void CRecentFileList::slotOpenRecentFile()
// -------------------------------------------------------------------------------
{
   QAction* pAction = qobject_cast<QAction*>( sender() );
   if ( !pAction )
      return;
      
      
   QString sFileName = pAction->data().toString();

   if ( QFile::exists(sFileName) )
   {
      emit openFile(sFileName);
      setOnTop(sFileName);
   }
   else
   {
      if (QMessageBox::Yes == QMessageBox::warning(0, "File not found",
                      "The file '"+sFileName+"' does not exist.\n"
                      "Do you want to remove it from the recent file menu?",
                      QMessageBox::Yes, QMessageBox::No))
      {
         remove( sFileName );
         update();
      }
   }
}


/**
 * Removes the specified file from the list and updates the
 * recent file menu.
 */
// -------------------------------------------------------------------------------
void CRecentFileList::remove( const QString& sAbsPath )
// -------------------------------------------------------------------------------
{
   if ( mFileList.contains( sAbsPath ) )
   {
      int iIndex = mFileList.indexOf( sAbsPath );
      if ( -1 != iIndex )
         mFileList.removeAt( iIndex );
      //std::cout<<" file removed; new list = "<<toString()<<std::endl;
   }
}


/**
 * Creates a comboBox with the current recent file list and embeds the
 * comboBox into the given parent widget <code>parentWidget</code>.
 * As long as the comboBox exist, it is updated.
 *
 * Please note: If a comboBox did already exist then the old one is <b>deleted</b>.
 */
// -------------------------------------------------------------------------------
void CRecentFileList::createComboBox( QWidget& parentWidget )
// -------------------------------------------------------------------------------
{
   if ( mpComboBox )
      DELETE( mpComboBox );
      
   mpComboBox = new QComboBox( &parentWidget );
   connect( mpComboBox, SIGNAL( activated( int ) ),
            this, SLOT( slotRecenlyOpenedFilesActivated( int ) ) );

   updateComboBox();
}

/**
 * Updates the data within the comboBox (if a comboBox exists).
 */
// -------------------------------------------------------------------------------
void CRecentFileList::updateComboBox()
// -------------------------------------------------------------------------------
{
   if ( !mpComboBox )
      return;
      
   mpComboBox->clear();
   mpComboBox->addItems( mFileList );
}


// -------------------------------------------------------------------------------
QString CRecentFileList::toString() const
// -------------------------------------------------------------------------------
{
   return mFileList.join( SEPARATOR );
}

