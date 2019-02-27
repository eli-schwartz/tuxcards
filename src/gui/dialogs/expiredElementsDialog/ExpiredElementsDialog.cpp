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

#include "ExpiredElementsDialog.h"

#include <qlabel.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QPixmap>

#include "../../../utilities/CIconManager.h"
#define  getIcon(x)  CIconManager::getInstance().getIcon(x)

#include "../propertyDialog/CPropertyDialog.h"
#include <qcursor.h>


// -------------------------------------------------------------------------------
ExpiredElementsDialog::ExpiredElementsDialog( QWidget* pParent )
   : QDialog( pParent )
   , mpCollection( NULLPTR )
   , mContextMenu( pParent )
// -------------------------------------------------------------------------------
{
   setModal( TRUE );
   ui.setupUi(this);
   
   connect( ui.pExpiryList, SIGNAL(clicked(Q3ListViewItem*)), this, SLOT(previewEntry(Q3ListViewItem*)) );
   connect( ui.pRemoveFromListButton, SIGNAL(clicked()), this, SLOT(removeSelectedItemsFromDeletionList()) );

   connect( ui.buttonOk, SIGNAL(clicked()), this, SLOT(deleteRemainingEntries()) );
   
   
   // context menu
   mContextMenu.addAction( getIcon("changeProperty"), "Change Properties...",this, SLOT(slotChangeActiveElementProperties()) );
   connect( ui.pExpiryList, SIGNAL(contextMenuRequested(Q3ListViewItem*,const QPoint&,int)),this, SLOT(slotShowContextMenu()) );
}


// -------------------------------------------------------------------------------
int ExpiredElementsDialog::setUp( CInformationCollection* pCollection )
// -------------------------------------------------------------------------------
{
   if ( (NULLPTR == pCollection) || (NULLPTR == ui.pExpiryList) || (NULLPTR == ui.buttonOk) )
      return 0;

   mpCollection = pCollection;

   expiredElementsMap.clear();
   ui.pExpiryList->clear();


   checkEntryForExpiryDate( mpCollection->getRootElement() );
   if ( ui.pExpiryList->topLevelItemCount() > 0 )
   {
      show();
      return exec();
   }
   else
   {
      return 0;
   }

   ui.buttonOk->setEnabled( true );
}


// -------------------------------------------------------------------------------
void ExpiredElementsDialog::checkEntryForExpiryDate( CInformationElement* pElement )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == pElement )
      return;

   if ( pElement->expires() )
   {
      //std::cout<<"ExpiredElementsDialog::checkEntryForExpiryDate; found: "<<pElement->getDescription()<<std::endl;
      if ( pElement->getExpiryDate() < QDate::currentDate() )
      {
         QTreeWidgetItem* pItem = new  QTreeWidgetItem( ui.pExpiryList );
         pItem->setText( 0, pElement->getDescription() );
         pItem->setIcon( 0, QPixmap(pElement->getIconFileName()) );
         expiredElementsMap.insert( pItem, pElement );
         //std::cout<<"icon="<<pElement->getIconFileName()<<std::endl;
      }
   }


   QList<CInformationElement*>* pChildrenList = pElement->getChildren();
   if ( pChildrenList )
   {
      CInformationElement* x;
      for ( int i = 0; i < pChildrenList->count(); i++ )
      {
         x = pChildrenList->at( i );
         checkEntryForExpiryDate( x );
      }
   }
}


// -------------------------------------------------------------------------------
void ExpiredElementsDialog::removeSelectedItemsFromDeletionList( void )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == ui.pExpiryList )
      return;

   
   QList<QTreeWidgetItem*> selectedItemList = ui.pExpiryList->selectedItems();
   QTreeWidgetItem*     pTreeItem;
   CInformationElement* pElement;
   while ( 0 < selectedItemList.count() )
   {
      pTreeItem = selectedItemList.first();
      if ( pTreeItem )
      {
         pElement = expiredElementsMap.value( pTreeItem );
         if ( pElement )
         {
            pElement->setExpiryDate( false );      // remove expiry date
         }
      }
      
      expiredElementsMap.remove( pTreeItem );      // remove from dictionary
      
      DELETE( pTreeItem );
   }


   clearPreview();

   // if list is empty, disable the button
   if ( 0 == ui.pExpiryList->topLevelItemCount() )
   {
      if ( ui.buttonOk )
         ui.buttonOk->setEnabled( false );
   }
}


// -------------------------------------------------------------------------------
// Removes everything from the preview.
// -------------------------------------------------------------------------------
void ExpiredElementsDialog::clearPreview( void )
// -------------------------------------------------------------------------------
{
   ui.pElementIconLabel->setPixmap( QPixmap("") );
   ui.pElementNameLabel->setText( "" );
   ui.pChildCountLabel->setText( "-" );
   ui.pTextEdit->clear();
}


// -------------------------------------------------------------------------------
void ExpiredElementsDialog::previewEntry( QTreeWidgetItem* pItem )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == pItem )
      return;

   CInformationElement* pElement = expiredElementsMap.value( pItem );
   previewEntry( pElement );

}

// -------------------------------------------------------------------------------
void ExpiredElementsDialog::previewEntry( CInformationElement* pElement )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == pElement )
      return;

   ui.pElementIconLabel->setPixmap( QPixmap(pElement->getIconFileName()) );
   ui.pElementNameLabel->setText( pElement->getDescription() );
   ui.pChildCountLabel->setText( QString::number( pElement->childCount() ) );
   
   if ( pElement->getInformationFormat()->equals( InformationFormat::ASCII ) )
      ui.pTextEdit->setPlainText( pElement->getInformation() );
   else
      ui.pTextEdit->setHtml( pElement->getInformation() );
}

// -------------------------------------------------------------------------------
void ExpiredElementsDialog::deleteRemainingEntries( void )
// -------------------------------------------------------------------------------
{
   //std::cout<<"ExpiredElementsDialog::deleteRemainingEntries()"<<std::endl;
   if ( !ui.pExpiryList )
      return;
   
   
   QTreeWidgetItem* pX;
   while ( 0 < ui.pExpiryList->topLevelItemCount() )
   {
      pX = ui.pExpiryList->topLevelItem( 0 );
      
      CInformationElement* pElement = expiredElementsMap.value( pX );

      // It could be the case that the parent of this element was deleted
      // before. Hence this element does not exist anymore.
      if ( pElement )
      {
         pElement->deleteSelf();          // remove "actual" information element from collection
      }
   }
}


// -------------------------------------------------------------------------------
void ExpiredElementsDialog::slotShowContextMenu()
// -------------------------------------------------------------------------------
{
   mContextMenu.popup( QCursor::pos() );
}


// -------------------------------------------------------------------------------
void ExpiredElementsDialog::slotChangeActiveElementProperties()
// -------------------------------------------------------------------------------
{
   if ( !ui.pExpiryList )
      return;
      
   QTreeWidgetItem* pCurrentItem = ui.pExpiryList->currentItem();
   if ( !pCurrentItem )
      return;
   
   CPropertyDialog* pDialog = CPropertyDialog::getInstance();
   if ( !pDialog )
      return;
   
   CInformationElement* pElement = expiredElementsMap.value( pCurrentItem );
   Q_ASSERT( NULLPTR != pElement );
   pDialog->setUp( pElement, CPropertyDialog::MODE_CHANGE_PROPERTIES );
}
