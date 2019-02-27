/***************************************************************************
                          CCollectionModel.h  -  description
                             -------------------
    begin                : Son May 14 12:23:15 CEST 2006
    copyright            : (C) 2006 by Alexander Theel
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

#ifndef CCOLLECTIONMODEL_H
#define CCOLLECTIONMODEL_H

#include <QStandardItemModel>
#include "../information/CInformationCollection.h"
class CTreeInformationElement;


/**
 * This class represents a model for a <code>CInformationCollection</code>.
 * It is derived from the <code>QStandardItemModel</code> and has a reference
 * to a <code>CInformationCollection</code>.
 *
 * However, the main purpose is: The method <code>index()</code> returns
 * <code>CCollectionIndex</code> which hold a reference to a
 * <code>CInformationElement</code>. Thus the model view controller architecture
 * is fully implemented.
 */
class CCollectionModel : public QStandardItemModel
{
    Q_OBJECT
public:
   CCollectionModel();
   ~CCollectionModel();

   void                     setInformationCollection( CInformationCollection& collection );
   void                     clearCollection();
   bool                     isValid() const;
   
   // Methods for standard one-dimensional models.
   virtual int              rowCount( const QModelIndex& parent = QModelIndex() ) const;
   virtual QVariant         data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
   virtual QVariant         headerData( int section, Qt::Orientation orientation,
                                        int role = Qt::DisplayRole ) const;
   
   //Methods for two-dimensional models.
   virtual int              columnCount( const QModelIndex& parent = QModelIndex() ) const;
   
   
   //Methods for hierarchical models.
   virtual bool             hasChildren( const QModelIndex& parent = QModelIndex() ) const;
   virtual QModelIndex      index( int row, int column,
                                   const QModelIndex& parent = QModelIndex() ) const;
   virtual QModelIndex      parent( const QModelIndex& index ) const;
   
   
   //Methods for editable models (data are allowed to be changed).
   virtual Qt::ItemFlags    flags( const QModelIndex& index ) const;
   virtual bool             setData( const QModelIndex& index, const QVariant& value, int iRole = Qt::EditRole );

   // Methods for editable models (data are allowed to be expanded and deleted)
   virtual bool             removeRow( int iRow, const QModelIndex& parent = QModelIndex() );
//   virtual bool             insertRow( int iRow, const QModelIndex& parent = QModelIndex() );
   bool                     insertRow( const QString&     sDescription,
                                       InformationFormat* pFormat,
                                       const QString&     sIconFileName,
                                       const QColor&      descriptionColor,
                                       bool               bExpires,
                                       const QDate&       date,
                                       
                                       int iRow, const QModelIndex& parent = QModelIndex()
                                     );


   void                     moveEntryOneUp( const QModelIndex& index );
   void                     moveEntryOneDown( const QModelIndex& index );

   void                     currentIndexChanged( const QModelIndex& index );

   CTreeInformationElement* mapIndexToIE( const QModelIndex& index ) const;
   QModelIndex              mapPathToIndex( const Path& path ) const;


   // Methods for drag & drop models
   virtual QStringList      mimeTypes() const;
   virtual QMimeData*       mimeData( const QModelIndexList & indexes ) const;
   virtual bool             dropMimeData( const QMimeData* pData, Qt::DropAction action,
                                          int iRow, int iColumn, const QModelIndex& parent );
   
private:
   CInformationCollection*  mpCollection;
//   void                     setupModelData();
//   void                     addChild( CInformationElement& parentElement, QModelIndex& parentIndex );


   QString                  getIEDescription( const QModelIndex& index ) const;
   QModelIndex              findChildWithDescription( QModelIndex parentIndex, QString sData ) const;

   QString                  getFileName() const;

   CTreeInformationElement* createNewChild( const QString&     sDescription,
                                            InformationFormat* pFormat,
                                            const QString&     sIconFileName,
                                            const QColor&      descriptionColor,
                                            bool               bExpires,
                                            const QDate&       date,
            
                                            CInformationElement* pParentIE
                                          );
};

#endif
