/***************************************************************************
                          CBase64Coder.h  -  description
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
#ifndef CBASE64_CODER_H
#define CBASE64_CODER_H


#include "qstring.h"

class CBase64Coder {

public:
   static const CBase64Coder& getInstance();

   void encode( QString& sDest, const QByteArray& source ) const;
   void decode( QByteArray& dest, const QString& sSource ) const;

private:
   CBase64Coder();
   int decodeChar( char cBase64Char ) const;

   static CBase64Coder* static_pInstance;
};

#endif

