/***************************************************************************
                          CConfigurationDialog.h  -  description
                             -------------------
    begin                : Sun Aug 12 2007
    copyright            : (C) 2007 by Alexander Theel
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

#ifndef CCONFIGURATION_DIALOG_H
#define CCONFIGURATION_DIALOG_H

#include "../../../../output/ui/ui_IConfigurationDialog.h"

#include <qtabbar.h>

#include <qcheckbox.h>

#include <qlabel.h>
#include <qlineedit.h>
#include <qvalidator.h>

#include <qpushbutton.h>
#include <qcolordialog.h>
#include <qstring.h>
#include <qfont.h>

class CCactusBar;

#include "../../../CTuxCardsConfiguration.h"

#include <iostream>
class CConfigurationDialog : public QDialog {
   Q_OBJECT
public:
   CConfigurationDialog(QWidget* parent, CTuxCardsConfiguration& config);
   int setUp( void );

   bool getAutosave();
   int  getMinutes();
   bool getSaveWhenLeaving();
   bool getCreateBackup();

   QColor  getTopColor();
   QColor  getBottomColor();
   QColor  getBarTextColor();
   bool    getEnableHText();
   QString getTextOne();
   QString getTextTwo();

   bool    getEnableVText();
   QString getVerticalText();
   bool    getAlignVText();

   bool    getEnableCactusBar();
   QString getFlowerDir();

   QFont   getTreeFont();
   QFont   getEditorFont();
   int     getTabSize();
   int     getLineWrap();

private slots:
   virtual void slotChangeTopColor();
   virtual void slotChangeBottomColor();
   virtual void slotChangeTextColor();
   
   virtual void slotChangeTreeFont();
   void         slotChangeEditorFont();
   virtual void chooseFlowerDir();
   
   virtual void changeProperties();

   void         slotApplyChangesToCactusBarPreview();
   void         slotToggleCactusPreview( bool bEnableCactus );

// virtual void setDefaults();

signals:
  void configurationChanged();

private:
   Ui::IConfigurationDialog ui;

   CCactusBar*  mpCactusBarPreview;
   void         addCactusBarToDialog();

   QString      calculateFontString( const QFont& f ) const;

   CTuxCardsConfiguration& mrefConfig;
};

#endif
