/***************************************************************************
                          strings.cpp  -  description
                             -------------------
    begin                : Fri Jul 19 2002
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

#include "strings.h"
#include <iostream>

#include <qregexp.h>

/**
 * returns a string that has length 'n' and is filled with spaces
 */
// -------------------------------------------------------------------------------
QString Strings::spaces(int n)
// -------------------------------------------------------------------------------
{
  QString result="";
  for (int i=0; i<n; i++)
    result+=" ";

  return result;
}


/**
 * Counts the number of words within the given string and
 * returns it.
 */
// -------------------------------------------------------------------------------
int Strings::wordCount( QString sText )
// -------------------------------------------------------------------------------
{
   sText = sText.simplified();
   if ( sText.isEmpty() )
      return 0;
   
   int iWords = 0;
   int iCurrentPos = 1;
   
   while ( (iCurrentPos = sText.indexOf(" ", iCurrentPos) +1) > 0 )
   {
      iWords++;
   }
   iWords++;
   
   return iWords;
}


/**
 * Removes the first line from the given string and returns that line.
 * Notice, the given string *is* changed.
 */
// -------------------------------------------------------------------------------
QString Strings::removeAndReturnFirstLine(QString& s)
// -------------------------------------------------------------------------------
{
  int i = s.indexOf("\n");
  if (i < 0 )
    return 0;

  QString retVal = s.left(i);
  s = s.mid(i+1);
  return retVal;
}


/**
 * Removes all html tags within the given string.
 * Any '<br />' will be replaced by a ' ' (Space).
 * Any '<li ... >' will be replaced by a ' <li ... >' (additional Space); in
 * order to count the words within a list correctly.
 */
// -------------------------------------------------------------------------------
QString Strings::removeHTMLTags(const QString sText)
// -------------------------------------------------------------------------------
{
   QString sRetVal = sText;
   
   sRetVal.replace(QRegExp("<br />"), " ");
   sRetVal.replace(QRegExp("<li"), " <li");        // in order to count the words in a list correctly
   
   int iTagStart = sRetVal.indexOf("<");
   int iTagEnd = 0;
   while ( iTagStart >= 0 )
   {
      iTagEnd = sRetVal.indexOf(">", iTagStart);
      sRetVal = sRetVal.remove(iTagStart, iTagEnd-iTagStart+1);
   
      // prepare next turn
      iTagStart = sRetVal.indexOf("<");
   }
   sRetVal = sRetVal.mid(1);           // kill first '\n' which
                                       // does not belong to the
                                       // real information text
   // replace existing encoded tags
   sRetVal.replace(QRegExp("&lt;"), "<");
   sRetVal.replace(QRegExp("&gt;"), ">");
   
   //std::cout<<"___"<<retVal<<"___"<<std::endl;
   return sRetVal;
}
