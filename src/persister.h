/***************************************************************************
                          persister.h  -  description
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
#ifndef PERSISTER_H
#define PERSISTER_H

#include <iostream>
#include <qstring.h>
#include "./information/CInformationCollection.h"
#include "./information/CTreeInformationElement.h"

class Persister{
public:
  // loading
	static CInformationCollection* createInformationCollection(QString s);
	
	// saving
  static void save(CInformationCollection*, QString filename);

private:
  // loading
  static CInformationCollection* unpack(QString s);
  static void unpackWithIcons(QString s, bool root, CTreeInformationElement* parent);
//  static void unpackWithoutIcons(QString s, bool root, CTreeInformationElement* parent);

};
#endif

