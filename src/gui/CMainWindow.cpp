/***************************************************************************
                          CMainWindow.cpp  -  description
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

#include "CMainWindow.h"

#include "../information/xmlpersister.h"
#include "./cactusbar/CCactusBar.h"
#include "./CTree.h"

#include <QHBoxLayout>
#include <QSplitter>
#include <QScrollBar>                  // for  mpTree->verticalScrollBar()

#include <QMenuBar>
#include <QToolBar>
#include <QComboBox>
#include "../utilities/CIconManager.h"
#define  getIcon(x)  CIconManager::getInstance().getIcon(x)

#include <QStatusBar>

#include "./editor/CEditor.h"
#include "./editor/CSingleEntryView.h"

#include <QColorDialog>
#include <QTextList>
#include <QFontDatabase>
#include <QFontMetrics>
#include <QFontComboBox>
#include <QLineEdit>
#include <QApplication>

#include <QPrinter>
#include <QPrintDialog>

#include "./dialogs/configurationDialog/CConfigurationDialog.h"
#include "./dialogs/searchDialog/CSearchDialog.h"
#include "./dialogs/searchDialog/CSearchPosition.h"

//#define DEBUGGING

//#include <iostream>
//#include <stdlib.h>
//#include <qfontdialog.h>
//

//#include <QTimerEvent>
//#include <QKeyEvent>
//#include <QTextStream>
//#include <QCloseEvent>
//#include <QPixmap>
//#include <QLabel>
//#include <QAction>
//
//#include "../icons/lo16-app-tuxcards.xpm"
//
//#ifdef DEBUGGING
//  #include "../icons/debug/showText.xpm"
//  #include "../icons/debug/xml.xpm"
//#endif
//
//#include "../information/converter.h"
//#include "../information/CInformationCollection.h"
//#include "../persister.h"
//#include "../information/xmlpersister.h"
//#include "../utilities/iniparser/configparser.h"

//#include <qpainter.h>

//#include <qapplication.h>
//#include <QTextCodec>
//#include <QScrollBar>

#include "../utilities/strings.h"
#include "../information/htmlwriter.h"
#include "./recentFileList/CRecentFileList.h"

//#include "./bookmark/BookmarkButton.h"
//#include "./dialogs/expiredElementsDialog/ExpiredElementsDialog.h"
//
//#include "../utilities/crypt/StringCrypter.h"

#include "../version.h"
//#include "../global.h"
//#include "../Greetings.h"

#include <QLabel>

// -------------------------------------------------------------------------------
CMainWindow::CMainWindow( const QString& sArg )
 : QMainWindow( NULLPTR )
 , mpCollection( NULLPTR )
 , mpMenu( NULLPTR )
 , mpConfigurationDialog( NULLPTR )
 , mConfiguration( CTuxCardsConfiguration::getInstance() )
 , miTimerID( 0 )
 , mpCactusBar( NULLPTR )
 , mpSplit( NULLPTR )
 , mpTree( NULLPTR )
 , mpSingleEntryView( NULLPTR )
 , mpEditor( NULLPTR )
// , mpQuickLoader( NULLPTR )
 , mpStatusBar_ChangeLabel( NULLPTR )
 , mpStatusBar_TextFormatLabel( NULLPTR )
 , mbChangesExist( FALSE )
 , mSaveChangesDialog( this )

 , mpEditFontCombo( NULLPTR )
 , mpEditFontSizeCombo( NULLPTR )

 , mpFileNewAction( NULLPTR )
 , mpFileOpenAction( NULLPTR )
 , mpFileSaveAction( NULLPTR )
 , mpFileSaveAsAction( NULLPTR )
 , mpFileSaveActiveEntryAction( NULLPTR )
 , mpFilePrintAction( NULLPTR )
 , mpFileExportHTMLAction( NULLPTR )
 , mpSearchEntryAction( NULLPTR )

 , mpEditUndoAction( NULLPTR )
 , mpEditRedoAction( NULLPTR )
 , mpEditCutAction( NULLPTR )
 , mpEditCopyAction( NULLPTR )
 , mpEditPasteAction( NULLPTR )
 , mpEditSelectAllAction( NULLPTR )
 , mpEditBoldAction( NULLPTR )
 , mpEditItalicAction( NULLPTR )
 , mpEditUnderAction( NULLPTR )
 , mpEditColorAction( NULLPTR )
 , mpEditTextLeftAction( NULLPTR )
 , mpEditTextCenterAction( NULLPTR )
 , mpEditTextRightAction( NULLPTR )
 , mpEditTextBlockAction( NULLPTR )

 , mpRecentFiles( NULLPTR )

 , mpMainToolBar( NULLPTR )
 , mpEntryToolBar( NULLPTR )
 , mpEditorToolBar( NULLPTR )
 , mpEditorToolBar2( NULLPTR )
 , mpMainToolBarAction( NULLPTR )
 , mpEntryToolBarAction( NULLPTR )
 , mpEditorToolBarAction( NULLPTR )
// #ifdef FEATURE_RECENTFILELIST_TOOLBAR
// , mpRecentFilesTools( NULLPTR )
// , miRecentFilesToolBarID( 0 )
// #endif
// -------------------------------------------------------------------------------
{
//   checkFirstTime();

   settingUpMainWindow();
   settingUpDialogs();

   applyConfiguration();

////  connect(mpTree, SIGNAL(freezeCHANGES()), this, SLOT(freezeCHANGES()));
//


   // build up tree, if config-file ('.tuxcards') was found
   loadLastSavedDataIfConfigFileIsFound( sArg );


   // show the completed window
   show();
   applyConfigurationAfterShow();

   showDonationMsg();
}


// -------------------------------------------------------------------------------
CMainWindow::~CMainWindow()
// -------------------------------------------------------------------------------
{
   DELETE( mpCollection );
}


//// -------------------------------------------------------------------------------
//// This is an emergency exit. The application will be quited if not enough
//// memory is available to create all needed objects.
//// -------------------------------------------------------------------------------
//void CMainWindow::lowMemoryExit( void )
//// -------------------------------------------------------------------------------
//{
//   std::cout<<"TuxCards ERROR\nNot enough memory to run application.\nTuxCards"
//              " will be stopped!!!"<<std::endl;
//   QMessageBox::critical( NULLPTR, "TuxCards", "Not enough memory to run "
//                          "application.\nThe program will be quit.",
//                          QMessageBox::Abort, QMessageBox::NoButton );
//   QApplication::exit( -1 );
//}
//
//// -------------------------------------------------------------------------------
//// Convenience method for 'lowMemoryExit()'.
//// -------------------------------------------------------------------------------
//void CMainWindow::checkPointer( void* pPointer )
//// -------------------------------------------------------------------------------
//{
//   if ( NULLPTR == pPointer )
//   {
//      lowMemoryExit();
//   }
//}



/**
 * This method builds up the mainwindow.
 */
// -------------------------------------------------------------------------------
void CMainWindow::settingUpMainWindow()
// -------------------------------------------------------------------------------
{
   setWindowIcon( getIcon("lo32-app-tuxcards") );
   CMainWindow::setWindowTitle();
//   CIconManager::getInstance().setIconDirectory( mConfiguration.getStringValue( CTuxCardsConfiguration::S_ICON_DIR ) );

   QWidget* pCentralWidget = new QWidget( this );
   Q_ASSERT( NULLPTR != pCentralWidget );
   if ( !pCentralWidget )
      return;

   setCentralWidget( pCentralWidget );
   QHBoxLayout* pLayout = new QHBoxLayout();
   Q_ASSERT( NULLPTR != pLayout );
   if ( !pLayout )
      return;

   centralWidget()->setLayout( pLayout );


   settingUpCactusBar( *pLayout );

   mpSplit = new QSplitter( 0 );
   Q_ASSERT( NULLPTR != mpSplit );
   if ( !mpSplit )
      return;

   mpSplit->setMinimumSize( 600,400 );
   pLayout->addWidget( mpSplit );
//   checkPointer( mpSplit );

   settingUpTree( *mpSplit );
   settingUpEditor( *mpSplit );

   mpSplit->setOpaqueResize(TRUE);

   settingUpActions();
   settingUpMenu();
   settingUpToolBar();
   settingUpComboBoxes();
   settingUpStatusBar();
}


// -------------------------------------------------------------------------------
void CMainWindow::setWindowTitle()
// -------------------------------------------------------------------------------
{
   QString sWindowTitle;
   if ( mpCollection )
   {
      QString sFileName = QFileInfo( mpCollection->getAbsFileName() ).fileName();
      sWindowTitle = QString("%1[*] - TuxCards").arg( sFileName );
   }
   else
   {
      sWindowTitle = "TuxCards";
   }

   QMainWindow::setWindowTitle( sWindowTitle );
}


/**
 * This method builds the necessary dialogs used within the application.
 */
// -------------------------------------------------------------------------------
void CMainWindow::settingUpDialogs()
// -------------------------------------------------------------------------------
{
//   CPropertyDialog::createInstance( this );
////  settingUpQuickLoader();

   settingUpConfigurationDialog();
   settingUpSaveChangesDialog();
   settingUpSearchDialog();
}

// -------------------------------------------------------------------------------
void CMainWindow::settingUpConfigurationDialog()
// -------------------------------------------------------------------------------
{
   mpConfigurationDialog = new CConfigurationDialog( this, mConfiguration );
   if ( mpConfigurationDialog )
   {
      connect( mpConfigurationDialog, SIGNAL(configurationChanged()), this, SLOT(applyConfiguration()) );
   }
}

// -------------------------------------------------------------------------------
void CMainWindow::settingUpSaveChangesDialog()
// -------------------------------------------------------------------------------
{
   mSaveChangesDialog.setWindowTitle( tr("Saving") );
   (void) new QLabel( tr("Saving data ..."), &mSaveChangesDialog );
}

// -------------------------------------------------------------------------------
void CMainWindow::settingUpSearchDialog()
// -------------------------------------------------------------------------------
{
   CSearchDialog::createInstance( this );

   CSearchDialog* pSearchDialog = CSearchDialog::getInstance();

   Q_ASSERT( NULLPTR != pSearchDialog );
   if ( !pSearchDialog )
      return;

   connect( pSearchDialog, SIGNAL( makeVisible(const CSearchPosition* const) ),
            this, SLOT( makeVisible(const CSearchPosition* const) ) );
}

// -------------------------------------------------------------------------------
void CMainWindow::slotShowSearchDialog()
// -------------------------------------------------------------------------------
{
   // Saving eventual changes - Set text to current active entry;
   // otherwise a search would not find newly written text within
   // this entry (that was not yet saved).
   if ( !mpEditor )
      return;
   mpEditor->writeCurrentTextToActiveInformationElement();


   CSearchDialog* pSearchDialog = CSearchDialog::getInstance();

   Q_ASSERT( NULLPTR != pSearchDialog );
   if ( !pSearchDialog )
      return;

   pSearchDialog->setUp( mpCollection );
}


// -------------------------------------------------------------------------------
void CMainWindow::slotActiveIEChangedViaHistory( CInformationElement* pIE )
// -------------------------------------------------------------------------------
{
   if ( pIE && mpCollection )
      mpCollection->setActiveElement( pIE );
}


// -------------------------------------------------------------------------------
void CMainWindow::settingUpEditor( QWidget& parent )
// -------------------------------------------------------------------------------
{
   mpSingleEntryView = new CSingleEntryView( &parent );
   Q_ASSERT( NULLPTR != mpSingleEntryView );
   if ( !mpSingleEntryView )
      return;
//   checkPointer( mpSingleEntryView );        // qt4-port maybe remove the 'checkpointer()'-stuff
//
   mpEditor = mpSingleEntryView->getEditor();
   Q_ASSERT( NULLPTR != mpEditor );
   if ( !mpEditor )
      return;


   connect(mpEditor, SIGNAL(textChanged()), this, SLOT(recognizeChanges()));
   connect(mpEditor, SIGNAL(formatRecognized(InformationFormat)), this, SLOT(showRecognizedFormat(InformationFormat)));
   connect(mpEditor, SIGNAL(currentHAlignmentChanged(Qt::Alignment)), this, SLOT(textAlignmentChanged(Qt::Alignment)) );

   // recognizing text format changes
   connect( mpEditor, SIGNAL( currentCharFormatChanged(const QTextCharFormat&) ),
            this,     SLOT( textFontChanged(const QTextCharFormat&) ) );

   connect( mpSingleEntryView, SIGNAL(signalEntryDecrypted()), mpTree, SLOT(slotAdjustEncryptionActions()) );

   // connect the model to the editor
   if ( mpTree )
      connect( &(mpTree->getModel()), SIGNAL(rowsInserted(const QModelIndex&, int, int )), mpEditor, SLOT(slotNewInformationElementAdded()) );
}


// -------------------------------------------------------------------------------
void CMainWindow::settingUpTree( QWidget& parent )
// -------------------------------------------------------------------------------
{
   mpTree = new CTree( &parent );

   Q_ASSERT( NULLPTR != mpTree );
   if ( !mpTree )
      return;

//   checkPointer( mpTree );

   connect( mpTree, SIGNAL(setActiveIEViaHistory(CInformationElement*)),
            this,   SLOT(slotActiveIEChangedViaHistory(CInformationElement*)) );;
   connect( mpTree, SIGNAL(showMessage(QString, int)),    this, SLOT(showMessage(QString, int)) );
//   connect( mpTree, SIGNAL(addEntryToBookmarksSignal()),  this, SLOT(addElementToBookmarksEvent()) );
}

// -------------------------------------------------------------------------------
void CMainWindow::settingUpCactusBar( QLayout& layout )
// -------------------------------------------------------------------------------
{
   // the cactusbar *must* be setup with some colors and text
   mpCactusBar = new CCactusBar( 0, QColor(0,0,0),QColor(33,72,170), "Tux","Cards");
   layout.addWidget( mpCactusBar );

//   checkPointer( mpCactusBar );
}


// -------------------------------------------------------------------------------
void CMainWindow::settingUpStatusBar( void )
// -------------------------------------------------------------------------------
{
   QStatusBar* pStatusBar = statusBar();

   Q_ASSERT( NULLPTR != pStatusBar );
   if ( !pStatusBar )
      return;

   // text format label
   mpStatusBar_TextFormatLabel = new QLabel();
   Q_ASSERT( NULLPTR != mpStatusBar_TextFormatLabel );
   if ( !mpStatusBar_TextFormatLabel )
      return;

   mpStatusBar_TextFormatLabel->setAlignment( Qt::AlignCenter );
   mpStatusBar_TextFormatLabel->setFixedWidth( 45 );
   pStatusBar->addPermanentWidget( mpStatusBar_TextFormatLabel );
   
   // change label
   mpStatusBar_ChangeLabel = new QLabel();
   Q_ASSERT( NULLPTR != mpStatusBar_ChangeLabel );
   if ( !mpStatusBar_ChangeLabel )
      return;

   mpStatusBar_ChangeLabel->setAlignment( Qt::AlignCenter );
   mpStatusBar_ChangeLabel->setFixedWidth( 15 );
   pStatusBar->addPermanentWidget( mpStatusBar_ChangeLabel );
}


// -------------------------------------------------------------------------------
void CMainWindow::settingUpActions( void )
// -------------------------------------------------------------------------------
{
   // file actions
   mpFileNewAction = new QAction( getIcon("filenew" ), tr("&New"), this );
   connect( mpFileNewAction, SIGNAL(triggered()), this, SLOT(clearAll()) );

   mpFileOpenAction = new QAction( getIcon("fileopen"), tr("&Open File..."), this );
   connect( mpFileOpenAction, SIGNAL(triggered()), this, SLOT(open()) );

   mpFileSaveAction = new QAction( getIcon("filesave"), tr("&Save"), this );
   mpFileSaveAction->setShortcut( Qt::ControlModifier + Qt::Key_S );
   connect( mpFileSaveAction, SIGNAL(triggered()), this, SLOT(save()) );

   mpFileSaveAsAction = new QAction( tr("Save &As..."), this );
   connect( mpFileSaveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()) );

   mpFileSaveActiveEntryAction = new QAction( tr("Save Active Entry"), this );
   connect( mpFileSaveActiveEntryAction, SIGNAL(triggered()), this, SLOT(saveActiveEntry()) );

   mpFilePrintAction = new QAction( getIcon("fileprint"), tr("&Print..."), this );
   connect( mpFilePrintAction, SIGNAL(triggered()), this, SLOT(print()) );

   mpFileExportHTMLAction = new QAction( tr("Export to &HTML..."), this );
   connect( mpFileExportHTMLAction, SIGNAL(triggered()), this, SLOT(exportHTML()) );

   mpSearchEntryAction = new QAction( getIcon("find"), tr("Search Entry..."), this );
   mpSearchEntryAction->setShortcut( Qt::ControlModifier + Qt::Key_F );
   connect( mpSearchEntryAction, SIGNAL(triggered()), this, SLOT(slotShowSearchDialog()) );

   //***************************************************************************//
   // editor actions - undo redo
   mpEditUndoAction = new QAction( getIcon("undo"), tr("&Undo"), this );
   mpEditUndoAction->setShortcut( Qt::ControlModifier + Qt::Key_Z );
   connect( mpEditor, SIGNAL(undoAvailable(bool)), mpEditUndoAction, SLOT(setEnabled(bool)) );
   connect( mpEditUndoAction, SIGNAL(triggered()), mpEditor, SLOT(undo()) );

   mpEditRedoAction = new QAction( getIcon("redo"), tr("&Redo"), this );
   mpEditRedoAction->setShortcut( Qt::ControlModifier + Qt::Key_Y );
   connect( mpEditor, SIGNAL(redoAvailable(bool)), mpEditRedoAction, SLOT(setEnabled(bool)) );
   connect( mpEditRedoAction, SIGNAL(triggered()), mpEditor, SLOT(redo()) );


   // editor actions - cut, copy, paste
   mpEditCutAction = new QAction( getIcon("editcut"), tr("Cut"), this );
   mpEditCutAction->setShortcut( Qt::ControlModifier + Qt::Key_X );
   mpEditCutAction->setEnabled( false );
   connect( mpEditCutAction, SIGNAL(triggered()), mpEditor, SLOT(cut()) );
   connect( mpEditor, SIGNAL(copyAvailable(bool)), mpEditCutAction, SLOT(setEnabled(bool)) );

   mpEditCopyAction = new QAction( getIcon("editcopy"), tr("&Copy"), this );
   mpEditCopyAction->setShortcut( Qt::ControlModifier + Qt::Key_C );
   mpEditCopyAction->setEnabled( false );
   connect( mpEditCopyAction, SIGNAL(triggered()), mpEditor, SLOT(copy()) );
   connect( mpEditor, SIGNAL(copyAvailable(bool)), mpEditCopyAction, SLOT(setEnabled(bool)) );

   mpEditPasteAction = new QAction( getIcon("editpaste"), tr("&Paste"), this );
   mpEditPasteAction->setShortcut( Qt::ControlModifier + Qt::Key_V );
   mpEditPasteAction->setEnabled( !QApplication::clipboard()->text().isEmpty() );
   connect( mpEditPasteAction, SIGNAL(triggered()), mpEditor, SLOT(paste()) );
   connect( QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()) );


   // editor action - select all
   mpEditSelectAllAction = new QAction( tr("Select &All"), this );
   mpEditSelectAllAction->setShortcut( Qt::ControlModifier + Qt::Key_A );
   connect( mpEditSelectAllAction, SIGNAL(triggered()), mpEditor, SLOT(selectAll()) );

   // editor actions - text formating
   mpEditBoldAction = new QAction( getIcon("text_bold"), tr("&Bold") /*"(Ctrl+B)"*/, this );
   QFont f = mpEditBoldAction->font(); f.setBold( TRUE ); mpEditBoldAction->setFont( f );
   mpEditBoldAction->setShortcut( Qt::ControlModifier + Qt::Key_B );
   mpEditBoldAction->setCheckable( TRUE );
   connect( mpEditBoldAction, SIGNAL(triggered()), mpEditor, SLOT(toggleFontBold()) );

   mpEditItalicAction = new QAction( getIcon("text_italic"), tr("&Italic") /*"(Ctrl+I)"*/, this );
   f = mpEditItalicAction->font(); f.setItalic( TRUE ); mpEditItalicAction->setFont( f );
   mpEditItalicAction->setShortcut( Qt::ControlModifier + Qt::Key_I );
   mpEditItalicAction->setCheckable( TRUE );
   connect( mpEditItalicAction, SIGNAL(triggered(bool)), mpEditor, SLOT(setFontItalic(bool)) );

   mpEditUnderAction = new QAction( getIcon("text_under"), tr("Underline") /*"(Ctrl+U)"*/, this );
   f = mpEditUnderAction->font(); f.setUnderline( TRUE ); mpEditUnderAction->setFont( f );
   mpEditUnderAction->setShortcut( Qt::ControlModifier + Qt::Key_U );
   mpEditUnderAction->setCheckable( TRUE );
   connect( mpEditUnderAction, SIGNAL(triggered(bool)), mpEditor, SLOT(setFontUnderline(bool)) );

   // color action
   QPixmap dummy(1,1);
   mpEditColorAction = new QAction( dummy, tr("Color..."), this );
   connect( mpEditColorAction, SIGNAL(triggered()), this, SLOT(setEditTextColor()) );
   textColorChanged( Qt::black );


   // alignment actions
   mpEditTextLeftAction = new QAction( getIcon("text_left"), tr("Align Left"), this );
   mpEditTextLeftAction->setCheckable( TRUE );
   connect( mpEditTextLeftAction, SIGNAL(triggered()), this, SLOT(textLeft()) );

   mpEditTextCenterAction = new QAction( getIcon("text_center"), tr("Center"), this );
   mpEditTextCenterAction->setCheckable( TRUE );
   connect( mpEditTextCenterAction, SIGNAL(triggered()), this, SLOT(textHCenter()) );

   mpEditTextRightAction = new QAction( getIcon("text_right"), tr("Align Right"), this );
   mpEditTextRightAction->setCheckable( TRUE );
   connect( mpEditTextRightAction, SIGNAL(triggered()), this, SLOT(textRight()) );

   mpEditTextBlockAction = new QAction( getIcon("text_block"), tr("Text Block"), this );
   mpEditTextBlockAction->setCheckable( TRUE );
   connect( mpEditTextBlockAction, SIGNAL(triggered()), this, SLOT(textBlock()) );


   // toggle toolbar actions
   mpMainToolBarAction = new QAction( tr("Show Main Toolbar"), this );
   mpMainToolBarAction->setCheckable( TRUE );
   mpMainToolBarAction->setChecked( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_SHOW_MAIN_TOOLBAR ) );
   connect( mpMainToolBarAction, SIGNAL(toggled(bool)), this, SLOT(slotSetMainToolBarVisible(bool)) );

   mpEntryToolBarAction = new QAction( tr("Show Entry Manipulation Toolbar"), this );
   mpEntryToolBarAction->setCheckable( TRUE );
   mpEntryToolBarAction->setChecked( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_SHOW_ENTRY_TOOLBAR ) );
   connect( mpEntryToolBarAction, SIGNAL(toggled(bool)), this, SLOT(slotSetEntryToolBarVisible(bool)) );

   mpEditorToolBarAction = new QAction( tr("Show Editor Toolbar"), this );
   mpEditorToolBarAction->setCheckable( TRUE );
   mpEditorToolBarAction->setChecked( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_SHOW_EDITOR_TOOLBAR ) );
   connect( mpEditorToolBarAction, SIGNAL(toggled(bool)), this, SLOT(slotSetEditorToolBarVisible(bool)) );
}


// -------------------------------------------------------------------------------
void CMainWindow::settingUpMenu( void )
// -------------------------------------------------------------------------------
{
   // File menu.
   QMenu* pFileMenu = new QMenu( tr("&File") );
   Q_ASSERT( NULLPTR != pFileMenu );
   if ( pFileMenu )
   {
      pFileMenu->addAction( mpFileNewAction );
      pFileMenu->addAction( mpFileOpenAction );
      pFileMenu->addAction( mpFileSaveAction );
      pFileMenu->addAction( mpFileSaveAsAction );
      //pFileMenu->addAction( mpFileSaveActiveEntryAction );
      pFileMenu->addAction( mpFilePrintAction );

      pFileMenu->addSeparator();
      mpRecentFiles = new CRecentFileList( this, pFileMenu, mConfiguration.getStringValue( CTuxCardsConfiguration::S_RECENT_FILES ) );
//    checkPointer( mpRecentFiles );
      connect( mpRecentFiles, SIGNAL(openFile(QString)), this, SLOT(slotSaveAndLoadNewFile(QString)) );

      pFileMenu->addSeparator();
      pFileMenu->addAction( mpFileExportHTMLAction );
      pFileMenu->addSeparator();
      pFileMenu->addAction( getIcon("exit"), tr("&Exit"), this, SLOT(exit()) );
   }


   // Edit menu.
   QMenu* pEditMenu = new QMenu( tr("&Edit") );
   Q_ASSERT( NULLPTR != pEditMenu );
   if ( pEditMenu )
   {
      // add editor undo,redo,cut,copy,paste actions
      pEditMenu->addAction( mpEditUndoAction );
      pEditMenu->addAction( mpEditRedoAction );
      pEditMenu->addAction( mpEditCutAction );
      pEditMenu->addAction( mpEditCopyAction );
      pEditMenu->addAction( mpEditPasteAction );

      // add editor "select all" action
      pEditMenu->addSeparator();
      pEditMenu->addAction( mpEditSelectAllAction );

      // add editor/font actions
      pEditMenu->addSeparator();
      pEditMenu->addAction( mpEditBoldAction );
      pEditMenu->addAction( mpEditItalicAction );
      pEditMenu->addAction( mpEditUnderAction );
      pEditMenu->addAction( mpEditColorAction );

      // add editor alignment actions
      pEditMenu->addSeparator();
      pEditMenu->addAction( mpEditTextLeftAction );
      pEditMenu->addAction( mpEditTextCenterAction );
      pEditMenu->addAction( mpEditTextRightAction );
      pEditMenu->addAction( mpEditTextBlockAction );
   }


   // Options menu - part 1.
   QMenu* pToggleToolBarsMenu = new QMenu( tr("ToolBars") );
   Q_CHECK_PTR( pToggleToolBarsMenu );

   if ( pToggleToolBarsMenu )
   {
      pToggleToolBarsMenu->addAction( mpMainToolBarAction );
      pToggleToolBarsMenu->addAction( mpEntryToolBarAction );
      pToggleToolBarsMenu->addAction( mpEditorToolBarAction );
// #ifdef FEATURE_RECENTFILELIST_TOOLBAR
// miRecentFilesToolBarID = (int)toolbars->addAction( "Show Recent Files Toolbar", this, SLOT(toggleRecentFilesToolbarVisability()) );
// #endif*/
   }

   // Options menu - part 2.
   QMenu* pOptionsMenu = new QMenu( tr("&Options") );
   Q_ASSERT( NULLPTR != pOptionsMenu );
   if ( pOptionsMenu )
   {
      pOptionsMenu->addMenu( pToggleToolBarsMenu );
      pOptionsMenu->addAction( tr("&Edit Options..."), this, SLOT(slotEditConfiguration()) );
   }


   // Extras menu.
   QMenu* pExtrasMenu = new QMenu( tr("E&xtras") );
   Q_ASSERT( NULLPTR !=  pExtrasMenu );
   if ( pExtrasMenu )
   {
      pExtrasMenu->addAction( mpSearchEntryAction );
      pExtrasMenu->addAction( tr("Word Count"),          this, SLOT(wordCount()),         Qt::ControlModifier+Qt::Key_W );
      pExtrasMenu->addAction( tr("Insert Current Date"), this, SLOT(insertCurrentDate()), Qt::ControlModifier+Qt::Key_D );
      pExtrasMenu->addAction( tr("Insert Current Time"), this, SLOT(insertCurrentTime()), Qt::ControlModifier+Qt::Key_T );
      pExtrasMenu->addAction( tr("Insert Image..."),     mpEditor, SLOT(insertImage()),   Qt::ControlModifier+Qt::Key_P );
   }


   // About menu.
   QMenu* pAboutMenu = new QMenu( tr("&About") );
   Q_ASSERT( NULLPTR !=  pAboutMenu );
   if ( pAboutMenu )
   {
      pAboutMenu->addAction( getIcon("lo32-app-tuxcards"), tr("About TuxCards"), this, SLOT(showAbout()) );
   }


   // Add the individual menus to the menubar.
   QMenuBar* pMenuBar = menuBar();
   Q_ASSERT( NULLPTR != pMenuBar );
   if ( pMenuBar )
   {
      pMenuBar->addMenu( pFileMenu );
      pMenuBar->addMenu( pEditMenu );
      pMenuBar->addMenu( pOptionsMenu );
      pMenuBar->addMenu( pExtrasMenu );
      pMenuBar->addMenu( pAboutMenu );
   }
}


// -------------------------------------------------------------------------------
void CMainWindow::settingUpToolBar( void )
// -------------------------------------------------------------------------------
{
   // main toolbar
   mpMainToolBar = addToolBar( "File Actions" );
   Q_ASSERT( NULLPTR != mpMainToolBar );
   if ( mpMainToolBar )
   {                                                      // Tool tips
      mpMainToolBar->addAction( mpFileNewAction );        // "Clear whole Tree"
      mpMainToolBar->addAction( mpFileOpenAction );       // "Open a new File"
      mpMainToolBar->addAction( mpFileSaveAction );       // "Save Data to File (Ctrl+S)"
      //mpMainToolBar->addAction( mpFileSaveAsAction );
      mpMainToolBar->addAction( mpFilePrintAction );      // Print current Entry"

      mpMainToolBar->addSeparator();
      mpMainToolBar->addAction( mpSearchEntryAction );    // Search an entry
   }

   // editor toolbar
   mpEditorToolBar = addToolBar( "Editor Actions" );
   Q_ASSERT( NULLPTR != mpEditorToolBar );
   if ( mpEditorToolBar )
   {
      mpEditorToolBar->addAction( mpEditUndoAction );
      mpEditorToolBar->addAction( mpEditRedoAction );
      mpEditorToolBar->addAction( mpEditCutAction );
      mpEditorToolBar->addAction( mpEditCopyAction );
      mpEditorToolBar->addAction( mpEditPasteAction );
   }

   // entry toolbar
   mpEntryToolBar = addToolBar( "Entry Actions" );
   Q_ASSERT( NULLPTR != mpEntryToolBar );
   Q_ASSERT( NULLPTR != mpTree );
   if ( mpEntryToolBar && mpTree )
   {
      mpTree->addActionsToToolBar( *mpEntryToolBar );
   }


//
//  // ----------- add toolbar for recent file list ------------------
//  #ifdef FEATURE_RECENTFILELIST_TOOLBAR
//  mpRecentFilesTools = new Q3ToolBar(this);
//  if ( mpRecentFiles && mpRecentFilesTools )
//  {
//     mpRecentFiles->createComboBox( *mpRecentFilesTools );
//  }
//  #endif



   slotSetMainToolBarVisible(   mConfiguration.getBoolValue( CTuxCardsConfiguration::B_SHOW_MAIN_TOOLBAR )   );
   slotSetEntryToolBarVisible(  mConfiguration.getBoolValue( CTuxCardsConfiguration::B_SHOW_ENTRY_TOOLBAR )  );
   slotSetEditorToolBarVisible( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_SHOW_EDITOR_TOOLBAR ) );
// #ifdef FEATURE_RECENTFILELIST_TOOLBAR
// setRecentFilesToolbarVisible( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_SHOW_RECENTFILES_TOOLBAR ) );
// #endif



//#ifdef DEBUGGING
//  // --- add debug mpMainTools"
//  std::cout<<"!!! still having debug turned on"<<std::endl;
//
//  Q3ToolBar* debugTools=new Q3ToolBar(this);
//  QPixmap debugShowRTFSource=QPixmap(showText_xpm);
//  (void) new QToolButton(debugShowRTFSource, "Debug: Shows the RTF-TextSource", 0,
//                         this, SLOT(debugShowRTFTextSource()), debugTools);
//
//  QPixmap debugShowXMLCode=QPixmap(xml_xpm);
//  (void) new QToolButton(debugShowXMLCode, "Debug: Shows the XML-Representation", 0,
//                         this, SLOT(debugShowXMLCode()), debugTools);
//#endif
}


// -------------------------------------------------------------------------------
void CMainWindow::settingUpComboBoxes()
// -------------------------------------------------------------------------------
{
   addToolBarBreak( Qt::TopToolBarArea );

   // create a separat toolbar for the comboboxes
   mpEditorToolBar2 = addToolBar( "Font_Paragraph Combos" );
   Q_ASSERT( NULLPTR != mpEditorToolBar2 );
   if ( mpEditorToolBar2 )
   {
      // paragraph list
      QComboBox* pComboListStyle = new QComboBox( mpEditorToolBar2 );
      pComboListStyle->addItem( tr("Standard") );
      pComboListStyle->addItem( tr("Bullet List (Disc)") );
      pComboListStyle->addItem( tr("Bullet List (Circle)") );
      pComboListStyle->addItem( tr("Bullet List (Square)") );
      pComboListStyle->addItem( tr("Ordered List (Decimal)") );
      pComboListStyle->addItem( tr("Ordered List (Alpha lower)") );
      pComboListStyle->addItem( tr("Ordered List (Alpha upper)") );
      connect( pComboListStyle, SIGNAL( activated(int) ), this, SLOT( setEditListStyle(int) ) );


      // font family list
      // -  old font family selector (no preview of font in comboBox) -
      //mpEditFontCombo = new QComboBox( pToolBar );
      //mpEditFontCombo->setEditable( true );
      QFontDatabase db;
      //mpEditFontCombo->addItems( db.families() );
      //connect( mpEditFontCombo, SIGNAL( activated(const QString&) ), this, SLOT( setEditFontFamily(const QString&) ) );
      //mpEditFontCombo->setCurrentIndex( mpEditFontCombo->findText( QApplication::font().family() ) );

      // -  new font family selector (with preview of font in comboBox) -
      mpEditFontCombo = new QFontComboBox( mpEditorToolBar2 );
      mpEditFontCombo->setEditable( true );
      connect( mpEditFontCombo, SIGNAL( activated(const QString&) ), this, SLOT( setEditFontFamily(const QString&) ) );
      mpEditFontCombo->setCurrentIndex( mpEditFontCombo->findText( QApplication::font().family() ) );


      // font size  list
      mpEditFontSizeCombo = new QComboBox( mpEditorToolBar2 );
      mpEditFontSizeCombo->setEditable( true );
      foreach( int iSize, db.standardSizes() )
         mpEditFontSizeCombo->addItem( QString::number(iSize) );
      connect( mpEditFontSizeCombo, SIGNAL( activated(const QString&) ), this, SLOT( setEditFontSize(const QString&) ) );
      mpEditFontSizeCombo->setCurrentIndex( mpEditFontSizeCombo->findText( QString::number(QApplication::font().pointSize()) ) );


      // add the comboboxes to the toolbar
      mpEditorToolBar2->addWidget( pComboListStyle );
      mpEditorToolBar2->addWidget( mpEditFontCombo );
      mpEditorToolBar2->addWidget( mpEditFontSizeCombo );


      // add 2nd part of editor actions here
      mpEditorToolBar2->addSeparator();
      mpEditorToolBar2->addAction( mpEditBoldAction );
      mpEditorToolBar2->addAction( mpEditItalicAction );
      mpEditorToolBar2->addAction( mpEditUnderAction );
      mpEditorToolBar2->addAction( mpEditColorAction );

      mpEditorToolBar2->addSeparator();
      mpEditorToolBar2->addAction( mpEditTextLeftAction );
      mpEditorToolBar2->addAction( mpEditTextCenterAction );
      mpEditorToolBar2->addAction( mpEditTextRightAction );
      mpEditorToolBar2->addAction( mpEditTextBlockAction );
   }
}


//// -------------------------------------------------------------------------------
//void CMainWindow::settingUpQuickLoader( void )
//// -------------------------------------------------------------------------------
//{
//   mpQuickLoader = new QToolBar(this);
//   checkPointer( mpQuickLoader );
//}
//
//
///**
// * Adds the currently active element to the bookmark list.
// */
//// -------------------------------------------------------------------------------
//void CMainWindow::addElementToBookmarksEvent( void )
//// -------------------------------------------------------------------------------
//{
//   if ( NULLPTR == mpCollection )
//      return;
//
//   CInformationElement* pElement = mpCollection->getActiveElement();
//   if ( NULLPTR == pElement )
//      return;
//
//   BookmarkButton* b = new BookmarkButton( QPixmap( pElement->getIconFileName() ),
//                                           pElement->getDescription(), mpQuickLoader,
//                                           Path( pElement ) );
//   connect( b, SIGNAL(activatedSignal(Path*)), this, SLOT(quicklyLoad(Path*)) );
//}
//
//
//// -------------------------------------------------------------------------------
//void CMainWindow::quicklyLoad(Path* path)
//// -------------------------------------------------------------------------------
//{
//   std::cout<<"load quickly "<<path->toString().toStdString()<<std::endl;
////  save();
////  open();
//}
//
//
//// -------------------------------------------------------------------------------
//void CMainWindow::changeInformationFormat()
//// -------------------------------------------------------------------------------
//{
///*
//   if ( (NULLPTR == mpCollection) || (NULLPTR == mpEditor) )
//      return;
//
//   CInformationElement* pActiveElement = mpCollection->getActiveElement();
//   if ( NULLPTR == pActiveElement )
//   {
//      QMessageBox::information( 0, "Converter", "There is no active entry.",
//                                QMessageBox::Abort );
//      return;
//   }
//
//   if ( pActiveElement->getInformationFormat() == &InformationFormat::RTF)
//   {
//      QMessageBox::information( 0, "Converter", "Sorry, but converting RTF to ASCII "
//                                "is not implemented yet.",
//                                QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
//      return;
//   }
//
//   if (QMessageBox::Cancel == QMessageBox::warning(this, "Converting Information Format",
//                                                   "Are you sure to change the information "
//                                                   "format.\nSome of the text layout will be lost.",
//                                                   QMessageBox::Yes,
//                                                   QMessageBox::Cancel,
//                                                   QMessageBox::NoButton))
//   {
//      return;
//   }
//
//
//   // converting
//   mpEditor->writeCurrentTextToActiveInformationElement();
//   Converter::convert( *pActiveElement );
//
//   mpEditor->setTextFormat( Qt::RichText );
//   mpEditor->setText( pActiveElement->getInformation() );
//
//   mpCollection->setActiveElement( pActiveElement );
//*/
//}



// -------------------------------------------------------------------------------
void CMainWindow::showRecognizedFormat( InformationFormat format )
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpStatusBar_TextFormatLabel );
   if ( !mpStatusBar_TextFormatLabel )
      return;

   mpStatusBar_TextFormatLabel->setText( format.toString() );

   // enabeling rtf-formatting stuff for rtf-information-items only
   bool b = format.equals( InformationFormat::getByString("RTF") );
   mpEditBoldAction->setEnabled(b);
   mpEditItalicAction->setEnabled(b);
   mpEditUnderAction->setEnabled(b);
   mpEditColorAction->setEnabled(b);

   mpEditTextLeftAction->setEnabled(b);
   mpEditTextCenterAction->setEnabled(b);
   mpEditTextRightAction->setEnabled(b);
   mpEditTextBlockAction->setEnabled(b);
}



/**************************** Editor methods start ******************************/
/**
 * Whenever the clipboard content changes, the action "paste" is enabled or
 * disabled accordingly.
 */
// -------------------------------------------------------------------------------
void CMainWindow::clipboardDataChanged()
// -------------------------------------------------------------------------------
{
   if ( !mpEditPasteAction )
      return;

    mpEditPasteAction->setEnabled( !QApplication::clipboard()->text().isEmpty() );
}


// -------------------------------------------------------------------------------
void CMainWindow::setEditListStyle( int iStyleIndex )
// -------------------------------------------------------------------------------
{
   if ( !mpEditor )
      return;


   QTextCursor cursor = mpEditor->textCursor();

   if ( 0 != iStyleIndex )
   {
      QTextListFormat::Style style = QTextListFormat::ListDisc;
      switch (iStyleIndex)
      {
         default:
         case 1:
            style = QTextListFormat::ListDisc;
            break;
         case 2:
            style = QTextListFormat::ListCircle;
            break;
         case 3:
            style = QTextListFormat::ListSquare;
            break;
         case 4:
            style = QTextListFormat::ListDecimal;
            break;
         case 5:
            style = QTextListFormat::ListLowerAlpha;
            break;
         case 6:
            style = QTextListFormat::ListUpperAlpha;
            break;
      }

      cursor.beginEditBlock();          // ******** edit start *************

      QTextBlockFormat blockFmt = cursor.blockFormat();   
      QTextListFormat listFmt;

      if ( cursor.currentList() )
      {
         listFmt = cursor.currentList()->format();
      }
      else
      {
         listFmt.setIndent( blockFmt.indent() + 1 );
         blockFmt.setIndent( 0 );
         cursor.setBlockFormat( blockFmt );
      }

      listFmt.setStyle( style );   
      cursor.createList( listFmt );

      cursor.endEditBlock();            // ******** edit end ***************
   }
   else
   {
      // ####
      QTextBlockFormat bfmt;
      bfmt.setObjectIndex( -1 );
      cursor.mergeBlockFormat( bfmt );
   }
}


// -------------------------------------------------------------------------------
void CMainWindow::setEditFontFamily( const QString& f )
// -------------------------------------------------------------------------------
{
   if ( !mpEditor )
      return;

   mpEditor->setFontFamily( f );
   mpEditor->setFocus( Qt::OtherFocusReason );
}

// -------------------------------------------------------------------------------
void CMainWindow::setEditFontSize( const QString& s )
// -------------------------------------------------------------------------------
{
   if ( !mpEditor )
      return;

   mpEditor->setFontPointSize( s.toInt() );
   mpEditor->setFocus( Qt::OtherFocusReason );
}


/**
* Whenever the font of the currently edited text within the editor
* is changed -> the toolbuttons are adjusted.
*/
// -------------------------------------------------------------------------------
void CMainWindow::textFontChanged( const QTextCharFormat& f )
// -------------------------------------------------------------------------------
{
   if ( !mpEditFontCombo || !mpEditFontSizeCombo || !mpEditBoldAction ||
        !mpEditItalicAction || !mpEditUnderAction
      )
      return;

   mpEditFontCombo->lineEdit()->setText( f.fontFamily() );
   mpEditFontSizeCombo->lineEdit()->setText( QString::number( f.fontPointSize() ) );
   mpEditBoldAction->setChecked( QFont::Bold == f.fontWeight() );
   mpEditItalicAction->setChecked( f.fontItalic() );
   mpEditUnderAction->setChecked( f.fontUnderline() );

   textColorChanged( f.foreground().color() );
}

// -------------------------------------------------------------------------------
void CMainWindow::textColorChanged( const QColor& c )
// -------------------------------------------------------------------------------
{
   QPixmap pix( getIcon("text_color").pixmap(24,24) );

   QPainter p;
   p.begin(&pix);
   p.fillRect(1,13, 16,4, QColor(c));
   p.end();

   mpEditColorAction->setIcon( pix );
}



// -------------------------------------------------------------------------------
void CMainWindow::textAlignmentChanged( Qt::Alignment a )
// -------------------------------------------------------------------------------
{
   mpEditTextLeftAction->setChecked( FALSE );
   mpEditTextCenterAction->setChecked( FALSE );
   mpEditTextRightAction->setChecked( FALSE );
   mpEditTextBlockAction->setChecked( FALSE );

   //std::cout<<"CMainWindow::textAlignmentChanged() to "<<a<<std::endl;

   switch (a)
   {
   case Qt::AlignHCenter:
      mpEditTextCenterAction->setChecked( TRUE );
      //std::cout<<"center"<<std::endl;
      break;
   case Qt::AlignRight:
      mpEditTextRightAction->setChecked( TRUE );
      //std::cout<<"right"<<std::endl;
      break;
   case Qt::AlignJustify:
      mpEditTextBlockAction->setChecked( TRUE );
      //std::cout<<"just"<<std::endl;
      break;
   case Qt::AlignLeft:
   default:
      mpEditTextLeftAction->setChecked( TRUE );
      //std::cout<<"left"<<std::endl;
      break;
   }

}

// -------------------------------------------------------------------------------
void CMainWindow::setEditTextColor()
// -------------------------------------------------------------------------------
{
   if ( !mpEditor )
      return;

   QColor c = QColorDialog::getColor( mpEditor->textColor(), this );
   if ( !c.isValid() )
      return;

   mpEditor->setTextColor( c );
   textColorChanged( c );
}


// -------------------------------------------------------------------------------
void CMainWindow::textLeft()
// -------------------------------------------------------------------------------
{
   if ( ! mpEditor )
      return;

   textAlignmentChanged( Qt::AlignLeft );
   mpEditor->setAlignment( Qt::AlignLeft );
}
// -------------------------------------------------------------------------------
void CMainWindow::textHCenter()
// -------------------------------------------------------------------------------
{
   if ( !mpEditor )
      return;

   textAlignmentChanged( Qt::AlignHCenter );
   mpEditor->setAlignment( Qt::AlignHCenter );
}
// -------------------------------------------------------------------------------
void CMainWindow::textRight()
// -------------------------------------------------------------------------------
{
   if ( !mpEditor )
      return;

   textAlignmentChanged( Qt::AlignRight );
   mpEditor->setAlignment( Qt::AlignRight );
}
// -------------------------------------------------------------------------------
void CMainWindow::textBlock()
// -------------------------------------------------------------------------------
{
   if ( !mpEditor )
      return;

   textAlignmentChanged( Qt::AlignJustify );
   mpEditor->setAlignment( Qt::AlignJustify );
}
/*************************** Editor methods end *********************************/



/*************************** File methods start *********************************/
/**
 * Checks whether tuxcards runs for the first time with this version
 * if yes -> write new features in file
 */
// // -------------------------------------------------------------------------------
// void CMainWindow::checkFirstTime()
// // -------------------------------------------------------------------------------
// {
//    QString configurationFileName = QDir::homePath() + "/.tuxcards";
// 
//    ConfigParser parser( configurationFileName, FALSE );
//    parser.setGroup("General");
//    QString version   = parser.readEntry("Version",   "previousVersion");
//    // TODO: Check whether this version is correct and does work
//    if( version != "TuxCardsV1.0" )
//    {
//       // write datafile
//       QFile file( QDir::homePath() + "/" + TUX_GREETING_FILE );
//       QTextStream* pLog = NULLPTR;
// 
//       if( !file.open(QIODevice::WriteOnly) )
//       {
//       std::cerr<<"TuxCards - cannot write to "<<configurationFileName.toStdString()<<"\n";
//       }
//       else
//       {
//       pLog = new QTextStream(&file);
// 
//       QTextCodec* pCodec = QTextCodec::codecForName( "UTF-8" );
//       if ( pLog && pCodec )
//          pLog->setCodec( pCodec );
//       }
// 
//       *pLog<<sGreetingsText;
// 
//       file.close();
//       DELETE( pLog );
//    }
// }


// -------------------------------------------------------------------------------
void CMainWindow::timerEvent( QTimerEvent* )
// -------------------------------------------------------------------------------
{
   if ( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_AUTOSAVE ) )
      save();
}

// -------------------------------------------------------------------------------
void CMainWindow::deleteCollectionAndCreateADefaultOne( bool bCreateDefaultCollection )
// -------------------------------------------------------------------------------
{
   if ( mpTree )
      mpTree->clearCollection();

   DELETE( mpCollection );

   if ( bCreateDefaultCollection )
   {
      mpCollection = CInformationCollection::createDefaultCollection();
      initializingCollection("");
   }
}

// -------------------------------------------------------------------------------
void CMainWindow::clearAll()
// -------------------------------------------------------------------------------
{
   if ( QMessageBox::Cancel == askForSaving( tr("Do you want to save before deleting "
                                                "all entries and creating a new file?") ) )
      return;

   deleteCollectionAndCreateADefaultOne( true );
}


// -------------------------------------------------------------------------------
QMessageBox::Button CMainWindow::askForSaving( const QString& sQuestion )
// -------------------------------------------------------------------------------
{
   QMessageBox::Button result = QMessageBox::No;
   if ( mbChangesExist )
   {
      result = (QMessageBox::Button) QMessageBox::warning( this, tr("Saving"), sQuestion,
                                                           QMessageBox::Yes,
                                                           QMessageBox::No,
                                                           QMessageBox::Cancel );
      if ( QMessageBox::Yes == result )
         save();
   }

   return result;
}


/**
 * Checks whether the config file (formerly ".tuxcards" file within the home
 * directory) exists. If yes, then the last saved file is loaded and the tree
 * is initialized with first data.
 *
 * Please note: If a file is give via parameter <code>sArg</code> it is treaded
 *              with higher priority and loaded (instead of a file from the
 *              configuration).
 */
// -------------------------------------------------------------------------------
void CMainWindow::loadLastSavedDataIfConfigFileIsFound( const QString& sArg )
// -------------------------------------------------------------------------------
{
   bool bResult = FALSE;

   if ( sArg != "" )
   {
      bResult = open( sArg );
   }
   else if ( mConfiguration.getStringValue( CTuxCardsConfiguration::S_DATA_FILE_NAME ) != "" )
   {
      bResult = open( mConfiguration.getStringValue( CTuxCardsConfiguration::S_DATA_FILE_NAME ) );
   }

   if ( !bResult )
   {
      deleteCollectionAndCreateADefaultOne( true );

      if ( mpCollection )
         mpCollection->setActiveElement( mpCollection->getRootElement() );
   }

   clearChanges();
}


/**
 * Before calling this method the informationCollection 'mpCollection'
 * must be valid !
 */
// -------------------------------------------------------------------------------
void CMainWindow::initializingCollection( const QString& sDataFileName )
// -------------------------------------------------------------------------------
{
   Q_ASSERT( NULLPTR != mpCollection );
   Q_ASSERT( NULLPTR != mpSingleEntryView );
   Q_ASSERT( NULLPTR != mpTree );
   Q_ASSERT( NULLPTR != mpEditor );
   if ( !mpCollection || !mpSingleEntryView || !mpTree || !mpEditor )
      return;

//   //std::cout<<"vorher"<<std::endl;
//   ExpiredElementsDialog expiredElementsDialog( this );
//   expiredElementsDialog.setUp( mpCollection);
//   //std::cout<<"expiredElementsDialog.setUp called"<<std::endl;
//   // expired elements are removed by now
//
//   checkPointer( mpTree );
//
   mpCollection->setAbsFileName( sDataFileName );
   setWindowTitle();

   connect( mpCollection,      SIGNAL(activeInformationElementChanged(CInformationElement*)),
            mpSingleEntryView, SLOT(activeInformationElementChanged(CInformationElement*)) );

//   connect( mpCollection,      SIGNAL(modelHasChanged()), this, SLOT(recognizeChanges()) );
//
   mpEditor->clear();

   // These two methods must be called after the connect of 'mpCollection' with
   // the signal 'activeInformationElementChanged()'. Otherwise: The editor will not
   // be updated with the last active element.
   mpTree->setCollection( *mpCollection );
   setLastActiveElement();

   // Collection successfully created and system set up with it.

   clearChanges();


   mpCollection->addView( mpSingleEntryView );

   mConfiguration.setStringValue( CTuxCardsConfiguration::S_DATA_FILE_NAME, sDataFileName );
   mConfiguration.saveToFile();                    // because the dataFileName has changed

}


/**
 * Adapter for <code>open( const QString& sFileName )</code>.
 */
// -------------------------------------------------------------------------------
void CMainWindow::open()
// -------------------------------------------------------------------------------
{
   // give a chance to save the file before opening another one
   if ( QMessageBox::Cancel == askForSaving( tr("Do you want to save before opening another data file?") ) )
      return;

   // getting dataFileName
   QString sFileName( QFileDialog::getOpenFileName() );
   if ( sFileName.isNull() || "" == sFileName )
      showMessage( tr("No Filename specified."), 5 );
   else
      open( sFileName );
}



/**
 * Returns TRUE, if file was opend successfully; otherwise FALSE.
 */
// -------------------------------------------------------------------------------
bool CMainWindow::open( const QString& sFileName )
// -------------------------------------------------------------------------------
{
   bool bRetVal = FALSE;

//  int format = getDataFileFormat(fileName);
//
//  if ( format == 2 )
   bRetVal = openXMLDataFile( sFileName );

//   else if ( format == 1 )
//      retVal = openOldDataFile(fileName);
//   else
//   {
//      QMessageBox::critical(    this, "Opening a data file",
//            "Could not open file '"+fileName+"'<br> or did not "
//            "recognize the dataformat.",
//            "Ok");
//   }

   if ( bRetVal )
   {
      if ( mpRecentFiles )
      {
         mpRecentFiles->setOnTop( sFileName );
      }
   }

   return bRetVal;
}


/**
 * Opens a file given by a valid name ('fileName'), creates an
 * informationcollection from it & sets latter one to be presented
 * within tuxcards.
 *
 * Returns TRUE, if file was opend successfully; otherwise FALSE.
 */
// -------------------------------------------------------------------------------
bool CMainWindow::openXMLDataFile( const QString& sFileName )
// -------------------------------------------------------------------------------
{
   // create absolute file name, in case a relative one is given
   QString sAbsoluteFileName = QFileInfo( sFileName ).absoluteFilePath();


   deleteCollectionAndCreateADefaultOne( false );

   QFile file( sAbsoluteFileName );
   mpCollection = XMLPersister::createInformationCollection( file );

   if ( !mpCollection )
   {
      QMessageBox::critical(this, tr("TuxCards - XML I/O"),
                           tr("ERROR - Could not open") + " '"+ sAbsoluteFileName
                           + "' " + tr("for reading or parse error."),
                           QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
      return FALSE;
   }

   initializingCollection( sAbsoluteFileName );

   return TRUE;
}

// -------------------------------------------------------------------------------
void CMainWindow::slotSaveAndLoadNewFile( const QString& sNewFile )
// -------------------------------------------------------------------------------
{
   save();
   open( sNewFile );
}


// -------------------------------------------------------------------------------
void CMainWindow::save()
// -------------------------------------------------------------------------------
{
   if ( "" == mConfiguration.getStringValue( CTuxCardsConfiguration::S_DATA_FILE_NAME ) )
   {
      saveAs();
   }
   else
   {
      save( mConfiguration.getStringValue( CTuxCardsConfiguration::S_DATA_FILE_NAME ) );
   }
}


// -------------------------------------------------------------------------------
void CMainWindow::saveAs()
// -------------------------------------------------------------------------------
{
   QString sNewFileName( QFileDialog::getSaveFileName() );
   if ( sNewFileName.isNull() || ("" == sNewFileName) )
   {
      showMessage( tr("No Filename specified."), 5);
      return;
   }

   save( sNewFileName );

   // At the currently saved file to the recent file list; this is necessary in
   // case a new file was created (a new file was not opened).
   if ( mpRecentFiles )
      mpRecentFiles->setOnTop( sNewFileName );


   mConfiguration.setStringValue( CTuxCardsConfiguration::S_DATA_FILE_NAME, sNewFileName );
   mConfiguration.saveToFile();
}


// -------------------------------------------------------------------------------
void CMainWindow::save( const QString& sFileName )
// -------------------------------------------------------------------------------
{
   if ( !mpCollection || !mpEditor )
      return;

   if ( (QDir::homePath() + TUX_CONFIG_FILE) == sFileName )
   {
      QMessageBox::warning( this, tr("Saving"), tr("File not saved.\n"
                           "Please do not use") + " \"" + sFileName + "\" " +
                           tr("as file name."),
                           QMessageBox::Abort, NULL );
      return;
   }

   if ( !mbChangesExist )
   {
      showMessage( tr("No changes."), 5);
      return;
   }

   // before saving -> move current file i.e. "myfile.data" to "myfile.data~"
   if ( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_CREATE_BACKUP_FILE ) )
   {
      QDir tmp;
      QString sFileName2 = mConfiguration.getStringValue( CTuxCardsConfiguration::S_DATA_FILE_NAME );
      tmp.rename( sFileName2, sFileName2 + "~" );
   }

   // saving eventual changes
   mpEditor->writeCurrentTextToActiveInformationElement();
   XMLPersister::save( *mpCollection, sFileName );


   mpCollection->setAbsFileName( sFileName );

   mConfiguration.setStringValue( CTuxCardsConfiguration::S_DATA_FILE_NAME, sFileName );
   mConfiguration.saveToFile();

   clearChanges();
   showMessage( tr("Saved to") + " '" + sFileName + "'.", 5);

   callingExecutionStatement();
}

// -------------------------------------------------------------------------------
void CMainWindow::exit()
// -------------------------------------------------------------------------------
{
   close();                      // calls 'closeEvent(..)' indirectly
}


/**
 * Saves the data automatically by closing/quitting the program.
 */
// -------------------------------------------------------------------------------
void CMainWindow::closeEvent( QCloseEvent* pEv )
// -------------------------------------------------------------------------------
{
   if ( !pEv )
   {
      return;
   }

   // accept signal
   pEv->accept();                  // default implementation of this method


   setWindowGeometryToConfiguration();
   if ( mpRecentFiles )
      mConfiguration.setStringValue( CTuxCardsConfiguration::S_RECENT_FILES, mpRecentFiles->toString() );
   mConfiguration.saveToFile();


   if ( !mbChangesExist )
      return;

   if ( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_SAVE_WHEN_LEAVING )
      || (QMessageBox::warning(  this, tr("Save before exiting."),
                                 tr("Do you want to save before leaving TuxCards?"),
                                 QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes) )
   {
      mSaveChangesDialog.show();
      save();
      mSaveChangesDialog.hide();
   }
}

///*
//// -------------------------------------------------------------------------------
//void CMainWindow::saveActiveEntry()
//// -------------------------------------------------------------------------------
//{
//  // saving eventual changes
//  editor->writeCurrentTextToActiveInformationElement();
//  QString description=mpCollection->getActiveElement()->getDescription();
//  QString fileName=QDir::homeDirPath()+description+".xml";
//  XMLPersister::save(mpCollection, fileName);
//
//  QPushButton* b= new QPushButton(QPixmap(mpCollection->getActiveElement()->getIconFileName()),
//                         description, mpQuickLoader);
//  connect( b, SIGNAL(clicked(b->text())), this, SLOT(quicklyLoad(QString)) );
//
////  mConfiguration.setStringValue( CTuxCardsConfiguration::S_DATA_FILE_NAME, fileName );
////  mConfiguration.saveToFile();
//
//  showMessage("Entry saved and added to QuickLoader.", 5);
//}*/
//
//
///**
// * Opens a file and detects the fileformat.
// *     i.e. '2' == XML-File (the new TuxCards format)
// *          '1' == Old-File (the old TuxCards format, that was used since version 0.5)
// *          '0' == unknown format
// */
//// -------------------------------------------------------------------------------
//int CMainWindow::getDataFileFormat(QString fileName)
//// -------------------------------------------------------------------------------
//{
//  QFile file(fileName);
//  if ( !file.open(QIODevice::ReadOnly) ) {
//    //showMessage("ERROR could not open '"+fileName+"' for reading.", 5);
//    return 0;
//  }
//
//
//  QTextStream t( &file );
//  QString line=t.readLine();
//  //cout<<"readLine="<<line<<endl;
//  file.close();
//
//  if (line.startsWith("TuxCardsV0.5"))
//    return 1;
//  else if (line.startsWith("<?xml"))
//    return 2;
//
//  return 0;
//}
//
//
//
///**
// * Opens a file given by a valid name ('fileName'), creates an
// * informationcollection from it & sets latter one to be presented
// * within tuxcards.
// *
// * Returns TRUE, if file was opend successfully; otherwise FALSE.
// */
//// -------------------------------------------------------------------------------
//bool CMainWindow::openOldDataFile(QString fileName)
//// -------------------------------------------------------------------------------
//{
//  QFile file(fileName);
//  if (! file.open(QIODevice::ReadOnly) ) {
//    showMessage("ERROR could not open '"+fileName+"' for reading.", 5);
//    return FALSE;
//  }
//
//  // create absolute file name, in case a relative one is given
//  fileName = QFileInfo(fileName).absoluteFilePath();
//
//
//  QString s="";
//  QTextStream t( &file );               // use a text stream
//  while ( !t.atEnd() ) {
//    s += QChar((char)10) + t.readLine();// the first chr(10) is wrong, but doesn't matter
//  }
//  file.close();
//  s=s.mid(1);                           // remove wrong chr(10) from beginning
//
//  DELETE( mpCollection);
//  mpCollection = Persister::createInformationCollection( s );
//  initializingCollection( fileName );
//
//  return TRUE;
//}
//
//
//
/*************************** File methods end ***********************************/


/********************* Auxiliary methods start **********************************/
/**
 * Sets the window geometry parameters to the configuration.
 * Please note: The configuration is only changed but not saved.
 */
// -------------------------------------------------------------------------------
void CMainWindow::setWindowGeometryToConfiguration()
// -------------------------------------------------------------------------------
{
   if ( mpTree )
      mConfiguration.setIntValue( CTuxCardsConfiguration::I_TREE_VSCROLLBAR_VALUE, mpTree->verticalScrollBar()->value() );

   mConfiguration.setIntValue( CTuxCardsConfiguration::I_WINDOW_XPOS,   pos().x() );
   mConfiguration.setIntValue( CTuxCardsConfiguration::I_WINDOW_YPOS,   pos().y() );
   mConfiguration.setIntValue( CTuxCardsConfiguration::I_WINDOW_WIDTH,  width() );
   mConfiguration.setIntValue( CTuxCardsConfiguration::I_WINDOW_HEIGHT, height() );

   if ( mpSplit )
   {
      mConfiguration.setIntValue( CTuxCardsConfiguration::I_TREE_WIDTH,  mpSplit->sizes().first() );
      mConfiguration.setIntValue( CTuxCardsConfiguration::I_EDITOR_WIDTH, mpSplit->sizes().last() );
   }

   mConfiguration.saveToFile();
}

///**
// * sets the text within the editor 'mpEditor' to 'text'
// */
///*void CMainWindow::updateText(QString text){
//    mpEditor->setText(text);
//}
//*/

//// -------------------------------------------------------------------------------
//CInformationElement* CMainWindow::getActiveIE()
//// -------------------------------------------------------------------------------
//{
//   if ( NULLPTR == mpCollection )
//      return NULLPTR;
//
//   return mpCollection->getActiveElement();
//}

/**
 * Reads the last active element from the configuration file and sets it into
 * information collection.
 */
// -------------------------------------------------------------------------------
void CMainWindow::setLastActiveElement()
// -------------------------------------------------------------------------------
{
   if ( !mpCollection )
      return;


   Path path( XMLPersister::getPathOfLastActiveElement() );

   if ( mpCollection->isPathValid(path) )
   {
      //std::cout<<"Path is valid"<<std::endl;
      mpCollection->setActiveElement( path );
   } else {
      //std::cout<<"Path is invalid"<<std::endl;
      mpCollection->setActiveElement( mpCollection->getRootElement() );
   }
}

/**
 * Shows the given messages via the statusbar vor <code>iSeconds</code> seconds.
 */
// -------------------------------------------------------------------------------
void CMainWindow::showMessage( const QString& sMessage, int iSeconds )
// -------------------------------------------------------------------------------
{
   QStatusBar* pStatusBar = statusBar();
   Q_ASSERT( NULLPTR != pStatusBar );
   if ( !pStatusBar )
      return;

   pStatusBar->showMessage( sMessage, iSeconds*1000 );
}



/**
 * To keep track of changes, put a marker in the statusbar,
 * and update the member <code>mbChangesExist</code> to prevent unnecessary savings.
 */
// -------------------------------------------------------------------------------
void CMainWindow::recognizeChanges()
// -------------------------------------------------------------------------------
{
   mbChangesExist = TRUE;

   setWindowModified( TRUE );  // With this the members 'mpStatusBar_ChangeLabel'
                               // and 'mbChangesExist' can be removed.

   if ( mpStatusBar_ChangeLabel )
      mpStatusBar_ChangeLabel->setText("*");
}

/**
 * To keep track of changes, remove the marker from the statusbar,
 * and update the member <code>mbChangesExist</code> to prevent unnecessary savings.
 */
// -------------------------------------------------------------------------------
void CMainWindow::clearChanges()
// -------------------------------------------------------------------------------
{
   mbChangesExist = FALSE;

   setWindowModified( FALSE );

   if ( mpStatusBar_ChangeLabel )
      mpStatusBar_ChangeLabel->setText(" ");
}
/********************* Auxiliary methods end ************************************/

// -------------------------------------------------------------------------------
void CMainWindow::callingExecutionStatement() const
// -------------------------------------------------------------------------------
{
   // If an empty execution statement is called under the windows os, then a
   // short flashing black window appears.
   // This is irritating; there fore we first check, whether something must be
   // executed.
   if ( mConfiguration.getStringValue( CTuxCardsConfiguration::S_EXECUTE_STATEMENT ).isEmpty() )
      return;

   system( mConfiguration.getStringValue( CTuxCardsConfiguration::S_EXECUTE_STATEMENT ).toAscii() );
}



/**
 * Exports the current collection to HTML.
 */
// -------------------------------------------------------------------------------
void CMainWindow::exportHTML()
// -------------------------------------------------------------------------------
{
  if ( !mpCollection )
     return;

  QString sDirPath = QFileDialog::getExistingDirectory(
                              this,
                              tr("Choose a directory"),
                              QDir::homePath(),
                              QFileDialog::ShowDirsOnly );

  if ( sDirPath.isEmpty() )
     return;

  bool bSuccess = HTMLWriter::writeCollectionToHTMLFile( *mpCollection, sDirPath );

  // done
  if ( FALSE != bSuccess )
  {
     QMessageBox::information( this, tr("HTML-Export"), tr("HTML") + "<em>" + tr("Export") +
                               "</em> <font size=-1>(" + QString(TUX_SHORT_VERSION) + ")</font> " +
                               tr("finished, successfully.") + "<p>" +
                               tr("The data are stored in") + "<br>"+sDirPath, QMessageBox::Ok );
  }
  else
  {
     QMessageBox::warning( this, tr("HTML-Export"), tr("HTML") + "<em>" + tr("Export") +
                           "</em> <font size=-1>(" + QString(TUX_SHORT_VERSION) + ")</font>"
                           " <b>" + tr("not successfully") + "</b>.<p>" +
                           tr("Please check write permission and disk space") + "<br>"
                           "(" + sDirPath + ").", QMessageBox::Abort );
  }
}


//// -------------------------------------------------------------------------------
//void CMainWindow::keyPressEvent(QKeyEvent* k)
//// -------------------------------------------------------------------------------
//{
//   if ( !k || !mpTree || !mpSingleEntryView )
//      return;
//
//   switch( k->modifiers() )
//   {
//   default:
//      if ( Qt::Key_F5 == k->key() )
//      {
//         if ( mpEditor->hasFocus() )
//            mpTree->setFocus();
//         else
//            mpEditor->setFocus();
//      }
//      else
//         k->ignore();
//      break;
//  }
//}




// -------------------------------------------------------------------------------
void CMainWindow::wordCount( void )
// -------------------------------------------------------------------------------
{
   if ( !mpEditor )
      return;

   int iWords  = Strings::wordCount( mpEditor->toPlainText() );
//   int iLines  = Strings::lineCount( mpEditor->toPlainText() ); //-1; //mpEditor->lines();
//   int parags = -1; //mpEditor->paragraphs();

   QMessageBox::information( this, tr("Word Count"),
                     "<center>" + tr("Current Text contains") + "<br>"
                     +QString::number(iWords)
                     +( ((iWords > 1) || (0 == iWords)) ? tr(" words.") + "<br>" : tr(" word.") + "<br>" )
//                     +QString::number(iLines)
//                     +( ((iLines  > 1) || (0 == iLines)) ? " lines<br>" : " line<br>" )
//                     +QString::number(parags) + (parags > 1 ? " paragraphs" : "paragraph" )
//                     +"."
                     +"</center>");
}

// -------------------------------------------------------------------------------
void CMainWindow::insertCurrentDate( void )
// -------------------------------------------------------------------------------
{
   if ( mpEditor )
      mpEditor->insertPlainText( QDate::currentDate().toString() );
}

// -------------------------------------------------------------------------------
void CMainWindow::insertCurrentTime()
// -------------------------------------------------------------------------------
{
   if ( mpEditor )
      mpEditor->insertPlainText( QTime::currentTime().toString() );
}

// -------------------------------------------------------------------------------
void CMainWindow::showAbout()
// -------------------------------------------------------------------------------
{
   QMessageBox::about(  this, "TuxCards",
                        "TuxCards - The Notebook for TuxFreaks\n"
                        + QString(TUX_VERSION) + "\n\n"
                        "www.tuxcards.de\n\n"
                        "Copyright (C) 2000-2010 Alexander Theel\n"
                        "alex.theel@gmx.net" );
}

// -------------------------------------------------------------------------------
void CMainWindow::slotSetMainToolBarVisible( bool bVisible )
// -------------------------------------------------------------------------------
{
   mConfiguration.setBoolValue( CTuxCardsConfiguration::B_SHOW_MAIN_TOOLBAR, bVisible );

   if ( mpMainToolBar )
   {
      if (bVisible) mpMainToolBar->show();
      else mpMainToolBar->hide();
   }
   mConfiguration.saveToFile();
}

// -------------------------------------------------------------------------------
void CMainWindow::slotSetEntryToolBarVisible( bool bVisible )
// -------------------------------------------------------------------------------
{
   mConfiguration.setBoolValue( CTuxCardsConfiguration::B_SHOW_ENTRY_TOOLBAR, bVisible );

   if ( mpEntryToolBar )
   {
      if (bVisible) mpEntryToolBar->show();
      else mpEntryToolBar->hide();
   }
   mConfiguration.saveToFile();
}

// -------------------------------------------------------------------------------
void CMainWindow::slotSetEditorToolBarVisible( bool bVisible )
// -------------------------------------------------------------------------------
{
   mConfiguration.setBoolValue( CTuxCardsConfiguration::B_SHOW_EDITOR_TOOLBAR, bVisible );

   if ( mpEditorToolBar && mpEditorToolBar2 )
   {
      if (bVisible)
      {
         mpEditorToolBar->show();
         mpEditorToolBar2->show();
      }
      else
      {
         mpEditorToolBar->hide();
         mpEditorToolBar2->hide();
      }
   }
   mConfiguration.saveToFile();
}

//#ifdef FEATURE_RECENTFILELIST_TOOLBAR
//// -------------------------------------------------------------------------------
//void CMainWindow::setRecentFilesToolbarVisible( bool bVisible )
//// -------------------------------------------------------------------------------
//{
//   mConfiguration.setBoolValue( CTuxCardsConfiguration::B_SHOW_RECENTFILES_TOOLBAR, bVisible );
//
//   if ( mpRecentFilesTools )
//   {
//      if (bVisible) mpRecentFilesTools->show();
//      else mpRecentFilesTools->hide();
//   }
////   if ( mpMenu ) mpMenu->setItemChecked( miRecentFilesToolBarID, bVisible );
//   mConfiguration.saveToFile();
//}
//#endif

// -------------------------------------------------------------------------------
void CMainWindow::slotEditConfiguration()
// -------------------------------------------------------------------------------
{
   if ( !mpConfigurationDialog )
      return;

   mpConfigurationDialog->setUp();
}


// -------------------------------------------------------------------------------
void CMainWindow::applyConfiguration()
// -------------------------------------------------------------------------------
{
   // cactusbar
   if ( mpCactusBar )
   {
      mpCactusBar->toggleCactus( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_IS_CACTUSBAR_ENABLED ) );


      QString t1("");
      QString t2("");
      if( mConfiguration.getBoolValue(CTuxCardsConfiguration::B_IS_HTEXT_ENABLED) )
      {
         t1 = mConfiguration.getStringValue( CTuxCardsConfiguration::S_TEXT_ONE );
         t2 = mConfiguration.getStringValue( CTuxCardsConfiguration::S_TEXT_TWO );
      }
      mpCactusBar->change( mConfiguration.getTopColor(), mConfiguration.getBottomColor(), t1, t2, mConfiguration.getFontColor() );

      if( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_IS_VTEXT_ENABLED ) )
      {
         t1 = mConfiguration.getStringValue( CTuxCardsConfiguration::S_VERTICAL_TEXT );
      }
      else
      {
         t1 = "";
      }
      mpCactusBar->setVerticalText( t1, mConfiguration.getBoolValue( CTuxCardsConfiguration::B_ALIGN_VTEXT ) );

      mpCactusBar->setFlowerDirectory( mConfiguration.getStringValue( CTuxCardsConfiguration::S_FLOWER_DIR ) );
      mpCactusBar->update();
   }


   // autosave
   killTimer( miTimerID );
   if ( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_AUTOSAVE ) )
      miTimerID = startTimer( 60000 * mConfiguration.getIntValue( CTuxCardsConfiguration::I_SAVE_ALL_MINUTES ) );

   // editor
   if ( mpEditor )
   {
      QFont editorFont = mConfiguration.getEditorFont().toFont();
      mpEditor->setEditorFont( editorFont );
      mpEditor->setTabStopWidth( mConfiguration.getIntValue( CTuxCardsConfiguration::I_TAB_SIZE ) * QFontMetrics( editorFont ).width('X') );
      mpEditor->setLineWrap( mConfiguration.getIntValue( CTuxCardsConfiguration::I_WORD_WRAP ) );

//       if ( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_LINEBREAK_WITH_CTRL_SHIFT ))
//          mpEditor->setLinebreakMode( LINEBREAK_WITH_CTRL_SHIFT );
//       else
//          mpEditor->setLinebreakMode( LINEBREAK_WITHOUT_CTRL_SHIFT );         
   }

   // tree
   mpTree->setFont( mConfiguration.getTreeFont().toFont() );


   // windowsize & splitter
   setWindowGeometry( mConfiguration.getIntValue( CTuxCardsConfiguration::I_WINDOW_XPOS ),
                      mConfiguration.getIntValue( CTuxCardsConfiguration::I_WINDOW_YPOS ),
                      mConfiguration.getIntValue( CTuxCardsConfiguration::I_WINDOW_WIDTH ),
                      mConfiguration.getIntValue( CTuxCardsConfiguration::I_WINDOW_HEIGHT ),
                      mConfiguration.getIntValue( CTuxCardsConfiguration::I_TREE_WIDTH ),
                      mConfiguration.getIntValue( CTuxCardsConfiguration::I_EDITOR_WIDTH )
                     );
}


/**
 * The following configurations must be applied after the method <code>show()</code>
 * for the window was called, otherwise the window is not yet painted and configuration
 * commands (f.i. the scrollbar of the tree) do not have an effect.
 */
// -------------------------------------------------------------------------------
void CMainWindow::applyConfigurationAfterShow()
// -------------------------------------------------------------------------------
{
   if ( !mpTree )
      return;
      
   mpTree->verticalScrollBar()->setValue(
       mConfiguration.getIntValue( CTuxCardsConfiguration::I_TREE_VSCROLLBAR_VALUE ) );

}


// -------------------------------------------------------------------------------
void CMainWindow::setWindowGeometry( int iWindowXPos,  int iWindowYPos,
                                     int iWindowWidth, int iWindowHeight,
                                     int iTreeWidth,   int iEditorWidth )
// -------------------------------------------------------------------------------
{
   move( iWindowXPos, iWindowYPos );
   resize( iWindowWidth, iWindowHeight);

   QList<int> sizeList;
   sizeList.append( iTreeWidth );
   sizeList.append( iEditorWidth );
   mpSplit->setSizes( sizeList );
}


// -------------------------------------------------------------------------------
void CMainWindow::print()
// -------------------------------------------------------------------------------
{
   if ( !mpCollection || !mpCollection->getActiveElement() )
      return;

   if ( mpCollection->getActiveElement()->getInformationFormat() == InformationFormat::getByString("ASCII") )
   {
      QMessageBox::information( this, tr("Printing"), tr("I suggest to convert this entry to "
                                "rtf before printing."), QMessageBox::Ok );
   }

   if ( !mpEditor )
      return;

   mpEditor->writeCurrentTextToActiveInformationElement();

#ifndef QT_NO_PRINTER
   QPrinter printer(QPrinter::HighResolution);
   printer.setFullPage(true);
   QPrintDialog *dlg = new QPrintDialog(&printer, this);
   if (dlg->exec() == QDialog::Accepted) {
      mpEditor->document()->print(&printer);
   }
   delete dlg;
#endif
}

// -------------------------------------------------------------------------------
void CMainWindow::makeVisible( const CSearchPosition* const pPosition )
// -------------------------------------------------------------------------------
{
   std::cout<<"CMainWindow::makeVisible()"<<std::endl;
   if ( !mpCollection || !pPosition || !mpEditor )
      return;

//    Path pPath = pPosition->getPath();
//    if ( !pPath )
//       return;

   mpCollection->setActiveElement( *(pPosition->getPath()) );

   // If a match was found within the entry's text, then set the cursor appropriatetly.
   // Otherwise not!
   if ( CSearchPosition::SP_INFORMATION == pPosition->getLocation() )
   {
      QTextCursor cursor = mpEditor->textCursor();
      cursor.setPosition( pPosition->getPos() );
      cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor, pPosition->getLen() );
      mpEditor->setTextCursor( cursor ); 
   }

   this->activateWindow();
   mpEditor->setFocus();

   //mpEditor->ensureCursorVisible();
}



// -------------------------------------------------------------------------------
void CMainWindow::showDonationMsg()
// -------------------------------------------------------------------------------
{
   if ( mConfiguration.getBoolValue( CTuxCardsConfiguration::B_SHOW_DONATION_MSG_2 ) )
      return;

   QMessageBox::about( this, "TuxCards",
         "<p><b>TuxCards "
         TUX_SHORT_VERSION +
		   tr(" is published as Donationware") + "</b></p>"
         ""
         "<p>" + tr("You may use it freely and without charge as stated within the GPL.") +"</p>"
         ""
         "<p><b><font COLOR=\"#990000\">" + tr("Do you like TuxCards ...") + "</font></b> " +
         tr("and you want to enable the development of new "
         "features? Or do you want to ensure a high quality of future releases "
         "or simply compensate for ftp-server costs? Then you are welcome to "
         "donate EUR 5 to the author. (At your wish US$ 5 or an equivalent amount "
         "in your currency.)") + "</p>"
         ""
         "<p><i>" + tr("Please, use") + "</i> www.tuxcards.de/donations.html <i>" + tr("for a secure donation via internet.") + "<i></p>"
         ""
         "<p><i>" + tr("Or simply send a letter with your amount to") + "</i></p>"
         ""
         "Alexander Theel<br>"
         "Am Trommlerbusch 24 d<br>"
         "02708 Loebau<br>" +
         tr("Germany") + "<br>" );

   mConfiguration.setBoolValue( CTuxCardsConfiguration::B_SHOW_DONATION_MSG_2, true );
   mConfiguration.saveToFile();
}


///*********************** debug methods **********************************/
//// -------------------------------------------------------------------------------
//void CMainWindow::debugShowRTFTextSource()
//// -------------------------------------------------------------------------------
//{
//   if ( NULLPTR == mpEditor )
//      return;
//
//   QTextEdit* outputWindow=new QTextEdit();
//   outputWindow->resize(400,400);
//   outputWindow->setPlainText(mpEditor->getText());
//   outputWindow->show();
//}
//
//
//// -------------------------------------------------------------------------------
//void CMainWindow::debugShowXMLCode()
//// -------------------------------------------------------------------------------
//{
//   if ( NULLPTR == mpCollection )
//      return;
//
//   QTextEdit* outputWindow=new QTextEdit();
//   outputWindow->resize(400,400);
//   outputWindow->setPlainText(mpCollection->toXML());
//   outputWindow->show();
//}
