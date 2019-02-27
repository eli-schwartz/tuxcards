/***************************************************************************
                          CIconSelectorItem.cpp  -  description
                             -------------------
    begin                : Sat Jun 16 2007
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

#include "CIconSelectorItem.h"

#include "../../../global.h"
#include <QFileInfo>
#include <QPainter>
#include <QDir>
#include <QMimeSource>
#include <QApplication>

static QPixmap *static_pIconFolderLocked = 0;
static QPixmap *static_pIconFolder       = 0;
static QPixmap *static_pIconFile         = 0;
static QPixmap *static_pIconLink         = 0;

//static void cleanup()
//{
//    DELETE( static_pIconFolderLocked );
//    DELETE( static_pIconFolder );
//    DELETE( static_pIconFile );
//    DELETE( static_pIconLink );
//}


// -------------------------------------------------------------------------------
CIconSelectorItem::CIconSelectorItem( QListWidget* pParent, QFileInfo* pFI )
 : QListWidgetItem( pParent )
 , mpItemFileInfo( NULLPTR )
// -------------------------------------------------------------------------------
{
   createStaticIcons();

   if ( pFI )
   {
      setText( pFI->fileName() );
      msItemFileName = pFI->filePath();
      mpItemFileInfo = pFI;
   }

   if ( mpItemFileInfo && mpItemFileInfo->isSymLink() )
   {
      // set font to italic
      QFont f( font() );
      f.setItalic( TRUE );
      setFont( f );
   }

   setSizeHint( QSize( 60,60 ) );
   setIcon();
}

// -------------------------------------------------------------------------------
void CIconSelectorItem::createStaticIcons()
// -------------------------------------------------------------------------------
{
   if ( !static_pIconFolderLocked )
   {
      static_pIconFolderLocked = new QPixmap( QApplication::style()->standardIcon( QStyle::SP_DirClosedIcon ).pixmap(22,22) );
      static_pIconFolder       = new QPixmap( QApplication::style()->standardIcon( QStyle::SP_DirIcon ).pixmap(22,22) );
      static_pIconFile         = new QPixmap( QApplication::style()->standardIcon( QStyle::SP_FileIcon ).pixmap(22,22) );
      static_pIconLink         = new QPixmap( QApplication::style()->standardIcon( QStyle::SP_FileLinkIcon ).pixmap(22,22) );
   }
}

// -------------------------------------------------------------------------------
bool CIconSelectorItem::isDir() const
// -------------------------------------------------------------------------------
{
   if ( !mpItemFileInfo )
      return FALSE;
      
   return mpItemFileInfo->isDir();
}

// -------------------------------------------------------------------------------
bool CIconSelectorItem::isLink() const
// -------------------------------------------------------------------------------
{
   if ( !mpItemFileInfo )
      return FALSE;
      
   return mpItemFileInfo->isSymLink();
}


// -------------------------------------------------------------------------------
void CIconSelectorItem::setIcon()
// -------------------------------------------------------------------------------
{
   QPixmap* pPixmap = 0;

   // determine icon
   if ( isDir() )
   {
      // directory
      if( !QDir(msItemFileName).isReadable() )
         pPixmap = static_pIconFolderLocked;
      else
         pPixmap = static_pIconFolder;
   }
   else if ( isLink() )
   {
      // link
      pPixmap = static_pIconLink;
   }
   else
   {
      // file
      QString s = msItemFileName.right(4).toLower();
      if( (s==".xpm"  || s==".gif" || s==".png" || s==".jpg"
                      || s==".bmp" || s==".xbm" || s==".pnm"
                      || s=="jpeg" || s==".mng" )
                      && QFile(msItemFileName).size()<10000 )
      {
         pPixmap = new QPixmap(msItemFileName);
      }
      else
      {
         pPixmap = static_pIconFile;
      }
   } // if done ( icon selected or created )
   

   Q_ASSERT( NULLPTR != pPixmap );
   if ( !pPixmap )
      return;
      
   QListWidgetItem::setIcon( QIcon( *pPixmap ) );
}

// -------------------------------------------------------------------------------
CIconSelectorItem::~CIconSelectorItem()
// -------------------------------------------------------------------------------
{
   DELETE( mpItemFileInfo );
}
