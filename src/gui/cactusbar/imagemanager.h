#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <qwidget.h>
#include <qstring.h>
#include "cactusshoot.h"

class ImageManager{
public:
  ImageManager(QWidget* component);
  ~ImageManager();

//  void setDisplayComponent(QWidget* component); not needed anymore
//                                                -> done by the constructor

//  int getWidth(Image image);
//  int getHeight(Image image);

//  static void loadEasterEggConfiguration();
//  static boolean applyEasterEgg(CactusShoot root);
//  Image loadImage(QString fileName);
  void initFlowers();
  void applyFlower(CactusShoot*);

  void setFlowerDirectory(QString);

protected:

  /** Alle Easter-eggs */
//  Vector easterEggs;

  /** Alle Bltennamen */
  QList<QString>* mpFlowerNames;

  /** Die Display-Komponente (wird in Java fr Bilder-Laden/Berechnungen ben�igt) */
  QWidget* mpComponent;


	/* aus CactusParameter.java */
  /** Der Verzeichnisname, in dem sich alle Konfigurationsdateien/Bilder
  befinden.
  Die Blten dort muessen jeweils in 5 Gr�en vorhanden sein, durchnummeriert
  NUM=0...4 und mit der Endung "_NUM.gif" versehen.
  Siehe Vorlagen. */
  static QString IMAGE_DIR;
  static const QString CACTUS_EGG_CONFIGURATION_FILE;
};

#endif
