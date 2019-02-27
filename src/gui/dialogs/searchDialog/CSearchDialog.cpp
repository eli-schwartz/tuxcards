/***************************************************************************
                          CSearchDialog.cpp  -  description
                             -------------------
    begin                : Sun Mar 26 2000
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

#include "CSearchDialog.h"
#include "../../../global.h"

#include "../../../information/CInformationCollection.h"
#include "CSearchListItemDelegate.h"
#include "CSearchListItem.h"


CSearchDialog* CSearchDialog::static_pInstance = NULLPTR;

// -------------------------------------------------------------------------------
CSearchDialog::CSearchDialog( QWidget* pParent )
 : QDialog( pParent )
 , mpCollection( NULLPTR )
// -------------------------------------------------------------------------------
{
   ui.setupUi( this );

   ui.mpWhereToSearchGroupBox->hide();

   // A user change is not possible any more. Size changes are managed by the layout.
   layout()->setSizeConstraint( QLayout::SetFixedSize );

   // Set size to prevent unwanted size changes of the dialog.
   ui.mpWhatToSearchGroupBox->setMinimumSize( ui.mpWhereToSearchGroupBox->sizeHint() );

   // Increase width of first column for better readability.
   ui.mpResultTreeWidget->setColumnWidth( 0, 150 );

   // Set connection for the "more button" text change and initialize it correctly.
   connect( ui.mpMoreButton, SIGNAL( toggled(bool) ), this, SLOT( slotChangeMoreButtonText(bool) ) );
   slotChangeMoreButtonText( ui.mpMoreButton->isChecked() );

   // Set connection for acutal search functionality.
   connect( ui.mpGoButton, SIGNAL( released() ), this, SLOT( slotStartSearching() ));

   // A connection between 'mpSearchLineEdit->returnPressed()' to 'mpGoButton->click()'
   // is not necessary. Since 'mpGoButton' is an "autodefault button" it does accept
   // a return-press automatically.

   // Set delegate for drawing the found entries the way we want it.
//   ui.mpResultTreeWidget->setItemDelegate( new CSearchListItemDelegate() );

   // Set connection: When item from result list is changed, it should be activated within
   // the tree and the found position should be highlighted.
   connect( ui.mpResultTreeWidget, SIGNAL( currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*) ),
            this, SLOT( slotSelectionChanged(QTreeWidgetItem*,QTreeWidgetItem*) ) );
}


// -------------------------------------------------------------------------------
CSearchDialog::~CSearchDialog( void )
// -------------------------------------------------------------------------------
{
   mpCollection = NULLPTR;
}


/**
 * Creates a property dialog instance with the given widget as parent.
 * This method should be called only once for the lifetime of the application.
 * Further calls are ignored.
 */
// -------------------------------------------------------------------------------
void CSearchDialog::createInstance( QWidget* pParent )
// -------------------------------------------------------------------------------
{
   if ( !static_pInstance )
   {
      static_pInstance = new CSearchDialog( pParent );
   }
}


/**
 * Returns the static instance.
 * If no property dialog was created with <code>createInstance()</code>,
 * then a <code>NULLPTR</code> is returned.
 */
// -------------------------------------------------------------------------------
CSearchDialog* CSearchDialog::getInstance()
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != static_pInstance );
   return static_pInstance;
}


/**
 * Changes the displayed text of the "More button".
 */
// -------------------------------------------------------------------------------
void CSearchDialog::slotChangeMoreButtonText( bool bChecked )
// -------------------------------------------------------------------------------
{
   QString sText = ( bChecked ? tr("More") + " <<<" : tr("More") + " >>>" );
   ui.mpMoreButton->setText( sText );
}

/**
 * Sets up the search dialog for a new search. The root element and active element
 * must be given.
 */
// -------------------------------------------------------------------------------
void CSearchDialog::setUp( CInformationCollection* pCollection )
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != pCollection );
   if ( !pCollection )
      return;

   mpCollection = pCollection;

   ui.mpStatusLine->setText("");

   //list->hide();
   //this->resize(550, 150);

   ui.mpSearchLineEdit->selectAll();
   ui.mpSearchLineEdit->setFocus();

   show();
   exec();
}

// -------------------------------------------------------------------------------
void CSearchDialog::slotStartSearching()
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return;

   QString sSearchString = ui.mpSearchLineEdit->text();
   if ( sSearchString.isEmpty() )
      return;

   ui.mpGoButton->setEnabled( false );
   ui.mpStatusLine->setText("Searching ...");

   // clear list
   ui.mpResultTreeWidget->clear();


   //// showing list
   //list->show();
   //resize(550, 350);


   // Preapare search.
   Qt::CaseSensitivity eCaseSensitive = ( ui.mpCaseSensitiveCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive );
   //std::cout<<"CSearchDialog::slotStartSearching() - case sensitiv = "<<eCaseSensitive<<std::endl;

   CInformationElement* pElement = NULLPTR;
   if ( isSearchOverAllElementsEnabled() )
      pElement = mpCollection->getRootElement();
   else
      pElement = mpCollection->getActiveElement();

   // Start search.
   if ( pElement )
      pElement->search( sSearchString, isRecursiveSearchEnabled(), eCaseSensitive, *(ui.mpResultTreeWidget) );


   // Show results.
   int iResultCount = ui.mpResultTreeWidget->topLevelItemCount();

   if ( iResultCount > 1 )
      ui.mpStatusLine->setText("<b>"+QString::number(iResultCount)+" matches found.</b>");
   else if ( iResultCount == 1 )
      ui.mpStatusLine->setText("<b>One match found.</b>");
   else
   {
      // no item found
      //list->hide(); this->resize(550, 150);
      ui.mpStatusLine->setText("<b>No match found.</b>");
   }

   ui.mpGoButton->setEnabled( true );
}


// -------------------------------------------------------------------------------
CSearchDialog::SD_SEARCHMODE CSearchDialog::getSearchMode() const
// -------------------------------------------------------------------------------
{
   SD_SEARCHMODE eSearchMode = SD_NONE;

   if ( ui.mpSearchAllEntriesRadioButton->isChecked() )
      eSearchMode = SD_ALL_ELEMENTS;
   else if ( ui.mpSearchCurrentEntryRecursivelyRadioButton->isChecked() )
      eSearchMode = SD_CURRENT_ELEMENT_AND_CHILDREN;
   else
      eSearchMode = SD_CURRENT_ELEMENT_ONLY;

   return eSearchMode;
}

/**
 * Convenience method for <code>getSearchMode()</code>.
 */
// -------------------------------------------------------------------------------
bool CSearchDialog::isRecursiveSearchEnabled() const
// -------------------------------------------------------------------------------
{
   return ( SD_CURRENT_ELEMENT_ONLY != getSearchMode() );
}

/**
 * Convenience method for <code>getSearchMode()</code>.
 */
// -------------------------------------------------------------------------------
bool CSearchDialog::isSearchOverAllElementsEnabled() const
// -------------------------------------------------------------------------------
{
   return ( SD_ALL_ELEMENTS == getSearchMode() );
}


/** **************** Make selection visible *************************************/
// -------------------------------------------------------------------------------
void CSearchDialog::slotSelectionChanged( QTreeWidgetItem* pCurrent, QTreeWidgetItem* )
// -------------------------------------------------------------------------------
{
   CSearchListItem* pItem = reinterpret_cast<CSearchListItem*>( pCurrent );
   if ( pItem )
      emit makeVisible( pItem->getSearchPosition() );
}
