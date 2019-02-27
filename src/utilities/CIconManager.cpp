/***************************************************************************
                          CIconManager.cpp  -  description
                             -------------------
    begin                : Tue May 01 2004
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

#include "CIconManager.h"
#include "../global.h"
#include <QFile>
#include <QPixmap>
#include <QApplication>
#include <QStyle>

#include "../icons/exit.xpm"
#include "../icons/addTreeElement.xpm"
#include "../icons/changeProperty.xpm"
#include "../icons/delete.xpm"
#include "../icons/lock.xpm"
#include "../icons/unlock.xpm"

#include "../icons/text_color.xpm"

#include "../icons/upArrow.xpm"
#include "../icons/downArrow.xpm"
#include "../icons/back.xpm"            // leftArrow
#include "../icons/forward.xpm"         // rightArrow


// -------------------------------------------------------------------------------
CIconManager::CIconManager()
// -------------------------------------------------------------------------------
  : mStyleIconMap()
  , mDefaultIconMap()
  , mAlternativeIconMap()
  , msIconDirectory()
  , mFileEndingList()
{
   buildupStyleIconMap();
   buildupDefaultIconMap();
   buildupListWithPossibleFileEndings();
}


// -------------------------------------------------------------------------------
CIconManager& CIconManager::getInstance()
// -------------------------------------------------------------------------------
{
   static CIconManager instance;
   return instance;
}

// -------------------------------------------------------------------------------
void CIconManager::buildupStyleIconMap()
// -------------------------------------------------------------------------------
{
   mStyleIconMap["back"]             = QApplication::style()->standardIcon( QStyle::SP_ArrowBack );
   mStyleIconMap["forward"]          = QApplication::style()->standardIcon( QStyle::SP_ArrowForward );
}

// -------------------------------------------------------------------------------
void CIconManager::buildupDefaultIconMap()
// -------------------------------------------------------------------------------
{
   Q_INIT_RESOURCE(icons);

   mDefaultIconMap["lo32-app-tuxcards"] = QIcon( ":/lo32-app-tuxcards.png" );
   mDefaultIconMap["fileopen"]          = QIcon( ":/fileopen.png"  );
   mDefaultIconMap["filenew"]           = QIcon( ":/filenew.png"   );
   mDefaultIconMap["filesave"]          = QIcon( ":/filesave.png"  );
   mDefaultIconMap["fileprint"]         = QIcon( ":/fileprint.png" );
   mDefaultIconMap["exit"]              = QPixmap(exit_xpm);
   mDefaultIconMap["addTreeElement"]    = QPixmap(addTreeElement_xpm);
   mDefaultIconMap["changeProperty"]    = QPixmap(changeProperty_xpm);
   mDefaultIconMap["delete"]            = QPixmap(delete_xpm);
   mDefaultIconMap["lock"]              = QPixmap(lock_xpm);
   mDefaultIconMap["unlock"]            = QPixmap(unlock_xpm);
   mDefaultIconMap["find"]              = QIcon( ":/find.png" );

   mDefaultIconMap["redo"]              = QIcon( ":/editredo.png" );
   mDefaultIconMap["undo"]              = QIcon( ":/editundo.png" );
   mDefaultIconMap["editcut"]           = QIcon( ":/editcut.png" );
   mDefaultIconMap["editcopy"]          = QIcon( ":/editcopy.png" );
   mDefaultIconMap["editpaste"]         = QIcon( ":/editpaste.png" );

   mDefaultIconMap["text_bold"]         = QIcon( ":/textbold.png" );
   mDefaultIconMap["text_italic"]       = QIcon( ":/textitalic.png" );
   mDefaultIconMap["text_under"]        = QIcon( ":/textunder.png" );
   mDefaultIconMap["text_color"]        = QPixmap(text_color_xpm);
   mDefaultIconMap["text_left"]         = QIcon( ":/textleft.png" );
   mDefaultIconMap["text_center"]       = QIcon( ":/textcenter.png" );
   mDefaultIconMap["text_right"]        = QIcon( ":/textright.png" );
   mDefaultIconMap["text_block"]        = QIcon( ":/textjustify.png" );

   mDefaultIconMap["upArrow"]           = QPixmap(upArrow_xpm);
   mDefaultIconMap["downArrow"]         = QPixmap(downArrow_xpm);
   mDefaultIconMap["back"]              = QPixmap(back_xpm);
   mDefaultIconMap["forward"]           = QPixmap(forward_xpm);
}


// -------------------------------------------------------------------------------
void CIconManager::buildupListWithPossibleFileEndings()
// -------------------------------------------------------------------------------
{
   mFileEndingList.append(".xpm");
   mFileEndingList.append(".png");
   mFileEndingList.append(".gif");
   mFileEndingList.append(".jpg");
   mFileEndingList.append(".bmp");
   mFileEndingList.append(".xbm");
   mFileEndingList.append(".pnm");
   mFileEndingList.append(".mng");
   mFileEndingList.append(".jpeg");
}


// -------------------------------------------------------------------------------
// Sets the directory where to get the icons from.
// -------------------------------------------------------------------------------
void CIconManager::setIconDirectory( const QString& sDir )
// -------------------------------------------------------------------------------
{
   msIconDirectory = sDir;
}


// -------------------------------------------------------------------------------
QString CIconManager::testExistenceOfFile( const QString& sFileName ) const
// -------------------------------------------------------------------------------
{
   for ( int i = 0; i < mFileEndingList.size(); i++ )
   {
      if( QFile(sFileName + mFileEndingList[i]).exists() )
      {
         return sFileName + mFileEndingList[i];
      }
   }

   return "";
}

// -------------------------------------------------------------------------------
const QIcon& CIconManager::getIcon( const QString& sFileName )
// -------------------------------------------------------------------------------
{
   // If an icon is already loaded, return that one.
   if ( !mAlternativeIconMap[sFileName].isNull() )
   {
      return mAlternativeIconMap[sFileName];
   }

   // Get icon from current style if possible. QStyle only supports "back" and "forward".
   if ( mStyleIconMap.contains(sFileName) )
      return mStyleIconMap[sFileName];


   // Otherwise, look whether a valid file for the icons does exist.
   QString sResult = testExistenceOfFile( msIconDirectory + "/" + sFileName );
   if ( !sResult.isEmpty() )
   {
      mAlternativeIconMap[sFileName] = QPixmap(sResult);
      return mAlternativeIconMap[sFileName];
   }

   // return default icon
   return mDefaultIconMap[sFileName];
}
