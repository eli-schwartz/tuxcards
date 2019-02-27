/***************************************************************************
                          BookmarkButton.cpp  -  description
                             -------------------
    begin                : Fri Jan 31 2003
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

#include "BookmarkButton.h"
//Added by qt3to4:
#include <QPixmap>


// -------------------------------------------------------------------------------
BookmarkButton::BookmarkButton(QPixmap pixmap, QString text, QWidget* parent,
                               Path aPath)
 :QPushButton(pixmap, text, parent)
// -------------------------------------------------------------------------------
{
  this->path = new Path(aPath.toString());
  connect( this, SIGNAL(clicked()), this, SLOT(sendActivateSignal()) );
}


// -------------------------------------------------------------------------------
BookmarkButton::~BookmarkButton()
// -------------------------------------------------------------------------------
{
}


// -------------------------------------------------------------------------------
void BookmarkButton::sendActivateSignal()
// -------------------------------------------------------------------------------
{
  emit activatedSignal(path);
}
