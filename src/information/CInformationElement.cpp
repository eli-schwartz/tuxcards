/***************************************************************************
                          CInformationElement.cpp  -  description
                             -------------------
    begin                : Fri Jul 19 2002
    copyright            : (C) 2002 by Alexander Theel
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

#include "CInformationElement.h"
#include "../global.h"
#include "../utilities/strings.h"
#include "../gui/dialogs/searchDialog/CSearchListItem.h"

#include <qregexp.h>
#include <QPixmap>
#include "../utilities/crypt/StringCrypter.h"

#include <QTextDocument>
#include <QTextCursor>


// -------------------------------------------------------------------------------
CInformationElement::CInformationElement( IParent* pParent,
                                        QString sDescription,
                                        QString sInformation,
                                        InformationFormat* pFormat,
                                        QString sIconFileName,
                                        bool    bExpires,
                                        QDate   expiryDate )
 : mpParent( pParent )
 , mbBatched( false )
 , mDescription( sDescription )
 , mpInformationFormat( pFormat )
 , mInformation( sInformation )
 , mIcon()
 , mIconFilename( sIconFileName )
 , mDescriptionColor( 0,0,0 )
 , mpChildObjects( NULLPTR )
 , mExpiryDate( expiryDate )
 , mbExpires( bExpires )
 , miInformationYPos( 0 )
 , mbIsEncryptionEnabled( false )
 , msTmpPasswd( "" )
 , mEncryptedData()
// -------------------------------------------------------------------------------
{
   if ( !mpInformationFormat )
   {
      mpInformationFormat = InformationFormat::getByString( "RTF" );
   }

   mpChildObjects = new QList<CInformationElement*>();   // no auto-deletion is done if a pointer is removed from the list
   if ( NULLPTR == mpChildObjects )
   {
      std::cout<<"TuxCards-ERROR: in constructor 'CInformationElement'\n"
               <<"NULLPTR == mpChildObjects\n"
               <<"Not enough memory to create objects. TuxCards might crash."
               <<std::endl;
      return;
   }

   if (!mbBatched) emit propertyChanged();
}

// -------------------------------------------------------------------------------
CInformationElement::~CInformationElement( void )
// -------------------------------------------------------------------------------
{
   //std::cout<<"\t~CIE: "<<getDescription()<<std::endl;

   if ( NULLPTR != mpChildObjects )
   {
      while ( !mpChildObjects->isEmpty() )
      {
         delete mpChildObjects->takeFirst();
      }
/*      while ( NULL < mpChildObjects->count() )
      {
         CInformationElement* pIE = mpChildObjects->first();
         removeChild( pIE );
         pIE = NULLPTR;
         std::cout<<"\t\tpIE="<<pIE<<std::endl;
      }
*/
   }
   DELETE( mpChildObjects );
   mpInformationFormat = NULLPTR;

   if ( NULLPTR != mpParent )
   {
      mpParent->aboutToRemoveElement(this);
      mpParent = NULLPTR;
   }
}




// **************************** IParent ******************************************
// -------------------------------------------------------------------------------
QString CInformationElement::getDescription( void ) const
// -------------------------------------------------------------------------------
{
  return mDescription;
}
// -------------------------------------------------------------------------------
IParent* CInformationElement::getParent( void ) const
// -------------------------------------------------------------------------------
{
   return mpParent;
}

// -------------------------------------------------------------------------------
void CInformationElement::removeChild( CInformationElement* pChild )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == mpChildObjects )
      return;

   int iIndex = mpChildObjects->indexOf( pChild );
   if ( -1 != iIndex )
   {
      mpChildObjects->removeAt( iIndex );
      DELETE( pChild );
   }
}

/**
 * Convenience method for <code>removeChild( CInformationElement* pChild )</code>.
 * <code>iChildIndex</code> is the index of the child.
 */
// -------------------------------------------------------------------------------
void CInformationElement::removeChild( const int iChildIndex )
// -------------------------------------------------------------------------------
{
   if ( !mpChildObjects )
      return;

   removeChild( mpChildObjects->at( iChildIndex ) );
}

// -------------------------------------------------------------------------------
void CInformationElement::aboutToRemoveElement( CInformationElement* pIE )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR != mpParent )
      mpParent->aboutToRemoveElement(pIE);
}
// **************************** IParent - End ************************************



// -------------------------------------------------------------------------------
void CInformationElement::deleteSelf( void )
// -------------------------------------------------------------------------------
{
  //cout<<"IE::deleteSelf(); parent="<<mpParent<<endl;
   if ( NULLPTR == mpParent )
      return;

   mpParent->removeChild(this);
}

// -------------------------------------------------------------------------------
void CInformationElement::setParent( IParent* pParent )
// -------------------------------------------------------------------------------
{
  mpParent = pParent;
}


// -------------------------------------------------------------------------------
void CInformationElement::setBatched( bool b )
// -------------------------------------------------------------------------------
{
   mbBatched = b;
   if (!mbBatched) emit propertyChanged();
}
// -------------------------------------------------------------------------------
bool CInformationElement::isBatched( void ) const
// -------------------------------------------------------------------------------
{
  return mbBatched;
}

// -------------------------------------------------------------------------------
void CInformationElement::addChild( CInformationElement* pElement )
// -------------------------------------------------------------------------------
{
   if ( (NULLPTR == pElement) || (NULLPTR == mpChildObjects) )
      return;

   mpChildObjects->append( pElement );
   if (!mbBatched) emit childAdded( pElement );
}
// -------------------------------------------------------------------------------
QList<CInformationElement*>* CInformationElement::getChildren( void )
// -------------------------------------------------------------------------------
{
  return mpChildObjects;
}
// -------------------------------------------------------------------------------
int CInformationElement::childCount( void ) const
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == mpChildObjects )
      return 0;

   return mpChildObjects->count();
}

/**
 * Returns the index of the given child within the list of all children of this
 * element.
 * This is important for the MVC-architecture from Qt.
 *
 * If the given element is no child of this element, then <code>-1</code> is
 * returned.
 */
// -------------------------------------------------------------------------------
int CInformationElement::getIndexOfChild( CInformationElement* pChild ) const
// -------------------------------------------------------------------------------
{
    if ( NULLPTR == mpChildObjects )
      return -1;

    return mpChildObjects->indexOf( pChild );
}


// -------------------------------------------------------------------------------
void CInformationElement::setDescription( QString description )
// -------------------------------------------------------------------------------
{
  mDescription = description;
  if (!mbBatched) emit propertyChanged();

//  std::cout<<"CInformationElement: data changed to '"<<description.toStdString()<<"'"<<std::endl;
}

// -------------------------------------------------------------------------------
QString CInformationElement::getInformation( void ) const
// -------------------------------------------------------------------------------
{
  return mInformation;
}

// -------------------------------------------------------------------------------
void CInformationElement::setInformation( const QString& information )
// -------------------------------------------------------------------------------
{
  mInformation = information;
  if (!mbBatched) emit propertyChanged();
}


// -------------------------------------------------------------------------------
bool CInformationElement::hasIcon( void ) const
// -------------------------------------------------------------------------------
{
  return !mIcon.isNull();
}

// -------------------------------------------------------------------------------
void CInformationElement::setIcon( QPixmap icon )
// -------------------------------------------------------------------------------
{
  mIcon = icon;
  if (!mbBatched) emit propertyChanged();
}
// -------------------------------------------------------------------------------
void CInformationElement::setIconFileName( const QString& file )
// -------------------------------------------------------------------------------
{
  mIconFilename = file;
  if (!mbBatched) emit propertyChanged();
}
// -------------------------------------------------------------------------------
QString CInformationElement::getIconFileName( void ) const
// -------------------------------------------------------------------------------
{
  return ( !mIconFilename.isEmpty() ? mIconFilename : QString(CInformationElement_NO_ICON));
}

/**
 * Return <code>true</code> if the member <code>mDescriptionColor</code> has a
 * color that is not equal to black (i.e. <code>QColor(0,0,0)</code>).
 */
// -------------------------------------------------------------------------------
bool CInformationElement::hasDescriptionColor() const
// -------------------------------------------------------------------------------
{
   return ( QColor(0,0,0) != mDescriptionColor );
}

// -------------------------------------------------------------------------------
void CInformationElement::setDescriptionColor( const QColor& c )
// -------------------------------------------------------------------------------
{ mDescriptionColor = c; }

// -------------------------------------------------------------------------------
QColor CInformationElement::getDescriptionColor() const
// -------------------------------------------------------------------------------
{ return mDescriptionColor; }

// -------------------------------------------------------------------------------
InformationFormat* CInformationElement::getInformationFormat( void ) const
// -------------------------------------------------------------------------------
{
  return mpInformationFormat;
}

// -------------------------------------------------------------------------------
void CInformationElement::setInformationFormat( InformationFormat* pFormat )
// -------------------------------------------------------------------------------
{
  mpInformationFormat = pFormat;
  if (!mbBatched) emit propertyChanged();
}

// -------------------------------------------------------------------------------
QString CInformationElement::toString( void ) const
// -------------------------------------------------------------------------------
{
  QString result = mDescription+" ["+mpInformationFormat->toString()+"]\n"
                   +mInformation;
  return result;
}

// -------------------------------------------------------------------------------
QString CInformationElement::getTreeString( int tab ) const
// -------------------------------------------------------------------------------
{
  QString result = Strings::spaces(tab)+mDescription+"\n";
  tab++;

  if ( NULLPTR == mpChildObjects )
    return result;


  CInformationElement* pX;
  for ( int i = 0; i < mpChildObjects->count(); i++ )
  {
     pX = mpChildObjects->at( i );
     if ( pX )
        result += pX->getTreeString( tab );
  }

  return result;
}

// -------------------------------------------------------------------------------
void CInformationElement::toXML( QDomDocument& xmlDocument, QDomNode& parent )
// -------------------------------------------------------------------------------
{
  QDomElement thisElement = xmlDocument.createElement("InformationElement");
  thisElement.setAttribute("informationFormat", getInformationFormat()->toString());
  thisElement.setAttribute("iconFileName", getIconFileName());
  thisElement.setAttribute("expires", expires() ? QString("true") : QString("false") );
  thisElement.setAttribute("expiryDate", getExpiryDate().toString());

  // add description
  QDomElement description = xmlDocument.createElement("Description");
  QDomText text = xmlDocument.createTextNode(getDescription());
  description.appendChild(text);
  if ( hasDescriptionColor() )
     description.setAttribute( "color", getDescriptionColor().name() );
  thisElement.appendChild(description);

  // add information
  QDomElement information = xmlDocument.createElement("Information");
  text = xmlDocument.createTextNode(getInformation());
  information.appendChild(text);
  thisElement.appendChild(information);


  // add children
  CInformationElement* pX;
  for ( int i = 0; i < mpChildObjects->count(); i++ )
  {
     pX = mpChildObjects->at( i );
     if ( pX )
        pX->toXML(xmlDocument, thisElement);
  }


  parent.appendChild(thisElement);
}


// -------------------------------------------------------------------------------
CInformationElement* CInformationElement::findChildWithDescription( QString desc )
// -------------------------------------------------------------------------------
{
  CInformationElement* pX;
  for ( int i = 0; i < mpChildObjects->count(); i++ )
  {
     pX = mpChildObjects->at( i );
     if ( pX && (pX->getDescription() == desc) )
        return pX;
  }

  return NULLPTR;
}


/**
 * find the specified 'QString pattern' within the text/information of the
 * appropriate 'CInformationElement' (evtl. recursive) and append the found
 * "places" as 'SearchListItem's at the list's end.
 */
// -------------------------------------------------------------------------------
void CInformationElement::search( const QString& sPattern, bool recursive,
                                  Qt::CaseSensitivity eCaseSensitive,
                                  QTreeWidget& list)
// -------------------------------------------------------------------------------
{
  searchDescription( sPattern, eCaseSensitive, list );
  searchInformation( sPattern, eCaseSensitive, list );


  // if recursive -> do so
  if (recursive)
  {
     CInformationElement* pX;
     for ( int i = 0; i < mpChildObjects->count(); i++ )
     {
        pX = mpChildObjects->at( i );
        if ( pX )
           pX->search( sPattern, true, eCaseSensitive, list );
     }
  }
}


// -------------------------------------------------------------------------------
void CInformationElement::searchDescription( QString pattern,
                                             Qt::CaseSensitivity eCaseSensitive,
                                             QTreeWidget& list)
// -------------------------------------------------------------------------------
{
   int pos = mDescription.indexOf( pattern, 0, eCaseSensitive );
   while (pos >= 0)
   {
      // found something -> add it to 'list'
      (void) new CSearchListItem( &list, new Path(this), CSearchPosition::SP_NAME,
                                  pos, pattern.length(), mDescription );

      // prepare for next evtl. occurance within this 'oneLine'
      pos = mDescription.indexOf( pattern, pos+pattern.length(), eCaseSensitive );
   }
}

// -------------------------------------------------------------------------------
void CInformationElement::searchInformation( const QString& sPattern,
                                             Qt::CaseSensitivity eCaseSensitive,
                                             QTreeWidget& list )
// -------------------------------------------------------------------------------
{
   QTextDocument textDocument;
   textDocument.setHtml( getInformation() );
   bool bFoundSomething = false;

   QTextCursor cursor( &textDocument );
   while ( !cursor.isNull() && !cursor.atEnd() )
   {
      if ( Qt::CaseSensitive == eCaseSensitive )
         cursor = textDocument.find( sPattern, cursor, QTextDocument::FindCaseSensitively );
      else
         cursor = textDocument.find( sPattern, cursor );

      if ( !cursor.isNull() )
      {
         bFoundSomething = true;
         //std::cout<<"searching("<<sPattern.toStdString()<<") ... "<<getDescription().toStdString()<<std::endl;
         //std::cout<<"\tfound - anchor="<<cursor.anchor()<<" position="<<cursor.position()<<std::endl;

         int iPosition = cursor.anchor();
         QString sLine = extractSearchLine( cursor );
         // found something -> add it to 'list'
         (void) new CSearchListItem( &list, new Path(this), CSearchPosition::SP_INFORMATION,
                                     iPosition, sPattern.length(), sLine );
      }
   }
}

/**
 * Auxiliary method for searching text and displaying the found locations.
 * This method returns the text line where the given cursor is located in.
 */
// -------------------------------------------------------------------------------
QString CInformationElement::extractSearchLine( const QTextCursor& c ) const
// -------------------------------------------------------------------------------
{
   static int iWORDS_BEFORE = 3;
   static int iWORDS_AFTER  = 4;

   QTextCursor cursor( c );         // Copy the cursor; Never change the one
                                    // from the calling method.

   // In qt4.3.3 the <code>QTextCursor::MoveOperation</code>s "StartOfLine" and "EndOfLine"
   // do not work. Thus "PreviousWord" and "NextWord" are used.
   //    cursor.movePosition( QTextCursor::StartOfLine, QTextCursor::MoveAnchor, 1 );

   // Also this very easy method has a bug in qt4.3.3 and does not return anything:
   //    cursor.select( QTextCursor::LineUnderCursor );
   //    QString sLine = cursor.selectedText();


   // remember original values
   int iOrgAnchor   = cursor.anchor();
   int iOrgPosition = cursor.position();
   //std::cout<<"\t[anchor="<<cursor.anchor()<<",pos="<<cursor.position()<<"]"<<std::endl;

   // get some context for the found string (i.e. some words before and after the found string) 
   cursor.movePosition( QTextCursor::PreviousWord, QTextCursor::MoveAnchor, iWORDS_BEFORE );
   cursor.movePosition( QTextCursor::NextWord, QTextCursor::KeepAnchor, iWORDS_BEFORE + iWORDS_AFTER );
   //std::cout<<"\t[anchor="<<cursor.anchor()<<",pos="<<cursor.position()<<"]"<<std::endl;
   QString sLine = cursor.selectedText();

   // trim the context; mostly a new line starts/ends with '0'
   int iStartWithinLine = iOrgAnchor - cursor.anchor();
   int iEndWithinLine   = iOrgPosition - cursor.anchor();

   //std::cout<<"\t(start="<<iStartWithinLine<<",end="<<iEndWithinLine<<", lineLength="<<sLine.length()<<")"<<std::endl;
   //std::cout<<"\t"<<sLine.toStdString()<<std::endl;

   int iTrimmedLineStart = 0;
   int i;
   for ( i = iStartWithinLine; i >= 0; i-- )
   {
      QChar cChar = sLine.at(i);
      if ( 0 == cChar.toAscii() )
      {
         iTrimmedLineStart = i+1;
         break;
      }
   }

   int iTrimmedLineEnd = sLine.length();
   for ( i = iEndWithinLine; i < sLine.length(); i++ )
   {
      QChar cChar = sLine.at(i);
      if ( 0 == cChar.toAscii() )
      {
         iTrimmedLineEnd = i;
         break;
      }
   }

   //std::cout<<"\t-> trimmedLineStart="<<iTrimmedLineStart<<", trimmedLineEnd="<<iTrimmedLineEnd<<std::endl;
   QString sTrimmedLine = sLine.mid( iTrimmedLineStart, iTrimmedLineEnd - iTrimmedLineStart );
   //std::cout<<"\t-> trimmedLine='"<<sTrimmedLine.toStdString()<<"'"<<std::endl;
   return sTrimmedLine;
}


// -------------------------------------------------------------------------------
QString CInformationElement::getInformationText( void ) const
// -------------------------------------------------------------------------------
{
  QString text;

  if ( getInformationFormat() == InformationFormat::getByString("ASCII") )
  {
    text = getInformation();
  }
  else
  {
    text = Strings::removeHTMLTags(getInformation());
  }

  return text;
}


/**
 * Appends the specified text to the end of the information
 * of this element.
 */
// -------------------------------------------------------------------------------
void CInformationElement::appendInformation( QString text )
// -------------------------------------------------------------------------------
{
  if ( mpInformationFormat == InformationFormat::getByString("RTF") )
  {
    text.replace( QRegExp("\n"), "<br>\n" );
  }

  mInformation += text;
  emit informationHasChanged();
}


// -------------------------------------------------------------------------------
bool CInformationElement::expires( void ) const
// -------------------------------------------------------------------------------
{ return mbExpires; }

// -------------------------------------------------------------------------------
QDate CInformationElement::getExpiryDate( void ) const
// -------------------------------------------------------------------------------
{ return mExpiryDate; }


/**
 * This method set the expire date to 'date'. If 'expires' == false, this
 * information element will not expire at all (independently of a given date).
 */
// -------------------------------------------------------------------------------
void CInformationElement::setExpiryDate( bool expires, QDate date )
// -------------------------------------------------------------------------------
{
   mbExpires    = expires;
   mExpiryDate = date;
}


// -------------------------------------------------------------------------------
void CInformationElement::setInformationYPos( int iPos )
// -------------------------------------------------------------------------------
{
   miInformationYPos = iPos;
}

// -------------------------------------------------------------------------------
int CInformationElement::getInformationYPos( void ) const
// -------------------------------------------------------------------------------
{
   return miInformationYPos;
}


// -------------------------------------------------------------------------------
// Adds information so that the element has the capability to be encrypted.
// To encrypt the element call 'encrypt()'.
// -------------------------------------------------------------------------------
void CInformationElement::enableEncryption( bool bIsEncryptionEnabled,
                                            const QString& sTmpPasswd )
// -------------------------------------------------------------------------------
{
   mbIsEncryptionEnabled = bIsEncryptionEnabled;
   msTmpPasswd = mbIsEncryptionEnabled ? sTmpPasswd : QString("");
   //std::cout<<"CInformationElement - passwd='"<<msTmpPasswd.toStdString()<<"'"<<std::endl;
}

// -------------------------------------------------------------------------------
// States whether this element has the capability to be encrypted.
// To encrypt the element call 'encrypt()'.
// -------------------------------------------------------------------------------
bool CInformationElement::isEncryptionEnabled( void ) const
// -------------------------------------------------------------------------------
{
   return mbIsEncryptionEnabled;
}

// -------------------------------------------------------------------------------
// States whether this element is currently encrypted.
// -------------------------------------------------------------------------------
bool CInformationElement::isCurrentlyEncrypted( void ) const
// -------------------------------------------------------------------------------
{
   return ( 0 != mEncryptedData.size() );
}


// -------------------------------------------------------------------------------
// This encrypts the element and makes its contents unreadable.
// -------------------------------------------------------------------------------
void CInformationElement::encrypt( void )
// -------------------------------------------------------------------------------
{
   if ( !mbIsEncryptionEnabled )
      return;

   StringCrypter::encryptString( mInformation, msTmpPasswd, mEncryptedData );
   mInformation = "";
}

// -------------------------------------------------------------------------------
// This decrypts the element and makes its contents readable.
// Returns 'true' if everything is correct and element could be decrypted.
// -------------------------------------------------------------------------------
bool CInformationElement::decrypt( const QString& sPasswd )
// -------------------------------------------------------------------------------
{
   int iError = StringCrypter::decryptString( mEncryptedData, sPasswd, mInformation );
   if ( StringCrypter::NO_ERROR == iError )
   {
      msTmpPasswd = sPasswd;
      mEncryptedData.resize(0);
   }
   else if (  StringCrypter::ERROR_INVALID_FILEHEADER == iError )
   {
      // this should never happen (if the user does not touch the data)
      std::cout<<"StringCrypter::ERROR_INVALID_FILEHEADER"<<std::endl;
   }

   return (StringCrypter::NO_ERROR == iError);
}


// -------------------------------------------------------------------------------
const QByteArray& CInformationElement::getEncryptedData() const
// -------------------------------------------------------------------------------
{
   return mEncryptedData;
}

// -------------------------------------------------------------------------------
void CInformationElement::setEncryptedData( const QByteArray& data )
// -------------------------------------------------------------------------------
{
   mEncryptedData = data;
   mbIsEncryptionEnabled = true;
}
