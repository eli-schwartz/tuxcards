/***************************************************************************
                          CInformationCollection.h  -  description
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
#ifndef CINFORMATION_COLLECTION_H
#define CINFORMATION_COLLECTION_H

#include "../global.h"
#include "IParent.h"

#include <iostream>
#include "CInformationElement.h"
#include <QList>
#include <qobject.h>
#include "Path.h"
#include "IView.h"

class CInformationCollection : public QObject,
                               public IParent
{
   Q_OBJECT
public:
   CInformationCollection( CInformationElement* pRoot = NULLPTR );
   ~CInformationCollection( void);
   static CInformationCollection* createDefaultCollection( void );


   void             setAbsFileName( const QString& sAbsFileName );
   const QString&   getAbsFileName() const;
   
   // ************* IParent - End ***********************************
   virtual QString  getDescription( void ) const;
   virtual IParent* getParent( void ) const;
   virtual void     removeChild( CInformationElement* pChild );
   virtual void     aboutToRemoveElement( CInformationElement* pIE );
   // ************* IParent - End ***********************************
   
   void setRootElement( CInformationElement* pRoot );
   CInformationElement* getRootElement( void ) const;
   
   void setActiveElement( CInformationElement* pElement );
   void setActiveElement( Path path );
   CInformationElement* getActiveElement( void );
   CInformationElement* getInformationElement( Path path );
   bool isPathValid( Path path );
   
   void registerAsListenerOf( CInformationElement* pElem );
   
   QString toString( void );
   QString toXML( void );
   QString toXML( CInformationElement* );
   
   
   void addView( IView* pView );
   void removeView( IView* pView );

signals:
   void activeInformationElementChanged( CInformationElement* );
   void modelHasChanged();

public slots:
   void slotChildAdded( CInformationElement* );


private:
   CInformationElement* mpRootElement;
   CInformationElement* mpActiveElement;
   
   QList<IView*> mViews;
   void notifyViewsToRemoveElement( CInformationElement* pIE );

   QString                  msAbsFileName;                         // This member holds the fileName during runtime of
                                                                   // the application. The fileName is not saved within
                                                                   // the xml-DataFile.
};
#endif
