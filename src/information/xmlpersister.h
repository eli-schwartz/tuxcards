/***************************************************************************
                          xmlpersister.h  -  description
                             -------------------
    begin                : Mit Aug 14 2002
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

#ifndef XMLPERSISTER_H
#define XMLPERSISTER_H

#include <iostream>
#include <qstring.h>
#include <qfile.h>
#include <qdom.h>
#include "CInformationCollection.h"
#include "CTreeInformationElement.h"


class XMLPersister {
public:
  // loading;
  static CInformationCollection* createInformationCollection( QFile& xmlFile );
  static CInformationCollection* createInformationCollection( QString xmlString );

  static QString getPathOfLastActiveElement( void );
  
  
  // saving
  static void save( CInformationCollection&, QString filename );

private:
  // loading;
  static CInformationCollection* createInformationCollection( QDomDocument& doc );
  static bool createDomDocumentFromFile( QFile& xmlFile, QDomDocument& doc );
  static QDomDocument* createDomDocumentFromString( QString xmlString );
  static void parseChildrenOf( QDomNode& node, CTreeInformationElement& parent );
  static void parseNode( QDomNode& node, CTreeInformationElement& informationElement );
  static void parseDescriptionAttributes( QDomElement& elem, CTreeInformationElement& informationElem );
  static void parseAttributes( QDomElement& elem, CTreeInformationElement& infoElem );
  
  static QDomElement* parseCollectionDataAndGetRootInformationElement( QDomNode node );
  static QString pathOfLastActiveElement;


  static bool bCurrentElementIsBase64Encrypted;  // used when reading from file only
};

#endif
