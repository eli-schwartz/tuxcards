/***************************************************************************
                          CMainWindow.h  -  description
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
#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QObject>
#include <QMainWindow>

#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QToolBar>
#include <QTextCharFormat>

class CCactusBar;
class QSplitter;
class CEditor;
class CSingleEntryView;
class QComboBox;
class CRecentFileList;


#include "./CTree.h"
#include "../information/CInformationCollection.h"
#include "../CTuxCardsConfiguration.h"
class CConfigurationDialog;
class CSearchPosition;

#include <iostream>

class CMainWindow : public QMainWindow
{
   Q_OBJECT
public:
   CMainWindow( const QString& sArg );
   ~CMainWindow();


public slots:
//  virtual void keyPressEvent( QKeyEvent* );

protected:
   virtual void            closeEvent( QCloseEvent* pEv );
   virtual void            timerEvent( QTimerEvent* );
//  void checkFirstTime( void );


private:
   CInformationCollection* mpCollection;
   QMenuBar*               mpMenu;

   CConfigurationDialog*   mpConfigurationDialog;
   void                    settingUpConfigurationDialog();
   CTuxCardsConfiguration& mConfiguration;
   int                     miTimerID;

   CCactusBar*             mpCactusBar;
   QSplitter*              mpSplit;
   CTree*                  mpTree;
   CSingleEntryView*       mpSingleEntryView;
   CEditor*                mpEditor;

   QLabel*                 mpStatusBar_ChangeLabel;
   QLabel*                 mpStatusBar_TextFormatLabel;
   bool                    mbChangesExist;           // remembers whether changes are done or not (for saving)

   QDialog                 mSaveChangesDialog;
   void                    settingUpSaveChangesDialog();

   void                    settingUpSearchDialog();

   void                    settingUpActions();
   void                    settingUpComboBoxes();
   void                    settingUpMenu();
   void                    settingUpToolBar();
   void                    settingUpStatusBar();
   void                    settingUpMainWindow();
   void                    settingUpDialogs();
   void                    settingUpCactusBar( QLayout& layout );
   void                    settingUpTree( QWidget& parent );
   void                    settingUpEditor( QWidget& parent );

   void                    setWindowTitle();
   void                    setWindowGeometry( int,int,int,int, int,int );


//  int  getDataFileFormat( QString fileName );
//  bool openOldDataFile( QString fileName );

   void                    deleteCollectionAndCreateADefaultOne( bool bCreateDefaultCollection );
   bool                    openXMLDataFile( const QString& sFileName );

   void                    loadLastSavedDataIfConfigFileIsFound( const QString& sArg );
   void                    initializingCollection( const QString& sDataFileName );
   QMessageBox::Button     askForSaving( const QString& sQuestion );
   void                    setLastActiveElement();

   void                    setWindowGeometryToConfiguration();
   void                    callingExecutionStatement() const;

   void                    textColorChanged( const QColor& c );

   QComboBox*              mpEditFontCombo;
   QComboBox*              mpEditFontSizeCombo;

   // file actions
   QAction*                mpFileNewAction;
   QAction*                mpFileOpenAction;
   QAction*                mpFileSaveAction;
   QAction*                mpFileSaveAsAction;
   QAction*                mpFileSaveActiveEntryAction;
   QAction*                mpFilePrintAction;
   QAction*                mpFileExportHTMLAction;
   QAction*                mpSearchEntryAction;

   // edit actions
   QAction*                mpEditUndoAction;
   QAction*                mpEditRedoAction;
   QAction*                mpEditCutAction;
   QAction*                mpEditCopyAction;
   QAction*                mpEditPasteAction;
   QAction*                mpEditSelectAllAction;

   QAction*                mpEditBoldAction;
   QAction*                mpEditItalicAction;
   QAction*                mpEditUnderAction;
   QAction*                mpEditColorAction;

   QAction*                mpEditTextLeftAction;
   QAction*                mpEditTextCenterAction;
   QAction*                mpEditTextRightAction;
   QAction*                mpEditTextBlockAction;

   CRecentFileList*        mpRecentFiles;

   // enable/disable toolbars
   QToolBar*               mpMainToolBar;
   QToolBar*               mpEntryToolBar;
   QToolBar*               mpEditorToolBar;
   QToolBar*               mpEditorToolBar2;

   QAction*                mpMainToolBarAction;
   QAction*                mpEntryToolBarAction;
   QAction*                mpEditorToolBarAction;

/*
  #ifdef FEATURE_RECENTFILELIST_TOOLBAR
  QToolBar*  mpRecentFilesTools;
  int        miRecentFilesToolBarID;
  #endif


  CInformationElement* getActiveIE();
*/

   void                    applyConfigurationAfterShow();
   void                    showDonationMsg();

private slots:
   void                    showMessage( const QString& sMessage, int iSeconds );

   void                    recognizeChanges();                   // to keep track of changes
   void                    clearChanges();

   // menu calls
   void                    clearAll();
   void                    open();
   bool                    open( const QString& sFileName );
   void                    save();
   void                    save( const QString& );
   void                    saveAs();
   void                    exportHTML();
   void                    exit();
   void                    showAbout();

   void                    setEditListStyle( int iStyleIndex );
   void                    setEditFontFamily( const QString& f );
   void                    setEditFontSize( const QString& s );
   void                    setEditTextColor();
   void                    textLeft();
   void                    textHCenter();
   void                    textRight();
   void                    textBlock();
/*
  void changeInformationFormat();  */
   void                    textFontChanged( const QTextCharFormat& f );
   void                    textAlignmentChanged( Qt::Alignment );
   void                    clipboardDataChanged();
   void                    showRecognizedFormat( InformationFormat format );

   void                    slotEditConfiguration();
   void                    applyConfiguration();

   void                    wordCount();
   void                    insertCurrentDate();
   void                    insertCurrentTime();

   void                    slotActiveIEChangedViaHistory( CInformationElement* pIE );

/*
  // debug slots
  void debugShowRTFTextSource();
  void debugShowXMLCode();

//  void saveActiveEntry();
  void quicklyLoad(Path*);
*/

   void                    print();



//  void addElementToBookmarksEvent();

   void                    slotSaveAndLoadNewFile( const QString& sNewFile );

   void                    slotShowSearchDialog();
   void                    makeVisible( const CSearchPosition* const pPosition );

  void                     slotSetMainToolBarVisible( bool bVisible );
  void                     slotSetEntryToolBarVisible( bool bVisible );
  void                     slotSetEditorToolBarVisible( bool bVisible );
/*
  #ifdef FEATURE_RECENTFILELIST_TOOLBAR
  void setRecentFilesToolbarVisible( bool bVisible );
  #endif
*/
};

#endif
