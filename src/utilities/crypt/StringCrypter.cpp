/***************************************************************************
                          StringCrypter.cpp  -  description
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

#include "BlowFish.h"
#include "MD5.h"

#include "qtextstream.h"
#include "qstring.h"
#include <iostream>

#include "StringCrypter.h"


const int StringCrypter::BUFFER_SIZE_IN  = 256;
const int StringCrypter::BUFFER_SIZE_OUT = 512;

#define TUX_ENCRYPT_HEADER "Fh_enc:BF10"


// TOFIX aditional parameter for output path
int StringCrypter::decryptString( const QByteArray& byteArray,
                                  const QString& sPassWd, QString& sOutputString )
{
   unsigned char buf_in[BUFFER_SIZE_IN];
   unsigned char buf_out[BUFFER_SIZE_OUT];

   CBlowFish fish;
   fish.Initialize( (unsigned char *)sPassWd.toLatin1().data(), sPassWd.length() );

   // check for header string
   char sHeader[15];
   for ( uint i = 0; i < strlen(TUX_ENCRYPT_HEADER); i++ )
   {
      sHeader[i] = byteArray[i];
   }


   if( 0 != strncmp(sHeader, TUX_ENCRYPT_HEADER, strlen(TUX_ENCRYPT_HEADER)) )
   {
      // the given string was not compressed with this code
      std::cout<<"Invalid file header (not encrypted with TuxCards)!"<<std::endl;
      return ERROR_INVALID_FILEHEADER;
   }



   // read hash value from file (md5 algorithm)
   unsigned char sHash[16];
   memset( sHash, 0, 16 );
   int offset = strlen(TUX_ENCRYPT_HEADER);
   for ( int i = 0; i < 16; i++ )
   {
      sHash[i] = byteArray[offset + i];
   }


   // calculate this-password hash value
   CMD5 md5;
   md5.MD5Init();
   md5.MD5Update( (unsigned char *)sPassWd.toLatin1().data(), sPassWd.length() );
   md5.MD5Final();

   // compare hash values to quickly validate given password
   if( 0 != memcmp(sHash, md5.GetDigestBinary(), 16) )
   {
      std::cout<<"Wrong password!"<<std::endl;
      return ERROR_INVALID_PASSWD;
   }

   union aInt iSize;
   iSize.i.byte0 = byteArray[offset+16+0];
   iSize.i.byte1 = byteArray[offset+16+1];
   iSize.i.byte2 = byteArray[offset+16+2];
   iSize.i.byte3 = byteArray[offset+16+3];


   int iIndexOfAsterisc = offset+16+3+1;
   QByteArray tmpArray( byteArray.size() - iIndexOfAsterisc - 1, (char)0 );
   for ( int i = iIndexOfAsterisc+1; i < byteArray.size(); i++ )
   {
      tmpArray[i-iIndexOfAsterisc-1] = byteArray[i];
   }


   QByteArray leftArray;
   getLeftBytes( leftArray, tmpArray, BUFFER_SIZE_IN );

   memset( buf_in, 0, BUFFER_SIZE_IN );
   memcpy( buf_in, leftArray.data(), leftArray.size() );

   sOutputString = "";


   while ( leftArray.size() > 0 )
   {
      memset( buf_out, 0, BUFFER_SIZE_OUT );
      fish.Decode( buf_in, buf_out, leftArray.size() );

      sOutputString += (char*)buf_out;

      getLeftBytes( leftArray, tmpArray, BUFFER_SIZE_IN );

      memset( buf_in, 0, BUFFER_SIZE_IN );
      memcpy( buf_in, leftArray.data(), leftArray.size() );
   }

   return NO_ERROR;
}


/**
 * Auxiliary method to retrieve the first 'len' left bytes from a byte array ('source').
 * The result is put into 'dest'.
 * The read bytes are removed from 'source'.
 *
 * The number of bytes copied, can be retrieved via 'dest.size()'.
 */
void StringCrypter::getLeftBytes( QByteArray& dest, QByteArray& source, uint len )
{
   dest.resize(0);

   if ( (uint)source.size() < len )
      len = source.size();

   dest.resize(len);
   for ( uint i = 0; i < len; i++ )
   {
      dest[i] = source[i];
   }

   QByteArray tmp( source.size() - len, (char)0 );
   for ( int i = 0; i < tmp.size(); i++ )
   {
      tmp[i] = source[i + len];
   }

   source.resize(0);
   source.resize(tmp.size());
   for ( int i = 0; i < tmp.size(); i++ )
   {
      source[i] = tmp[i];
   }
}



void StringCrypter::encryptString( const QString& sInputString, const QString& sPassWd,
                                   QByteArray& byteArray )
{
   if ( sPassWd.isNull() || sPassWd == "" )
   {
      std::cerr<<"TuxCards-ERROR: no passwd for '"
              <<sInputString.toStdString()<<"' given!"
              <<std::endl;
      return;
   }

   // calculate hash-value from given password
   CMD5 md5;
   md5.MD5Init();
   md5.MD5Update((unsigned char *)sPassWd.toLatin1().data(), sPassWd.length());
   md5.MD5Final();

   //NOTE: important => buffer size must be multiple of 8 bytes (block cypher)
   unsigned char buf_in[BUFFER_SIZE_IN];
   unsigned char buf_out[BUFFER_SIZE_OUT];

   CBlowFish fish;
   fish.Initialize( (unsigned char*)sPassWd.toLatin1().data(), sPassWd.length() );


   byteArray.resize(0);

   // write header string + hash + original file size
   byteArray.append( TUX_ENCRYPT_HEADER );

   // write password hash value (md5 algorithm)
   unsigned char passWordHashValue[16];
   memset( passWordHashValue, 0, 16 );
   memcpy( passWordHashValue, md5.GetDigestBinary(), 16);
   for ( int i = 0; i < 16; i++ )
   {
      byteArray.append( (char)passWordHashValue[i] );
   }

   // write original file size
   union aInt iSize;
   iSize.theInt = sInputString.length();

   byteArray.append( (char)iSize.i.byte0 );
   byteArray.append( (char)iSize.i.byte1 );
   byteArray.append( (char)iSize.i.byte2 );
   byteArray.append( (char)iSize.i.byte3 ).append( "*" );


   int iRead, iEncodedSize;
   QString sTmp = sInputString;
   QString sLeftPart = sTmp.left( BUFFER_SIZE_IN );
   memset( buf_in, 0, BUFFER_SIZE_IN );
   memcpy( buf_in, sLeftPart.toLatin1().data(), sLeftPart.length() );

   while( sLeftPart.length() > 0 )
   {
      sTmp = sTmp.mid( BUFFER_SIZE_IN );

      memset( buf_out, 0, BUFFER_SIZE_IN );

      iRead = sLeftPart.length();
      iEncodedSize = fish.Encode( buf_in, buf_out, iRead );
      for ( int i = 0; i < iEncodedSize; i++ )
         byteArray.append( buf_out[i] );

      sLeftPart = sTmp.left( BUFFER_SIZE_IN );
      memset( buf_in, 0, BUFFER_SIZE_IN );
      memcpy( buf_in, sLeftPart.toLatin1().data(), sLeftPart.length() );
   }
}
