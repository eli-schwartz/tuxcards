/***************************************************************************
                          CCactusBar.cpp  -  description
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

#include "../../global.h"
#include "CCactusBar.h"
#include <iostream>

#include <qbrush.h>
#include <qtimer.h>

#include <QPixmap>
#include <QResizeEvent>
#include <QPaintEvent>


// -------------------------------------------------------------------------------
CCactusBar::CCactusBar( QWidget* parent )
 : CColorBar( parent )
 , mpCactus( NULLPTR )
 , mDoubleBuffer()
 , mbDrawCactus( true )
// -------------------------------------------------------------------------------
{
   mpCactus = new Cactus( this );
}


// -------------------------------------------------------------------------------
CCactusBar::CCactusBar( QWidget *parent,   const QColor& c1, const QColor& c2,
                      const QString& t1, const QString& t2)
 : CColorBar( parent, c1, c2, t1, t2 )
 , mpCactus( NULLPTR )
 , mDoubleBuffer()
 , mbDrawCactus( true )
// -------------------------------------------------------------------------------
{
   mpCactus = new Cactus( this );

   mDoubleBuffer = QPixmap(170, this->height());        // doubleBuffer with constant width

   connect( mpCactus, SIGNAL(triggerPaint()), this, SLOT(update()) );
   QTimer::singleShot( 1000, mpCactus, SLOT(run()) );   // mpCactus.run(); starts in 1 second
}


// -------------------------------------------------------------------------------
CCactusBar::~CCactusBar( void )
// -------------------------------------------------------------------------------
{
   DELETE( mpCactus );
}


// -------------------------------------------------------------------------------
void CCactusBar::setFlowerDirectory( const QString& sDir )
// -------------------------------------------------------------------------------
{
   if ( NULLPTR != mpCactus )
      mpCactus->setFlowerDirectory( sDir );
}


/**
 * turns the cactus on/off
 */
// -------------------------------------------------------------------------------
void CCactusBar::toggleCactus( bool b )
// -------------------------------------------------------------------------------
{ mbDrawCactus = b; }


// -------------------------------------------------------------------------------
void CCactusBar::paintEvent( QPaintEvent* pE )
// -------------------------------------------------------------------------------
{
//  old code - non-doublebuffering code; in that case
//  only this method is needed; i.e. the paint()-method below
//  is not necessary when doing without a doublebuffer
//
// ColorBar::paintEvent(e);
//
// QPainter p;
// p.begin(this);
//
// mpCactus.paint(&p, QRect(0,0, this->width(),this->height()-45));
// p.end();

   if ( !pE )
      return;

   //QRect r = pE->rect();
   //bitBlt( this, r.x(), r.y(), &mDoubleBuffer, r.x(), r.y(), r.width(), r.height() );

   QPainter painter(this);
   paint( &painter );
   painter.drawPixmap( pE->rect(), mDoubleBuffer, pE->rect() );
}


// -------------------------------------------------------------------------------
void CCactusBar::paint( QPainter* pPainter )
// -------------------------------------------------------------------------------
{
   if ( !mpCactus )
      return;

   mDoubleBuffer.fill( QColor(123,100,35) );

   QPainter p2;
   p2.begin( &mDoubleBuffer );  // start painting
   CColorBar::paint( p2 );
   if ( mbDrawCactus )
      mpCactus->paint( &p2, QRect( 0,0, this->width(),this->height()-45 ) );
   p2.end();                    // painting done

   pPainter->drawPixmap( 0,0, mDoubleBuffer );
}


// -------------------------------------------------------------------------------
void CCactusBar::resizeEvent( QResizeEvent* )
// -------------------------------------------------------------------------------
{
   //mDoubleBuffer.resize(mDoubleBuffer.width(), height());
   mDoubleBuffer = mDoubleBuffer.scaled( mDoubleBuffer.width(), height() );
   update();
}
