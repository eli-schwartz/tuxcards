/***************************************************************************
                          informationformat.cpp  -  description
                             -------------------
    begin                : Sat Jul 13 2002
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

#include "converter.h"

#include <qregexp.h>
#include <qmessagebox.h>

/**
 * Takes an informationElement as parameter, recognizes whether it is
 * an ascii
 */
// -------------------------------------------------------------------------------
void Converter::convert( CInformationElement& informationElement )
// -------------------------------------------------------------------------------
{
   QString resultingText;
   if (informationElement.getInformationFormat() == InformationFormat::RTF)
   {
      resultingText = convertRTF2ASCII( informationElement.getInformation() );
      informationElement.setInformationFormat( InformationFormat::ASCII );
   }
   else
   {
      resultingText = convertASCII2RTF( informationElement.getInformation() );
      informationElement.setInformationFormat( InformationFormat::RTF );
   }
   informationElement.setInformation(resultingText);
}



/**
 * Converts ASCII-Text to RichtText i.e. not to loos ascii formating.
 * (if not done; everything will be interpreted as rtf/html)
 */
// -------------------------------------------------------------------------------
QString Converter::convertASCII2RTF(QString asciiText)
// -------------------------------------------------------------------------------
{
   //QString result = QStyleSheet::convertFromPlainText(asciiText);
   return asciiText.replace( QRegExp( "\n" ), "<br/>" );
}



/**
 * Converts RichText to Ascii-Text.
 */
// -------------------------------------------------------------------------------
QString Converter::convertRTF2ASCII(QString rtfText)
// -------------------------------------------------------------------------------
{
   //QString result = rtfText.replace( QRegExp( "<p>" ), "" );
   return rtfText;

   /*
      Idee:
      alle Tags entfernen
         <html>
         <head>              |-> ""
         <meta>
         <body ...>

         <p>  |-> ""   </p>  |-> "\n\n"
         <br />  |-> "\n"
         <span ...>  |-> ""   </span>  |->  ""
         <li> |-> "- "   </li>  |-> "\n"
         <ul ...>  |-> ""   </ul>  |-> ""
   */
}
