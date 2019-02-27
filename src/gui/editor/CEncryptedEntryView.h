/***************************************************************************
                          CEncryptedEntryView.h  -  description
                             -------------------
    begin                : Fri Jan 09 2004
    copyright            : (C) 2004 by Alexander Theel
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
#ifndef CENCRYPTED_ENTRY_VIEW_H
#define CENCRYPTED_ENTRY_VIEW_H

#include "../../global.h"
#include "./../../information/IView.h"

#include <QWidget>
class QLabel;
class QLineEdit;


class CEncryptedEntryView : public QWidget,
                            public IView
{
   Q_OBJECT
public:
   CEncryptedEntryView( QWidget* pParent = NULLPTR );
   ~CEncryptedEntryView();

   // ************** IView *************************************
   virtual void          aboutToRemoveElement( CInformationElement* pIE );
   // ************** IView - End *******************************

   void                  activeInformationElementChanged( CInformationElement* pIE );

signals:
   void                  signalEntryDecrypted();


private:
   QLineEdit*            mpPasswdLine;
   QLabel*               mpResultLabel;

   CInformationElement*  mpActiveElement;

private slots:
   void                  slotPasswdEntered();
};

#endif
