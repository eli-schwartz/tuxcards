/***************************************************************************
                          Path.h  -  description
                             -------------------
    begin                : Fri Jan 09 2002
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

#ifndef PATH_H
#define PATH_H

#include <qstringlist.h>
#include "IParent.h"


class Path{

public:
   // Use a 'IParent'-ptr within the constructor; if we have a better
   // idea we might use 'CInformationElement*' again.
   Path( IParent* pElement );
   Path( const QString& sPathString );

   bool                     isEmpty( void ) const;
   QStringList              getPathList( void ) const;
   QString                  toString( void ) const;

private:
   QStringList              mPathList;

   static /*const*/ QString separator;

};
#endif
