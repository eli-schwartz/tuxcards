/***************************************************************************
                          CTreeInformationElement.cpp  -  description
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

#include "CTreeInformationElement.h"
#include "../global.h"

#include "../utilities/base64/CBase64Coder.h"

// -------------------------------------------------------------------------------
CTreeInformationElement::CTreeInformationElement( CInformationElement* pParent,
                         QString description,
                         QString information,
                         InformationFormat* pFormat,
                         QString sIconFileName,
                         bool expires,
                         QDate date,
                         bool bOpen )
 : CInformationElement( pParent, description, information, pFormat, sIconFileName,
                        expires, date )
 , mbOpen( false )
// -------------------------------------------------------------------------------
{
   setOpen( bOpen );
}


/**
 * Adds the CTreeInformationElement 'element' to this element.
 */
// -------------------------------------------------------------------------------
void CTreeInformationElement::addChild( CTreeInformationElement* pElement )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == pElement )
      return;

   mpChildObjects->append( pElement );
   pElement->setParent( this );

   emit childAdded( pElement );
//  if (!mbBatched) emit(childAdded(element));
}


// -------------------------------------------------------------------------------
bool CTreeInformationElement::isOpen( void )
// -------------------------------------------------------------------------------
{
   return mbOpen;
}


// -------------------------------------------------------------------------------
void CTreeInformationElement::setOpen( bool bOpen )
// -------------------------------------------------------------------------------
{
   mbOpen = bOpen;
   if (!mbBatched) emit propertyChanged();
}


// -------------------------------------------------------------------------------
QString CTreeInformationElement::toStringObsoleted( void )
// -------------------------------------------------------------------------------
{
  QString result="";
  QString h;

  // get own name
  h = mDescription;
  result += QString::number(h.length()) + "*" + h;

  // get own text
  h = mInformation;
  result += QString::number(h.length()) + "*" + h;

  //state whether this Knoten is open or not
  h = ( isOpen() ? "-" : "+" );
  result += h;

  // include iconFilename
  result += mIconFilename+"*";

  int n = childCount();
  result += QString::number(n);

  // add each child
  CTreeInformationElement* pX;
  for ( int i = 0; i < mpChildObjects->count(); i++ )
  {
     pX = (CTreeInformationElement*)mpChildObjects->at( i );
     if ( pX )
        result += pX->toString();
  }

  // add terminationString & header
  result= "*" +result+ "***";
  result= "***" + QString::number(result.length()) + result;

  return result;
}


// -------------------------------------------------------------------------------
void CTreeInformationElement::toXML( QDomDocument& xmlDocument, QDomNode& parent )
// -------------------------------------------------------------------------------
{
   QDomElement thisElement = xmlDocument.createElement("InformationElement");
   thisElement.setAttribute("informationFormat", getInformationFormat()->toString());
   thisElement.setAttribute("iconFileName", getIconFileName());
   thisElement.setAttribute("expires", expires() ? QString("true") : QString("false") );
   thisElement.setAttribute("expiryDate", getExpiryDate().toString());
   thisElement.setAttribute("isOpen", isOpen() ? "true" : "false" );
   thisElement.setAttribute("isEncrypted", isEncryptionEnabled() ? "true" : "false" );

   // add description
   QDomElement description = xmlDocument.createElement("Description");
   QDomText text = xmlDocument.createTextNode(getDescription());
   description.appendChild(text);
   if ( hasDescriptionColor() )
      description.setAttribute( "color", getDescriptionColor().name() );
   thisElement.appendChild(description);

   // add information
   QDomElement information = xmlDocument.createElement("Information");
   if ( isEncryptionEnabled() )
   {
      if ( !isCurrentlyEncrypted() )
      {
         encrypt();
      }

      // do base64 encodeing
      CBase64Coder b64Coder = CBase64Coder::getInstance();
      QString sB64Representation;
      b64Coder.encode( sB64Representation, getEncryptedData() );

      text = xmlDocument.createCDATASection( sB64Representation );
   }
   else
   {
      //text = xmlDocument.createTextNode( getInformation() );
      text = xmlDocument.createCDATASection( getInformation() );
   }

   information.appendChild(text);
   thisElement.appendChild(information);


   // add children
   if ( mpChildObjects )
   {
      CInformationElement* pX;
      for ( int i = 0; i < mpChildObjects->count(); i++ )
      {
         pX = mpChildObjects->at( i );
         if ( pX )
            pX->toXML( xmlDocument, thisElement );
      }
   }

   parent.appendChild( thisElement );
}


/**
 * Move this element one position upwards within the sibling
 * list. If moving upwards is not possible nothing is done.
 */
// -------------------------------------------------------------------------------
void CTreeInformationElement::moveOneUp( void )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == mpParent )
   {
      std::cout<<"parent == 0 -> moving not possible"<<std::endl;
      return;
   }

   ((CTreeInformationElement*)mpParent)->moveChildOneUp(this);
}


/**
 * Move the specified child element one position upwards within
 * children list. If moving upwards is not possible or the given
 * element is not a child of this element nothing is done.
 */
// -------------------------------------------------------------------------------
void CTreeInformationElement::moveChildOneUp( CTreeInformationElement* pChild )
// -------------------------------------------------------------------------------
{
   int pos = mpChildObjects->indexOf( pChild );
   if ( (pos == -1) || (pos == 0) )
   {
      //std::cout<<"child not found or at first position -> moving not possible"<<std::endl;
      return;
   }

   
   mpChildObjects->removeAt( pos );            // do not kill the pointer
   mpChildObjects->insert( pos-1, pChild );

   if (!mbBatched) emit childMoved(pos, pos-1);

   //std::cout<<getTreeString(1)<<std::endl;
}

/**
 * Move this element one position upwards within the sibling
 * list. If moving upwards is not possible nothing is done.
 */
// -------------------------------------------------------------------------------
void CTreeInformationElement::moveOneDown( void )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == mpParent )
   {
      std::cout<<"parent == 0 -> moving not possible"<<std::endl;
      return;
   }

   ((CTreeInformationElement*)mpParent)->moveChildOneDown(this);
}

/**
 * Move the specified child element one position downwards within
 * children list. If moving downwards is not possible or the given
 * element is not a child of this element nothing is done.
 */
// -------------------------------------------------------------------------------
void CTreeInformationElement::moveChildOneDown( CTreeInformationElement* pChild )
// -------------------------------------------------------------------------------
{
   int pos = mpChildObjects->indexOf( pChild );
   if ( (pos == -1) || (pos == childCount()-1) )
   {
      //std::cout<<"child not found or at last position -> moving not possible"<<std::endl;
      return;
   }

   mpChildObjects->removeAt( pos );                  // do not kill the pointer
   mpChildObjects->insert( pos+1, pChild );

   if (!mbBatched) emit childMoved(pos, pos+1);

   //std::cout<<getTreeString(1)<<std::endl;
}

