/***************************************************************************
                          CDateInputDialog.h  -  description
                             -------------------
    begin                : Fri Jun 08 2007
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


#ifndef C_DATE_INPUT_DIALOG_H
#define C_DATE_INPUT_DIALOG_H

#include "../../../../output/ui/ui_IDateInputDialog.h"

class CDateInputDialog : public QDialog {
   Q_OBJECT
public:
   ~CDateInputDialog();
   
   static void              createInstance( QWidget* pParent );
   static CDateInputDialog* getInstance();
   
   void                     setUp( const QDate& initialDate = QDate::currentDate() );
   QDate                    getSelectedDate() const;


private:
   CDateInputDialog( QWidget* pParent );
   static CDateInputDialog* static_pInstance;

   //#define DEFAULT_DURATION 1
   
   Ui::IDateInputDialog ui;
};

#endif
