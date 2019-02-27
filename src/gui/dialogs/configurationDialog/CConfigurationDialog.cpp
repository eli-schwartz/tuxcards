/***************************************************************************
                          CConfigurationDialog.cpp  -  description
                             -------------------
    begin                : Sun Aug 12 2007
    copyright            : (C) 2007 by Alexander Theel
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

#include "CConfigurationDialog.h"
#include "../../cactusbar/CCactusBar.h"
#include <qradiobutton.h>
#include <qfontdialog.h>
#include <QFileDialog>
#include <iostream>

#include "../../../global.h"

// -------------------------------------------------------------------------------
CConfigurationDialog::CConfigurationDialog( QWidget* parent, CTuxCardsConfiguration& config )
 : QDialog( parent )
 , mpCactusBarPreview( NULLPTR )
 , mrefConfig( config )
// -------------------------------------------------------------------------------
{
   ui.setupUi(this);
   addCactusBarToDialog();

   // do not use the lineBreak mode stuff, this is not needed
   ui.mpLineBreakGroupBox->setVisible( FALSE );

   setModal( TRUE );
  
   connect( ui.mpTopColorButton,    SIGNAL(clicked()), this, SLOT(slotChangeTopColor()) );
   connect( ui.mpBottomColorButton, SIGNAL(clicked()), this, SLOT(slotChangeBottomColor()) );
   connect( ui.mpTextColorButton,   SIGNAL(clicked()), this, SLOT(slotChangeTextColor()) );

   connect( ui.mpChangeTreeFontButton, SIGNAL(clicked()), this, SLOT(slotChangeTreeFont()) );
   connect( ui.mpChangeEditorFontButton, SIGNAL(clicked()), this, SLOT(slotChangeEditorFont()) );
   
   connect( this, SIGNAL(accepted()), this, SLOT(changeProperties()) );

   // TODO: ToolTips are still missing within the configuration dialog.
}

// -------------------------------------------------------------------------------
void CConfigurationDialog::addCactusBarToDialog()
// -------------------------------------------------------------------------------
{
   QHBoxLayout* pLayout = new QHBoxLayout();
   pLayout->setContentsMargins ( 0,0,9,0 );     // TODO: The calculation of the
                                                //       CColorBar width and thus
                                                //       the painting of it is not
                                                //       yet correct.
                                                
   ui.mpPlaceHolder->setLayout( pLayout );
   //CColorBar* pCB = new CColorBar( 0, QColor(0,0,0),QColor(33,72,170), "Tux","Cards");
   mpCactusBarPreview = new CCactusBar( 0, QColor(0,0,0),QColor(33,72,170), "Tux","Cards");
   pLayout->addWidget( mpCactusBarPreview );

   connect( ui.mpEnableCactusBar, SIGNAL(toggled(bool)), this, SLOT(slotToggleCactusPreview(bool)) );
   connect( ui.mpFirstTextLineLineEdit, SIGNAL(editingFinished()), this, SLOT(slotApplyChangesToCactusBarPreview()) );
   connect( ui.mpSecondTextLineLineEdit, SIGNAL(editingFinished()), this, SLOT(slotApplyChangesToCactusBarPreview()) );
   // TODO: If the horizontal text is turned off, then it is still shown within
   //       the preview.
   // TODO: The vertical text is not shown within the preview.
}


// -------------------------------------------------------------------------------
int CConfigurationDialog::setUp()
// -------------------------------------------------------------------------------
{
   ui.mpAutoSaveCheckBox        ->setChecked( mrefConfig.getBoolValue( CTuxCardsConfiguration::B_AUTOSAVE ) );
   ui.mpSaveMinutesLineEdit     ->setText( QString::number(mrefConfig.getIntValue( CTuxCardsConfiguration::I_SAVE_ALL_MINUTES )) );
   ui.mpSaveWhenQuittingCheckBox->setChecked( mrefConfig.getBoolValue( CTuxCardsConfiguration::B_SAVE_WHEN_LEAVING ) );
   ui.mpCreateBackupFileCheckBox->setChecked( mrefConfig.getBoolValue( CTuxCardsConfiguration::B_CREATE_BACKUP_FILE ) );

   QString sCommand = mrefConfig.getStringValue( CTuxCardsConfiguration::S_EXECUTE_STATEMENT );
   ui.mpExecuteCommandGroupBox->setChecked( !sCommand.isEmpty() );
   ui.mpCommandLineEdit       ->setText( sCommand );
   
   ui.mpIconDirectoryLineEdit ->setText( mrefConfig.getStringValue( CTuxCardsConfiguration::S_ICON_DIR ) );
   ui.mpAutoEncryptCheckBox   ->setChecked( mrefConfig.getBoolValue( CTuxCardsConfiguration::B_AUTOENCRYPT_ENTRY ) );

   ui.mpTopColorButton        ->setPalette( mrefConfig.getTopColor() );
   ui.mpBottomColorButton     ->setPalette( mrefConfig.getBottomColor() );
   ui.mpHorizontalTextEnabled ->setChecked( mrefConfig.getBoolValue( CTuxCardsConfiguration::B_IS_HTEXT_ENABLED ) );
   ui.mpFirstTextLineLineEdit ->setText( mrefConfig.getStringValue(  CTuxCardsConfiguration::S_TEXT_ONE ) );
   ui.mpSecondTextLineLineEdit->setText( mrefConfig.getStringValue(  CTuxCardsConfiguration::S_TEXT_TWO ) );
   // barTextColor
   QPalette p = ui.mpTextColorButton->palette();
   p.setColor( QPalette::ButtonText, mrefConfig.getFontColor() );
   ui.mpTextColorButton->setPalette( p );

   ui.mpVerticalTextEnabled     ->setChecked(mrefConfig.getBoolValue( CTuxCardsConfiguration::B_IS_VTEXT_ENABLED ) );
   ui.mpVerticalTextLineLineEdit->setText(mrefConfig.getStringValue( CTuxCardsConfiguration::S_VERTICAL_TEXT ) );
   if ( CColorBar::ALIGN_TOP == mrefConfig.getBoolValue( CTuxCardsConfiguration::B_ALIGN_VTEXT ) )
      ui.mpAlineTop   ->setChecked(true);
   else
      ui.mpAlineBottom->setChecked(true);

   ui.mpEnableCactusBar         ->setChecked( mrefConfig.getBoolValue(CTuxCardsConfiguration::B_IS_CACTUSBAR_ENABLED ) );
   slotToggleCactusPreview( mrefConfig.getBoolValue(CTuxCardsConfiguration::B_IS_CACTUSBAR_ENABLED ) );
   ui.mpFlowerDirectoryLineEdit ->setText( mrefConfig.getStringValue( CTuxCardsConfiguration::S_FLOWER_DIR ) );

   slotApplyChangesToCactusBarPreview();

   //TODO: Eventually, the font settings are not needed any more within qt4.
   QFont treeFont = mrefConfig.getTreeFont().toFont();
   ui.mpTreeFontExample  ->setFont( treeFont );
   ui.mpTreeFontExample  ->setText( calculateFontString(treeFont) );

   ui.mpEditorFontExample->setFont( mrefConfig.getEditorFont().toFont() );
   ui.mpEditorFontExample->setText( mrefConfig.getEditorFont().toFont().rawName().section(':', 0,0) );

   QFont editorFont = mrefConfig.getEditorFont().toFont();
   ui.mpEditorFontExample->setFont( editorFont );
   ui.mpEditorFontExample->setText( calculateFontString(editorFont) );

   ui.mpTabSizeLineEdit  ->setText( QString::number(mrefConfig.getIntValue( CTuxCardsConfiguration::I_TAB_SIZE )) );

   // 'iWordWrap' is a non-negative number; 0 means noWrap, 1 means widgetWrap,
   // 'iWordWrap' >1 mean wrap at column 'iWordWrap'
   int iWordWrap = mrefConfig.getIntValue( CTuxCardsConfiguration::I_WORD_WRAP );
   switch ( iWordWrap )
   {
   case 0:
      ui.mpNoWordWrap->setChecked( true );
      break;

   case 1:
      ui.mpWindowWidthWrap->setChecked( true );
      break;

   default:
      ui.mpColumnWrap->setChecked( true );
      ui.mpWrapAtColumnLineEdit->setText( QString::number(iWordWrap) );
      break;
   };

   // linebreak
   if ( mrefConfig.getBoolValue( CTuxCardsConfiguration::B_LINEBREAK_WITH_CTRL_SHIFT ) )
   {
      ui.mpEnableLineBreakWithCtrlShift->setChecked(true);
   }
   else
   {
      ui.mpEnableLineBreakWithoutCtrlShift->setChecked(true);
   }

   show();
   return exec();
}

/**
 * This method is called whenever the ok-button is pressed.
 * Then all changes are set with the configuratio-object. The mainwindow
 * is also informed about the clicking of the ok-button and adjusts
 * own things.
 */
// -------------------------------------------------------------------------------
void CConfigurationDialog::changeProperties()
// -------------------------------------------------------------------------------
{
 	mrefConfig.setBoolValue(   CTuxCardsConfiguration::B_AUTOSAVE,          getAutosave() );
 	mrefConfig.setIntValue(    CTuxCardsConfiguration::I_SAVE_ALL_MINUTES,  getMinutes());
 	mrefConfig.setBoolValue(   CTuxCardsConfiguration::B_SAVE_WHEN_LEAVING, getSaveWhenLeaving() );
 	mrefConfig.setBoolValue(   CTuxCardsConfiguration::B_CREATE_BACKUP_FILE,getCreateBackup() );

   if ( ui.mpExecuteCommandGroupBox && ui.mpExecuteCommandGroupBox->isChecked() )
      mrefConfig.setStringValue( CTuxCardsConfiguration::S_EXECUTE_STATEMENT, ui.mpCommandLineEdit->text() );
   else
      mrefConfig.setStringValue( CTuxCardsConfiguration::S_EXECUTE_STATEMENT, "" );
   mrefConfig.setStringValue( CTuxCardsConfiguration::S_ICON_DIR,          ui.mpIconDirectoryLineEdit->text() );
   mrefConfig.setBoolValue(   CTuxCardsConfiguration::B_AUTOENCRYPT_ENTRY, ui.mpAutoEncryptCheckBox->isChecked() );

 	mrefConfig.setTopColor( getTopColor() );
 	mrefConfig.setBottomColor( getBottomColor() );
 	mrefConfig.setBoolValue(   CTuxCardsConfiguration::B_IS_HTEXT_ENABLED,  getEnableHText() );
 	mrefConfig.setStringValue( CTuxCardsConfiguration::S_TEXT_ONE,          getTextOne() );
 	mrefConfig.setStringValue( CTuxCardsConfiguration::S_TEXT_TWO,          getTextTwo() );
 	mrefConfig.setFontColor( getBarTextColor() );
 	mrefConfig.setBoolValue(   CTuxCardsConfiguration::B_IS_VTEXT_ENABLED,  getEnableVText() );
 	mrefConfig.setStringValue( CTuxCardsConfiguration::S_VERTICAL_TEXT,     getVerticalText());
 	mrefConfig.setBoolValue(   CTuxCardsConfiguration::B_ALIGN_VTEXT,       getAlignVText() );
 	mrefConfig.setBoolValue(   CTuxCardsConfiguration::B_IS_CACTUSBAR_ENABLED, getEnableCactusBar() );
 	mrefConfig.setStringValue( CTuxCardsConfiguration::S_FLOWER_DIR,        getFlowerDir());

   // editor-font
   QFont f                 = getEditorFont();
   QString FONT_FAMILY     =f.family();
   int     FONT_SIZE       =f.pointSize();
   bool    FONT_BOLD	      =f.bold();
   bool    FONT_ITALIC     =f.italic();
   bool    FONT_UNDERLINE  =f.underline();
   bool    FONT_STRIKEOUT  =f.strikeOut();
   FontSettings fontSettingsEditor( FONT_FAMILY, FONT_SIZE, FONT_BOLD, FONT_ITALIC, FONT_UNDERLINE, FONT_STRIKEOUT );
   mrefConfig.setEditorFont( fontSettingsEditor );

   // tree-font
   f              = getTreeFont();
   FONT_FAMILY    =f.family();
   FONT_SIZE      =f.pointSize();
   FONT_BOLD	   =f.bold();
   FONT_ITALIC    =f.italic();
   FONT_UNDERLINE =f.underline();
   FONT_STRIKEOUT =f.strikeOut();
   FontSettings fontSettingsTree( FONT_FAMILY, FONT_SIZE, FONT_BOLD, FONT_ITALIC, FONT_UNDERLINE, FONT_STRIKEOUT );
   mrefConfig.setTreeFont( fontSettingsTree );

   mrefConfig.setIntValue(  CTuxCardsConfiguration::I_TAB_SIZE,  getTabSize() );
   mrefConfig.setIntValue(  CTuxCardsConfiguration::I_WORD_WRAP, getLineWrap() );
   mrefConfig.setBoolValue( CTuxCardsConfiguration::B_LINEBREAK_WITH_CTRL_SHIFT,
                            ui.mpEnableLineBreakWithCtrlShift->isChecked() );

   // done
   mrefConfig.saveToFile();
   emit configurationChanged();
}

// -------------------------------------------------------------------------------
bool CConfigurationDialog::getAutosave(){      return ui.mpAutoSaveCheckBox->isChecked(); }
// -------------------------------------------------------------------------------

// -------------------------------------------------------------------------------
int  CConfigurationDialog::getMinutes()
// -------------------------------------------------------------------------------
{
   int iMinutes = ui.mpSaveMinutesLineEdit->text().toInt();
   if ( 0 > iMinutes )
      return 0;

   return iMinutes;
}
bool CConfigurationDialog::getSaveWhenLeaving(){ return ui.mpSaveWhenQuittingCheckBox->isChecked(); }
bool CConfigurationDialog::getCreateBackup(){    return ui.mpCreateBackupFileCheckBox->isChecked(); }

QColor CConfigurationDialog::getTopColor(){      return ui.mpTopColorButton->palette().color( QPalette::Window ); }
QColor CConfigurationDialog::getBottomColor(){   return ui.mpBottomColorButton->palette().color( QPalette::Window ); }
QColor CConfigurationDialog::getBarTextColor(){  return ui.mpTextColorButton->palette().buttonText().color(); }

bool CConfigurationDialog::getEnableHText(){     return ui.mpHorizontalTextEnabled->isChecked(); }
QString CConfigurationDialog::getTextOne(){      return ui.mpFirstTextLineLineEdit->text();  }
QString CConfigurationDialog::getTextTwo(){      return ui.mpSecondTextLineLineEdit->text(); }

bool CConfigurationDialog::getEnableVText(){     return ui.mpVerticalTextEnabled->isChecked();}
QString CConfigurationDialog::getVerticalText(){ return ui.mpVerticalTextLineLineEdit->text(); }
bool    CConfigurationDialog::getAlignVText(){   return ui.mpAlineBottom->isChecked(); }

/**
 * returns 'true' if the cactusbar should be used
 * otherwise(the colorbar is selected) 'false' is returned
 */
// -------------------------------------------------------------------------------
bool CConfigurationDialog::getEnableCactusBar()
// -------------------------------------------------------------------------------
{
	if(ui.mpEnableCactusBar->isChecked()) return true; else return false;
}

// -------------------------------------------------------------------------------
QString CConfigurationDialog::getFlowerDir()
// -------------------------------------------------------------------------------
{
	QString s = ui.mpFlowerDirectoryLineEdit->text();
	if ( s.right(1) != "/" ) s+="/";  		// make the directory ends with '/'

	return s;
}

QFont CConfigurationDialog::getTreeFont(){   return ui.mpTreeFontExample->font();   }
QFont CConfigurationDialog::getEditorFont(){ return ui.mpEditorFontExample->font(); }

int CConfigurationDialog::getTabSize(){ return ui.mpTabSizeLineEdit->text().toInt(); }

// -------------------------------------------------------------------------------
int CConfigurationDialog::getLineWrap()
// -------------------------------------------------------------------------------
{
	if ( ui.mpNoWordWrap->isChecked() )
		return 0;
	else if ( ui.mpWindowWidthWrap->isChecked() )
		return 1;
	else
   {
		return ui.mpWrapAtColumnLineEdit->text().toInt();
	}
}

//void CConfigurationDialog::autosave(){
//	if (autosave->isChecked()){
//		saveMinutes->setEnabled(true);  label->setEnabled(true);
//	}else{
//		saveMinutes->setEnabled(false);  label->setEnabled(false);
//	}
//}

/**
 * Select a color from the colorDialog for 'topColor'.
 */
// -------------------------------------------------------------------------------
void CConfigurationDialog::slotChangeTopColor()
// -------------------------------------------------------------------------------
{
	QColor c = QColorDialog::getColor( ui.mpTopColorButton->palette().color( QPalette::Button ) );
   if( !c.isValid() ) return;

	ui.mpTopColorButton->setPalette( QPalette(c, c) );
   slotApplyChangesToCactusBarPreview();
}

/**
 * Select a color from the colorDialog for 'bottomColor'.
 */
// -------------------------------------------------------------------------------
void CConfigurationDialog::slotChangeBottomColor()
// -------------------------------------------------------------------------------
{
	QColor c = QColorDialog::getColor( ui.mpBottomColorButton->palette().color( QPalette::Button ) );
   if( !c.isValid() ) return;

	ui.mpBottomColorButton->setPalette( QPalette(c, c) );
   slotApplyChangesToCactusBarPreview();
}

/**
 * Select a color from the colorDialog for 'textColor' of the "ColorBarText".
 */
// -------------------------------------------------------------------------------
void CConfigurationDialog::slotChangeTextColor()
// -------------------------------------------------------------------------------
{
	QColor c = QColorDialog::getColor( ui.mpTextColorButton->palette().color( QPalette::ButtonText ) );
   if( !c.isValid() ) return;

	QPalette p = ui.mpTextColorButton->palette();
	p.setColor( QPalette::ButtonText, c );
	ui.mpTextColorButton->setPalette( p );

   slotApplyChangesToCactusBarPreview();
}

/**
 * calls the fontdialog and changes the option for the font of the
 * tuxcards-tree if necessary
 */
// -------------------------------------------------------------------------------
void CConfigurationDialog::slotChangeTreeFont()
// -------------------------------------------------------------------------------
{
 	bool bOk;
	QFont f = QFontDialog::getFont( &bOk, ui.mpTreeFontExample->font(), this, "Select Tree Font");

	// a valid font was selected
	if ( bOk )
   {
		ui.mpTreeFontExample->setFont( f );
		ui.mpTreeFontExample->setText( calculateFontString(f) );
	}
}

/**
 * calls the fontdialog and changes the option for the font of the
 * tuxcards-editor if necessary
 */
// -------------------------------------------------------------------------------
void CConfigurationDialog::slotChangeEditorFont()
// -------------------------------------------------------------------------------
{
 	bool bOk;
	QFont f = QFontDialog::getFont( &bOk, ui.mpEditorFontExample->font(), this );

	// a valid font was selected
	if( bOk )
   {
		ui.mpEditorFontExample->setFont( f );
		ui.mpEditorFontExample->setText( calculateFontString(f) );
	}
}

/**
 * Calculate a string from a given font. This string can be presented within
 * the dialog.
 */
// -------------------------------------------------------------------------------
QString CConfigurationDialog::calculateFontString( const QFont& f ) const
// -------------------------------------------------------------------------------
{
   return f.family() + " - Size " + QString::number( f.pointSize() );
}


/**
 * opens a dialog and lets the user select a directory; the result is
 * written in 'flowerDir'
 */
// -------------------------------------------------------------------------------
void CConfigurationDialog::chooseFlowerDir()
// -------------------------------------------------------------------------------
{
	QString s = QFileDialog::getExistingDirectory( this, 
						"Select a Flower-Directory", getFlowerDir() );
	if( s.isEmpty() ) return;
	
	ui.mpFlowerDirectoryLineEdit->setText(s);
   
   // TODO: The flower directory is not really used yet.
}

//void CConfigurationDialog::setDefaults(){
//	autosaveCheckBox->setChecked(true);
//	saveMinutes->setText(QString::number(15)); saveMinutes->setEnabled(true);
//	saveWhenLeaving->setChecked(false);          label->setEnabled(true);
//	
//	topColor->setPalette(QColor(0,0,0));
//	bottomColor->setPalette(QColor(143,0,0));
//	textOne->setText("Tux");
//	textTwo->setText("Cards");
//}


/**
 * Auxiliary method to show the colorbar changes in preview.
 */
// -------------------------------------------------------------------------------
void CConfigurationDialog::slotApplyChangesToCactusBarPreview()
// -------------------------------------------------------------------------------
{
   if ( !mpCactusBarPreview || !ui.mpTopColorButton || !ui.mpBottomColorButton
        || !ui.mpFirstTextLineLineEdit ||!ui.mpSecondTextLineLineEdit
        || !ui.mpTextColorButton )
      return;
      
   mpCactusBarPreview->change( ui.mpTopColorButton->palette().color( QPalette::Button ),
                               ui.mpBottomColorButton->palette().color( QPalette::Button ),
                               ui.mpFirstTextLineLineEdit->text(),
                               ui.mpSecondTextLineLineEdit->text(), //"Tux","Cards",
                               ui.mpTextColorButton->palette().color( QPalette::ButtonText ) );
}

/**
 * Auxiliary method to toggle the cactus within the preview.
 */
// -------------------------------------------------------------------------------
void CConfigurationDialog::slotToggleCactusPreview( bool bEnableCactus )
// -------------------------------------------------------------------------------
{
   if ( !mpCactusBarPreview )
      return;

   mpCactusBarPreview->toggleCactus( bEnableCactus );
}
