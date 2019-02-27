/***************************************************************************
                          CSingleEntryView.h  -  description
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
#ifndef CSINGLE_ENTRY_VIEW_H
#define CSINGLE_ENTRY_VIEW_H

#include <QStackedWidget>
#include "./../../information/IView.h"
#include "./CEditor.h"
class CEncryptedEntryView;


class CSingleEntryView : public QStackedWidget,
                         public IView
{
   Q_OBJECT
public:
   CSingleEntryView( QWidget* pParent );
   ~CSingleEntryView();

   CEditor*             getEditor();

   // methods added because of editor
   QString              getText();
   virtual void         setText( const QString& sText );

   void                 writeCurrentTextToActiveInformationElement( void );

//   int                countBRs( void );
   // methods added because of editor - end

   // ************** IView *************************************
   virtual void         aboutToRemoveElement( CInformationElement* pIE );
   // ************** IView - End *******************************

public slots:
   // methods added because of editor
   void                 activeInformationElementChanged( CInformationElement* pIE );
   // methods added because of editor - end

protected:

private:
   CInformationElement* mpActiveElement;
   CEditor*             mpEditor;
   CEncryptedEntryView* mpPasswdView;

   enum { EDITOR, PASSWD };

private slots:
   void                 slotEntryDecrypted();

signals:
   void                 signalEntryDecrypted();
};

#endif
