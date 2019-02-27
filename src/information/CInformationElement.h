/***************************************************************************
                          CInformationElement.h  -  description
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
#ifndef CINFORMATIONELEMENT_H
#define CINFORMATIONELEMENT_H

#include "IParent.h"
#include <iostream>
#include <qstring.h>
#include <QPixmap>
#include <QList>
#include "informationformat.h"
#include <qobject.h>
#include <QTreeWidget>
class QTextCursor;

#include <qdatetime.h>

#include <QDomDocument>


#define CInformationElement_NO_ICON  "none"

class CInformationElement : public QObject,
                            public IParent
{
  Q_OBJECT
public:
  CInformationElement( IParent* pParent,
                     QString sDescription="", QString sInformation="",
                     InformationFormat* pFormat = InformationFormat::getByString( "NONE" ),
                     QString sIconFileName = CInformationElement_NO_ICON,
                     bool    bExpires = false,
                     QDate   expiryDate = QDate::currentDate());
  ~CInformationElement( void );

  // ************* IParent *****************************************
  virtual QString  getDescription( void ) const;
  virtual IParent* getParent( void ) const;
  virtual void     removeChild( CInformationElement* pChild );
  virtual void     aboutToRemoveElement( CInformationElement* pIE );
  // ************* IParent - End ***********************************
   void            removeChild( const int iChildIndex );


  void setBatched( bool );
  bool isBatched( void ) const;

  virtual void addChild( CInformationElement* pElement );
  QList<CInformationElement*>* getChildren( void );
  int getIndexOfChild( CInformationElement* pChild ) const;
  int childCount( void ) const;
  void setParent( IParent* pParent = 0 );

  void deleteSelf( void );

  void    setDescription( QString );
  QString getInformation( void ) const;
  void    setInformation( const QString& );
  QString getInformationText( void ) const;
  void    appendInformation( QString text );

  bool    hasIcon( void ) const;
  void    setIcon(QPixmap);
  void    setIconFileName( const QString& );
  QString getIconFileName( void ) const;

  bool    hasDescriptionColor() const;
  void    setDescriptionColor( const QColor& c );
  QColor  getDescriptionColor() const;

  InformationFormat* getInformationFormat( void ) const;
  void    setInformationFormat( InformationFormat* );

  QString toString( void ) const;
  QString getTreeString(int tab=0) const;
  virtual void toXML( QDomDocument& xmlDocument, QDomNode& parent );

  void search( const QString& sPattern, bool recursive, Qt::CaseSensitivity eCaseSensitive, QTreeWidget& list );

  CInformationElement* findChildWithDescription( QString desc );


  bool  expires( void ) const;
  QDate getExpiryDate( void ) const;
  void  setExpiryDate( bool expires, QDate date = QDate::currentDate() );

  void  setInformationYPos( int iPos );
  int   getInformationYPos( void ) const;

  void  enableEncryption( bool bIsEncryptionEnabled, const QString& sTmpPasswd  );
  bool  isEncryptionEnabled( void ) const;
  bool  isCurrentlyEncrypted( void ) const;
  void  encrypt( void );
  bool  decrypt( const QString& sPasswd );
  const QByteArray& getEncryptedData() const;
  void  setEncryptedData( const QByteArray& data );

signals:
  void propertyChanged( void );
  void childAdded( CInformationElement* );
  void childMoved( int oldPos, int newPos );
  void informationHasChanged( void );


protected:
   // Returns a pointer to the parent element. If this informationElement
   // is the root element, then the mpParent ptr contains a pointer to the
   // collection.
  IParent*            mpParent;

  // this should reduce update-behavior;
  // if batched==true -> the signal 'propertyChanged()' is not emitted
  bool                mbBatched;

  QString             mDescription;
  InformationFormat*  mpInformationFormat;
  QString             mInformation;
  QPixmap             mIcon;
  QString             mIconFilename;
  QColor              mDescriptionColor;           // the color which is used for drawing the description

  QList<CInformationElement*>* mpChildObjects;

  QDate               mExpiryDate;
  bool                mbExpires;

  int                 miInformationYPos;


  bool                mbIsEncryptionEnabled;
  QString             msTmpPasswd;
  QByteArray          mEncryptedData;


private:
  void searchDescription( QString pattern, Qt::CaseSensitivity eCaseSensitive, QTreeWidget& list );
  void searchInformation( const QString& sPattern, Qt::CaseSensitivity eCaseSensitive, QTreeWidget& list );
  QString             extractSearchLine( const QTextCursor& c ) const;
};
#endif
