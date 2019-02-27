/***************************************************************************
                          CColorBar.cpp  -  description
                             -------------------
    begin                : Sun Mar 26 2000
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
#include "CColorBar.h"

#include <qpainter.h>
#include <qfontmetrics.h>
//Added by qt3to4:
#include <QPaintEvent>

#include <iostream>

/**
 * Default-constructor with default values for the colors and the strings.
 * the properties should be reset with 'change(..)'
 */
// -------------------------------------------------------------------------------
CColorBar::CColorBar( QWidget* pParent )
 : QWidget( pParent )
 , mColorTop( 123,34,76 )
 , mColorBottom( 30,30,30 )
 , mFontColor( Qt::white )
 , msTextTop( "Test" )
 , msTextBottom( "Bar" )
 , msVerticalText( "" )
 , mbAlignVerticalText( ALIGN_BOTTOM )
// -------------------------------------------------------------------------------
{
}

// -------------------------------------------------------------------------------
CColorBar::CColorBar( QWidget* pParent,  const QColor& c1,  const QColor& c2,
                    const QString& t1, const QString& t2, const QColor& c3 )
 : QWidget( pParent )
 , mColorTop( c1 )
 , mColorBottom( c2 )
 , mFontColor( c3 )
 , msTextTop( t1 )
 , msTextBottom( t2 )
 , msVerticalText( "" )
 , mbAlignVerticalText( ALIGN_BOTTOM )
// -------------------------------------------------------------------------------
{
}


// -------------------------------------------------------------------------------
void CColorBar::change( const QColor& c1,  const QColor& c2,
                        const QString& t1, const QString& t2, const QColor& c3 )

// -------------------------------------------------------------------------------
{
   mColorTop    = c1;
   mColorBottom = c2;
   mFontColor   = c3;

   msTextTop    = t1;
   msTextBottom = t2;

   CColorBar::TextWidth width = calcWidth();
   setMinimumWidth( width.maxWidth );
   setMaximumWidth( width.maxWidth );
   update();
}



// dummy method
// -------------------------------------------------------------------------------
void CColorBar::setFlowerDirectory( const QString& )
// -------------------------------------------------------------------------------
{}



/**
 * lets the user set a vertical Text; this one must be given in 'text'.
 * the argument 'top_bottom' indicates wether 'text' is aligned to the
 * top(=false) of the colorbar or to the bottom(=true)
 */
// -------------------------------------------------------------------------------
void CColorBar::setVerticalText( const QString& sText, bool bAlign )
// -------------------------------------------------------------------------------
{
   msVerticalText      = sText;
   mbAlignVerticalText = bAlign;
}


// -------------------------------------------------------------------------------
CColorBar::TextWidth CColorBar::calcWidth() const
// -------------------------------------------------------------------------------
{
   TextWidth textWidth = {0,0,0};

   // calculate minimum width
   QFont f= /*p.*/font();
   f.setBold(true);
   QFontMetrics fm(f);
   textWidth.text1Width = fm.width( msTextTop );
   textWidth.text2Width = fm.width( msTextBottom );

   textWidth.maxWidth = qMax( textWidth.text1Width, textWidth.text2Width );
   if ( textWidth.maxWidth < VERTICAL_TEXT_SIZE )
      textWidth.maxWidth = VERTICAL_TEXT_SIZE + 5;

   return textWidth;
}


// -------------------------------------------------------------------------------
void CColorBar::paintEvent( QPaintEvent* )
// -------------------------------------------------------------------------------
{
   QPainter p(this);                           // our painter
   paint(p);
}


// -------------------------------------------------------------------------------
void CColorBar::paint( QPainter& p )
// -------------------------------------------------------------------------------
{
   // calculate width
   CColorBar::TextWidth textWidth = calcWidth();

   QFont f=p.font(); f.setBold(true);
   p.setFont(f);

   int h=height();

   // define BasisColor & DestinationColor
   int rBasis=mColorTop.red(),  gBasis=mColorTop.green(),  bBasis=mColorTop.blue();
   int rDest=mColorBottom.red(),gDest=mColorBottom.green(),bDest=mColorBottom.blue();

   // difference between BasisColor & DestinationColor
   int rDiff=rDest-rBasis;
   int gDiff=gDest-gBasis;
   int bDiff=bDest-bBasis;

   int stepSize=4;
   int y;
   for (y=0; y<h; y+=stepSize)
   {
      QColor color(rBasis+y*rDiff/h, gBasis+y*gDiff/h, bBasis+y*bDiff/h);
      QBrush b(color);
      p.fillRect(0,y, textWidth.maxWidth,y+stepSize, b);
   }

   // writing
   p.setPen( mFontColor );
   p.drawText( textWidth.maxWidth/2 - textWidth.text1Width/2, h-30, msTextTop );
   p.drawText( textWidth.maxWidth/2 - textWidth.text2Width/2, h-15, msTextBottom );

   // vertical Text
   if(msVerticalText!="")
   {
      f.setBold(true); f.setPointSize(VERTICAL_TEXT_SIZE); p.setFont(f);
      QFontMetrics fm(f);

      int width=fm.width(msVerticalText);
      int x = ( mbAlignVerticalText==ALIGN_TOP ?  -1 * width :  -1 * this->height() );

      p.rotate(-90);
      p.drawText(x, this->width()/2 + fm.height()/2-3, msVerticalText);
      p.rotate(90);                       // rotate back for the other drawing-things
   }
   // TODO: The vertical text is not really positioned within the middle of the bar.
}
