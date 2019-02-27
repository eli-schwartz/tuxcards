/***************************************************************************
                          CSearchListItem.cpp  -  description
                             -------------------
    begin                : Fri Mar 31 2000
    copyright            : (C) 2000 by Alexander Theel
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

#include "CSearchListItem.h"
#include <QPainter>
#include <iostream>
/**
 * 'parent' is a pointer to the QTreeWidget where we want to insert this item
 */
// -------------------------------------------------------------------------------
CSearchListItem::CSearchListItem( QTreeWidget* pParent, Path* pPath, int iLocation,
                                  int iPos, int iLen, const QString& s)
 : QTreeWidgetItem( pParent ,QTreeWidgetItem::UserType /*, parent->lastItem(), path->getPathList().last(), (location == SearchPosition::SP_NAME ? QString("") : s)*/ )
// -------------------------------------------------------------------------------
{
   mpSearchPosition = new CSearchPosition( pPath, iLocation, iPos, iLen, s );

   if ( pPath )
      setText( 0, pPath->getPathList().last() );

   setText( 1, (iLocation == CSearchPosition::SP_NAME ? QString("") : s) );

   // Sets the font of column 0 to bold.
   //QFont f = font(0); f.setBold(TRUE); setFont(0, f);
}

// -------------------------------------------------------------------------------
CSearchListItem::~CSearchListItem()
// -------------------------------------------------------------------------------
{
   DELETE( mpSearchPosition );
}

// -------------------------------------------------------------------------------
const CSearchPosition* CSearchListItem::getSearchPosition() const
// -------------------------------------------------------------------------------
{
   return mpSearchPosition;
}

// -------------------------------------------------------------------------------
void CSearchListItem::paintCell( QPainter* /*pPainter*/, /*const QColorGroup &cg,*/
                                 int /*iColumn*/, int /*iWidth*/, int /*iAlignment*/ )
// -------------------------------------------------------------------------------
{
//TODO: This was moved to "CSearchListItemDelegate". Is it still needed??
/*
   //if (column==1){
   if ( mpSearchPosition->getLocation() == iColumn )
   {
      // cache often needed variables
      int iPos = mpSearchPosition->getPos();
      int iLen = mpSearchPosition->getLen();

      pPainter->fillRect( 0,0, iWidth,30, Qt::white );

      // draw first part
      QFont f = pPainter->font(); f.setBold( false ); QFontMetrics fm( f ); pPainter->setFont( f );
      if ( iPos > 0)
         pPainter->drawText( 0,15, text(iColumn).mid(0, iPos) );

      int x = fm.width( text(iColumn).mid(0, iPos) );

      // draw 2nd part = "searchPart"
      f.setBold( true );	pPainter->setFont( f );
      pPainter->drawText( x,15, text(iColumn).mid(iPos, iLen) );
      x += fm.width( text(iColumn).mid(iPos, iLen) ) + 1;       // '+1' -> so that it looks nicer

      // draw last part
      f.setBold( false );	pPainter->setFont( f );
      pPainter->drawText( x,15, text(iColumn).mid(iPos + iLen) );
   }
   else
   {
   //    Q3ListViewItem::paintCell( p, cg, column, width, alignment );
   }
*/
}

// -------------------------------------------------------------------------------
QString CSearchListItem::toString() const
// -------------------------------------------------------------------------------
{
   return text(0)
          + ": iPos=" + QString::number( mpSearchPosition->getPos() )
          + " iLen="  + QString::number( mpSearchPosition->getLen() );
}
