/***************************************************************************
                          xmlpersister.cpp  -  description
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

#include "xmlpersister.h"
#include "../global.h"

#include <QTextStream>
#include "../utilities/base64/CBase64Coder.h"

QString XMLPersister::pathOfLastActiveElement("");
bool XMLPersister::bCurrentElementIsBase64Encrypted = false;


/************************ loading **********************************/

// -------------------------------------------------------------------------------
CInformationCollection* XMLPersister::createInformationCollection( QFile& xmlFile )
// -------------------------------------------------------------------------------
{
   QDomDocument xmlDocument( "tuxcards_data_file" );
   bool bResult = createDomDocumentFromFile( xmlFile, xmlDocument );

   if ( bResult )
   {
      return createInformationCollection( xmlDocument );
   }

   return NULLPTR;
}

// -------------------------------------------------------------------------------
CInformationCollection* XMLPersister::createInformationCollection( QString xmlString )
// -------------------------------------------------------------------------------
{
   QDomDocument* xmlDocument = createDomDocumentFromString(xmlString);
   if ( NULLPTR != xmlDocument )
   {
      return createInformationCollection( *xmlDocument );
   }

   return NULLPTR;
}

// -------------------------------------------------------------------------------
CInformationCollection* XMLPersister::createInformationCollection( QDomDocument& doc )
// -------------------------------------------------------------------------------
{
   // create the collection
   CInformationCollection* collection = new CInformationCollection();
   CTreeInformationElement* e = new CTreeInformationElement();
   collection->registerAsListenerOf( e );

   if ( !doc.documentElement().isNull() )
   {
      QDomElement* pDomElem = parseCollectionDataAndGetRootInformationElement( doc.documentElement() );

      if ( NULLPTR != pDomElem )
      {
         parseAttributes(*pDomElem, *e);
         parseChildrenOf(*pDomElem, *e);
      }
   }
   collection->setRootElement( e );

   return collection;
}
/*
  if ( !doc->documentElement().isNull() ){
     parseAttributes(doc->documentElement(), e);
    parseChildrenOf(doc->documentElement(), e);
  }
  collection->setRootElement(e);
*/



// -------------------------------------------------------------------------------
QDomElement* XMLPersister::parseCollectionDataAndGetRootInformationElement(
                                                                   QDomNode node )
// -------------------------------------------------------------------------------
{
  QDomElement* retVal = NULLPTR;

  if ( node.isElement() )
  {
    QDomElement elem = node.toElement();
    //cout<<"element; tagname= "<<elem.tagName()<<endl;

    if ( elem.tagName() == "InformationCollection" )
    {
      QDomNode child = node.firstChild();
      QDomElement domElem_LastActiveElement = child.toElement();
      pathOfLastActiveElement = domElem_LastActiveElement.text();
      //std::cout<<"xmlpersister -> lastActiveElement = "<<pathOfLastActiveElement<<std::endl;

      retVal = new QDomElement( child.nextSibling().toElement() );
    }

    // backwards compatibility
    else if ( elem.tagName() == "InformationElement" )
    {
      retVal = new QDomElement( elem );
    }
  }

  return retVal;
}


// -------------------------------------------------------------------------------
void XMLPersister::parseChildrenOf( QDomNode& node, CTreeInformationElement& parent )
// -------------------------------------------------------------------------------
{
   QDomNode child = node.firstChild();
   while ( !child.isNull() )
   {
      parseNode(child, parent);
      child = child.nextSibling();
   }
}


// -------------------------------------------------------------------------------
void XMLPersister::parseNode( QDomNode& node,
                                     CTreeInformationElement& informationElement )
// -------------------------------------------------------------------------------
{
   if ( node.isElement() )
   {
      QDomElement elem = node.toElement();
      //cout<<"element; tagname= "<<elem.tagName()<<endl;

      if ( elem.tagName() == "Description" )
      {
         informationElement.setDescription( elem.text() );
         parseDescriptionAttributes( elem, informationElement );
      }
      else if ( elem.tagName() == "Information" )
      {
         // QDomText textNode=node.toText();
         //std::cout<<"==="<<elem.text()<<std::endl;

         // determine whether current elemenet contains encrypted data
         if ( false != bCurrentElementIsBase64Encrypted )
         {
            // do base64 decodeing
            //std::cout<<"decoding64"<<std::endl;
            CBase64Coder b64Coder = CBase64Coder::getInstance();
            QByteArray encryptedData;
            b64Coder.decode( encryptedData, elem.text() );

            informationElement.setEncryptedData( encryptedData );
         }
         else
         {
            informationElement.setInformation( elem.text() );
         }
      }
      else if ( elem.tagName() == "InformationElement" ) {
         CTreeInformationElement* e = new CTreeInformationElement( &informationElement );
         parseAttributes( elem, *e );
         informationElement.addChild( e );
         parseChildrenOf( node, *e );
      }

//    cout<<"!!! still guessing for richtext"<<endl;
//    if ( QStyleSheet::mightBeRichText(informationElement->getInformation()) )
//      informationElement->setInformationFormat(&InformationFormat::RTF);
//    else
//      informationElement->setInformationFormat(&InformationFormat::ASCII);

   }
}


/**
 * Parses the attributes of the description for a CTreeInformationElement
 * (which is essentially the description color).
 */
// -------------------------------------------------------------------------------
void XMLPersister::parseDescriptionAttributes( QDomElement& elem,
                                        CTreeInformationElement& informationElem )
// -------------------------------------------------------------------------------
{
   QDomNamedNodeMap attributes = elem.attributes();

   if ( !attributes.namedItem("color").isNull() )
   {
      QColor c( attributes.namedItem("color").toAttr().value() );
      Q_ASSERT( c.isValid() );
      informationElem.setDescriptionColor( c );
   }
}

/**
 * parses the attributes for a CTreeInformationElement
 */
// -------------------------------------------------------------------------------
void XMLPersister::parseAttributes( QDomElement& elem,
                                        CTreeInformationElement& informationElem )
// -------------------------------------------------------------------------------
{
   QDomNamedNodeMap attributes = elem.attributes();

   QString s = ( attributes.namedItem("informationFormat").isNull() ? QString("ASCII") : attributes.namedItem("informationFormat").toAttr().value() );
   Q_ASSERT( 0 != s.compare("NONE") );
   informationElem.setInformationFormat(InformationFormat::getByString(s));
   //std::cout<<"XMLP::parseAttr; format= "<<s.toStdString()<<std::endl;

   s = ( attributes.namedItem("iconFileName").isNull() ? QString("") : attributes.namedItem("iconFileName").toAttr().value() );
   informationElem.setIconFileName(s);
   //cout<<"XMLP::parseAttr; iconFileName ="<<s<<endl;


   s = ( attributes.namedItem("expires").isNull() ? QString("false") : attributes.namedItem("expires").toAttr().value() );
   QString s2 = ( attributes.namedItem("expiryDate").isNull() ? QString(QDate::currentDate().toString()) : attributes.namedItem("expiryDate").toAttr().value() );
   informationElem.setExpiryDate( s=="true" ? true : false, QDate::fromString(s2) );
   //std::cout<<"XMLP::parseAttr; expires ="<<s<<" expiryDate="<<s2<<std::endl;

   s = ( attributes.namedItem("isOpen").isNull() ? QString("false") : attributes.namedItem("isOpen").toAttr().value() );
   informationElem.setOpen( s=="true" ? true : false );
   //cout<<"XMLP::parseAttr; isOpen ="<<s<<endl;


   // Due to a typo within the xml file, we ask for "isEncripted" (the typo) and the "isEncrypted"
   // (the corrected name). TODO: This can be simplified in further versions; i.e. the differentiation
   //  between both versions can be removed.
   if ( !attributes.namedItem("isEncripted").isNull() )
   {
      // value with old/wrong name exists, use it.
      s = attributes.namedItem("isEncripted").toAttr().value();
   }
   else
   {
      // check whether the correct attribute exists and use it
      s = ( attributes.namedItem("isEncrypted").isNull() ? QString("false") : attributes.namedItem("isEncrypted").toAttr().value() );
   }
   bCurrentElementIsBase64Encrypted = ( s=="true" ? true : false );
   //std::cout<<"XMLP::parseAttr; isEncrypted ="<<s<<std::endl;
}


// -------------------------------------------------------------------------------
// Returns 'true' if the dom-document could be created correctly.
// -------------------------------------------------------------------------------
bool XMLPersister::createDomDocumentFromFile( QFile& xmlFile, QDomDocument& doc )
// -------------------------------------------------------------------------------
{
   bool bRetVal = FALSE;

   if ( !xmlFile.open( QIODevice::ReadOnly ) )
      return bRetVal;

   bRetVal = doc.setContent( &xmlFile );
   xmlFile.close();

   return bRetVal;

//  QDomDocument* doc = new QDomDocument( "tuxcards_data_file" );
//  if ( !xmlFile.open( IO_ReadOnly ) )
//    return 0;
//  if ( !doc->setContent( &xmlFile ) ) {
//      xmlFile.close();
//      return 0;
//  }
//  xmlFile.close();
//
//  return doc;
}

// -------------------------------------------------------------------------------
QDomDocument* XMLPersister::createDomDocumentFromString( QString xmlString )
// -------------------------------------------------------------------------------
{
   QDomDocument* doc = new QDomDocument( "tuxcards_data_file" );
   if ( !doc->setContent(xmlString) )
      return NULLPTR;

   return doc;
}


// -------------------------------------------------------------------------------
QString XMLPersister::getPathOfLastActiveElement( void )
// -------------------------------------------------------------------------------
{
   return pathOfLastActiveElement;
}


/************************ saving ***********************************/
// -------------------------------------------------------------------------------
void XMLPersister::save( CInformationCollection& collection, QString fileName )
// -------------------------------------------------------------------------------
{
   // TODO: save something like a version string with it (like: "TuxCardsV0.5")
   QString s = collection.toXML();

   QFile f(fileName);
   if ( f.open(QIODevice::WriteOnly) )
   {
      QTextStream t( &f );
      t.setCodec( "UTF-8" );
      t<<s;
      f.close();
   }
}
