/***************************************************************************
                          informationformat.h  -  description
                             -------------------
    begin                : Sat Jul 13 2002
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
#ifndef CONVERTER_H
#define CONVERTER_H

#include <iostream>

#include <qstring.h>
#include "CInformationElement.h"

class Converter{
public:
   static void convert( CInformationElement& );

   static QString convertASCII2RTF( QString );
   static QString convertRTF2ASCII( QString );
};

#endif

