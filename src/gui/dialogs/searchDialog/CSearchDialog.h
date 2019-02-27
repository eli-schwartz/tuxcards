/***************************************************************************
                          CSearchDialog.h  -  description
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

#ifndef C_SEARCH_DIALOG_H
#define C_SEARCH_DIALOG_H

#include "../../../../output/ui/ui_ISearchDialog.h"

class CInformationCollection;
class CSearchPosition;


class CSearchDialog : public QDialog {
   Q_OBJECT
public:
   ~CSearchDialog();

   static void               createInstance( QWidget* pParent );
   static CSearchDialog*     getInstance();

   void                      setUp( CInformationCollection* pCollection );

private slots:
   void                      slotChangeMoreButtonText( bool bChecked );
   void                      slotStartSearching();

   void                      slotSelectionChanged( QTreeWidgetItem* pCurrent, QTreeWidgetItem* );

signals:
   void                      makeVisible( const CSearchPosition* const );

private:
   CSearchDialog( QWidget* pParent );
   static CSearchDialog*     static_pInstance;

   CInformationCollection*   mpCollection;
   enum SD_SEARCHMODE
   {
      SD_NONE = -1,
      SD_ALL_ELEMENTS,
      SD_CURRENT_ELEMENT_AND_CHILDREN,
      SD_CURRENT_ELEMENT_ONLY
   };
   SD_SEARCHMODE             getSearchMode() const;
   bool                      isRecursiveSearchEnabled() const;
   bool                      isSearchOverAllElementsEnabled() const;

   Ui::ISearchDialog ui;
};

#endif
