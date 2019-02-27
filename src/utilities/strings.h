/***************************************************************************
                          strings.h  -  description
                             -------------------
    begin                : Fri Jul 19 2002
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
#ifndef STRINGS_H
#define STRINGS_H

#include <qstring.h>

class Strings{
public:
  static QString spaces(int n=0);
  
  static int wordCount( QString sText="" );

  static QString removeAndReturnFirstLine(QString& s);
  
  static QString removeHTMLTags(const QString sText="");
};
#endif

