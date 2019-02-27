/***************************************************************************
                          cactusshoot.h  -  description
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

#ifndef CACTUSSHOOT_H
#define CACTUSSHOOT_H

#include "cactustools.h"

#include <qpoint.h>
#include <qpainter.h>
#include <qcolor.h>
#include <qimage.h>
#include <QList>

class CactusShoot {
public: 
	CactusShoot();
	~CactusShoot();
	
	void paint(QPainter* p, /*ImageObserver obs,*/ QPoint* location);
	bool isFinishedGrowing();
	bool isFinishedFlowering();
	void flower();
	int  grow();
	
 	/**die Bilder fuer die Blueten (transparente GIFs)*/
  static QList<QImage*> flowers;

protected:
	CactusShoot(QColor c,int minRadius,int maxRadius,int maxHeight,
              int rad,int x,int y,bool back,
              double dd,int maxChildren,int depth, int stachelZahl);
  void generalSettings(QColor c,int minRadius,int maxRadius,int maxHeight,
              int rad,int x,int y,bool back,
              double dd,int maxChildren,int depth, int stachelZahl);

  void createChild();
	
private:
	static QColor INITIAL_COLOR;
  static const int STACHELZAHL    =  4; //Anzahl an Stacheln auf der Vorderseite
  static const int MINSTACHELZAHL =  2;
  static const int MINRADIUS   =     2;
  static const int MAXRADIUS   =     6;
  static const int MINHEIGHT   =     4; //die Mindestgr. fr ausgewachsenen Ast
  static const int MAXHEIGHT   =    35; //die Maximalgr. fr ausgewachsenen Ast
  static const int MAX_TOTAL_HEIGHT = 200; //die Maximalgr. fr den gesamten Kaktus
  static const int MAXCHILDREN =     3;//maximale Anzahl an Nachfolgern pro Ast
  static const int MAXDEPTH    =     3; //maximale Rekursionstiefe

	int      radius[200];   //Radius der einzelnen Scheiben
	int      rotation[200]; //Drehung der Scheiben gegeneinander
	int      delate[200];   //Verschiebung der Scheibe gegenber start.x nach  rechts (+) bzw. links (-)
	int      height;
	int      maxHeight;
	double   delay;         //aktueller Wert der Verschiebungsweite
	double   deltaDelay;    //aktuelles Inkrement der Verschiebung
	double   rotate;        //aktueller Wert der Verdrehung
	double   deltaRotate;   //aktuelles Inkrement der Verschiebung
	int      minRadius,maxRadius;

	QPoint    start; //Fu�unkt des Stammes/Astes

  /**die Nachfolger=�te
  Achtung: Die Array-Gr�e ist fest, die Anzahl der Nachfolger steht in noChildren!!!*/
	CactusShoot* mpChildren[200];
	int      maxChildren;   //Maximale Anzahl von Nachfolgern
	int      noChildren;    //Anzahl bereits vorhandener Nachfolger
	int      depth;         //Rekursionstiefe

	QColor   stammFarbe;
	QColor   stachelFarbe;
	int stachelZahl;
	bool  back;
	
	bool hasFlower;
	int  flowerHeight;
	int  flowerDelay;
	int  maxFlowerSize;
	int  flowerSize;
	bool finishedFlowering;
	bool finishedGrowing;
	
	void paintSelf(QPainter* p, /*ImageObserver obs,*/ QPoint* location);

protected:

  //Nicht-notwendiges Feature easter-egg
//	Image imageEasterEgg;
  QPoint easterEggLocation;
  bool easterEggFrontmost;

};

#endif
