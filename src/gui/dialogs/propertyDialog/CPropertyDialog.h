/***************************************************************************
                          CPropertyDialog.h  -  description
                             -------------------
    begin                : Tue Mar 28 2000
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

#ifndef C_PROPERTY_DIALOG_H
#define C_PROPERTY_DIALOG_H

#include "../../../../output/ui/ui_IPropertyDialog.h"

#include <QDate>
#include <QPixmap>

class CInformationElement;
class CTuxCardsConfiguration;
class CDateInputDialog;
class CIconSelectorDialog;
class InformationFormat;


class CPropertyDialog : public QDialog {
   Q_OBJECT
public:
   ~CPropertyDialog();
   
   static void               createInstance( QWidget* pParent );
   static CPropertyDialog*   getInstance();
   
   enum {
      MODE_NONE,
      MODE_CHANGE_PROPERTIES,
      MODE_CREATE_NEW_ELEMENT
   };
   int                       getMode() const;
   
   void                      setUp( CInformationElement* pElement, int iMode );
   
   QString                   getName() const;
   QString                   getIconFileName() const;
   InformationFormat*        getInformationFormat() const;
   QColor                    getDescriptionColor() const;
   bool                      hasExpiryDateSet() const;
   QDate                     getExpiryDate() const;


signals:
    void                     dataSuccessfullyEntered();
    
private slots:
   void                      changeProperties();

   void                      openDateInputDialog();
   void                      changeExpiryDate();
   
   void                      chooseIcon();
   void                      iconSelected();

   void                      slotChooseDescriptionColor();

//   void checkEncryption();

private:
   CPropertyDialog( QWidget* pParent/*, CTuxCardsConfiguration& refTuxConfiguration*/ );
   static CPropertyDialog*   static_pInstance;

   QDate                     mExpiryDate;
   CDateInputDialog*         mpDateInputDialog;
   
   QPixmap                   mBlankIcon;
   CIconSelectorDialog*      mpIconSelectorDialog;


   void                      setAttributes( QString, QString,
                                            bool bExpires = false, QDate expiryDate = QDate::currentDate(),
                                            const QColor& desColor = QColor(0,0,0)/*,
                                            bool bIsEncryptionEnabled = false,
                                            bool isCurrentlyEncrypted = true*/ );
   int                       miMode;
   int                       miChoice;

   CInformationElement*      mpEditingElement;

   QColor                    mDescriptionColor;
   void                      setDescriptionColor( const QColor& c );
   
//   CTuxCardsConfiguration&   mrefTuxConfiguration;

   Ui::IPropertyDialog ui;
};

#endif
