/***************************************************************************
                          CSearchListItem.h  -  description
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

#ifndef CSEARCHLISTITEM_H
#define CSEARCHLISTITEM_H

#include <QTreeWidget>
#include <QString>
#include "CSearchPosition.h"

class CSearchListItem : public QTreeWidgetItem {
public:

   CSearchListItem( QTreeWidget* pParent, Path* pPath, int iLocation,
                    int iPos, int iLen, const QString& s );

   ~CSearchListItem();

   const CSearchPosition*     getSearchPosition() const;

protected:

   virtual void               paintCell( QPainter *p, /*const QColorGroup &cg,*/
                                         int iColumn, int iWidth, int iAlignment );
   QString                    toString() const;

   CSearchPosition*           mpSearchPosition;
};

#endif
