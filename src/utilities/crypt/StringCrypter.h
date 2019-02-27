/***************************************************************************
                          StringCrypter.h  -  description
                             -------------------
    begin                : Fri Jul 18 2003
    copyright            : (C) 2003 by Alexander Theel
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
#ifndef STRING_CRYPTER_H
#define STRING_CRYPTER_H

#include <qstring.h>


class StringCrypter{
public:
   static int decryptString( const QByteArray& byteArray, const QString& sPassWd, QString& sOutputString );

   static void encryptString( const QString& sInputString, const QString& sPassWd,
                              QByteArray& byteArray );

   // Errorcodes
   enum {
      NO_ERROR                 =  0,
      ERROR_INVALID_FILEHEADER = -1,
      ERROR_INVALID_PASSWD     = -2
   };

private:
   static void getLeftBytes( QByteArray& dest, QByteArray& source, uint len );

   union aInt
   {
      int theInt;
      struct {
         /*unsigned*/ int byte3:8;
         /*unsigned*/ int byte2:8;
         /*unsigned*/ int byte1:8;
         /*unsigned*/ int byte0:8;
     } i;
   };

   static const int BUFFER_SIZE_IN;
   static const int BUFFER_SIZE_OUT;
};

#endif

