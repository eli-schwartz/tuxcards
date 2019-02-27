/***************************************************************************
                          fontsettings.h  -  description
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

#ifndef FONTSETTINGS_H
#define FONTSETTINGS_H

#include <qstring.h>
#include <qfont.h>

class FontSettings {
public: 
	FontSettings();
  FontSettings(QString family, int size, bool bold,
               bool italic, bool underline, bool strikeout);
  QFont toFont();               

  QString getFamily();
  int     getSize();
  bool    isBold();
  bool    isItalic();
  bool    isUnderlined();
  bool    isStrikeout();

  void    setFamily(QString family);
  void    setSize(int size);
  void    setBold(bool bold);
  void    setItalic(bool italic);
  void    setUnderlined(bool underlined);
  void    setStrikeout(bool strikeout);

private:
	QString family;
	int			size;
	bool		bold;
	bool		italic;
	bool		underline;
	bool		strikeout;

};

#endif
