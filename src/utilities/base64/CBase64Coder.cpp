/***************************************************************************
                          CBase64Coder.cpp  -  description
                             -------------------
    begin                : Mon Feb 23 2004
    copyright            : (C) 2004 by Alexander Theel
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


#include "../../global.h"
#include "CBase64Coder.h"

#include <iostream>



CBase64Coder* CBase64Coder::static_pInstance = NULLPTR;

static char static_base64Table[] =
{  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
   'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
   'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
   't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
   '8', '9', '+', '/'
};

// -------------------------------------------------------------------------------
// Private constructor.
// -------------------------------------------------------------------------------
CBase64Coder::CBase64Coder()
// -------------------------------------------------------------------------------
{
   static_pInstance = this;
}

// -------------------------------------------------------------------------------
const CBase64Coder& CBase64Coder::getInstance()
// -------------------------------------------------------------------------------
{
   if ( NULLPTR == static_pInstance )
   {
      (void) new CBase64Coder();
      if ( NULLPTR == static_pInstance )
      {
         std::cerr<<"ERROR: CBase64Coder::getInstance() - \
                     not enough memory to allocate object!!"<<std::endl;
      }
   }

   return *static_pInstance;
}


// -------------------------------------------------------------------------------
// The string 'sDest' is emptied and afterwards filled with base64 characters
// representing the information kept in 'source'.
// -------------------------------------------------------------------------------
void CBase64Coder::encode( QString& sDest, const QByteArray& source ) const
// -------------------------------------------------------------------------------
{
   //std::cout<<"source before base64-en:"<<std::endl;
   //for ( int i=0; i < (int)source.size(); i++)
   //   std::cout<<i<<": "<<source[i]<<" "<<(int)source[i]<<std::endl;

   sDest = "";

   unsigned long int iData   = 0;

   int iLengthOfCurrentLine  = 0;
   int iPaddingCount         = 0;
   int iCurrentSourcePos     = 0;

   while ( source.size() > iCurrentSourcePos )
   {
      iData = 0;
      unsigned char cSourceChar = source[iCurrentSourcePos++];
      //printf( "\nsource[%d]=0x%x / %d",iCurrentSourcePos-1,cSourceChar,cSourceChar,cSourceChar);
      iData = iData | ((unsigned long int) cSourceChar << (8 * 2));
      //printf( " iData=0x%x\n", iData);





      for ( int i = 1; i >= 0; i-- )
      {
         if ( source.size() > iCurrentSourcePos )
         {
            unsigned char cSourceChar = source[iCurrentSourcePos++];
            //printf( "source[%d]=0x%x / %d",iCurrentSourcePos-1,cSourceChar,cSourceChar,cSourceChar);
            iData = iData | ((unsigned long int) cSourceChar << (8 * i));
            //printf( " iData=0x%x\n", iData);
         }
         else
         {
            iPaddingCount++;
         }
      }

      for ( int i = 3; i >= iPaddingCount; i-- )
      {
         unsigned char cDestChar = 63 & (iData >> (6 * i));
         sDest += static_base64Table[cDestChar];
         //printf("   lookingUp: %d / 0x%x |->%c\n", cDestChar, cDestChar, static_base64Table[cDestChar] );

         if (++iLengthOfCurrentLine == 76)
         {
            sDest += '\n';
            iLengthOfCurrentLine = 0;
         }
      }

      for ( int i = 0; i < iPaddingCount; i++ )
      {
         sDest += '=';
      }
   }
}


// -------------------------------------------------------------------------------
void CBase64Coder::decode( QByteArray& dest, const QString& sSource ) const
// -------------------------------------------------------------------------------
{
   dest.resize(0);
   dest.resize(sSource.length());        // Resize to a bigger array than needed,
                                         // shrink it later on.
   unsigned long int iData = 0;
   int iCurrentSourcePos   = 0;
   uint uiCurrentDestPos   = 0;
   int iPaddingCount       = 0;


   while ( sSource.length() > iCurrentSourcePos )
   {
      iData = 0;
      for ( int i = 3; i >= 0; i-- )
      {
         if ( sSource.length() <= iCurrentSourcePos )
            break;

         unsigned char cSourceChar = sSource[iCurrentSourcePos++].toAscii();
         if ( ('\n' != cSourceChar) && ('\r' != cSourceChar) )
         {
               if (('=' != cSourceChar) && (sSource.length() > iCurrentSourcePos))
               {
                  iData = iData | ((long int) decodeChar((char)cSourceChar) << (i * 6));
                  //printf("   lookingUp: %c |->%d / 0x%x\n", (char)cSourceChar, decodeChar((char)cSourceChar), decodeChar((char)cSourceChar) );
               }
               else
               {
                  iPaddingCount++;
               }
         }
         else
         {
            i++;
         }
      }

      //printf( "decode-pos=%d iData=0x%x\n", uiCurrentDestPos, iData);
      for ( int i = 2; i >= iPaddingCount; i--)
      {
         char cDestChar = (iData >> (i * 8));
         dest[uiCurrentDestPos++] = cDestChar;
      }
   }

   // shrink destination to actual size
   dest.resize(uiCurrentDestPos);

   //std::cout<<"dest after base64-de"<<dest<<std::endl;
   //for ( int i=0; i < (int)dest.size(); i++)
   //   std::cout<<i<<": "<<dest[i]<<" "<<(int)dest[i]<<std::endl;
}


// -------------------------------------------------------------------------------
int CBase64Coder::decodeChar( char cBase64Char ) const
// -------------------------------------------------------------------------------
{
   int iIndex;
   for ( iIndex = 0; (static_base64Table[iIndex] != cBase64Char)
                     && (iIndex < (int)ARRAYLEN(static_base64Table)); iIndex++ )
      ; // empty for-loop

   return iIndex;
}
