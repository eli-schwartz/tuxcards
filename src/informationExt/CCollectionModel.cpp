/***************************************************************************
                          CCollectionModel.cpp  -  description
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

#include "../global.h"
#include "CCollectionModel.h"
#include "../information/CTreeInformationElement.h"

#include <QMimeData>                         // for drag&drop
#include "../information/xmlpersister.h"     // for drag&drop

#include <QtGlobal>        // for 'Q_ASSERT()'
#include <QFileInfo>       // for extracting the fileName
#include <iostream>

// -------------------------------------------------------------------------------
CCollectionModel::CCollectionModel()
 : QStandardItemModel()
 , mpCollection( NULLPTR )
// -------------------------------------------------------------------------------
{
}

// -------------------------------------------------------------------------------
CCollectionModel::~CCollectionModel()
// -------------------------------------------------------------------------------
{
   mpCollection = NULLPTR;
}

// -------------------------------------------------------------------------------
void CCollectionModel::setInformationCollection( CInformationCollection& collection )
// -------------------------------------------------------------------------------
{
   beginResetModel();
   mpCollection = &collection;
   endResetModel();
}

// -------------------------------------------------------------------------------
void CCollectionModel::clearCollection()
// -------------------------------------------------------------------------------
{
   if ( !mpCollection )
      return;

   beginResetModel();
   mpCollection = NULLPTR;
   endResetModel();
}

/**
 * Returns <code>true</code> if this model is valid. This is the case if it owns
 * an underlying collection (i.e. if <code>mpCollection</code> is valid and not
 * <code>NULLPTR</code>).
 *
 * Returns <code>false</code> if <code>mpCollection</code> was not set (yet).
 * (This means that no real model exists.)
 */
// -------------------------------------------------------------------------------
bool CCollectionModel::isValid() const
// -------------------------------------------------------------------------------
{
   if ( mpCollection )
      return TRUE;

   return FALSE;
}

// -------------------------------------------------------------------------------
void CCollectionModel::currentIndexChanged( const QModelIndex& index )
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::currentIndexChanged()   ->    "
   //         <<" row="<< index.row()<<" column="<<index.column()
   //         <<" Addr="<<index.internalPointer()<<std::endl;

   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return;


   if ( index.isValid() )
      mpCollection->setActiveElement( mapIndexToIE( index ) );
}


/** *************** Model interface implementation ******************************/
// -------------------------------------------------------------------------------
int CCollectionModel::rowCount( const QModelIndex& parent /*= QModelIndex()*/ ) const
// -------------------------------------------------------------------------------
{
   if ( !mpCollection )
      return 0;


   if ( parent.isValid() )
   {
      CInformationElement* pIE = static_cast<CInformationElement*>(parent.internalPointer());
      Q_ASSERT( NULLPTR != pIE );
      if ( pIE )
      {
         //std::cout<<"CCollectionModel::rowCount() = "<<pIE->childCount()<<std::endl;
         return pIE->childCount();
      }
   }

   //std::cout<<"CCollectionModel::rowCount() = 1"<<std::endl;
   // If no valid parent was given, then only the root-element is there.
   return 1;
}

// -------------------------------------------------------------------------------
int CCollectionModel::columnCount( const QModelIndex& /*parent = QModelIndex()*/ ) const
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::columnCount() = 1 // parent is valid = "<<parent.isValid()<<std::endl;

   if ( !mpCollection )
      return 0;

   return 1;
}

// -------------------------------------------------------------------------------
QVariant CCollectionModel::data( const QModelIndex& index,
                                 int iRole /*= Qt::DisplayRole*/ ) const
// -------------------------------------------------------------------------------
{
   QVariant retVal;

   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return retVal;                  // returns an invalid variant

   switch ( iRole )
   {
   case Qt::DisplayRole:              // fall through
   case Qt::EditRole:
      // Returns the description for the information element corresponding to the
      // given model index.
      retVal = QVariant( getIEDescription(index) );
      break;

   case Qt::DecorationRole:
      {
         // Returns an icon if there is one for the information element.
         CInformationElement* pIE = mapIndexToIE(index);

         Q_ASSERT( NULLPTR != pIE );
         if ( pIE && (CInformationElement_NO_ICON != pIE->getIconFileName()) )
            retVal = QIcon( pIE->getIconFileName() );

         break;
      }
   };

   return retVal;
}

// -------------------------------------------------------------------------------
QVariant CCollectionModel::headerData( int iSection, Qt::Orientation orientation,
                                       int role /*= Qt::DisplayRole*/ ) const
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::headerData( section = "<<section<<" )"<<std::endl;

   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return QVariant("");


   if ( (Qt::Horizontal == orientation) && (Qt::DisplayRole == role) && (0 == iSection) )
      return QVariant( getFileName() );
   else
      return QVariant("");
}


// -------------------------------------------------------------------------------
bool CCollectionModel::hasChildren ( const QModelIndex& parent /*= QModelIndex()*/ ) const
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::hasChildren( parent is valid = "<<parent.isValid()<<" )";

   bool bRetVal = FALSE;

   if ( !mpCollection )
      return bRetVal;

   if ( !parent.isValid() )
   {
      // This should never happen, because each collection has a root element.
      Q_ASSERT( NULLPTR != mpCollection->getRootElement() );

      if ( mpCollection->getRootElement() )
         bRetVal = TRUE;
   }
   else
   {
      // 'parent' is valid -> Check whether it has children.
      CInformationElement* pIE = static_cast<CInformationElement*>(parent.internalPointer());
      Q_ASSERT( NULLPTR != pIE );
      if ( pIE )
         bRetVal = ( 0 < pIE->childCount() );
   }

   //std::cout<<"    -> bRetVal = "<<bRetVal<<std::endl;
   return bRetVal;
}

// -------------------------------------------------------------------------------
QModelIndex CCollectionModel::index( int row, int column,
                                     const QModelIndex& parent /*= QModelIndex()*/ ) const
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::index() - requesting index( row = "<<row
   //         <<", column = "<<column
   //         <<", parent is valid = "<<parent.isValid()<<" )"<<std::endl;

   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return QModelIndex();


   Q_ASSERT( 0 == column );

   if ( !parent.isValid() )
   {
      // The parent index is not valid. -> The root element is requested.
      //Q_ASSERT( 0 == row );  // Don't do this assert (with qt4.4.3 and mac 'row' will
                               // be '-1' for new files; which leads to an assertion).      
      return createIndex( 0,0, mpCollection->getRootElement() );
   }


   // The parent index is valid; therefore a parent IE exists.
   CInformationElement* pParentIE = static_cast<CInformationElement*>(parent.internalPointer());

   Q_ASSERT( NULLPTR != pParentIE );
   if ( !pParentIE )
   {
      // This should never happen. Since a valid model index is given, a valid
      // information element must exist!!

      // Leave the method.
      return QModelIndex();
   }

   CInformationElement* pChildIE = pParentIE->getChildren()->value( row );
   if ( pChildIE )
      return createIndex( row, 0, pChildIE );
   else
      return QModelIndex();        // no valid child available
}

// -------------------------------------------------------------------------------
QModelIndex CCollectionModel::parent( const QModelIndex& index ) const
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::parent( index is valid = "<<index.isValid()<<" )   //"
   //         <<" parent from '"<<getIEDescription(index).toStdString()<<"'"<<std::endl;

   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return QModelIndex();

   if ( !index.isValid() )
      return QModelIndex();

   // A valid model index is given; thus a valid information element must be available.
   CInformationElement* pIE = static_cast<CInformationElement*>(index.internalPointer());

   Q_ASSERT( NULLPTR != pIE );
   if ( !pIE )
   {
      // This should never happen. Since a valid model index is given, then a valid
      // information element must exist!!

      // Leave the method.
      return QModelIndex();
   }


   if ( pIE == mpCollection->getRootElement() )
   {
      // The root information element does not have a parent.
      return QModelIndex();
   }


   CInformationElement* pParentIE = dynamic_cast<CInformationElement*>(pIE->getParent());

   Q_ASSERT( NULLPTR != pParentIE );
   if ( !pParentIE )
   {
      // This should never happen.
      return QModelIndex();
   }


   // Calculate the row of the 'pParentIE'.
   int iRow = 0;
   if ( pParentIE == mpCollection->getRootElement() )
   {
      // Do nothing; row is 0.
   }
   else
   {
      // Get the parent of 'pParentIE'.
      CInformationElement* pParentOfParentIE = dynamic_cast<CInformationElement*>(pParentIE->getParent());
      Q_ASSERT( NULLPTR != pParentOfParentIE );
      if ( pParentOfParentIE )
      {
         iRow = pParentOfParentIE->getIndexOfChild( pParentIE );
         Q_ASSERT( -1 != iRow );
      }
   }

   return createIndex( iRow, 0, pParentIE );
}

// -------------------------------------------------------------------------------
Qt::ItemFlags CCollectionModel::flags( const QModelIndex& index ) const
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::flags() - requesting flags( index is valid = "<<index.isValid()<<" )"<<std::endl;

   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return Qt::NoItemFlags;

   if (!index.isValid())
      return Qt::ItemIsEnabled;

   // readable, writable and drag&drop model
   return Qt::ItemIsEnabled | Qt::ItemIsSelectable |                // for readable models
          Qt::ItemIsEditable |                                      // for writeable models
          Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;            // for drag & drop models
}

// -------------------------------------------------------------------------------
bool CCollectionModel::setData( const QModelIndex& index, const QVariant& value,
                                int iRole /*= Qt::EditRole*/ )
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return false;


   if (!index.isValid())
      return FALSE;

   if ( Qt::EditRole != iRole )
      return TRUE;

   CInformationElement* pIE = mapIndexToIE( index );
   Q_ASSERT( NULLPTR != pIE );
   if ( !pIE )
      return TRUE;

   pIE->setDescription( value.toString() );
   return TRUE;

   // TODO: dataChanged() signal is not emitted. Is this really necessary?
}


/**
 * Maps the given model index to the underlying information element. I.e. returns
 * the internal pointer casted to an information element.
 *
 * If the given model index is not valid, then a <code>NULLPTR</code> is returned.
 */
// -------------------------------------------------------------------------------
CTreeInformationElement* CCollectionModel::mapIndexToIE( const QModelIndex& index ) const
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return NULLPTR;


   if ( index.isValid() )
   {
      CInformationElement* pIE = static_cast<CInformationElement*>(index.internalPointer());
      CTreeInformationElement* pTIE = dynamic_cast<CTreeInformationElement*>( pIE );
      Q_ASSERT( NULLPTR != pTIE );
      return pTIE;
   }
   else
      return NULLPTR;
}


/**
 * Method that maps the given path to a model index. If the path was not valid,
 * then an index is returned that is not valid either.
 */
// -------------------------------------------------------------------------------
QModelIndex CCollectionModel::mapPathToIndex( const Path& path ) const
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return QModelIndex();


   if ( !hasChildren() )
      return QModelIndex();
   
   QStringList list = path.getPathList();
   QModelIndex rootIndex = index( 0,0 );
   if ( getIEDescription( rootIndex ) != list[0] )
      return QModelIndex();
      

   QModelIndex index = rootIndex;
   for ( int i = 1; i < list.size(); i++ )
   {
      index = findChildWithDescription( index, list[i] );
      if ( !index.isValid() )
         return QModelIndex();
   }

   return index;
}

/**
 * Auxiliary method for <code>mapPathToIndex()</code>.
 *
 * A child of <code>parentIndex</code> is searched and returned which has
 * <code>sData</code> as "IE-Description". If such a child is not found,
 * then an invalid model index is returned.
 */
// -------------------------------------------------------------------------------
QModelIndex CCollectionModel::findChildWithDescription( QModelIndex parentIndex,
                                                        QString sData ) const
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return QModelIndex();


   if ( !hasChildren(parentIndex) )
      return QModelIndex();

   QModelIndex x;
   for ( int iRow = 0; iRow < rowCount( parentIndex ); iRow++ )
   {
      x = index( iRow, 0, parentIndex );
      if ( x.isValid() && (getIEDescription(x) == sData) )
         return x;
   }

   return QModelIndex();
}



/**
 * This method returns the description of an information element for a given
 * model index.
 * If the given model index is valid, then its internal pointer is used to
 * retrieve the underlying information element. Then the description of the
 * information element is returned.
 *
 * If the given model index is not valid, then an empty string is returned.
 */
// -------------------------------------------------------------------------------
QString CCollectionModel::getIEDescription( const QModelIndex& index ) const
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return QString();


   if ( index.isValid() )
   {
      CInformationElement* pIE = static_cast<CInformationElement*>(index.internalPointer());
      Q_ASSERT( NULLPTR != pIE );
      if ( pIE )
      {
         return pIE->getDescription();
      }
   }

   return QString();
}


/**
 * Method that returns the file name only; and not the whole absolute path and
 * file name of the data file that belongs to the underlying collection.
 */
// -------------------------------------------------------------------------------
QString CCollectionModel::getFileName() const
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return QString();


   QFileInfo fileInfo( mpCollection->getAbsFileName() );
   return fileInfo.fileName();
}


/** ****************** Methods for inserting and removing new entries ***********/
/**
 * This method is called if a new element should be added.
 */
// -------------------------------------------------------------------------------
bool CCollectionModel::insertRow( const QString&     sDescription,
                                  InformationFormat* pFormat,
                                  const QString&     sIconFileName,
                                  const QColor&      descriptionColor,
                                  bool               bExpires,
                                  const QDate&       date,

                                  int iRow, const QModelIndex& parent
                                )
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return FALSE;


   if ( !parent.isValid() )
      return FALSE;

   CInformationElement* pParentIE = mapIndexToIE( parent );
   CTreeInformationElement* pNewElement = createNewChild( sDescription,
                                                          pFormat,
                                                          sIconFileName,
                                                          descriptionColor,
                                                          bExpires,
                                                          date,
                                                          pParentIE );

   if ( !pNewElement )
      return FALSE;

      
   beginInsertRows( parent, iRow, iRow );  // begin insertion
   pParentIE->addChild( pNewElement );
   endInsertRows();                        // end insertion

   mpCollection->setActiveElement( pNewElement );
   return TRUE;
}

/**
 * Auxiliary class to create a new entry. This is called from <code>insertRow()</code>
 * and <code>dropMimeData()</code>.
 *
 * A pointer to the newly created entry is returned.
 */
// -------------------------------------------------------------------------------
CTreeInformationElement*
     CCollectionModel::createNewChild( const QString&     sDescription,
                                       InformationFormat* pFormat,
                                       const QString&     sIconFileName,
                                       const QColor&      descriptionColor,
                                       bool               bExpires,
                                       const QDate&       date,
      
                                       CInformationElement* pParentIE
                                     )
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return NULLPTR;


   Q_ASSERT( NULLPTR != pParentIE );
   if ( !pParentIE )
      return NULLPTR;
      
   // create default entry which will be added to the current entry as child
   CTreeInformationElement* pNewElement = new CTreeInformationElement( pParentIE,
                                                                       sDescription,
                                                                       "",
                                                                       pFormat,
                                                                       sIconFileName,
                                                                       bExpires,
                                                                       date );
   Q_ASSERT( NULLPTR != pNewElement );
   if ( !pNewElement )
      return NULLPTR;

   pNewElement->setDescriptionColor( descriptionColor );

   return pNewElement;
}

/**
 * This method is called if an element should be deleted.
 * The corresponding collection is called to delete this element.
 */
// -------------------------------------------------------------------------------
bool CCollectionModel::removeRow( int iRow, const QModelIndex& parent )
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return FALSE;


   if ( !parent.isValid() )
      return FALSE;

   CInformationElement* pParentIE = mapIndexToIE( parent );
   Q_ASSERT( NULLPTR != pParentIE );
   if ( !pParentIE )
      return FALSE;

   beginRemoveRows( parent, iRow, iRow );  // begin removal
   pParentIE->removeChild( iRow );
   endRemoveRows();                        // end removal
   
   return TRUE;
}


/** ****************** Methods for moving entries up/down ***********************/
/**
 * This method is called if a new element should be added.
 */
// -------------------------------------------------------------------------------
void CCollectionModel::moveEntryOneUp( const QModelIndex& index )
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::moveEntryOneUp()"<<std::endl;

   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return;


   if ( !index.isValid() )
      return;

   CTreeInformationElement* pIE = mapIndexToIE( index );
   Q_ASSERT( NULLPTR != pIE );
   if ( !pIE )
      return;

   // The root information element cannot be moved.
   if ( pIE == mpCollection->getRootElement() )
      return;

   layoutAboutToBeChanged();
   pIE->moveOneUp();
   layoutChanged();
}

// -------------------------------------------------------------------------------
void CCollectionModel::moveEntryOneDown( const QModelIndex& index )
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::moveEntryOneDown()"<<std::endl;

   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return;


   if ( !index.isValid() )
      return;

   CTreeInformationElement* pIE = mapIndexToIE( index );
   Q_ASSERT( NULLPTR != pIE );
   if ( !pIE )
      return;

   // The root information element cannot be moved.
   if ( pIE == mpCollection->getRootElement() )
      return;

   layoutAboutToBeChanged();
   pIE->moveOneDown();
   layoutChanged();
}


/** ****************** Methods for drag & drop **********************************/
/********************* Methods for drag *****************************************/
// -------------------------------------------------------------------------------
QStringList CCollectionModel::mimeTypes() const
// -------------------------------------------------------------------------------
{
   QStringList types;
   types << TUXCARDS_DRAGDROP_TYPE;

   return types;
}

// -------------------------------------------------------------------------------
QMimeData* CCollectionModel::mimeData( const QModelIndexList& indexes ) const
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::mimeData()"<<std::endl;
   
   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return NULLPTR;


   if ( indexes.isEmpty() )
      return NULLPTR;

   QModelIndex firstIndex = indexes.first();
   if ( !firstIndex.isValid() )
      return NULLPTR;

   CTreeInformationElement* pTIE = mapIndexToIE( firstIndex );
   if ( !pTIE )
      return NULLPTR;


   QByteArray encodedData;
   encodedData.append( mpCollection->toXML( pTIE ) );
   //std::cout<<"[DragContent]"<<QString(encodedData).toStdString()<<"[/DragContent]"<<std::endl;


   // TODO: Check whether the encoding for umlauts, accents, etc. is correct.
   //       If not it might be accomplished like this:
   //QTextStream t( &encodedData, QIODevice::WriteOnly );
   //t.setCodec( "UTF-8" );
   //t << mpCollection->toXML( pTIE );

   QMimeData* pMimeData = new QMimeData();
   pMimeData->setData( TUXCARDS_DRAGDROP_TYPE, encodedData );
   
   return pMimeData;
}


/********************* Methods for drop *****************************************/
/**
 * This method is called when some mime data should be dropped here.
 */
// -------------------------------------------------------------------------------
bool CCollectionModel::dropMimeData( const QMimeData* pData, Qt::DropAction,
                                     int iRow, int, const QModelIndex& parent )
// -------------------------------------------------------------------------------
{
   //std::cout<<"CCollectionModel::dropMimeData()"<<std::endl;

   Q_ASSERT( NULLPTR != mpCollection );
   if ( !mpCollection )
      return FALSE;


   if ( !pData )
      return FALSE;

   
   if ( !parent.isValid() )
      return FALSE;

   CTreeInformationElement* pParentTIE = dynamic_cast<CTreeInformationElement*>( mapIndexToIE(parent) );
   Q_ASSERT( NULLPTR != pParentTIE );
   if ( !pParentTIE )
      return FALSE;

   //std::cout<<"CCollectionModel::dropMimeData() - possible formats are:"<<std::endl;
   //QStringList possibleDropFormats = pData->formats();
   //foreach ( QString sFormat, possibleDropFormats )
   //   std::cout<<"   - "<<sFormat.toStdString()<<std::endl;
   
   // create a new entry from plain text
   if ( pData->hasFormat("text/plain") )
   {
      // if data are empty then return
      QString sInformationString = pData->text();
      if ( sInformationString.isEmpty() )
         return FALSE;

      // use first word for entry description
      QString sDescription = sInformationString.section( ' ', 0, 0, QString::SectionSkipEmpty );

      CTreeInformationElement* pNewElement = createNewChild( sDescription,
                                                             InformationFormat::getByString("ASCII"),
                                                             "",
                                                             QColor(0,0,0),
                                                             FALSE,
                                                             QDate::currentDate(),
                                                             pParentTIE );

      if ( !pNewElement )
         return FALSE;
   
      pNewElement->setInformation( sInformationString );
      
      beginInsertRows( parent, iRow, iRow );  // begin insertion
      pParentTIE->addChild( pNewElement );
      endInsertRows();                        // end insertion

      //std::cout<<"CCollectionModel::dropMimeData() - new entry created"<<std::endl;
   }
   else if ( pData->hasFormat( TUXCARDS_DRAGDROP_TYPE ) )
   {
      QString sDropData( pData->data( TUXCARDS_DRAGDROP_TYPE ) );
      //std::cout<<"[DropContents]"<<sDropData.toStdString()<<"[/DropContents]"<<std::endl;

      CInformationCollection* pCollection = XMLPersister::createInformationCollection( sDropData );
      if ( !pCollection )
         return FALSE;
         
      CInformationElement* pRoot = pCollection->getRootElement();
      CTreeInformationElement* pTIE = dynamic_cast<CTreeInformationElement*>(pRoot);

      Q_ASSERT( NULLPTR != pTIE );
      if ( !pTIE )
         return FALSE;
   
      beginInsertRows( parent, iRow, iRow );  // begin insertion
      pParentTIE->addChild( pTIE );
      endInsertRows();                        // end insertion

      //std::cout<<"CCollectionModel::dropMimeData() - entry moved"<<std::endl;
   }
   
   return TRUE;
}
