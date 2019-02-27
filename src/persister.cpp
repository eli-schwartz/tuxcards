/***************************************************************************
                          persister.cpp  -  description
                             -------------------
    begin                : Fri Jul 19 2002
    copyright            : (C) 2002 by Alexander Theel
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

#include "persister.h"
#include <qfile.h>
//Added by qt3to4:
#include <QTextStream>

CInformationCollection* Persister::createInformationCollection(QString s){
  return unpack(s);
}


/**
 * checks for Version and decides whether to use
 *       unpackWithoutIcons(..)   Version 0.4
 *       unpackWithIcons(..)      Version 0.5
 *
 * if 'x' is specified, then we do not clear the tree and
 * unpack the String using 'x' as parent; otherwise, the
 * complete tree is erased and rebuild from 's'
 */
CInformationCollection* Persister::unpack(QString s){
  CTreeInformationElement* e = new CTreeInformationElement();

  QString decide=s.mid(0, 12);
  if (decide=="TuxCardsV0.5"){
    s=s.mid(12);										// remove VersionString from front
    unpackWithIcons(s, true, e);
  }
  
  return new CInformationCollection(e);
}
// *** old method ***
//void Tree::interface(QString s, Knoten* x){
//	QString decide=s.mid(0, 12);
//
//	if (decide=="TuxCardsV0.5"){
//		s=s.mid(12);										// remove VersionString from front
//		if (x) unpackWithIcons(s, false, x);
//		else unpackWithIcons(s, true, 0);
//	}else
//		if (x) unpackWithoutIcons(s, false, x);
//		else unpackWithoutIcons(s, true, 0);
//}


void Persister::unpackWithIcons(QString s, bool root, CTreeInformationElement* parent){
	// --- exactly the same routines as in 'unpack(..)'
	//     only that here the very first (root-) QListViewItem
	//     is created & that the root-CTreeInformationElement is altered, while all childs are
	//		 inserted
	//     (Therefor, changes done here must be done in 'unpack(..)' too!)

	// declares necessary variables
	int i;
	QString name; int l1;		// l1=l�ge von 'name'
	QString text; int l2;		// l2=l�ge von 'text'
	QString iconFile;
	bool isOpen;						// whether this Knoten is open or not
	int amountOfChilds;

	//packe Paket aus
	s=s.mid(3);							// remove "***" from front
	i=s.indexOf("*");
	int l=s.mid(0, i).toInt();		// l= verbleibende L�ge d. gesamten
													// Knotens (inkl. Kinder)

	QString knoten=s.mid(i+1, l); s=s.mid(l+1);
	i=knoten.indexOf("*");
	l1=knoten.mid(0, i).toInt();
	name=knoten.mid(i+1, l1);

	knoten=knoten.mid(i+1+l1);
	i=knoten.indexOf("*");
	l2=knoten.mid(0, i).toInt();
	text=knoten.mid(i+1, l2);

	knoten=knoten.mid(i+1+l2);

	// check whether this Knoten was opened or closed
	isOpen = (knoten.left(1)=="+" ? FALSE : TRUE );
	knoten=knoten.mid(1);

	// extract icon file
	i=knoten.indexOf("*");
	iconFile=knoten.mid(0, i);
	knoten=knoten.mid(i+1);

	// check for # of children
	i=knoten.indexOf("*");
	amountOfChilds=knoten.mid(0, i).toInt();

	knoten=knoten.mid(i);

  /*cerr<<"UNPACKED"<<"\n l="<<l<<"\n l1="<<l1<<"\n name="<<name
     <<"\n l2="<<l2<<"\n text="<<text<<"\n iconFile="<<iconFile
     <<"\n amountOfChilds="<<amountOfChilds<<"\n s="<<s<<endl;
  */

	CTreeInformationElement* self;
	if (root){
	  parent->setDescription(name);
		parent->setInformation(text);
		parent->setInformationFormat(InformationFormat::ASCII);
		parent->setIconFileName(iconFile);
  	parent->setOpen(isOpen);
		self = parent;
	}else{
  	self=new CTreeInformationElement(parent, name, text, InformationFormat::ASCII);
		self->setIconFileName(iconFile);
 	  self->setOpen(isOpen);
		parent->addChild(self);
  } 
  //cout<<self->getDescription()<<endl;
  
	for (i=1; i<=amountOfChilds; i++){
		knoten=knoten.mid(3);														// cutting "***" off
		int i=knoten.indexOf("*");													//get length of child
		int l=knoten.mid(0, i).toInt();

		unpackWithIcons("***"+knoten.mid(0, l+i), false, self);
		knoten=knoten.mid(l+i);
	}

	//cout<<"done..all\n";
}


///**
// * unpacks all childs of 'parent' and
// * their childs recursivly
// *
// * analyse 's' & build up the tree
// * the boolean 'root' must be set to 'true', if we start to unpack 's', 'root'
// * is automatically set to 'false' by this function, don't worry about it
// * -> now we don't need to have two methods doing exactly the same (i.e.
// *    unpacking)
// * if 'root'='true', then 'parent' is ignored, otherwise 'parent' is the
// * actual parent
// */
//void Tree::unpackWithoutIcons(QString s, bool root, Knoten* parent){
//
//	if (root)	clearTree();
//
//	// --- exactly the same routines as in 'unpack(..)'
//	//     only that here the very first (root-) QListViewItem
//	//     is created & that the root-Knoten is altered, while all childs are
//	//		 inserted
//	//     (Therefor, changes done here must be done in 'unpack(..)' too!)
//
//	// declares necessary variables
//	int i;
//	QString name; int l1;		// l1=l�ge von 'name'
//	QString text; int l2;		// l2=l�ge von 'text'
//	bool isOpen;						// whether this Knoten is open or not
//	int amountOfChilds;
//
//	//packe Paket aus
//	s=s.mid(3);							// remove "***" from front
//	i=s.find("*");
//	int l=s.mid(0, i).toInt();		// l= verbleibende L�ge d. gesamten
//													// Knotens (inkl. Kinder)
//
//	QString knoten=s.mid(i+1, l); s=s.mid(l+1);
//	i=knoten.find("*");
//	l1=knoten.mid(0, i).toInt();
//	name=knoten.mid(i+1, l1);
//
//	knoten=knoten.mid(i+1+l1);
//	i=knoten.find("*");
//	l2=knoten.mid(0, i).toInt();
//	text=knoten.mid(i+1, l2);
//
//	knoten=knoten.mid(i+1+l2);
//
//	// check whether this Knoten was opened or closed
//	isOpen = (knoten.left(1)=="+" ? FALSE : TRUE );
//	knoten=knoten.mid(1);
//
//	i=knoten.find("*");
//	amountOfChilds=knoten.mid(0, i).toInt();
//
//	knoten=knoten.mid(i);
//
//	//cout<<"UNPACKED"<<"\n l="<<l<<"\n l1="<<l1<<"\n name="<<name
//	// <<"\n l2="<<l2<<"\n text="<<text<<"\n amountOfChilds="<<amountOfChilds
//	// <<"\n s="<<s<<endl;
//	//Knoten* self=new Knoten(tree, name, text);  		<- once upon a times
//
//	Knoten* self;
//	if (root){
//		self=(Knoten*) (this->firstChild());		// <- nowadays
//		self->setName(name);
//		self->setText(text);  //editor->setText(text);
//		self->setPixmap("none");
//  	self->setOpen(isOpen);
//
//  	root=false;																			// "switch mode"
//  																									// we actually don't need
//  																									// to do this
//	}else{
//  	self=new Knoten(parent, name, text, "none");
//  	self->setOpen(isOpen);
//  }
//
//	for (i=1; i<=amountOfChilds; i++){
//		knoten=knoten.mid(3);														// cutting "***" off
//		int i=knoten.find("*");													//get length of child
//		int l=knoten.mid(0, i).toInt();
//
//		unpackWithoutIcons("***"+knoten.mid(0, l+i), false, self);
//		knoten=knoten.mid(l+i);
//	}
//
//	//cout<<"done..all\n";
//}




void Persister::save(CInformationCollection* collection, QString filename)
{
   CTreeInformationElement* pRootElem = (CTreeInformationElement*)collection->getRootElement();
   if ( NULLPTR == pRootElem )
      return;
      
   QString s= "TuxCardsV0.5" + pRootElem->toStringObsoleted();

   QFile f(filename);
   if ( f.open(QIODevice::WriteOnly) ) {          // file opened successfully
      QTextStream t( &f );                // use a text stream
      t<<s;
      f.close();
   }
}
