/***************************************************************************
                          CTreeInformationElement.h  -  description
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
#ifndef CTREE_INFORMATIONELEMENT_H
#define CTREE_INFORMATIONELEMENT_H

#include <iostream>
#include "CInformationElement.h"
#include <qdom.h>
#include "../global.h"

class CTreeInformationElement : public CInformationElement{
public:
   CTreeInformationElement( CInformationElement* pParent = NULLPTR,
                           QString description="", QString information="",
                           InformationFormat* pFormat = InformationFormat::getByString( "NONE" ),
                           QString sIconFileName = CInformationElement_NO_ICON,
                           bool expires = false,
                           QDate date = QDate::currentDate(),
                           bool isOpen=false );

   void addChild( CTreeInformationElement* pElement );

   bool isOpen( void );
   void setOpen( bool );

   QString toStringObsoleted( void );
   virtual void toXML( QDomDocument& xmlDocument, QDomNode& parent );

   void moveOneUp( void );
   void moveChildOneUp( CTreeInformationElement* pChild );
   void moveOneDown( void );
   void moveChildOneDown( CTreeInformationElement* pChild );

private:
   bool mbOpen;
};
#endif

