/***************************************************************************
                          CSearchListItemDelegate.h  -  description
                             -------------------
    begin                : Fri Jan 04 2008
    copyright            : (C) 2008 by Alexander Theel
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
#ifndef C_SEARCH_LISTITEM_DELEGATE_H
#define C_SEARCH_LISTITEM_DELEGATE_H

#include <QItemDelegate>

class CSearchListItemDelegate : public QItemDelegate {
public:
   CSearchListItemDelegate( QObject* pParent = 0 );

protected:
   virtual void               drawDisplay( QPainter* pPainter, const QStyleOptionViewItem& option, const QRect& rect, const QString& sText ) const;
   //virtual void               paint ( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};

#endif
