/***************************************************************************
                          CIconManager.h  -  description
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

#ifndef CICON_MANAGER_H
#define CICON_MANAGER_H

#include <QStringList>
#include <QIcon>
#include <QMap>

class CIconManager {
public:

   static CIconManager&  getInstance();
   void                  setIconDirectory( const QString& sDir );

   const QIcon&    getIcon( const QString& sFileName );


private:
   CIconManager();
   void            buildupStyleIconMap();
   void            buildupDefaultIconMap();
   void            buildupListWithPossibleFileEndings();

   QString         testExistenceOfFile( const QString& sFileName ) const;

   typedef QMap<QString, QIcon> IconMap;
   IconMap         mStyleIconMap;            // from the current desktop style
   IconMap         mDefaultIconMap;
   IconMap         mAlternativeIconMap;

   QString         msIconDirectory;
   QStringList     mFileEndingList;
};

#endif
