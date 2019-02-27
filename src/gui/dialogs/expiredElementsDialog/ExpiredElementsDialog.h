/***************************************************************************
                          ExpiredElementsDialog.h  -  description
                             -------------------
    begin                : Sat Jun 21 2003
    copyright            : (C) 2003 by Alexander Theel
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

#ifndef EXPIRED_ELEMENTS_DIALOG_H
#define EXPIRED_ELEMENTS_DIALOG_H

#include "ExpiredElementsDialogInterface.h"

#include "../../../information/CInformationCollection.h"

#include <QHash>
#include <QMenu>
#include <iostream>

class ExpiredElementsDialog : public QDialog {
   Q_OBJECT
public:
   ExpiredElementsDialog( QWidget* pParent );
   int setUp( CInformationCollection* pCollection );

private slots:
   void removeSelectedItemsFromDeletionList( void );

   void previewEntry( QTreeWidgetItem* pItem );
   void previewEntry( CInformationElement* pElement );

   void deleteRemainingEntries( void );
   
   void slotShowContextMenu();
   void slotChangeActiveElementProperties();

private:
   // The ui-element.
   Ui::ExpiredElementsDialogInterface ui;
   
   
   // Other private members.
   CInformationCollection* mpCollection;

   QHash<QTreeWidgetItem*, CInformationElement*> expiredElementsMap;
   
   QMenu                   mContextMenu;

   void checkEntryForExpiryDate( CInformationElement* pElement );

   void clearPreview( void );

};

#endif

