/***************************************************************************
                          CEditor.cpp  -  description
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
#include "CEditor.h"

#include <qregexp.h>
#include <QKeyEvent>
#include "../../CTuxCardsConfiguration.h"
#include <QScrollBar>

#include <qapplication.h>

#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>


// -------------------------------------------------------------------------------
CEditor::CEditor( QWidget *pParent )
  : QTextEdit( pParent )
  , mpActiveElement( NULLPTR )
  , SEMAPHORE_TEXT_WAS_SET( FALSE )
//  , meLinebreakMode( LINEBREAK_WITH_CTRL_SHIFT )
  , mbUseRichText( true )
  , mEditorFont()
// -------------------------------------------------------------------------------
{
   initialize();

   connect( this, SIGNAL(cursorPositionChanged()), this, SLOT(checkAlignment()) );
}


// -------------------------------------------------------------------------------
CEditor::~CEditor( void )
// -------------------------------------------------------------------------------
{
   mpActiveElement = NULLPTR;
}



// ************** IView *********************************************************
// -------------------------------------------------------------------------------
void CEditor::aboutToRemoveElement( CInformationElement* pIE )
// -------------------------------------------------------------------------------
{
   if ( mpActiveElement == pIE )
   {
      mpActiveElement = NULLPTR;
   }
}
// ************** IView - End ****************************************************


/**
 * Returns the complete Text.
 */
// -------------------------------------------------------------------------------
QString CEditor::getText( void )
// -------------------------------------------------------------------------------
{
  return toPlainText();
}



// -------------------------------------------------------------------------------
void CEditor::setText( const QString& sText )
// -------------------------------------------------------------------------------
{
   SEMAPHORE_TEXT_WAS_SET = TRUE;
   
   if ( mbUseRichText )
      setHtml( sText );
   else
   {
      setCurrentFont( mEditorFont );     // use the same font for all ASCII contents
      setPlainText( sText );
   }
}




/**
 * Rereads the information from the currently active
 * informationElement; usually in course of the event
 * 'CInformationElement::informationHasChanged()'.
 */
// // -------------------------------------------------------------------------------
// void CEditor::rereadInformation( void )
// // -------------------------------------------------------------------------------
// {
//    if ( NULLPTR == mpActiveElement )
//       return;
//
//    setText( mpActiveElement->getInformation() );
// }



// -------------------------------------------------------------------------------
void CEditor::clear( void )
// -------------------------------------------------------------------------------
{
   initialize();
}


/**
 * Set the editor font (= font for ASCII texts and default font for RTF texts).
 * If an ASCII text is visible, then its font is changed right away.
 */
// -------------------------------------------------------------------------------
void CEditor::setEditorFont( const QFont& f )
// -------------------------------------------------------------------------------
{
   mEditorFont = f;

   // if ascii entry is activ then change the font for the displayed entry right away
   if ( !mbUseRichText )
   {
      if ( mpActiveElement )
      {
         // Remember the cursor position and set it after font change.
         // (Just remebering the cursor and setting it does not work with qt4.3.1
         //  there is in error within the implementation or documentation.)
         int iCursorPos = textCursor().position();
         
         writeCurrentTextToActiveInformationElement();
         setText( mpActiveElement->getInformation() );

         QTextCursor c = textCursor(); c.setPosition( iCursorPos );
         setTextCursor( c );
      }
   }
}


// -------------------------------------------------------------------------------
void CEditor::initialize( void )
// -------------------------------------------------------------------------------
{
   setText("");
   mpActiveElement = NULLPTR;

   SEMAPHORE_TEXT_WAS_SET = FALSE;
//  connect( this, SIGNAL(textChanged()), this, SLOT(sendUndoAvailableSignal()));
//  connect( this, SIGNAL(textChanged()), this, SLOT(sendRedoAvailableSignal()));
}


/**
 * Checks whether the current horizontal text alignment has changed. If yes,
 * then an appropriate signal is sent.
 */
// -------------------------------------------------------------------------------
void CEditor::checkAlignment()
// -------------------------------------------------------------------------------
{
   static Qt::Alignment static_lastAlignment = Qt::AlignLeft;

   if ( alignment() != static_lastAlignment )
   {
      static_lastAlignment = alignment();
      emit( currentHAlignmentChanged(static_lastAlignment) );
   }
}


/**
 * This method sends the signal 'undoAvailable()' because the base
 * class (qtextedit qt-3.0.5) does not send it correctly. But the
 * signal is not send whenever the text was set  with 'setText()'
 * method to prevent that the user gets the text from another
 * document while doing an undo.
 */
/* maybe this is not needed with qt4
// -------------------------------------------------------------------------------
void CEditor::sendUndoAvailableSignal( void )
// -------------------------------------------------------------------------------
{
  if ( isUndoAvailable() )
  {
    if ( SEMAPHORE_TEXT_WAS_SET )
      SEMAPHORE_TEXT_WAS_SET = FALSE;
    else
      emit undoAvailable( TRUE );
  }
}
*/


/**
 * Similar to the method above ('sendUndoAvailableSignal()').
 * This method sends the signal 'redoAvailable()' because the base
 * class (qtextedit qt-3.0.5) does not send it correctly.
 */
/* maybe this is not needed with qt4
// -------------------------------------------------------------------------------
void CEditor::sendRedoAvailableSignal( void )
// -------------------------------------------------------------------------------
{
  if ( isRedoAvailable() )
    emit redoAvailable(true);
}
*/



// -------------------------------------------------------------------------------
void CEditor::writeCurrentTextToActiveInformationElement( void )
// -------------------------------------------------------------------------------
{
   if ( !mpActiveElement )
      return;

 	// saving cursor- and scrollbarPositions
//	int line, pos; getCursorPosition(&line, &pos);
//  int verticalScrollBarPosition   = verticalScrollBar()->value();
//  int horizontalScrollBarPosition = horizontalScrollBar()->value();

//  cout<<"vorher: Text "<<getText().contains("<br>")<<endl;
// //  printBRs();
// //    if ( mbUseRichText )
// //    {
// // 	   QMessageBox::information( this, "Debugging", QString("<b>Richtext</b><p>")
// // 		                         + "vorher: Text [" 
// // 								 + toHtml()
// // 								 + "<br>]"
// // 							 , "Ok" );
// //    }
// //    else
// //    {
// // 	   QMessageBox::information( this, "Debugging", QString("<b>Plaintext</b><p>")
// // 		                         + "vorher: Text [" 
// // 								 + toPlainText()
// // 								 + "<br>]"
// // 							 , "Ok" );
// //    }


   if ( mbUseRichText )
      mpActiveElement->setInformation(toHtml());
   else
      mpActiveElement->setInformation(toPlainText());

// //    std::cout<<"nachher: mpActiveElement="<<mpActiveElement->getDescription().toStdString()<<"\n"
// // 	        <<"         Text ["<<getText().toStdString()<<"]"<<std::endl;
// //    //std::cout<<"nachher: Text ["<<getText()<<"]"<<std::endl;

//  cout<<"nachher: Text "<<getText().contains("<br>")<<endl;
//  printBRs();

  // reset cursor- and scrollbarPositions
//  setCursorPosition(line, pos);
//  verticalScrollBar()->setValue(verticalScrollBarPosition);
//  horizontalScrollBar()->setValue(horizontalScrollBarPosition);
}


// -------------------------------------------------------------------------------
void CEditor::setLineWrap( int iLineWrap )
// -------------------------------------------------------------------------------
{
  if ( 0 == iLineWrap )
    QTextEdit::setLineWrapMode( QTextEdit::NoWrap );
  else if( 1 == iLineWrap )
    QTextEdit::setLineWrapMode( QTextEdit::WidgetWidth );
  else
  {
    QTextEdit::setLineWrapMode( QTextEdit::FixedColumnWidth );
    QTextEdit::setLineWrapColumnOrWidth( iLineWrap );
  }
}


/**
 * Ignores the key combinations we need within 'mainwindow'.
 */
// -------------------------------------------------------------------------------
void CEditor::keyPressEvent( QKeyEvent* pKeyEv )
// -------------------------------------------------------------------------------
{
   if ( !pKeyEv )
      return;

//    //std::cout<<"key = "<<pKeyEv->key();
//
//    // If
//    //   CTRL + S (save)  or
//    //   ALT + left  cursor (history one back / previous entry) or
//    //   ALT + right cursor (history one forward / next entry)
//    //   CTRL + F (shortcut for "Searching")
//    // is called, then ignore it.
//    if( ( (pKeyEv->modifiers() == Qt::ControlModifier)  &&  (pKeyEv->key() == Qt::Key_S) ) ||
//        ( (pKeyEv->modifiers() == Qt::AltModifier)      &&  (pKeyEv->key() == Qt::Key_Left) ) ||
//        ( (pKeyEv->modifiers() == Qt::AltModifier)      &&  (pKeyEv->key() == Qt::Key_Right) ) ||
//        ( (Qt::ControlModifier == pKeyEv->modifiers())  &&  (Qt::Key_F == pKeyEv->key()) ) )
//    {
//       //std::cout<<" -> ignore"<<std::endl;
//       pKeyEv->ignore();
//    }
//
//    // if the shift button is down while pressing return or enter, insert a new paragraph
//    else if  ( ( (Qt::ShiftModifier == pKeyEv->modifiers()) || (Qt::ControlModifier == pKeyEv->modifiers()) )
//               &&
//               ( (Qt::Key_Return == pKeyEv->key()) || (Qt::Key_Enter == pKeyEv->key()) )
//             )
//    {
//       if ( LINEBREAK_WITHOUT_CTRL_SHIFT == meLinebreakMode )
//       {
//          //std::cout<<" -> shift/ctrl + enter/return"<<std::endl;
//          QTextEdit::keyPressEvent( new QKeyEvent( pKeyEv->type(),  Qt::Key_Return,
//                                                   Qt::NoModifier, pKeyEv->text() ) );
//       }
//       else
//       {
//          QTextEdit::keyPressEvent( pKeyEv );
//       }
//    }
//
//    // if a simple return or enter is pressed, do not insert a new paragraph
//    else if  ( (Qt::Key_Return == pKeyEv->key()) || (Qt::Key_Enter == pKeyEv->key()) )
//    {
//       if ( LINEBREAK_WITHOUT_CTRL_SHIFT == meLinebreakMode )
//       {
//          //std::cout<<" -> simple enter/return"<<std::endl;
//          QTextEdit::keyPressEvent( new QKeyEvent( pKeyEv->type(),  Qt::Key_Return,
//                                                   Qt::ControlModifier, pKeyEv->text() ) );
//       }
//       else
//       {
//          QTextEdit::keyPressEvent( pKeyEv );
//       }
//    }
//    /*else if ( (pKeyEv->state() == Qt::ShiftModifier)  && (pKeyEv->key() == Qt::Key_Return) )
//    {
//       QTextEdit::keyPressEvent( new QKeyEvent( pKeyEv->type(),  Qt::Key_Enter,
//                                                pKeyEv->ascii(), ControlButton ) );
//    }
//    */
//
//    // CTRL + A -> select all
//    else if ( (Qt::ControlModifier == pKeyEv->modifiers()) && (Qt::Key_A == pKeyEv->key()) )
//    {
//       selectAll();
//    }
//

//   else
   {
      // otherwise call the super-method
      //std::cout<<std::endl;
      QTextEdit::keyPressEvent( pKeyEv );
   }
}


/**
 * Toggles the bold state of the currently highlighted text.
 */
// -------------------------------------------------------------------------------
void CEditor::toggleFontBold()
// -------------------------------------------------------------------------------
{
   //if ( Qt::RichText == textFormat() )
      QFont::Weight newWeight = ( QFont::Bold == fontWeight() ? QFont::Normal : QFont::Bold );
      setFontWeight( newWeight );
}


/**
 * Responses to the change of the active informationelement.
 * Saves the information of the old one and displays the new one.
 */
// -------------------------------------------------------------------------------
void CEditor::activeInformationElementChanged( CInformationElement* pElement )
// -------------------------------------------------------------------------------
{
/*  QMimeSourceFactory::defaultFactory()->setPixmap("newImg", QPixmap("/home/alex/aufgehoben/bilder/alex.gif"));
  setText( " Hier ist ein Bild <img source=\"newImg\">." );
return;
*/


   if ( !pElement )
      return;

   //std::cout<<"CEditor::activeInformationElementChanged()  "
   //         <<"new Element: "<<pElement->getDescription().toStdString()<<std::endl;
   if ( mpActiveElement )
   {
      disconnect( mpActiveElement, SIGNAL(informationHasChanged()), this, SLOT(rereadInformation()) );

      writeCurrentTextToActiveInformationElement();
//      autoEncryptActiveInformationElement( mpActiveElement, pElement );
      mpActiveElement->setInformationYPos( ((QAbstractSlider*)verticalScrollBar())->sliderPosition() );
   }

   if ( pElement->getInformationFormat() == InformationFormat::getByString("RTF") )
   {
      emit formatRecognized( *InformationFormat::getByString("RTF") );
      mbUseRichText = true;                    // setTextFormat( Qt::RichText );
   }
   else if ( pElement->getInformationFormat() == InformationFormat::getByString("ASCII") )
   {
      emit formatRecognized( *InformationFormat::getByString("ASCII") );
      mbUseRichText = false;                   // setTextFormat( Qt::PlainText );
   }
   else
   {
      // This should not happen. An informationFormat should always be != NONE.
      Q_ASSERT( FALSE );
   }

   setAcceptRichText( mbUseRichText );
   setText( pElement->getInformation() );
   verticalScrollBar()->setSliderPosition( pElement->getInformationYPos() );

   mpActiveElement = pElement;
   connect( mpActiveElement, SIGNAL(informationHasChanged()), this, SLOT(rereadInformation()) );
}

/**
 * Use the information about a newly inserted information element in order to
 * set the focus for the editor.
 * This enables the user to start writing text right after the new information
 * element is available.
 */
// -------------------------------------------------------------------------------
void CEditor::slotNewInformationElementAdded()
// -------------------------------------------------------------------------------
{
   //std::cout << "CEditor::slotNewInformationElementAdded()" << std::endl;

   // Set font for all new entries (this is only necessary for RTF-texts
   // (= setting of default font), but it does not harm for ASCII-texts.
   // (It is not necessary for ASCII-texts, because they get the font set
   // anyway within method 'CEditor::setText()'.)
   setCurrentFont( mEditorFont );

   // switch focus to the editor, so that editing can be started right away
   setFocus();
}

// -------------------------------------------------------------------------------
// If a new entry ('pNewIE') is selected and if autoencryption is turned on, the
// currently active entry ('pActiveIE') is encrypted.
//
// The element 'pNewIE' is the new selected one. It is needed for comparision with
// the active one. If both are the same elements nothing is done within this
// method.
// // -------------------------------------------------------------------------------
// void CEditor::autoEncryptActiveInformationElement( CInformationElement* pActiveIE,
//                                                   CInformationElement* pNewIE )
// // -------------------------------------------------------------------------------
// {
//    if ( !pActiveIE || !pNewIE )
//       return;
//
//    if ( pActiveIE == pNewIE )
//       return;
//
//    //std::cout<<"autoencrypt .. ";
//    if ( CTuxCardsConfiguration::getInstance().getBoolValue( CTuxCardsConfiguration::B_AUTOENCRYPT_ENTRY ) )
//    {
//       if ( pActiveIE->isEncryptionEnabled() )
//       {
//          if ( !pActiveIE->isCurrentlyEncrypted() )
//          {
//             //std::cout<<"(encrypt) .. ";
//             pActiveIE->encrypt();
//          }
//       }
//    }
//    //std::cout<<"done."<<std::endl;
// }



// // -------------------------------------------------------------------------------
// void CEditor::printBRs( void )
// // -------------------------------------------------------------------------------
// {
//   std::cout<<countBRs()<<std::endl;
// }


// // -------------------------------------------------------------------------------
// int CEditor::countBRs( void )
// // -------------------------------------------------------------------------------
// {
//    if ( !mpActiveElement )
//       return 0;
//
//    return mpActiveElement->getInformation().count( "<br />" );
// }


// // -------------------------------------------------------------------------------
// void CEditor::setLinebreakMode( eLinebreakMode eMode )
// // -------------------------------------------------------------------------------
// {
//    meLinebreakMode = eMode;
// }



/**
 * Override the regular copy() methode, so that an enter within
 * a richtext element (separating paragraphs) is represented by
 * two \n.
 */
/*void CEditor::copy(){
   if ( textFormat() == Qt::PlainText )
   {
      QTextEdit::copy();
      return;
   }


   QString text = selectedText();//mpActiveElement->getInformation();
   text = text.replace(QRegExp("\n"), "\n\n");

   QClipboard *cb = QApplication::clipboard();
   cb->setText( text );
}
*/



/**
 * Override the regular paste() methode, so that lines are
 * not separated by each other with an blank line.
 */
// // -------------------------------------------------------------------------------
// void CEditor::paste()
// // -------------------------------------------------------------------------------
// {
//    //std::cout<<"CEditor::paste()"<<std::endl;
//
//    //QClipboard *cb = QApplication::clipboard();
//    //for (int i=0; i<20; i++)
//    //    std::cout<<"\tformat="<<cb->data()->format(i)<<std::endl;
//
//    QClipboard::Mode mode = QClipboard::Selection;
//    adaptClipboardText( mode );
//
//    mode = QClipboard::Clipboard;
//    adaptClipboardText( mode );
//
//    QTextEdit::paste();
//    return;
// /*
//    QClipboard::Mode mode = cb->selectionModeEnabled() ? QClipboard::Selection : QClipboard::Clipboard;
//
//    if ( (textFormat() == Qt::RichText)
//          && (! cb->data(mode)->provides("application/x-qrichtext")) )
//    {
//       // Copy text from the clipboard (paste)
//       //QClipboard *cb = QApplication::clipboard();
//       QString text = cb->text();
//
//       if (text)
//       {
//          // do not create new paragraphs
//          text = text.replace(QRegExp("\n"), QChar( 0x2028));
//          insert( text, false, false);
//       }
//    }
//    else
//    {
//       std::cout<<"\tPlaintext detected"<<std::endl;
//       QTextEdit::paste();
//    }
// */
// }


/**
 * Auxiliar method that takes the text from the clipboard - using the
 * specified 'mode' -, replaces all '\n' within that text and writes
 * it back to the clipboard.
 */
// // -------------------------------------------------------------------------------
// void CEditor::adaptClipboardText( QClipboard::Mode mode )
// // -------------------------------------------------------------------------------
// {
//    QClipboard* pCb = QApplication::clipboard();
//    if ( NULLPTR == pCb )
//       return;
//
//    //if ( (textFormat() == Qt::RichText)
//    if ( mbUseRichText
//          && (! pCb->mimeData(mode)->hasFormat("application/x-qrichtext")) )
//    {
//       QString text = pCb->text(mode);
//
//       if ( !text.isEmpty() )
//       {
//          // do not create new paragraphs
//          text = text.replace(QRegExp("\n"), QString( QChar( 0x2028) ) );
//          pCb->setText( text, mode );
//       }
//    }
// }


/**
 * This method is used for inserting images into the current edited text.
 */
// -------------------------------------------------------------------------------
void CEditor::insertImage()
// -------------------------------------------------------------------------------
{
   //std::cout<<"CEditor::insertImage()"<<std::endl;

   QString sImageLocation = QFileDialog::getOpenFileName( this, tr("Insert Image"),
                                                          QDir::currentPath(),
                                                          tr("Images (*.png *.xpm *.jpg)") );
   if ( sImageLocation.isEmpty() )
      return;

   //std::cout<<"CEditor::insertImage() - load image '"<<sImageLocation.toStdString()<<"'"<<std::endl;
   QImage image( sImageLocation );
   if ( image.isNull() )
   {
      QMessageBox::critical( this, "Loading an image",
                             "Error - Could not open image '" + sImageLocation + "'.",
                             "Cancel");
      return;
   }

   QTextDocument* pDocument = this->document();
   pDocument->addResource( QTextDocument::ImageResource, QUrl(sImageLocation), image );

   QTextCursor cursor = this->textCursor();
   cursor.insertImage( sImageLocation );
}
