/***************************************************************************
                          CDateInputDialog.cpp  -  description
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


#include "CDateInputDialog.h"
#include "../../../global.h"


CDateInputDialog* CDateInputDialog::static_pInstance = NULLPTR;

// -------------------------------------------------------------------------------
CDateInputDialog::CDateInputDialog( QWidget* pParent )
 : QDialog( pParent )
// -------------------------------------------------------------------------------
{
   ui.setupUi( this );
   setModal( TRUE );
}


// -------------------------------------------------------------------------------
CDateInputDialog::~CDateInputDialog()
// -------------------------------------------------------------------------------
{
}


/**
 * Creates a date input dialog instance with the given widget as parent.
 * This method should be called only once for the lifetime of the application.
 * Further calls are ignored.
 */
// -------------------------------------------------------------------------------
void CDateInputDialog::createInstance( QWidget* pParent )
// -------------------------------------------------------------------------------
{
   if ( !static_pInstance )
   {
      static_pInstance = new CDateInputDialog( pParent );
   }
}


/**
 * Returns the static instance.
 * If no date input dialog was created with <code>createInstance()</code>,
 * then a <code>NULLPTR</code> is returned.
 */
// -------------------------------------------------------------------------------
CDateInputDialog* CDateInputDialog::getInstance()
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != static_pInstance );
   return static_pInstance;
}


// -------------------------------------------------------------------------------
void CDateInputDialog::setUp( const QDate& initialDate )
// -------------------------------------------------------------------------------
{
   ui.mpCalendarWidget->setFirstDayOfWeek( Qt::Monday );
   
   QDate currentDate = QDate::currentDate();
   ui.mpCalendarWidget->setMinimumDate( currentDate );

   if ( initialDate < currentDate )
      ui.mpCalendarWidget->setSelectedDate( currentDate /*.addDays( DEFAULT_DURATION )*/ );
   else
      ui.mpCalendarWidget->setSelectedDate( initialDate );

   show();
//   exec();
}

// -------------------------------------------------------------------------------
QDate CDateInputDialog::getSelectedDate() const
// -------------------------------------------------------------------------------
{
   return ui.mpCalendarWidget->selectedDate();
}
