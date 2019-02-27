/***************************************************************************
                          CInformationCollection.cpp  -  description
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

#include "CInformationCollection.h"

#include "CTreeInformationElement.h"
#include <qdom.h>


// -------------------------------------------------------------------------------
CInformationCollection::CInformationCollection( CInformationElement* pRoot )
 : mpRootElement( NULLPTR )
 , mpActiveElement( NULLPTR )
 , msAbsFileName("")
// -------------------------------------------------------------------------------
{
  setRootElement( pRoot );
  setActiveElement( pRoot );
}


// -------------------------------------------------------------------------------
CInformationCollection::~CInformationCollection( void )
// -------------------------------------------------------------------------------
{
   DELETE( mpRootElement );
   mpActiveElement = NULLPTR;

   mViews.clear();
}


// -------------------------------------------------------------------------------
CInformationCollection* CInformationCollection::createDefaultCollection( void )
// -------------------------------------------------------------------------------
{
  CTreeInformationElement* pE = new CTreeInformationElement( 0, "root", "", InformationFormat::getByString("RTF") );
  CInformationCollection* pCollection = new CInformationCollection(pE);
  pCollection->registerAsListenerOf(pE);
  return pCollection;
}


// -------------------------------------------------------------------------------
void CInformationCollection::setAbsFileName( const QString& sAbsFileName )
// -------------------------------------------------------------------------------
{
   msAbsFileName = sAbsFileName;
}

// -------------------------------------------------------------------------------
const QString& CInformationCollection::getAbsFileName() const
// -------------------------------------------------------------------------------
{
   return msAbsFileName;
}



// **************************** IParent ******************************************
// -------------------------------------------------------------------------------
QString CInformationCollection::getDescription( void ) const
// -------------------------------------------------------------------------------
{
  return INFORMATION_COLLECTION_DESC;
}
// -------------------------------------------------------------------------------
IParent* CInformationCollection::getParent( void ) const
// -------------------------------------------------------------------------------
{
   return NULLPTR;
}

// -------------------------------------------------------------------------------
void CInformationCollection::removeChild( CInformationElement* /*pChild*/ )
// -------------------------------------------------------------------------------
{
   // Root element cannot be deleted; do nothing.
}

// -------------------------------------------------------------------------------
void CInformationCollection::aboutToRemoveElement( CInformationElement* pIE )
// -------------------------------------------------------------------------------
{
   notifyViewsToRemoveElement( pIE );
}
// **************************** IParent - End ************************************


// -------------------------------------------------------------------------------
void CInformationCollection::setRootElement( CInformationElement* pRoot )
// -------------------------------------------------------------------------------
{
   mpRootElement = pRoot;

   if ( NULLPTR != pRoot )
      pRoot->setParent(this);
}


// -------------------------------------------------------------------------------
CInformationElement* CInformationCollection::getRootElement( void ) const
// -------------------------------------------------------------------------------
{
  return mpRootElement;
}


// -------------------------------------------------------------------------------
void CInformationCollection::setActiveElement( CInformationElement* pElement )
// -------------------------------------------------------------------------------
{
   if (!pElement)
      return;
   //std::cout<<"CInformationCollection::mpActiveElement changed to "
   //         <<pElement->getDescription()<<std::endl;

   mpActiveElement = pElement;
   emit activeInformationElementChanged( pElement );
}


// -------------------------------------------------------------------------------
void CInformationCollection::setActiveElement( Path path )
// -------------------------------------------------------------------------------
{
   CInformationElement* pElement = getInformationElement(path);

   if ( NULLPTR == pElement )
   {
      setActiveElement( mpRootElement );
      return;
   }

   setActiveElement( pElement );
}


// -------------------------------------------------------------------------------
CInformationElement* CInformationCollection::getActiveElement( void )
// -------------------------------------------------------------------------------
{
  return mpActiveElement;
}


// -------------------------------------------------------------------------------
QString CInformationCollection::toString( void )
// -------------------------------------------------------------------------------
{
  //cout<<"printing collection with rootElement("<<mpRootElement<<")"<<endl;
  if ( NULLPTR == mpRootElement )
   return "";

  return mpRootElement->getTreeString(0);
}


// -------------------------------------------------------------------------------
QString CInformationCollection::toXML( void )
// -------------------------------------------------------------------------------
{
// QDomDocument KoDocument::createDomDocument( const QString& appName, const QString& tagName, const QString& version )
//1312 {
//1313     QDomImplementation impl;
//1314     QString url = QString("http://www.koffice.org/DTD/%1-%1.dtd").arg(appName).arg(version);
//1315     QDomDocumentType dtype = impl.createDocumentType( tagName,
//1316                                                       QString("-//KDE//DTD %1 %1//EN").arg(appName).arg(version),
//1317                                                       url );
//1318     // The namespace URN doesn't need to include the version number.
//1319     QString namespaceURN = QString("http://www.koffice.org/DTD/%1").arg(appName);
//1320     QDomDocument doc = impl.createDocument( namespaceURN, tagName, dtype );
//1321     doc.insertBefore( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ), doc.documentElement() );
//1322     return doc;
// }


  QDomDocument xmlDocument("tuxcards_data_file");  
  xmlDocument.insertBefore( xmlDocument.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"" ), xmlDocument.documentElement() );
  //xmlDocument.insertBefore( xmlDocument.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ), xmlDocument.documentElement() );
//  QDomElement rootElem = xmlDocument.createElement("Menschen");
//  xmlDocument.appendChild(rootElem);
//
//  QDomElement elem = xmlDocument.createElement("Mensch");
//  elem.setAttribute("name", "alex");
//  elem.setAttribute("age", "26");
//  rootElem.appendChild(elem);
//
//  QDomText text = xmlDocument.createTextNode( "Hello<b> & World" );
//  elem.appendChild( text );



  // CInformationCollection-MetaData
  QDomElement thisElement = xmlDocument.createElement("InformationCollection");
  xmlDocument.appendChild( thisElement );

  QDomElement pathOfLastActiveIE = xmlDocument.createElement("LastActiveElement");
  QDomText text = xmlDocument.createTextNode( Path(mpActiveElement).toString() );
  pathOfLastActiveIE.appendChild( text );
  thisElement.appendChild( pathOfLastActiveIE );

  //std::cout<<"LastActiveElement = "<<Path(mpActiveElement).toString()<<std::endl;


  if ( NULLPTR != mpRootElement )
  {
    //std::cout<<"root != 0"<<std::endl;
    mpRootElement->toXML(xmlDocument, thisElement);
  }

  return xmlDocument.toString();
}



/**
 * Allows to create an XML-document with an arbitrary informationelement
 * as root.
 */
// -------------------------------------------------------------------------------
QString CInformationCollection::toXML( CInformationElement* pElem )
// -------------------------------------------------------------------------------
{
   QDomDocument xmlDocument("tuxcards_data_file");
   xmlDocument.insertBefore( xmlDocument.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"utf-8\"" ), xmlDocument.documentElement() );
   if ( NULLPTR != pElem )
      pElem->toXML( xmlDocument, xmlDocument );

   return xmlDocument.toString();
}


// -------------------------------------------------------------------------------
void CInformationCollection::slotChildAdded( CInformationElement* pElem )
// -------------------------------------------------------------------------------
{
  registerAsListenerOf( pElem );
  emit modelHasChanged();
}


// -------------------------------------------------------------------------------
void CInformationCollection::registerAsListenerOf( CInformationElement* pElem )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == pElem )
      return;

   connect( pElem, SIGNAL(propertyChanged()), this, SIGNAL(modelHasChanged()) );
   connect( pElem, SIGNAL(childAdded(CInformationElement*)), this, SLOT(slotChildAdded(CInformationElement*)) );
}


// -------------------------------------------------------------------------------
CInformationElement* CInformationCollection::getInformationElement( Path path )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == mpRootElement )
      return NULLPTR;

   QStringList list = path.getPathList();
   if ( mpRootElement->getDescription() != list[0] )
      return NULLPTR;

   CInformationElement* pElem = mpRootElement;
   for ( int i=1; i < list.size(); i++ )
   {
      pElem = pElem->findChildWithDescription( list[i] );
      if ( NULLPTR == pElem )
      {
         return NULLPTR;
      }
   }

   return pElem;
}


/**
 * Checks whether the given path is not empty and valid
 * within this information collection.
 */
// -------------------------------------------------------------------------------
bool CInformationCollection::isPathValid( Path path )
// -------------------------------------------------------------------------------
{
  if ( path.isEmpty() )
    return false;

  CInformationElement* pElement = getInformationElement(path);
  if ( pElement )
    return true;

  return false;
}


// -------------------------------------------------------------------------------
void CInformationCollection::addView( IView* pView )
// -------------------------------------------------------------------------------
{
   mViews.append( pView );
}

// -------------------------------------------------------------------------------
void CInformationCollection::removeView( IView* pView )
// -------------------------------------------------------------------------------
{
   int iIndex = mViews.indexOf( pView );
   if ( -1 != iIndex )
      mViews.removeAt( iIndex );
}


// -------------------------------------------------------------------------------
void CInformationCollection::notifyViewsToRemoveElement( CInformationElement* pIE )
// -------------------------------------------------------------------------------
{
   for ( int i = 0; i < mViews.count(); i++ )
   {
      IView* pView = mViews.at( i );
      if ( pView )
         pView->aboutToRemoveElement( pIE );
   }
}
