/***************************************************************************
                          CIconSelectorDialog.cpp  -  description
                             -------------------
    begin                : Sun Jun 10 2007
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


#ifndef C_IconSelectorDialog_H
#define C_IconSelectorDialog_H

#include "../../../../output/ui/ui_ISelectorDialog.h"
#include <QDir>

class CIconSelectorDialog : public QDialog {
   Q_OBJECT
public:
   static CIconSelectorDialog*   getInstance( QWidget* pParent );
   QString                       getIconFileName() const;
   
private slots:
   void                          cdUp();
   void                          itemDoubleClicked( QListWidgetItem* pItem );
   void                          fileSelected( QListWidgetItem* pItem );

private:
   CIconSelectorDialog( QWidget* pParent );
   static CIconSelectorDialog*   static_pInstance;

   void                          directoryChanged( const QString& );
   void                          setPathForComboBox();
   void                          readDirectory( const QDir& dir );

   void                          progressBar_startReadDir( int iDirCount );
   void                          progressBar_readNextDir();
   void                          progressBar_readDirDone();
   
   QDir                          mCurrentDir;
   
   Ui::ISelectorDialog           ui;
};


#endif
