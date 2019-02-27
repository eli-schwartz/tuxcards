/***************************************************************************
                          fontsettings.cpp  -  description
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

#include "fontsettings.h"

FontSettings::FontSettings(){
  family="Helvetica";
  size=12;
  bold=false;
  italic=false;
  underline=false;
  strikeout=false;
}

FontSettings::FontSettings(QString family, int size, bool bold,
                           bool italic, bool underline, bool strikeout)
{
  this->family=family;
  this->size=size;
  this->bold=bold;
  this->italic=italic;
  this->underline=underline;
  this->strikeout=strikeout;
}

QFont FontSettings::toFont(){
  QFont font=QFont(family, size);
  font.setBold(bold);
  font.setItalic(italic);
  font.setUnderline(underline);
  font.setStrikeOut(strikeout);
  return font;
}
           

QString FontSettings::getFamily(){ return family; }
int FontSettings::getSize(){ return size; }
bool FontSettings::isBold(){ return bold; }
bool FontSettings::isItalic(){ return italic; }
bool FontSettings::isUnderlined(){ return underline; }
bool FontSettings::isStrikeout(){ return strikeout; }

void FontSettings::setFamily(QString family){ this->family=family; }
void FontSettings::setSize(int size){ this->size=size; }
void FontSettings::setBold(bool bold){ this->bold=bold; }
void FontSettings::setItalic(bool italic){ this->italic=italic; }
void FontSettings::setUnderlined(bool underlined){ this->underline=underlined; }
void FontSettings::setStrikeout(bool strikeout){ this->strikeout=strikeout; }
