/***************************************************************************
                          CTree.cpp  -  description
                             -------------------
    begin                : Mon Mar 27 2000
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

#include "CTree.h"
#include "../informationExt/CCollectionModel.h"
#include "../information/CTreeInformationElement.h"
#include "dialogs/propertyDialog/CPropertyDialog.h"

#include <QToolBar>
#include "../utilities/CIconManager.h"
#define  getIcon(x)  CIconManager::getInstance().getIcon(x)

#include <QMessageBox>
#include <QDragEnterEvent>
#include "../CTuxCardsConfiguration.h"

#define CTREE_COLUMN_ZERO  0
#define CTREE_AUTO_EXPAND_TIME 750


// -------------------------------------------------------------------------------
CTree::CTree( QWidget* pParent )
 : QTreeView( pParent )
 , mModel()

 , mpAddElementAction( NULLPTR )
 , mpChangeElementPropertyAction( NULLPTR )
 , mpRemoveElementAction( NULLPTR )
 , mpLockAction( NULLPTR )
 , mpRemoveLockAction( NULLPTR )
 , mpIEMoveUpAction( NULLPTR )
 , mpIEMoveDownAction(NULLPTR )

 , mHistory()
 , mpHistoryPrevAction( NULLPTR )
 , mpHistoryNextAction( NULLPTR )
 , mIndexToBeOpenedAutomaticallyDuringDrag()
 , mAutoExpandTimer()
 , mDragStartPoint()
 , mDraggedIndex()

 , mPasswdDialog()           // with parent as NULLPTR
// -------------------------------------------------------------------------------
{
   CPropertyDialog::createInstance( this );

   CPropertyDialog* pDialog = CPropertyDialog::getInstance();
   connect( pDialog, SIGNAL(dataSuccessfullyEntered()),
            this,      SLOT(slotPropertyDialogSuccessfullyClosed()) );

   connect( &mAutoExpandTimer, SIGNAL(timeout()), this, SLOT(slotTimeout()) );

   settingUpActions();
   settingUpContextMenu();
   mHistory.setListener( this );

   setAcceptDrops( TRUE );

   initModel();
}


/** ************************** Actions ******************************************/
// -------------------------------------------------------------------------------
void CTree::settingUpActions()
// -------------------------------------------------------------------------------
{
   // entry actions - add, change, remove
   mpAddElementAction = new QAction( getIcon("addTreeElement"), tr("Add Entry (INSERT)"), this );
   mpAddElementAction->setShortcut( Qt::Key_Insert );
   connect( mpAddElementAction, SIGNAL(triggered()),  this, SLOT(addElement()) );

   mpChangeElementPropertyAction = new QAction( getIcon("changeProperty"), tr("Change Properties"), this );
   connect( mpChangeElementPropertyAction, SIGNAL(triggered()), this, SLOT(changeActiveElementProperties()) );

   mpRemoveElementAction = new QAction( getIcon("delete"), tr("Remove active Entry (DELETE)"), this );
   mpRemoveElementAction->setShortcut( Qt::Key_Delete );
   connect( mpRemoveElementAction, SIGNAL(triggered()), this, SLOT(askForElementDeletionAndDeleteIt()) );

   // move up/down actions
   mpIEMoveUpAction = new QAction( getIcon("upArrow"), tr("Move Current Entry Upwards"), this );
   connect( mpIEMoveUpAction, SIGNAL(triggered()), this, SLOT(slotMoveElementUp()) );

   mpIEMoveDownAction = new QAction( getIcon("downArrow"), tr("Move Current Entry Downwards"), this );
   connect( mpIEMoveDownAction, SIGNAL(triggered()), this, SLOT(slotMoveElementDown()) );

   // history actions
   mpHistoryPrevAction = new QAction( getIcon("back"), tr("Back, Previous Entry (Alt+Left)"), this );
   mpHistoryPrevAction->setShortcut( Qt::AltModifier + Qt::Key_Left );
   connect( mpHistoryPrevAction, SIGNAL(triggered()), this, SLOT(slotActivatePreviousHistoryElement()) );
   mpHistoryPrevAction->setEnabled( FALSE );

   mpHistoryNextAction = new QAction( getIcon("forward"), tr("Forward, Next Entry (Alt+Right)"), this );
   mpHistoryNextAction->setShortcut( Qt::AltModifier + Qt::Key_Right );
   connect( mpHistoryNextAction, SIGNAL(triggered()), this, SLOT(slotActivateNextHistoryElement()) );
   mpHistoryNextAction->setEnabled( FALSE );


   mpLockAction = new QAction( getIcon("lock"), tr("Encrypt active Entry"), this );
   connect( mpLockAction, SIGNAL(triggered()), this, SLOT(slotEncryptActiveElement()) );

   mpRemoveLockAction = new QAction( getIcon("unlock"), tr("Remove Encryption from active Entry"), this );
   connect( mpRemoveLockAction, SIGNAL(triggered()), this, SLOT(slotRemoveEncryptionFromActiveEntry()) );
   mpRemoveLockAction->setEnabled( FALSE );

}


/**
 * Adding action to this widget in order to create a context menu.
 */
// -------------------------------------------------------------------------------
void CTree::settingUpContextMenu()
// -------------------------------------------------------------------------------
{
   addAction( mpAddElementAction );
   addAction( mpChangeElementPropertyAction );
   addAction( mpRemoveElementAction );
   addAction( mpLockAction );
   addAction( mpRemoveLockAction );

   // add a separator
   QAction* pSeparator = new QAction( "", this );
   Q_ASSERT( pSeparator );
   pSeparator->setSeparator(TRUE);
   addAction( pSeparator );

   addAction( mpIEMoveUpAction );
   addAction( mpIEMoveDownAction );

   // add another separator
   pSeparator = new QAction( "", this );
   Q_ASSERT( pSeparator );
   pSeparator->setSeparator(TRUE);
   addAction( pSeparator );

   addAction( mpHistoryPrevAction );
   addAction( mpHistoryNextAction );
   setContextMenuPolicy( Qt::ActionsContextMenu );
}

/**
 * Adds the entry actions to the given toolbar.
 */
// -------------------------------------------------------------------------------
void CTree::addActionsToToolBar( QToolBar& toolBar )
// -------------------------------------------------------------------------------
{
   toolBar.addAction( mpAddElementAction );
   toolBar.addAction( mpChangeElementPropertyAction );
   toolBar.addAction( mpRemoveElementAction );
   toolBar.addAction( mpLockAction );
   toolBar.addAction( mpRemoveLockAction );
   toolBar.addSeparator();
   toolBar.addAction( mpIEMoveUpAction );
   toolBar.addAction( mpIEMoveDownAction );
   toolBar.addSeparator();
   toolBar.addAction( mpHistoryPrevAction );
   toolBar.addAction( mpHistoryNextAction );
}

// -------------------------------------------------------------------------------
void CTree::historyStatusChanged( bool bHasPreviousElement, bool bHasNextElement )
// -------------------------------------------------------------------------------
{
   //std::cout<<"historyStatusChanged("<<bHasPreviousElement<<","<<bHasNextElement<<")"<<std::endl;
   if ( mpHistoryPrevAction )
      mpHistoryPrevAction->setEnabled( bHasPreviousElement );

   if ( mpHistoryNextAction )
      mpHistoryNextAction->setEnabled( bHasNextElement );
}


// -------------------------------------------------------------------------------
void CTree::slotActivatePreviousHistoryElement()
// -------------------------------------------------------------------------------
{
   emit setActiveIEViaHistory( mHistory.getPrevious() );
}

// -------------------------------------------------------------------------------
void CTree::slotActivateNextHistoryElement()
// -------------------------------------------------------------------------------
{
   emit setActiveIEViaHistory( mHistory.getNext() );
}
/** ************************** Actions end **************************************/

// -------------------------------------------------------------------------------
void CTree::initModel()
// -------------------------------------------------------------------------------
{
   setModel( &mModel );

   // connect model to a slot
   connect( &mModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
               this, SLOT(slotModelDataChanged(const QModelIndex&)) );
}

/**
 * Returns a reference to the model, so that it can be used by other components
 * as well.
 */
// -------------------------------------------------------------------------------
CCollectionModel& CTree::getModel()
// -------------------------------------------------------------------------------
{
   return mModel;
}

// -------------------------------------------------------------------------------
void CTree::setCollection( CInformationCollection& collection )
// -------------------------------------------------------------------------------
{
   CInformationElement* pRootElement = collection.getRootElement();
   if ( !pRootElement )
      return;

   // TODO: Check all pointers!
   mModel.setInformationCollection( collection );

   // disconnect from the expand/collapse signals
   connectTreeSignalsToSlots( DISCONNECT );

   setInitialExpandStateForTreeElements( QModelIndex() );

   // connect to the expand/collapse signals
   connectTreeSignalsToSlots( CONNECT );

   selectLastActiveElement( collection );

   connect( &collection, SIGNAL(activeInformationElementChanged(CInformationElement*)),
            this,        SLOT(activeInformationElementChanged(CInformationElement*)) );

   collection.addView( &mHistory );
}


/**
 * Disconnects this tree from the currently connected collection.
 * It is necessary to clear the current collection right before it is
 * deleted and a new one is loaded.
 */
// -------------------------------------------------------------------------------
void CTree::clearCollection()
// -------------------------------------------------------------------------------
{
   mModel.clearCollection();
   setCurrentIndex( QModelIndex() );
}


/**
 * In order to apply correct expand/close-property for each "tree element"
 * after loading of a collection, this method must be called after a new
 * information collection has been set.
 *
 * Set the tree items expanded or closed depending on information in
 * informationelements.
 *
 * Call this method with an invalid modelindex in order to start from root tree
 * element.
 */
// -------------------------------------------------------------------------------
void CTree::setInitialExpandStateForTreeElements( const QModelIndex& parentIndex )
// -------------------------------------------------------------------------------
{
   if ( !mModel.hasChildren( parentIndex ) )
      return;

   QModelIndex childIndex;
   CTreeInformationElement* pTIE = NULLPTR;
   for ( int iRow = 0; iRow < mModel.rowCount( parentIndex); iRow++ )
   {
      childIndex = mModel.index( iRow, CTREE_COLUMN_ZERO, parentIndex );
      pTIE = mModel.mapIndexToIE( childIndex );

      Q_ASSERT( NULLPTR != pTIE );
      if ( pTIE )
      {
         //std::cout<<"desc = "<<pTIE->getDescription().toStdString()<<std::endl;
         if ( pTIE->isOpen() )
            expand( childIndex );
      }
      setInitialExpandStateForTreeElements( childIndex );
   }
}


/**
 * This method checks whether the last active element (that was selected before
 * saving the information collection to file) is valid. If yes, then it is set as
 * the current element and thus it is selected.
 *
 * This method must be called once after setting a new collection to this
 * view in order to preserve the "last mode".
 */
// -------------------------------------------------------------------------------
void CTree::selectLastActiveElement( CInformationCollection& collection )
// -------------------------------------------------------------------------------
{
   CInformationElement* pActiveElement = collection.getActiveElement();
   if ( pActiveElement )
   {
      activeInformationElementChanged( pActiveElement );

   }
}

/**
 * Through this slot, the tree will be notified, if the active element
 * within the structure has changed.
 */
// -------------------------------------------------------------------------------
void CTree::activeInformationElementChanged( CInformationElement* pElement )
// -------------------------------------------------------------------------------
{
   if ( !pElement )
      return;

   //std::cout<<"CTree::activeInformationElementChanged() - "<<pElement->getDescription().toStdString()<<std::endl;
   Path path( pElement );
   QModelIndex lastActiveIndex = mModel.mapPathToIndex( path );
   //std::cout<<"CTree::activeInformationElementChanged() - "<<mpModel->getIEDescription(lastActiveIndex).toStdString()<<std::endl;
   if ( lastActiveIndex.isValid() )
   {
      setCurrentIndex( lastActiveIndex );
   }



   // Responses to the change of the active informationelement. Adds the new element
   // to the history.
   mHistory.addElement( *pElement );
   //mHistory.dump();

   slotAdjustEncryptionActions();
}


/**
 * Connects/Disconnects some signals of the tree to this class (depending on the
 * given value <code>val</code>). As parameters the elements from
 * <code>eConnectionState</code> shall be used.
 *
 * The disconnection is needed, because during setting up a new collection.
 * During build up of the initial expand state of the entries, the signals can
 * be ignored.
 */
// -------------------------------------------------------------------------------
void CTree::connectTreeSignalsToSlots( const eConnectionState val )
// -------------------------------------------------------------------------------
{
   if ( CONNECT == val )
   {
      //std::cout<<"*** connect to tree signals"<<std::endl;
      connect( this, SIGNAL(collapsed(const QModelIndex&)),
               this, SLOT(slotTreeItemCollapsed(const QModelIndex&)) );
      connect( this, SIGNAL(expanded(const QModelIndex&)),
               this, SLOT(slotTreeItemExpanded(const QModelIndex&)) );
   }
   else
   {
      //std::cout<<"*** disconnect from tree signals"<<std::endl;
      disconnect( this, SIGNAL(collapsed(const QModelIndex&)),
                  this, SLOT(slotTreeItemCollapsed(const QModelIndex&)) );
      disconnect( this, SIGNAL(expanded(const QModelIndex&)),
                  this, SLOT(slotTreeItemExpanded(const QModelIndex&)) );
   }
}

// -------------------------------------------------------------------------------
void CTree::slotModelDataChanged( const QModelIndex& index )
// -------------------------------------------------------------------------------
{
   if ( index.isValid() )
   {
      QString newValue = mModel.data( index ).toString();
      std::cout<<"data changed to '"<<newValue.toStdString()<<"'"<<std::endl;
/*
      CInformationElement* pIE = mpModel->mapToInformationElement( const_cast<QModelIndex&>(index) );
      if ( pIE )
      {
         pIE->setDescription( newValue );
      }*/
   }
}

// -------------------------------------------------------------------------------
void CTree::slotTreeItemCollapsed( const QModelIndex& index )
// -------------------------------------------------------------------------------
{
   if ( index.isValid() )
   {
      //QVariant data = mModel.data( index );
      //std::cout<<"item '"<<data.toString().toStdString()<<"' collapsed"<<std::endl;

      CTreeInformationElement* pTIE = mModel.mapIndexToIE( index );
      if ( pTIE )
         pTIE->setOpen( FALSE );
   }
}

// -------------------------------------------------------------------------------
void CTree::slotTreeItemExpanded( const QModelIndex& index )
// -------------------------------------------------------------------------------
{
   if ( index.isValid() )
   {
      //QVariant data = mModel.data( index );
      //std::cout<<"item '"<<data.toString().toStdString()<<"' expanded"<<std::endl;

      CTreeInformationElement* pTIE = mModel.mapIndexToIE( index );
      if ( pTIE )
         pTIE->setOpen( TRUE );
   }
}

// -------------------------------------------------------------------------------
void CTree::currentChanged( const QModelIndex& current, const QModelIndex& previous )
// -------------------------------------------------------------------------------
{
   QTreeView::currentChanged( current, previous );

   if ( current.isValid() )
   {
      //QVariant data = mpModel->data( current );
      //std::cout<<"current item changed; new value = '"<<data.toString().toStdString()<<"'"<<std::endl;

      mModel.currentIndexChanged( current );
   }
}


/***************** 'Entry insertion', 'change' and 'removal' start **************/
/**
 * This slot is called if an element is added to the tree (and underlying data/model).
 * Adds a child element to the currently selected element.
 */
// -------------------------------------------------------------------------------
void CTree::addElement()
// -------------------------------------------------------------------------------
{
   Q_ASSERT( TRUE == mModel.isValid() );
   if ( !mModel.isValid() )
      return;

   if ( !currentIndex().isValid() )
   {
      QMessageBox::information( this, tr("Add Entry"),
                                tr("No parent entry selected."),
                                QMessageBox::Ok );
      return;
   }

   // open property dialog for new entry
   CPropertyDialog* pDialog = CPropertyDialog::getInstance();
   if ( !pDialog )
      return;

   pDialog->setUp( mModel.mapIndexToIE( currentIndex() ),
                   CPropertyDialog::MODE_CREATE_NEW_ELEMENT );
}

/**
 * Slot that is called when the property dialog is accepted.
 * In mode <code>CPropertyDialog::MODE_CREATE_NEW_ELEMENT</code> a new entry is
 * created with the data from the dialog.
 *
 * In mode <code>CPropertyDialog::MODE_CHANGE_PROPERTIES</code> nothing is done
 * (the entry is changed within the dialogs context).
 */
// -------------------------------------------------------------------------------
void CTree::slotPropertyDialogSuccessfullyClosed()
// -------------------------------------------------------------------------------
{
   CPropertyDialog* pPropertyDialog = CPropertyDialog::getInstance();
   if ( !pPropertyDialog )
      return;

   if ( CPropertyDialog::MODE_CREATE_NEW_ELEMENT == pPropertyDialog->getMode() )
   {
      // add the new entry at the end of the currently selected one
      mModel.insertRow( pPropertyDialog->getName(),
                        pPropertyDialog->getInformationFormat(),
                        pPropertyDialog->getIconFileName(),
                        pPropertyDialog->getDescriptionColor(),
                        pPropertyDialog->hasExpiryDateSet(),
                        pPropertyDialog->getExpiryDate(),

                        mModel.rowCount( currentIndex() ), currentIndex()
                      );

      setExpanded ( currentIndex(), TRUE );
   }
}


/**
 * Changes the properties of the currently active element by using the property
 * dialog.
 */
// -------------------------------------------------------------------------------
void CTree::changeActiveElementProperties()
// -------------------------------------------------------------------------------
{
   CPropertyDialog* pDialog = CPropertyDialog::getInstance();
   if ( !pDialog )
      return;

   pDialog->setUp( mModel.mapIndexToIE( currentIndex() ),
                   CPropertyDialog::MODE_CHANGE_PROPERTIES );
}


/**
 * This slot is called if an element should be deleted.
 * The corresponding datamodel is called to delete this element (after confirmation
 * of the user).
 */
// -------------------------------------------------------------------------------
void CTree::askForElementDeletionAndDeleteIt()
// -------------------------------------------------------------------------------
{
   Q_ASSERT( TRUE == mModel.isValid() );
   if ( !mModel.isValid() )
      return;


   if ( (0 == selectedIndexes().count()) || (!currentIndex().isValid()) )
   {
      QMessageBox::information( this, tr("Delete the active Entry"),
                                tr("No entry selected."),
                                QMessageBox::Ok );
      return;
   }


   if ( !currentIndex().parent().isValid() )
   {
      // root index selected for deleting
      QMessageBox::information( this, tr("Delete the active Entry"),
                                tr("The root entry cannot be deleted."),
                                QMessageBox::Ok );
      return;
   }


   QString sHasChildren = ( mModel.hasChildren(currentIndex()) ? tr("(and all children)?") : tr("?") );
   if( QMessageBox::No == QMessageBox::warning( this, tr("Delete the active Entry"),
                              tr("Do you really want to delete") + " '"
                              + getDescriptionOfCurrentActiveEntry() + "' "
                              + sHasChildren,
                              QMessageBox::Yes, QMessageBox::No) )
   {
      return;
   }

   mModel.removeRow( currentIndex().row(), mModel.parent(currentIndex()) );
}
/***************** 'Entry insertion', 'change' and 'removal' end ****************/

/***************** 'Entry encryption' start *************************************/
// -------------------------------------------------------------------------------
void CTree::slotEncryptActiveElement()
// -------------------------------------------------------------------------------
{
   Q_ASSERT( TRUE == mModel.isValid() );
   if ( !mModel.isValid() )
      return;

   QModelIndex curIndex = currentIndex();
   CTreeInformationElement* pActiveTIE = mModel.mapIndexToIE( curIndex );
   if ( !pActiveTIE )
   {
      QMessageBox::information( 0, "TuxCards", tr("To use encryption, an entry"
                                " must be selected."), QMessageBox::Abort );
      return;
   }



    if ( ! CTuxCardsConfiguration::getInstance().askForUsingEncryption() )
       return;

   if ( !pActiveTIE->isEncryptionEnabled() )
   {
      // Passwd needed
      mPasswdDialog.setUp( pActiveTIE->getDescription() );
      //std::cout<<"Entered Passwd: '"<<mPasswdDialog.getPasswd().toStdString()<<"'"<<std::endl;
      if ( !mPasswdDialog.getPasswd().isEmpty() )
      {
         //std::cout<<"CTree::slotEncryptActiveElement() - Passwd is not empty - Encryption is enabled."<<std::endl;
         pActiveTIE->enableEncryption( true, mPasswdDialog.getPasswd() );
      }
      else
      {
         //std::cout<<"Passwd is empty - Cancel was pressed."<<std::endl;
         return;
      }

   }

   if ( !pActiveTIE->isCurrentlyEncrypted() )
   {
      //std::cout<<"CTree::slotEncryptActiveElement() - Encrypt the entry."<<std::endl;
      currentChanged( currentIndex(), currentIndex() );   // necessary to save the current information from the editor to the element (before encryption)
      pActiveTIE->encrypt();
      //mpSingleEntryView->activeInformationElementChanged( pActiveTIE );
      currentChanged( currentIndex(), currentIndex() );   // necessary to hide the information of the currently encrypted element
      slotAdjustEncryptionActions();
   }
}

// -------------------------------------------------------------------------------
void CTree::slotRemoveEncryptionFromActiveEntry()
// -------------------------------------------------------------------------------
{
   Q_ASSERT( TRUE == mModel.isValid() );
   if ( !mModel.isValid() )
      return;

   QModelIndex curIndex = currentIndex();
   CTreeInformationElement* pActiveTIE = mModel.mapIndexToIE( curIndex );
   if ( !pActiveTIE )
   {
      QMessageBox::information( 0, "TuxCards", tr("There is no active entry."),
                                QMessageBox::Abort );
      return;
   }


   if ( pActiveTIE->isCurrentlyEncrypted() )
   {
      QMessageBox::information( 0, "TuxCards", tr("Entry must be decrypted "
                                "before encryption can be removed."),
                                QMessageBox::Abort );
      return;
   }

   pActiveTIE->enableEncryption( false, "" );
   slotAdjustEncryptionActions();
}

// -------------------------------------------------------------------------------
// Enables and disables the "encryption-" and "remove encryption-" actions/buttons.
// The state of the buttons is set using the currently active information element.
// -------------------------------------------------------------------------------
void CTree::slotAdjustEncryptionActions()
// -------------------------------------------------------------------------------
{
   CTreeInformationElement* pActiveTIE = mModel.mapIndexToIE( currentIndex() );
   if ( !pActiveTIE || !mpLockAction || !mpRemoveLockAction )
      return;


   if ( pActiveTIE->isEncryptionEnabled() )
   {
      // if entry is decrypted, then enable 'mpLockAction'
      //                        and  enable 'mpRemoveLockAction'
      bool bCurrentlyEncrypted = pActiveTIE->isCurrentlyEncrypted();
      mpLockAction->setEnabled( !bCurrentlyEncrypted );
      mpRemoveLockAction->setEnabled( !bCurrentlyEncrypted );
   }
   else
   {
      mpLockAction->setEnabled( true );
      mpRemoveLockAction->setEnabled( false );
   }
}
/***************** 'Entry encryption' end ***************************************/

/***************** 'Entry move up/down' start ***********************************/
// -------------------------------------------------------------------------------
void CTree::slotMoveElementUp()
// -------------------------------------------------------------------------------
{
   mModel.moveEntryOneUp( currentIndex() );
   currentChanged( currentIndex(), currentIndex() );   // necessary to move the position of the selection
}

// -------------------------------------------------------------------------------
void CTree::slotMoveElementDown()
// -------------------------------------------------------------------------------
{
   mModel.moveEntryOneDown( currentIndex() );
   currentChanged( currentIndex(), currentIndex() );   // necessary to move the position of the selection
}
/***************** 'Entry move up/down' end *************************************/

/**
 * Auxiliary method. Returns the desciption of the currently active item.
 */
// -------------------------------------------------------------------------------
QString CTree::getDescriptionOfCurrentActiveEntry() const
// -------------------------------------------------------------------------------
{
   QString sDescription("");

   QModelIndex curIndex = currentIndex();
   if ( curIndex.isValid() )
      sDescription = curIndex.data().toString();

   return sDescription;
}


/**
 * Overwritten method for using different colors within the tree for individual
 * entries.
 */
// -------------------------------------------------------------------------------
void CTree::drawRow( QPainter* pPainter, const QStyleOptionViewItem& option,
                     const QModelIndex& index ) const
// -------------------------------------------------------------------------------
{
   CTreeInformationElement* pTIE = mModel.mapIndexToIE( index );
   if ( !pTIE )
      return;


   QStyleOptionViewItem op = option;
   op.palette.setColor( QPalette::Text, pTIE->getDescriptionColor() );

   QTreeView::drawRow( pPainter, op, index );
}


/** *********************** Drag & Drop - start *********************************/
/*********** The next methods are for Drop **************************************/
// -------------------------------------------------------------------------------
void CTree::dragEnterEvent( QDragEnterEvent* pEvent )
// -------------------------------------------------------------------------------
{
   //std::cout<<"CTree::dragEnterEvent()"<<std::endl;

   if ( pEvent && (pEvent->mimeData()->hasFormat("text/plain") || pEvent->mimeData()->hasFormat(TUXCARDS_DRAGDROP_TYPE) ) )
      pEvent->acceptProposedAction();
}

// -------------------------------------------------------------------------------
void CTree::dragMoveEvent( QDragMoveEvent* pEvent )
// -------------------------------------------------------------------------------
{
   //std::cout<<"CTree::dragMoveEvent()"<<std::endl;

   if ( !pEvent || !mModel.isValid() )
      return;


   // prepare statusbar message
   QString sStatusBarMessage;
   if ( pEvent->mimeData()->hasFormat("text/plain") )
   {
      sStatusBarMessage = tr("Create entry below") + " '";
   }
   else if ( pEvent->mimeData()->hasFormat(TUXCARDS_DRAGDROP_TYPE) )
   {
      sStatusBarMessage = tr("Move to") + " '";
   }
   else
   {
      // if no supported mime format -> leave
      return;
   }



   // show statusbar message and open the tree element if necessary
   QModelIndex indexAtDragMovePos = indexAt( pEvent->pos() );
   if ( indexAtDragMovePos.isValid() )
   {
      currentChanged( indexAtDragMovePos, indexAtDragMovePos );  // TODO: Why do we have to give both arguments for the method 'currentChanged()'?
      emit showMessage( sStatusBarMessage + mModel.data( indexAtDragMovePos ).toString() + "'", 1 );


      if ( mIndexToBeOpenedAutomaticallyDuringDrag != indexAtDragMovePos )
      {
         mAutoExpandTimer.stop();
         mIndexToBeOpenedAutomaticallyDuringDrag = indexAtDragMovePos;
         mAutoExpandTimer.start( CTREE_AUTO_EXPAND_TIME );
      }
   }
   else
   {
      emit showMessage( "", 1 );
      //pEvent->ignore();
      mAutoExpandTimer.stop();
      mIndexToBeOpenedAutomaticallyDuringDrag = QModelIndex();   // invalidate the 'mIndexToBeOpenedAutomaticallyDuringDrag'
   }

   pEvent->acceptProposedAction();
}


/**
 * Overwritten method for accepting drop events.
 */
// -------------------------------------------------------------------------------
void CTree::dropEvent( QDropEvent* pEvent )
// -------------------------------------------------------------------------------
{
   //std::cout<<"CTree::dropEvent()"<<std::endl;

   if ( !pEvent || !mModel.isValid() )
      return;

   if ( pEvent->mimeData()->hasFormat("text/plain") || pEvent->mimeData()->hasFormat(TUXCARDS_DRAGDROP_TYPE) )
   {
      QModelIndex parentIndex = indexAt( pEvent->pos() );

      // 'mDraggedIndex' is the index where the drag operation started.
      if ( (pEvent->source() == this ) && isEntryDroppedOnItself( parentIndex, mDraggedIndex ) )
      {
         // entry is moved on itself or on an own child -> this is no valid action
         QMessageBox::warning( this, tr("Drag & Drop"), tr("An Entry cannot be moved onto itself or a child."), QMessageBox::Ok );
         emit showMessage( tr("Drag & Drop operation not possible."), 5 );
         return;
      }

      expand( parentIndex );

      // Create a new entry below <code>parent</code>. <code>iColumn</code> is
      // not used and thus set to '-1'.
      bool bAcceptDrop = mModel.dropMimeData( pEvent->mimeData(), pEvent->dropAction(),
                                              mModel.rowCount( currentIndex() ), -1,
                                              parentIndex );

      if ( !bAcceptDrop )
         pEvent->ignore();
   }

   pEvent->acceptProposedAction();
}


/**
 * This method is called whenever the <code>mAutoExpandTimer</code> times out.
 * It is used to automatically expand an entry during drag&drop.
 */
// -------------------------------------------------------------------------------
void CTree::slotTimeout()
// -------------------------------------------------------------------------------
{
   mAutoExpandTimer.stop();

   if ( !mIndexToBeOpenedAutomaticallyDuringDrag.isValid() )
      return;

   if ( !isExpanded( mIndexToBeOpenedAutomaticallyDuringDrag ) )
      expand( mIndexToBeOpenedAutomaticallyDuringDrag );
}

/*********** The next methods are for Drag **************************************/
// -------------------------------------------------------------------------------
void CTree::mousePressEvent( QMouseEvent* pEvent )
// -------------------------------------------------------------------------------
{
   //std::cout<<"CTree::mousePressEvent()"<<std::endl;
   if ( !pEvent )
      return;

   if ( Qt::LeftButton == pEvent->button() )
   {
      mDragStartPoint = pEvent->pos();

      // TODO: Some special cases are still open and might be implemented:
      //        - Currently the drag is started after the manhattan distance was
      //          passed with the pressed mouse button. If one is very close to
      //          the border of another entry and start a drag, then this other
      //          entry will be dragged. (Sources can be found in the previous
      //          implementation.)
      //        - A drag started from the "item's decoration" (i.e. from the
      //          "plus/minus-sign" for expanding/closing the entry) will happen
      //          this should be prohibited. (Sources can be found in the previous
      //          implementation.)
      //        - The root entry itself should not be dragable. It is possible to
      //          start a drag there (nevertheless it not allowed after releasing
      //          the mouse button).
   }

   QTreeView::mousePressEvent( pEvent );
}

// -------------------------------------------------------------------------------
void CTree::mouseMoveEvent( QMouseEvent* pEvent )
// -------------------------------------------------------------------------------
{
   //std::cout<<"CTree::mouseMoveEvent()"<<std::endl;
   if ( !pEvent )
      return;

   if ( pEvent->buttons() & Qt::LeftButton )
   {
      int iDistance = (mDragStartPoint - pEvent->pos()).manhattanLength();
      if ( iDistance > QApplication::startDragDistance() )
         startDrag();
   }

   QTreeView::mouseMoveEvent( pEvent );
}

// -------------------------------------------------------------------------------
void CTree::startDrag()
// -------------------------------------------------------------------------------
{
   if ( !mModel.isValid() )
      return;

   QModelIndex curIndex = currentIndex();
   if ( !curIndex.isValid() )
      return;
   mDraggedIndex = curIndex;           // needed to check whether an entry is dropped on itself or a child

   QModelIndexList list;
   list.append( curIndex );
   QMimeData* pMimeData = mModel.mimeData( list );
   QDrag* pDrag = new QDrag( this );
   pDrag->setMimeData( pMimeData );
   //TODO: dragObject->setPixmap(QPixmap(picture_xpm),QPoint(8,8));

   //std::cout<<"start drag..."<<std::endl;
   if ( Qt::MoveAction == pDrag->exec(Qt::CopyAction | Qt::MoveAction) )
   {
      mModel.removeRow( curIndex.row(), mModel.parent(curIndex) );
      //std::cout<<"... moveAction -> old entry is deleted"<<std::endl;

      mAutoExpandTimer.stop();
   }
}

/**
 * Auxiliary method that checks whether an entry is dropped on itself or on an
 * own child (during a drag&drop action).
 *
 * <code>dropIndex</code> is the index where the drag&drop operation ended (i.e.
 * where the dragged entry should be dropped on).
 *
 * <code>draggedIndexOrChild</code> is the entry that is dragged (i.e. where the
 * drag operation started).
 *
 * Returns <code>true</code> if it is dropped on itself. Otherwise <code>false</code>.
 */
// -------------------------------------------------------------------------------
bool CTree::isEntryDroppedOnItself( const QModelIndex& dropIndex,
                                    const QModelIndex& draggedIndexOrChild ) const
// -------------------------------------------------------------------------------
{
   //std::cout<<"CTree::isEntryDroppedOnItself() - test '"<<mpModel->data(dropIndex).toString().toStdString()<<"' "
   //         <<"against '"<<mpModel->data(mDraggedIndexOrChild).toString().toStdString()<<"'"<<std::endl;

   if ( dropIndex ==  draggedIndexOrChild )
      return TRUE;

   QModelIndex parentIndex = draggedIndexOrChild;

   QModelIndex x;
   for ( int iRow = 0; iRow < mModel.rowCount( parentIndex ); iRow++ )
   {
      x = mModel.index( iRow, 0, parentIndex );
      if ( isEntryDroppedOnItself( dropIndex, x ) )
         return TRUE;
   }

   return FALSE;
}
/** *********************** Drag & Drop - end ***********************************/
