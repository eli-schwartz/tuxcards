/***************************************************************************
                          CTuxCardsConfiguration.cpp  -  description
                             -------------------
    begin                : Sam Aug 17 2002
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

#include "CTuxCardsConfiguration.h"

#include "version.h"
#include "global.h"
#include <QSettings>
#include <qdir.h>

#include <qmessagebox.h>


// -------------------------------------------------------------------------------
CTuxCardsConfiguration::CTuxCardsConfiguration()
// -------------------------------------------------------------------------------
 : mBoolMap()
 , mStringMap()
 , mIntMap()
{
   readConfigurationFile();
}

// -------------------------------------------------------------------------------
CTuxCardsConfiguration& CTuxCardsConfiguration::getInstance()
// -------------------------------------------------------------------------------
{
   static CTuxCardsConfiguration static_Instance;
   return static_Instance;
}

// -------------------------------------------------------------------------------
void CTuxCardsConfiguration::readConfigurationFile()
// -------------------------------------------------------------------------------
{
   // set default options just to be sure; if the config-file
   // might not be there or version 0.4 was used before
   QSettings settings( TUX_CONFIG_FILE, QSettings::IniFormat );

   // Do not use a group "General" because in newer Qt-Versions this would be treated as
   // group "%General". All set keys without a group are automatically put into "General".
   //settings.beginGroup("General");
   QString version   = settings.value("Version",   QString("TuxCards_") + TUX_VERSION).toString();

   mStringMap[S_DATA_FILE_NAME]     = settings.value("Data_File",          "").toString(); //QDir::homePath()+"/"+TUX_GREETING_FILE).toString();
   mBoolMap  [B_AUTOSAVE]           = settings.value("Autosave",            1).toInt();
   mIntMap   [I_SAVE_ALL_MINUTES]   = settings.value("Save_All_Minutes",   15).toInt();
   mBoolMap  [B_SAVE_WHEN_LEAVING]  = settings.value("Save_when_Leaving",   1).toInt();
   mBoolMap  [B_CREATE_BACKUP_FILE] = settings.value("Create_Backupfile",   1).toInt();

   mStringMap[S_EXECUTE_STATEMENT]  = settings.value("Execution_Statement",   "").toString();
   mStringMap[S_RECENT_FILES]       = settings.value("Recent_Files",          "").toString();
   mBoolMap  [B_ENCRYPTION_USED]    = settings.value("Encryption_Used",     0).toInt();
   mBoolMap  [B_AUTOENCRYPT_ENTRY]  = settings.value("AutoEncrypt_Entry",   0).toInt();

   int colorR                       = settings.value("Color_Top_R"         ,0).toInt();
   int colorG                       = settings.value("Color_Top_G"         ,0).toInt();
   int colorB                       = settings.value("Color_Top_B"         ,0).toInt();
   topColor                         = QColor(colorR, colorG, colorB);
   colorR                           = settings.value("Color_Bottom_R"     ,33).toInt();
   colorG                           = settings.value("Color_Bottom_G"     ,72).toInt();
   colorB                           = settings.value("Color_Bottom_B"    ,170).toInt();
   bottomColor                      = QColor(colorR, colorG, colorB);
   mBoolMap  [B_IS_HTEXT_ENABLED]   = settings.value("Enable_H_Text",       1).toInt();
   mStringMap[S_TEXT_ONE]           = settings.value("Text_One",           "Tux").toString();
   mStringMap[S_TEXT_TWO]           = settings.value("Text_Two",         "Cards").toString();
   colorR                           = settings.value("Color_Font_R"      ,255).toInt();
   colorG                           = settings.value("Color_Font_G"      ,255).toInt();
   colorB                           = settings.value("Color_Font_B"      ,255).toInt();
   fontColor                        = QColor(colorR, colorG, colorB);

   mBoolMap  [B_IS_VTEXT_ENABLED]   = settings.value("Enable_V_Text",       0).toInt();
   mStringMap[S_VERTICAL_TEXT]      = settings.value("Vertical_Text",         "").toString();
   mBoolMap  [B_ALIGN_VTEXT]        = settings.value("Align_Vertical_Text", 0).toInt();
   mBoolMap  [B_IS_CACTUSBAR_ENABLED] = settings.value("Enable_CactusBar",  0).toInt();
   mStringMap[S_FLOWER_DIR]         = settings.value("Flower_Dir", "/usr/local/doc/tuxcards/flowers/").toString();
   mStringMap[S_ICON_DIR]           = settings.value("Icon_Dir",              "").toString();;

   QString FONT_family              = settings.value("Font_Family",  "Lucida Grande").toString();
   int  FONT_size                   = settings.value("Font_Size",          13).toInt();
   bool FONT_bold                   = settings.value("Font_Bold",           0).toInt();
   bool FONT_italic                 = settings.value("Font_Italic",         0).toInt();
   bool FONT_underline              = settings.value("Font_Underline",      0).toInt();
   bool FONT_strikeout              = settings.value("Font_Strikeout",      0).toInt();
   editorFont                       = FontSettings( FONT_family, FONT_size, FONT_bold,
                                                    FONT_italic, FONT_underline,
                                                    FONT_strikeout );

   FONT_family                      = settings.value("Tree_Font_Family", "Lucida Grande").toString();
   FONT_size                        = settings.value("Tree_Font_Size",         13).toInt();
   FONT_bold                        = settings.value("Tree_Font_Bold",          0).toInt();
   FONT_italic	                     = settings.value("Tree_Font_Italic",        0).toInt();
   FONT_underline                   = settings.value("Tree_Font_Underline",     0).toInt();
   FONT_strikeout                   = settings.value("Tree_Font_Strikeout",     0).toInt();
   treeFont                         = FontSettings( FONT_family, FONT_size, FONT_bold,
                                                    FONT_italic, FONT_underline,
                                                    FONT_strikeout );

   mIntMap   [I_TAB_SIZE]           = settings.value("TabSize",             8).toInt();
   mIntMap   [I_WORD_WRAP]          = settings.value("WordWrap",            1).toInt();
   mBoolMap  [B_LINEBREAK_WITH_CTRL_SHIFT]   = settings.value("Linebreak_WithCtrlShift", 1).toInt();


   mBoolMap  [B_SHOW_MAIN_TOOLBAR]  = settings.value("Show_Main_Toolbar",   1).toInt();
   mBoolMap  [B_SHOW_ENTRY_TOOLBAR] = settings.value("Show_Entry_Toolbar",  1).toInt();
   mBoolMap  [B_SHOW_EDITOR_TOOLBAR]= settings.value("Show_Editor_Toolbar", 1).toInt();
   mBoolMap  [B_SHOW_RECENTFILES_TOOLBAR]= settings.value("Show_RecentFiles_Toolbar", 0).toInt();
   //settings.endGroup();

   settings.beginGroup("WindowGeometry");
   mIntMap   [I_WINDOW_WIDTH]       = settings.value("WindowWidth",       750).toInt();
   mIntMap   [I_WINDOW_HEIGHT]      = settings.value("WindowHeight",      550).toInt();
   mIntMap   [I_TREE_WIDTH]         = settings.value("TreeWidth",         240).toInt();
   mIntMap   [I_EDITOR_WIDTH]       = settings.value("EditorWidth",       500).toInt();
   mIntMap   [I_TREE_VSCROLLBAR_VALUE] = settings.value("Tree_VScrollBar",  0).toInt();
   mIntMap   [I_WINDOW_XPOS]        = settings.value("WindowXPos",         10).toInt();
   mIntMap   [I_WINDOW_YPOS]        = settings.value("WindowYPos",         10).toInt();
   settings.endGroup();

   settings.beginGroup("Donation");
   mBoolMap  [B_SHOW_DONATION_MSG_2] = settings.value("ShowDonation2",     0).toInt();
   settings.endGroup();
}


// -------------------------------------------------------------------------------
void CTuxCardsConfiguration::saveToFile()
// -------------------------------------------------------------------------------
{
   // -- write to config-file ".tuxcards" ---------------------------
   // "Version" is the version of this program (not the version of a datafile)
   QSettings settings( TUX_CONFIG_FILE, QSettings::IniFormat );

   // Do not use a group "General" because in newer Qt-Versions this would be treated as
   // group "%General". All set keys without a group are automatically put into "General".
   //settings.beginGroup("General");
   settings.setValue("Version",                  QString("TuxCards_") + TUX_VERSION );
   settings.setValue("Data_File",                mStringMap[S_DATA_FILE_NAME]    );
   settings.setValue("Autosave",            (int)mBoolMap  [B_AUTOSAVE]          );
   settings.setValue("Save_All_Minutes",         mIntMap   [I_SAVE_ALL_MINUTES]  );
   settings.setValue("Save_when_Leaving",   (int)mBoolMap  [B_SAVE_WHEN_LEAVING] );
   settings.setValue("Create_Backupfile",   (int)mBoolMap  [B_CREATE_BACKUP_FILE]);
   settings.setValue("Execution_Statement",      mStringMap[S_EXECUTE_STATEMENT] );
   settings.setValue("Recent_Files",             mStringMap[S_RECENT_FILES]      );
   settings.setValue("Encryption_Used",     (int)mBoolMap  [B_ENCRYPTION_USED]   );
   settings.setValue("AutoEncrypt_Entry",   (int)mBoolMap  [B_AUTOENCRYPT_ENTRY] );
   settings.setValue("Color_Top_R",              topColor.red());
   settings.setValue("Color_Top_G",              topColor.green());
   settings.setValue("Color_Top_B",              topColor.blue());
   settings.setValue("Color_Bottom_R",           bottomColor.red());
   settings.setValue("Color_Bottom_G",           bottomColor.green());
   settings.setValue("Color_Bottom_B",           bottomColor.blue());
   settings.setValue("Enable_H_Text",       (int)mBoolMap  [B_IS_HTEXT_ENABLED]  );
   settings.setValue("Text_One",                 mStringMap[S_TEXT_ONE]          );
   settings.setValue("Text_Two",                 mStringMap[S_TEXT_TWO]          );
   settings.setValue("Color_Font_R",             fontColor.red());
   settings.setValue("Color_Font_G",             fontColor.green());
   settings.setValue("Color_Font_B",             fontColor.blue());
   settings.setValue("Enable_V_Text",       (int)mBoolMap  [B_IS_VTEXT_ENABLED]  );
   settings.setValue("Vertical_Text",            mStringMap[S_VERTICAL_TEXT]     );
   settings.setValue("Align_Vertical_Text", (int)mBoolMap  [B_ALIGN_VTEXT]       );
   settings.setValue("Enable_CactusBar",    (int)mBoolMap  [B_IS_CACTUSBAR_ENABLED]);
   settings.setValue("Flower_Dir",               mStringMap[S_FLOWER_DIR]        );
   settings.setValue("Icon_Dir",                 mStringMap[S_ICON_DIR]          );

   settings.setValue("Font_Family",              editorFont.getFamily());
   settings.setValue("Font_Size",                editorFont.getSize());
   settings.setValue("Font_Bold",                editorFont.isBold());
   settings.setValue("Font_Italic",              editorFont.isItalic());
   settings.setValue("Font_Underline",           editorFont.isUnderlined());
   settings.setValue("Font_Strikeout",           editorFont.isStrikeout());
   settings.setValue("Tree_Font_Family",         treeFont.getFamily());
   settings.setValue("Tree_Font_Size",           treeFont.getSize());
   settings.setValue("Tree_Font_Bold",           treeFont.isBold());
   settings.setValue("Tree_Font_Italic",         treeFont.isItalic());
   settings.setValue("Tree_Font_Underline",      treeFont.isUnderlined());
   settings.setValue("Tree_Font_Strikeout",      treeFont.isStrikeout());

   settings.setValue("TabSize",                  mIntMap[I_TAB_SIZE]             );
   settings.setValue("WordWrap",                 mIntMap[I_WORD_WRAP]            );
   settings.setValue("Linebreak_WithCtrlShift",  (int)mBoolMap[B_LINEBREAK_WITH_CTRL_SHIFT]);

   settings.setValue("Show_Main_Toolbar",   (int)mBoolMap[B_SHOW_MAIN_TOOLBAR]   );
   settings.setValue("Show_Entry_Toolbar",  (int)mBoolMap[B_SHOW_ENTRY_TOOLBAR]  );
   settings.setValue("Show_Editor_Toolbar", (int)mBoolMap[B_SHOW_EDITOR_TOOLBAR] );
   settings.setValue("Show_RecentFiles_Toolbar", (int)mBoolMap[B_SHOW_RECENTFILES_TOOLBAR]);
   //settings.endGroup();

   settings.beginGroup("WindowGeometry");
   settings.setValue("WindowWidth",              mIntMap[I_WINDOW_WIDTH]         );
   settings.setValue("WindowHeight",             mIntMap[I_WINDOW_HEIGHT]        );
   settings.setValue("TreeWidth",                mIntMap[I_TREE_WIDTH]           );
   settings.setValue("EditorWidth",              mIntMap[I_EDITOR_WIDTH]         );
   settings.setValue("Tree_VScrollBar",          mIntMap[I_TREE_VSCROLLBAR_VALUE]);
   settings.setValue("WindowXPos",               mIntMap[I_WINDOW_XPOS]          );
   settings.setValue("WindowYPos",               mIntMap[I_WINDOW_YPOS]          );
   settings.endGroup();

   settings.beginGroup("Donation");
   settings.setValue("ShowDonation2",       (int)mBoolMap[B_SHOW_DONATION_MSG_2] );
   settings.endGroup();
}




/****** getter *************************/
QColor CTuxCardsConfiguration::getTopColor() const { return topColor; }
QColor CTuxCardsConfiguration::getBottomColor() const { return bottomColor; }
QColor CTuxCardsConfiguration::getFontColor() const { return fontColor; }

FontSettings CTuxCardsConfiguration::getEditorFont() const { return editorFont; }
FontSettings CTuxCardsConfiguration::getTreeFont() const { return treeFont; }

/****** setter *************************/
void CTuxCardsConfiguration::setTopColor(QColor c){ this->topColor=c; }
void CTuxCardsConfiguration::setBottomColor(QColor c){ this->bottomColor=c; }
void CTuxCardsConfiguration::setFontColor(QColor c){ this->fontColor=c; }

void CTuxCardsConfiguration::setEditorFont(FontSettings f){ this->editorFont=f; }
void CTuxCardsConfiguration::setTreeFont(FontSettings f){ this->treeFont=f; }




// -------------------------------------------------------------------------------
bool CTuxCardsConfiguration::getBoolValue( eBoolValue eKey ) const
// -------------------------------------------------------------------------------
{
   return mBoolMap[eKey];
}

// -------------------------------------------------------------------------------
void CTuxCardsConfiguration::setBoolValue( eBoolValue eKey, bool bValue )
// -------------------------------------------------------------------------------
{
   mBoolMap[eKey] = bValue;
}





// -------------------------------------------------------------------------------
QString CTuxCardsConfiguration::getStringValue( eStringValue eKey ) const
// -------------------------------------------------------------------------------
{
   return mStringMap[eKey];
}

// -------------------------------------------------------------------------------
void CTuxCardsConfiguration::setStringValue( eStringValue eKey, const QString& sValue )
// -------------------------------------------------------------------------------
{
   mStringMap[eKey] = sValue;
}





// -------------------------------------------------------------------------------
int CTuxCardsConfiguration::getIntValue( eIntValue eKey ) const
// -------------------------------------------------------------------------------
{
   return mIntMap[eKey];
}

// -------------------------------------------------------------------------------
void CTuxCardsConfiguration::setIntValue( eIntValue eKey, int iValue )
// -------------------------------------------------------------------------------
{
   mIntMap[eKey] = iValue;
}





// -------------------------------------------------------------------------------
bool CTuxCardsConfiguration::askForUsingEncryption()
// -------------------------------------------------------------------------------
{
   if ( getBoolValue(B_ENCRYPTION_USED) )
   {
      return true;
   }

   int iUseEncryption = QMessageBox::warning( 0, "TuxCards",
                           "You are about to use encryption "
                           "for the first time.<p>"
                           "The encryption algorithm was written "
                           "with the best intents nevertheless "
                           "err is human.<p>"
                           "Therefore, you are solely responsible for "
                           "using this feature. This includes possible "
                           "errors within the algorithm, loss "
                           "of data, forgotten passwords, etc.<p>"
                           "Are you sure that you want to use the "
                           "encryption feature?",
                           QMessageBox::Yes, QMessageBox::No );

   if ( QMessageBox::Yes == iUseEncryption )
   {
      setBoolValue( B_ENCRYPTION_USED, true );
      saveToFile();
      return true;
   }

   return false;
}
