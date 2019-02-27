/***************************************************************************
                          CSearchListItemDelegate.cpp  -  description
                             -------------------
    begin                : Fri Jan 04 2008
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

#include "CSearchListItemDelegate.h"
#include <QPainter>

#include <QMessageBox>

/**
 * 'parent' is a pointer to the QTreeWidget where we want to insert this item
 */
// -------------------------------------------------------------------------------
CSearchListItemDelegate::CSearchListItemDelegate( QObject* pParent )
 : QItemDelegate( pParent /*, parent->lastItem(), path->getPathList().last(), (location == SearchPosition::SP_NAME ? QString("") : s)*/ )
// -------------------------------------------------------------------------------
{
}

//// -------------------------------------------------------------------------------
//void CSearchListItemDelegate::paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const
//// -------------------------------------------------------------------------------
//{
//   QStyleOptionViewItem myOption = option;
//   myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
//
//
//   //painter->fillRect( option.rect, option.palette.highlight() );
//   painter->setPen( Qt::red );
//   painter->drawText( option.rect, "abc" );
//   
//   drawDisplay( painter, myOption, myOption.rect, "abc" );
//   drawFocus( painter, myOption, myOption.rect );
//   
//   //QItemDelegate::paint( painter, option, rect, sText );
//}


// -------------------------------------------------------------------------------
void CSearchListItemDelegate::drawDisplay( QPainter* pPainter,
                                           const QStyleOptionViewItem& option, 
                                           const QRect& rect, 
                                           const QString& sText ) const
// -------------------------------------------------------------------------------
{

   if ( pPainter )
   {
      //QMessageBox::information( 0, "", "<i>CSearchListItemDelegate::drawDisplay()</i>" );

      QFont f = pPainter->font();
      f.setItalic( TRUE );
      pPainter->setFont( f );
      pPainter->setPen( Qt::red );
      pPainter->drawText( option.rect, "abc" );
   }
   QItemDelegate::drawDisplay( pPainter, option, rect, sText );
}


//// -------------------------------------------------------------------------------
//void CSearchListItem::paintCell( QPainter* pPainter, /*const QColorGroup &cg,*/
//                                 int iColumn, int iWidth, int iAlignment )
//// -------------------------------------------------------------------------------
//{
//   //if (column==1){
//   if ( mpSearchPosition->getLocation() == iColumn )
//   {
//      // cache often needed variables
//      int iPos = mpSearchPosition->getPos();
//      int iLen = mpSearchPosition->getLen();
//
//      pPainter->fillRect( 0,0, iWidth,30, Qt::white );
//
//      // draw first part
//      QFont f = pPainter->font(); f.setBold( false ); QFontMetrics fm( f ); pPainter->setFont( f );
//      if ( iPos > 0)
//         pPainter->drawText( 0,15, text(iColumn).mid(0, iPos) );
//
//      int x = fm.width( text(iColumn).mid(0, iPos) );
//
//      // draw 2nd part = "searchPart"
//      f.setBold( true );	pPainter->setFont( f );
//      pPainter->drawText( x,15, text(iColumn).mid(iPos, iLen) );
//      x += fm.width( text(iColumn).mid(iPos, iLen) ) + 1;       // '+1' -> so that it looks nicer
//
//      // draw last part
//      f.setBold( false );	pPainter->setFont( f );
//      pPainter->drawText( x,15, text(iColumn).mid(iPos + iLen) );
//   }
//   else
//   {
//   //    Q3ListViewItem::paintCell( p, cg, column, width, alignment );
//   }
//}
