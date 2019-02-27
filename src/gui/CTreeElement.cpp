/***************************************************************************
                          treeelement.cpp  -  description
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
#include <iostream>

#include "CTreeElement.h"
#include "./dialogs/searchDialog/searchlistitem.h"

#include "../global.h"
//Added by qt3to4:
#include <QPixmap>


/**
 * creates a treeElement from the treeInformatoinElement and
 * adds it to 'parent'
 */
// -------------------------------------------------------------------------------
CTreeElement::CTreeElement( QTreeWidget* pParent, CTreeInformationElement& element )
  : QTreeWidgetItem( pParent )
  , mpInformationElement( NULLPTR )
// -------------------------------------------------------------------------------
{
   setText( 0, element.getDescription() );
   init(element);
}

/**
 * creates a treeElement from the treeInformatoinElement and
 * adds it to 'parent'
 */
// -------------------------------------------------------------------------------
CTreeElement::CTreeElement( CTreeElement* pParent, CTreeInformationElement& element )
  : QTreeWidgetItem(pParent, pParent->getLastChild())
  , mpInformationElement( NULLPTR )
// -------------------------------------------------------------------------------
{
   setText( 0, element.getDescription() );
   init(element);
}

// -------------------------------------------------------------------------------
CTreeElement::~CTreeElement( void )
// -------------------------------------------------------------------------------
{
   mpInformationElement = NULLPTR;
}

// -------------------------------------------------------------------------------
void CTreeElement::init( CTreeInformationElement& element )
// -------------------------------------------------------------------------------
{
  mpInformationElement = &element;
  copyPropertiesFromInformationElement();
  connect( &element, SIGNAL(propertyChanged()), this, SLOT(propertyChangeEvent()) );
  connect( &element, SIGNAL(childAdded(CInformationElement*)), this, SLOT(childAddEvent(CInformationElement*)) );
  connect( &element, SIGNAL(childMoved(int, int)), this, SLOT(childMovedEvent(int, int)) );
  
//  setRenameEnabled(0, TRUE);
}

/**
 * Returns the last child of this element. Utility-method needed to
 * place the elements in correct order.
 */
// -------------------------------------------------------------------------------
CTreeElement* CTreeElement::getLastChild( void )
// -------------------------------------------------------------------------------
{
   return (CTreeElement*) child( childCount() - 1 );
}

// -------------------------------------------------------------------------------
CTreeInformationElement* CTreeElement::getInformationElement( void )
// -------------------------------------------------------------------------------
{
   return mpInformationElement;
}

/**
 * This slot is called if an attribut of the informationelement
 * belonging to this object was changed.
 */
// -------------------------------------------------------------------------------
void CTreeElement::propertyChangeEvent( void )
// -------------------------------------------------------------------------------
{
   //cout<<"CTreeElement::propertyChangeEvent()"<<endl;
   copyPropertiesFromInformationElement();
}

/**
 * Copies all necessary attributes from the informationelement (data)
 * to this object (view).
 */
// -------------------------------------------------------------------------------
void CTreeElement::copyPropertiesFromInformationElement( void )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == mpInformationElement )
      return;

   setText( 0, mpInformationElement->getDescription() );
   setIcon( 0, QPixmap(mpInformationElement->getIconFileName()) );
   if ( treeWidget() )
      treeWidget()->setItemExpanded( this, mpInformationElement->isOpen() );
}


/**
 * This method is called whenever a child is added within the "data-model"
 * to the equivalent of this treeElement.
 */
// -------------------------------------------------------------------------------
void CTreeElement::childAddEvent( CInformationElement* pChild )
// -------------------------------------------------------------------------------
{
   //cout<<"CTreeElement::childAddEvent()"<<endl;
   if ( NULLPTR == pChild )
      return;

   CTreeElement* pNewElement = new CTreeElement(this, *((CTreeInformationElement*)pChild));


   // recursive (i.e. if a whole subtree was added during drag&drop)
   QList<CInformationElement*>* pList = pChild->getChildren();
   if ( !pList )
      return;
      
      
   CInformationElement* pX = NULLPTR;
   for ( int i = 0; i < pList->count(); i++ )
   {
      pX = pList->at( i );
      if ( pX )
         pNewElement->childAddEvent(pX);
   }
}


/**
 * This slot is called, whenever one of the element's children
 * has been moved upwards or downwards.
 */
// -------------------------------------------------------------------------------
void CTreeElement::childMovedEvent( int oldPos, int newPos )
// -------------------------------------------------------------------------------
{
/*
  if ( oldPos == newPos )
    return;

  CTreeElement* pElementToMove = getChildAtPosition(oldPos);
  if ( NULLPTR == pElementToMove )
    return;

  QTreeWidgetItem* pTmp = getChildAtPosition(newPos);
  if ( NULLPTR == pTmp )
    return;

  if ( newPos < oldPos )
  {
    // moving upwards
    if ( (newPos - 1) >= 0  )
    {
      pTmp = getChildAtPosition(newPos-1);
      pElementToMove->moveItem(pTmp);
    }
    else
    {
      // insert temporary dummy element
      QTreeWidgetItem* pDummy = new QTreeWidgetItem(this);
      pDummy->setText(0, "pDummy");
      pTmp = pDummy;
      pElementToMove->moveItem(pTmp);
      DELETE( pDummy );
    }
  }
  else
  {
    // moving downwards
    pElementToMove->moveItem(pTmp);
  }
*/
}


/**
 * Returns the child position at 'pos'. If it does not exist
 * 0 is returned.
 */
// -------------------------------------------------------------------------------
CTreeElement* CTreeElement::getChildAtPosition( int pos )
// -------------------------------------------------------------------------------
{
   return dynamic_cast<CTreeElement*>( child( pos ) );
}



/**
 * returns true if this Knoten or one of its children
 * is selected right now
 */
// -------------------------------------------------------------------------------
bool CTreeElement::isChildOrSelfSelected( void )
// -------------------------------------------------------------------------------
{
   if ( treeWidget() && treeWidget()->isItemSelected(this) )
      return true;


   CTreeElement* pElem = NULLPTR;
   for ( int i = 0; i < childCount(); i++ )
   {
      pElem = dynamic_cast<CTreeElement*>( child( i ) );
      if ( pElem )
      {
         if ( pElem->isChildOrSelfSelected() )
            return true;
      }
   }

   return false;
}

/**
 * find the specified 'QString pattern' within the text/information of the
 * appropriate 'CInformationElement' (evtl. recursive) and append the found
 * "places" as 'SearchListItem's at the list's end.
 */
// -------------------------------------------------------------------------------
void CTreeElement::search( QString pattern, bool recursive,
                           Qt::CaseSensitivity eCaseSensitive,
                           QTreeWidget& list )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == mpInformationElement )
      return;

   mpInformationElement->search(pattern, recursive, eCaseSensitive, list);
}


/**
 * Returns the first child whose "name"/text(0) equals
 * 'name'. If it does not exist return 0. (searches within direct children
 * only)
 */
// -------------------------------------------------------------------------------
CTreeElement* CTreeElement::findChildWithName( QString name )
// -------------------------------------------------------------------------------
{
  QTreeWidgetItem* pX = NULLPTR;
  for ( int i = 0; i < childCount(); i++ )
  {
     pX = child(i);
     if ( pX )
     {
        //std::cout<<"***TE::search through "<<x->text(0)<<std::endl;
        if ( pX->text(0) == name )
           return (CTreeElement*)pX;
     }
  }

  return NULLPTR;
}

