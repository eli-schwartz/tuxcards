/***************************************************************************
                          CPasswdDialog.h  -  description
                             -------------------
    begin                : Wed Jan 14 2004
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
#ifndef CPASSWDDIALOG_H
#define CPASSWDDIALOG_H

#include "../../../global.h"
#include "../../../../output/ui/ui_IPasswdDialog.h"
#include <QString>


class CPasswdDialog : public QDialog {
  Q_OBJECT
public:
  CPasswdDialog( QWidget* pParent = NULLPTR );

  void               setUp( const QString& sIEDescription );
  QString            getPasswd();

protected slots:
   void              slotVerifyPasswd();

private:
   void              startPasswdInput();
   QString           msPasswd;

   Ui::IPasswdDialog ui;
};

#endif
