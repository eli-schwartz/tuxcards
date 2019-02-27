/***************************************************************************
                          BookmarkButton.h  -  description
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
#ifndef BOOKMARK_BUTTON_H
#define BOOKMARK_BUTTON_H

#include <qpushbutton.h>
//Added by qt3to4:
#include <QPixmap>
#include <iostream>

#include "../../information/Path.h"

class BookmarkButton : public QPushButton{
  Q_OBJECT
public:
  BookmarkButton(QPixmap pixmap, QString text, QWidget* parent, Path aPath);
  ~BookmarkButton();

private slots:
  void sendActivateSignal();


private:
  Path* path;


signals:
  void activatedSignal(Path*);

};
#endif

