/***************************************************************************
                          CRecentFileList.h  -  description
                             -------------------
    begin                : Sun Feb 09 2003
    copyright            : (C) 2003 by Alexander Theel
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

#ifndef RECENTFILELIST_H
#define RECENTFILELIST_H

#include <qobject.h>
#include <qstringlist.h>

class QMenu;
class QComboBox;
class QAction;


/**
 * Class that provides a recent file list. The data structure of the file list
 * is keeped as well as popup menu for opening one of the recently used files.
 *
 * The popup menu is inserted as submenu into the given <code>pParentMenu</code>.
 *
 * At wish a comboBox displaying the recent file files may be created, too.
 * To do so, call <code>createComboBox()</code>. The comboBox may be added to
 * any widget.s
 */
class CRecentFileList : public QObject {
  Q_OBJECT
public:
   CRecentFileList( QWidget* pParent, QMenu* pParentMenu, const QString& sFiles="" );
   
   void          setOnTop( const QString& sAbsPath );
   
   void          createComboBox( QWidget& parentWidget );
   
   QString       toString() const;

private:
   QMenu*        mpParentMenu;
   QStringList   mFileList;
   
   QMenu*        mpRecentlyFilesMenu;
   QComboBox*    mpComboBox;
   
   static const QString SEPARATOR;
   static const uint    MAX_ELEMENT_COUNT;


   QList<QAction*> mRecentFileActs;
   
   void          createActions( QWidget* pParent );
   
   void          setList( const QString& sFiles );
   void          checkSize();
   void          update();
   void          updateMenu();
   void          updateComboBox();
   
   void          remove( const QString& sAbsPath );

private slots:
   void          slotOpenRecentFile();
  
signals:
   void          openFile( QString sFileName );
};

#endif
