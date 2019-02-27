/***************************************************************************
                          informationformat.h  -  description
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
#ifndef INFORMATION_FORMAT_H
#define INFORMATION_FORMAT_H

#include <iostream>
#include <qstring.h>
#include <QImage>

class InformationFormat{
public:
   static InformationFormat* getByString( const QString& sFormat );
   
   bool equals( InformationFormat* );

   InformationFormat* canbeConvertedTo( void );

   QPixmap getPixmap( void );
   QString toString( void );

private:
   QString mDescription;
   QImage  mImage;


   /**
    * These must be pointers. If these are "non-pointer" variables then the message
    * "QPaintDevice: Must construct a QApplication before a QPaintDevice" appears
    * within the shell and the application cannot be started.
    *
    * Since an <code>InformationFormat</code> contains a <code>QImage</code>
    * (i.e. a <code>QPaintDevice</code>), the static members must be initialized
    * after the application was started. This is done via "pointer-variables".
    */   
   static /*const*/ InformationFormat* NONE;
   static /*const*/ InformationFormat* ASCII;
   static /*const*/ InformationFormat* RTF;


   InformationFormat( QString, QImage );
   //~InformationFormat();

};

#endif

