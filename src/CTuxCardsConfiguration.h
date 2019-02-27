/***************************************************************************
                          CTuxCardsConfiguration.h  -  description
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

#ifndef CTUXCARDS_CONFIGURATION_H
#define CTUXCARDS_CONFIGURATION_H

#include <qstring.h>
#include <qcolor.h>
#include "fontsettings.h"

#include <qmap.h>

class CTuxCardsConfiguration {
public:
  static CTuxCardsConfiguration& getInstance();

  void saveToFile();

  enum eBoolValue {
    B_AUTOSAVE,
    B_SAVE_WHEN_LEAVING,
    B_CREATE_BACKUP_FILE,

    B_IS_HTEXT_ENABLED,
    B_IS_VTEXT_ENABLED,
    B_ALIGN_VTEXT,

    B_IS_CACTUSBAR_ENABLED,

    B_SHOW_MAIN_TOOLBAR,
    B_SHOW_ENTRY_TOOLBAR,
    B_SHOW_EDITOR_TOOLBAR,
    B_SHOW_RECENTFILES_TOOLBAR,

    B_LINEBREAK_WITH_CTRL_SHIFT,
    B_ENCRYPTION_USED,
    B_AUTOENCRYPT_ENTRY,

    B_SHOW_DONATION_MSG_2
  };
  bool    getBoolValue( eBoolValue eKey ) const;
  void    setBoolValue( eBoolValue eKey, bool bValue );


  enum eStringValue {
    S_DATA_FILE_NAME,
    S_RECENT_FILES,

    S_EXECUTE_STATEMENT,

    S_TEXT_ONE,
    S_TEXT_TWO,
    S_VERTICAL_TEXT,

    S_FLOWER_DIR,
    S_ICON_DIR
  };
  QString getStringValue( eStringValue eKey ) const;
  void    setStringValue( eStringValue eKey, const QString& sValue );


  enum eIntValue {
    I_SAVE_ALL_MINUTES,
    I_TAB_SIZE,
    I_WORD_WRAP,

    I_WINDOW_WIDTH,
    I_WINDOW_HEIGHT,
    I_TREE_WIDTH,
    I_EDITOR_WIDTH,
    I_TREE_VSCROLLBAR_VALUE,
    I_WINDOW_XPOS,
    I_WINDOW_YPOS
  };
  int     getIntValue( eIntValue eKey ) const;
  void    setIntValue( eIntValue eKey, int iValue );


  bool    askForUsingEncryption();

  /****** getter *************************/
  QColor  getTopColor() const;
  QColor  getBottomColor() const;
  QColor  getFontColor() const;

  FontSettings getEditorFont() const;
  FontSettings getTreeFont() const;

  /****** setter *************************/
  void    setTopColor(QColor c);
  void    setBottomColor(QColor c);
  void    setFontColor(QColor c);

  void    setEditorFont(FontSettings f);
  void    setTreeFont(FontSettings f);


private:
  CTuxCardsConfiguration();
  void readConfigurationFile();

  QColor  topColor;
  QColor  bottomColor;
  QColor  fontColor;

  FontSettings editorFont;
  FontSettings treeFont;


  typedef QMap<int, bool> BoolMap;
  BoolMap   mBoolMap;

  typedef QMap<int, QString> StringMap;
  StringMap mStringMap;

  typedef QMap<int, int> IntMap;
  IntMap    mIntMap;
};

#endif
