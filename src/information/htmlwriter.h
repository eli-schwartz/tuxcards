/***************************************************************************
                          htmlwriter.h  -  description
                             -------------------
    begin                : Son Jul 28 2002
    copyright            : (C) 2002 by Alexander Theel
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

#ifndef HTMLWRITER_H
#define HTMLWRITER_H

#include <qstring.h>
#include <qdir.h>
#include "CInformationCollection.h"

#include <qstringlist.h>

/**
  *@author Alexander Theel
  */

class HTMLWriter {
public:
   HTMLWriter();

   static bool writeCollectionToHTMLFile( CInformationCollection& collection, const QString& dirPath );


private:
   static void createIndexFile( const QDir& dir, const QString& sRootDescription );
   static void createTreeFrame( CInformationCollection& collection, const QDir& dir );
   static void convertInformationElementToHTML( CInformationElement& element,
                                                const QDir& dir, QString& sTreeHTML,
                                                int iTabCount = 0 );
   static void createTopFrame( const QDir& dir );

   static bool createSubDir( const QDir& dir );

   static bool createImages( const QDir& dir );

   static const QString TUX_HTMLEXPORT_SUBDIR;
   static const QString TUX_SPACER;
   static const QString TUX_DOT;
   static const QString USE_UTF8_CHARSET;

   static QStringList  static_List;
};

#endif
