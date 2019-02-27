/***************************************************************************
                          CColorBar.h  -  description
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
#ifndef CCOLORBAR_H
#define CCOLORBAR_H

#include <qwidget.h>
//Added by qt3to4:
#include <QPaintEvent>
class QPainter;

#include <qfont.h>
#include <qstring.h>

class CColorBar : public QWidget{

public:
   enum {ALIGN_TOP=0, ALIGN_BOTTOM};

   CColorBar( QWidget* pParent );
   CColorBar( QWidget* pParent, const QColor&,  const QColor&,
              const QString&,   const QString&, const QColor& fontColor=QColor(Qt::white) );

   void         change( const QColor&,  const QColor&,
                        const QString&, const QString&, const QColor& );
   void         setVerticalText( const QString&, bool bAlign = ALIGN_BOTTOM );
   virtual void paint( QPainter& );

   virtual void setFlowerDirectory( const QString& );             // dummy method

protected:
   QColor  mColorTop;
   QColor  mColorBottom;
   QColor  mFontColor;
   QString msTextTop;
   QString msTextBottom;

   QString msVerticalText;
   bool    mbAlignVerticalText;
   static  const int VERTICAL_TEXT_SIZE=24;

   virtual void paintEvent( QPaintEvent* );

   struct TextWidth
   {
      int maxWidth;
      int text1Width;
      int text2Width;
   };
   TextWidth   calcWidth() const;
};

#endif
