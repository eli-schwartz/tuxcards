/***************************************************************************
                          CTree.h  -  description
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

#ifndef CTREE_H
#define CTREE_H

#include "../information/CInformationCollection.h"
#include <QTreeView>
#include "../informationExt/CCollectionModel.h"
#include "../information/CInformationElementHistory.h"

#include <QTimer>
class QToolBar;

#include "./dialogs/passwdDialog/CPasswdDialog.h"


class CTree : public QTreeView,
              public IHistoryListener
{
   Q_OBJECT
public:
   CTree( QWidget* pParent = 0 );

   void                       setCollection( CInformationCollection& collection );
   void                       clearCollection();

   CCollectionModel&          getModel();

   void                       addActionsToToolBar( QToolBar& toolBar );

   // ************* IHistoryListener ******************
   virtual void               historyStatusChanged( bool bHasPreviousElement, bool bHasNextElement );


public slots:
   void                       addElement();
   void                       changeActiveElementProperties();
   void                       askForElementDeletionAndDeleteIt();
   void                       slotMoveElementUp();
   void                       slotMoveElementDown();

signals:
   void                       setActiveIEViaHistory( CInformationElement* pIE );
   void                       showMessage( QString sMessage, int iSeconds );

protected:
   void                       currentChanged( const QModelIndex& current,
                                              const QModelIndex& previous );

   virtual void               drawRow( QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

   // Drag & Drop
   virtual void               dragEnterEvent( QDragEnterEvent* pEvent );
   virtual void               dragMoveEvent(QDragMoveEvent* pEvent);
   virtual void               dropEvent( QDropEvent* pEvent );
   virtual void               mousePressEvent( QMouseEvent* pEvent );
   virtual void               mouseMoveEvent( QMouseEvent* pEvent );

private:
   enum eConnectionState
   {
      CONNECT,
      DISCONNECT
   };
   void                       connectTreeSignalsToSlots( const eConnectionState val );

   void                       setInitialExpandStateForTreeElements( const QModelIndex& parentIndex );

   void                       initModel();
   void                       selectLastActiveElement( CInformationCollection& collection );
   QString                    getDescriptionOfCurrentActiveEntry() const;

   CCollectionModel           mModel;

   // entry actions
   QAction*                   mpAddElementAction;
   QAction*                   mpChangeElementPropertyAction;
   QAction*                   mpRemoveElementAction;
   QAction*                   mpLockAction;
   QAction*                   mpRemoveLockAction;
   QAction*                   mpIEMoveUpAction;
   QAction*                   mpIEMoveDownAction;

   // history
   CInformationElementHistory mHistory;
   QAction*                   mpHistoryPrevAction;
   QAction*                   mpHistoryNextAction;

   void                       settingUpActions();
   void                       settingUpContextMenu();

   // Drag & Drop
   QModelIndex                mIndexToBeOpenedAutomaticallyDuringDrag;
   QTimer                     mAutoExpandTimer;
   QPoint                     mDragStartPoint;
   void                       startDrag();
   QModelIndex                mDraggedIndex;             // index that is dragged
   bool                       isEntryDroppedOnItself( const QModelIndex& dropIndex,
                                                      const QModelIndex& draggedIndexOrChild ) const;

   // Passwd dialog for encryption
   CPasswdDialog              mPasswdDialog;

private slots:
   void                       slotModelDataChanged(  const QModelIndex& index );
   void                       slotTreeItemCollapsed( const QModelIndex& index );
   void                       slotTreeItemExpanded(  const QModelIndex& index );

   void                       activeInformationElementChanged( CInformationElement* );
   void                       slotPropertyDialogSuccessfullyClosed();

   void                       slotEncryptActiveElement();
   void                       slotRemoveEncryptionFromActiveEntry();
   void                       slotAdjustEncryptionActions();

   void                       slotActivatePreviousHistoryElement();
   void                       slotActivateNextHistoryElement();

   void                       slotTimeout();
};

#endif
