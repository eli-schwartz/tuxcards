/***************************************************************************
                          CIconSelectorItem.h  -  description
                             -------------------
    begin                : Sat Jun 16 2007
    copyright            : (C) 2007 by Alexander Theel
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


#ifndef C_ICONSELECTORITEM_H
#define C_ICONSELECTORITEM_H

#include <QListWidgetItem>

#include <QTimer>
class QMimeSource;
class QFileInfo;

class CIconSelectorItem : public QListWidgetItem
{

public:
   CIconSelectorItem( QListWidget* pParent, QFileInfo* pFI );
   virtual ~CIconSelectorItem();
   
   QString                    filename() const { return msItemFileName; }
   bool                       isDir() const;
   bool                       isLink() const;

protected:
   void                       setIcon();

   QString                    msItemFileName;
   QFileInfo*                 mpItemFileInfo;

private:
   void                       createStaticIcons();

};


#endif
