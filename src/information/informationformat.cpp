/***************************************************************************
                          informationformat.cpp  -  mDescription
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

#include "../global.h"
#include "informationformat.h"
#include <QPixmap>

#include "format_none.xpm"
#include "format_ascii.xpm"
#include "format_rtf.xpm"

/*const*/ InformationFormat* InformationFormat::NONE  = NULLPTR;
/*const*/ InformationFormat* InformationFormat::ASCII = NULLPTR;
/*const*/ InformationFormat* InformationFormat::RTF   = NULLPTR;


// -------------------------------------------------------------------------------
InformationFormat::InformationFormat(QString description, QImage image)
 : mDescription( description )
 , mImage( image )
// -------------------------------------------------------------------------------
{
}


/**
 * Two InformationFormats are equal if the mDescription is equal.
 */
// -------------------------------------------------------------------------------
bool InformationFormat::equals( InformationFormat* pAnotherFormat )
// -------------------------------------------------------------------------------
{
   if ( !pAnotherFormat )
      return false;
      
   return ( toString() == pAnotherFormat->toString() );
}


// -------------------------------------------------------------------------------
InformationFormat* InformationFormat::canbeConvertedTo( void )
// -------------------------------------------------------------------------------
{
   // needs to be implemented
   return this;
}


// -------------------------------------------------------------------------------
QPixmap InformationFormat::getPixmap( void )
// -------------------------------------------------------------------------------
{
   QPixmap pixmap;
   pixmap.fromImage( mImage, Qt::AutoColor );
   return pixmap;
}


// -------------------------------------------------------------------------------
QString InformationFormat::toString( void )
// -------------------------------------------------------------------------------
{
   return mDescription;
}



/**
 * Returns a format that was parsed from the string 'format'; if
 * no informationFormat was recognized -> 'NONE' is returned.
 */
// -------------------------------------------------------------------------------
InformationFormat* InformationFormat::getByString( const QString& sFormat )
// -------------------------------------------------------------------------------
{
   static bool static_bInitializationDone = false;
   if ( !static_bInitializationDone )
   {
      NONE  = new InformationFormat( "NONE",  QImage(format_none_xpm)  );
      ASCII = new InformationFormat( "ASCII", QImage(format_ascii_xpm) );
      RTF   = new InformationFormat( "RTF",   QImage(format_rtf_xpm)   );
      
      static_bInitializationDone = true;
   }
   
  if ( sFormat == "RTF")
    return RTF;
  else if ( sFormat == "ASCII")
    return ASCII;
  else
    return NONE;
}
