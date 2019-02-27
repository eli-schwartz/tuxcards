/***************************************************************************
                          cactusmanager.h  -  description
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

#ifndef CACTUS_H
#define CACTUS_H

#include "cactusshoot.h"
#include "imagemanager.h"
#include <qwidget.h>
#include <qrect.h>
#include <qtimer.h>

class Cactus: public QObject{
 Q_OBJECT
public:
	Cactus(QWidget* displayComponent);
	~Cactus();
	
	void paint( QPainter* p, const QRect& region );
	void setFlowerDirectory( const QString& );

signals:
   void triggerPaint();
   
public slots:
	void run();

protected slots:
	void simulate();

protected:
	ImageManager* mpImageManager;
	QTimer* mpTimer;

  /** Die Kompoennte, innerhalb derer der Kaktus dargestellt wird*/
  QWidget* mpDisplayComponent;

  /** Die Wurzel des Kaktus*/
  CactusShoot* mpCactusRootShoot;

  /** Der aktuelle Zustand des Wachstums */
  int state;

  /** Konstanten fr die Zust�de: Wachsend*/
  static const int GROWING   = 0;

  /** Konstanten fr die Zust�de: Blhend*/
  static const int FLOWERING = 1;

  /** Konstanten fr die Zust�de: Wachstum beendet*/
  static const int FINISHED  = 2;

  /** Ein nicht-notwendiges Feature: Das easter-egg.*/
  bool easterEgg;

	void drawEasterEgg();
	void createNewPlant();
	
// from 'CactusParameters'
	 /** Der Verzeichnisname, in dem sich alle Konfigurationsdateien/Bilder
  befinden.
  Die Blten dort muessen jeweils in 5 Gr�en vorhanden sein, durchnummeriert
  NUM=0...4 und mit der Endung "_NUM.gif" versehen.
  Siehe Vorlagen. */
  QString IMAGE_DIR;
  QString CACTUS_EGG_CONFIGURATION_FILE;


  //Simulationspausen in Millisekunden
  //
  // z.B. bei einer Simulationsdauer von 150 Zyklen (100-200 sind normal):
  //     1000 = 2,5 Minuten / Kaktus

  /** Bei der Wachstumsphase eine geringe Zeit zwischen Wachstumsschben */
  static const int THREAD_SLEEP_TIME_GROWING   =  1000; //1000; //[ms]

  /** Bei der Blh-phase eine l�gere Zeit zwischen Wachstumsschben */
  static const int THREAD_SLEEP_TIME_FLOWERING =  6000; //6000; //[ms]

  /** Die Pause nach Beendigung des Wachstums, bis eine neue Pflanze erzeugt wird */
  static const int THREAD_SLEEP_TIME_FINISHED  = 60000; //60000; [ms]

};

#endif
