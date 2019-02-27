/***************************************************************************
                          CEditor.h  -  description
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
#ifndef EDITOR_H
#define EDITOR_H

#include "../../global.h"
#include "./../../information/IView.h"
#include <QTextEdit>
#include <qclipboard.h>
#include <QKeyEvent>

#include "./../../information/CInformationElement.h"
#include <iostream>


/**
 * Using <code>LINEBREAK_WITH_CTRL_SHIFT</code> a single line break is
 * done by holding down the Ctrl or Shift key and pressing Enter/Return.
 * In this mode a paragraph is created with Enter/Return.
 *
 * Using <code>LINEBREAK_WITHOUT_CTRL_SHIFT</code> it is vice versa.
 * I.e. a single linebreak is done with Enter/Return. A new paragraph is
 * done by holding down the Ctrl or Shift key and pressing Enter/Return.
 */
enum eLinebreakMode
{
   LINEBREAK_WITH_CTRL_SHIFT,
   LINEBREAK_WITHOUT_CTRL_SHIFT
};

class CEditor : public QTextEdit,
                public IView
{
   Q_OBJECT
public:
   CEditor( QWidget *pParent = NULLPTR );
   virtual ~CEditor( void );

   QString                 getText( void );
   virtual void            setText( const QString& sText );
   void                    clear( void );

   void                    setEditorFont( const QFont& f );
   void                    setLineWrap( int iLineWrap );
//   void                    setLinebreakMode( eLinebreakMode eMode );

   void                    writeCurrentTextToActiveInformationElement( void );

//   int           countBRs( void );



   // ************** IView *************************************
   virtual void            aboutToRemoveElement( CInformationElement* pIE );
   // ************** IView - End *******************************

signals:
    void                   formatRecognized( InformationFormat );
    void                   currentHAlignmentChanged( Qt::Alignment );

public slots:
   void        		      toggleFontBold();
   void                    activeInformationElementChanged( CInformationElement* );

   void                    slotNewInformationElementAdded();

   //void        copy();
//   virtual void  paste();

   void                    insertImage();

private slots:
   void                    checkAlignment();
//   void          sendUndoAvailableSignal( void );
//   void          sendRedoAvailableSignal( void );

//   void          rereadInformation();


protected:
    void                   initialize( void );
    virtual void           keyPressEvent( QKeyEvent* pKeyEv );

   CInformationElement*    mpActiveElement;


private:
//    void                  printBRs( void );
//    void                  adaptClipboardText( QClipboard::Mode mode );

   bool                    SEMAPHORE_TEXT_WAS_SET;
//   eLinebreakMode          meLinebreakMode;


//    void          autoEncryptActiveInformationElement( CInformationElement* pActiveIE,
//                                                       CInformationElement* pNewIE );

   bool                    mbUseRichText;
   QFont                   mEditorFont;    // font used for ASCII text and as default for RTF text
};

#endif

