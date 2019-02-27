/***************************************************************************
                          cactusbar.h  -  description
                             -------------------
    begin                : Mon Apr 10 2000
    copyright            : (C) 2000 by Alexander Theel
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

#ifndef CACTUSBAR_H
#define CACTUSBAR_H

#include "../colorbar/CColorBar.h"
#include "cactus.h"

#include <qpixmap.h>
//Added by qt3to4:
#include <QResizeEvent>
#include <QPaintEvent>

class CCactusBar : public CColorBar {
public:
   CCactusBar( QWidget* );
   CCactusBar( QWidget*, const QColor&, const QColor&, const QString&, const QString& );
   ~CCactusBar( void );

   void    toggleCactus( bool );
   void    paint( QPainter* pPainter );

   virtual void setFlowerDirectory( const QString& );

protected:
   Cactus* mpCactus;
   QPixmap mDoubleBuffer;
   bool    mbDrawCactus;

   void    paintEvent( QPaintEvent* );
   void    resizeEvent( QResizeEvent* );
};

#endif
