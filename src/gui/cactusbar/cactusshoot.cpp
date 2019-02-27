/***************************************************************************
                          cactusshoot.cpp  -  description
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

#include "../../global.h"
#include "cactusshoot.h"

#include <iostream>
#include <stdlib.h>
#include <math.h>


QColor CactusShoot::INITIAL_COLOR=QColor(0,128,0);
QList<QImage*> CactusShoot::flowers;

/**
* Konstruktor zum erzeugen des allerersten Astes=Stamm (root-Sch�ling).
*/
CactusShoot::CactusShoot()
{
	INITIAL_COLOR=QColor(0,128,0);

  generalSettings(INITIAL_COLOR,
       				MAXRADIUS*3/4,
       				MAXRADIUS,
       				MAXHEIGHT,
      				5,// 				CactusTools.ran(MAXRADIUS*3/4,MAXRADIUS), //Radius
       				0,//x
       				0,//y
       				true,
              0.0,
              MAXCHILDREN,
              0,
              STACHELZAHL);          //aktuelle Rekursionstiefe

}

CactusShoot::CactusShoot(QColor c, int minRadius, int maxRadius, int maxHeight,
              int rad, int x, int y, bool back,
              double dd, int maxChildren, int depth, int stachelZahl)
{

  // general settings
  generalSettings(c, minRadius, maxRadius, maxHeight,
                  rad, x, y, back,
                  dd, maxChildren, depth, stachelZahl);
}

//general settings
void CactusShoot::generalSettings(QColor c, int minRadius, int maxRadius, int maxHeight,
              int rad, int x, int y, bool back,
              double dd, int maxChildren, int depth, int stachelZahl)
{
	
  stachelFarbe      = QColor(0,0,0);
	hasFlower         = false;
	flowerHeight      = 0;
	flowerDelay       = 0;
	maxFlowerSize     = 0;
	flowerSize        = 0;
	finishedFlowering = false;
	finishedGrowing   = false;



  this->depth       = depth;
  this->maxChildren = maxChildren;
  this->maxHeight   = maxHeight;
  this->maxRadius   = maxRadius;
  this->minRadius   = minRadius;
  this->back        = back;
  this->stachelZahl=stachelZahl;
  start=QPoint(x,y);

  if (this->depth==MAXDEPTH)
    this->maxChildren=0;

  this->maxHeight=CactusTools::max(this->maxHeight,MINHEIGHT);
  if (this->maxHeight<MAXRADIUS)
    this->maxChildren=0;

  this->minRadius=CactusTools::max(this->minRadius,MINRADIUS);
  if (this->maxRadius<this->minRadius)
    this->maxRadius=this->minRadius+2;

  this->maxHeight=CactusTools::min(this->maxHeight, (MAX_TOTAL_HEIGHT-start.y()-2*this->maxRadius-1)/2);

  if (depth==0)
    stammFarbe=c;
  else
  if (back)
		stammFarbe=c.light(60); 			// returns a color that is 40% darker than 'c'
  else
		stammFarbe=c.light(140); 			// returns a color that is 40% brither than 'c'

//  mpChildren=new CactusShoot[this->maxChildren]; ------------------------
  noChildren=0;

/*  radius   = new int[this->maxHeight];  ---------------------------------
  delate   = new int[this->maxHeight];
  rotation = new int[this->maxHeight];
*/
  height=0;

  deltaDelay=dd;
  delay=0.0;

  deltaRotate=rand() % 100 - 50;
  rotate=deltaRotate;

  delate[0]=0;
  radius[0]=rad;

  rotation[0]=(int)(15*rotate);
  height++;
}


CactusShoot::~CactusShoot(){
   for(int i=0; i<noChildren; i++)
   {
      DELETE( mpChildren[i] );
   }
}

/**
* Stellt zuerst alle Kinder hinter sich dar, dann sich selbst und
* anschliessend die davor gelegenen �te
*/
void CactusShoot::paint(QPainter* p, /*ImageObserver obs,*/ QPoint* location){
	//alle �te, die hinter diesem liegen darstellen
  for (int i=0;i<noChildren;i++){
    if (mpChildren[i]->back)
      mpChildren[i]->paint(p, /*obs,*/ location);
  }

  //sich selbst darstellen
  paintSelf(p, /*obs,*/ location);
	
  //alle �te vor sich drbermalen
  for (int i=0;i<noChildren;i++){
    if (!mpChildren[i]->back)
      mpChildren[i]->paint(p, /*obs,*/ location);
  }

  //ggf. Easter-egg darstellen
/*  if (imageEasterEgg!=null && easterEggFrontmost){
    g.drawImage(imageEasterEgg, location.x+start.x + easterEggLocation.x,
                                location.y+start.y + easterEggLocation.y, obs);
  }
*/
}

/**
* Stellt sich selbst dar.
*/
void CactusShoot::paintSelf(QPainter* p, /*ImageObserver obs,*/ QPoint* location){
  //Stamm
  p->setPen(stammFarbe);

  //Den Ursprung fr die Darstellung auf xx,yy festlegen
  int xx=start.x()+location->x();
  int yy=start.y()+location->y();

  //Die unterste Scheibe/Halbkreis
  int r=radius[0];

  if (depth>0){
    //Bei Seiten�ten den Anfang als Halbkreis darstellen
    // draw a pie (it is automatically filled by the current brush
		//             -> we set it right here)
		p->setBrush(stammFarbe);
		p->drawPie(xx+delate[0]-r,yy-r,  2*r,2*r,  0, -180*16);
  }else
    //Beim Hauptstamm mit einem Rechteck beginnen
    p->drawRect(xx+delate[0]-r, yy,   2*r-1, 1);


  double rr;

  //Die Scheiben dazwischen
  int y = yy-2;
  for (int i=1;i<height-1;i++){
    r=radius[i];
    p->drawRect(xx+ delate[i]-r ,y, 2*r-1,0);
    rr=(radius[i]+radius[i-1])/2.0;
    p->drawRect(xx+ (int)((delate[i]+delate[i-1])/2.0 - rr) ,y+1, (int)(2*rr)-1,2);
    y-=2;
  }

  y+=2;
  //Die Spitze
  r=radius[height-1];
  // draw a pie (it is automatically filled by the current brush
  //             -> we set it right here)
	p->setBrush(stammFarbe);
	p->drawPie(xx+delate[height-1]-r,y-r,  2*r,2*r,   0,180*16);


	//Stacheln
  y=yy;
  p->setPen(stachelFarbe);
  double ra=0.0;
  double ss=(double)(stachelZahl-1.0)/ 3.14159;

  int x;
  if(depth>0){
    r=radius[0];
    for (int j=0;j<r-1;j+=2){
      y+=2;
      ra=sqrt(r*r-j*j-2*j-1);
      for (int s=0;s<stachelZahl;s++){
        x=xx+delate[0]+(int)((cos((double)s/ss+(rotation[0]/100.0))*ra))-1;
        p->drawLine(x,y,x,y);
      }
    }
  }
  y=yy;
  for (int i=0;i<height;i++){
    r=radius[i];
    for (int s=0;s<stachelZahl;s++){
      x=xx+delate[i]+(int)((cos((double)s/ss +(rotation[i]/100.0))*r))-1;
      p->drawLine(x,y,x,y);
    }
    y-=2;
  }
y+=2;
  r=radius[height-1];
  for (int j=0;j<r-1;j+=2){
    ra=sqrt(r*r-j*j-2*j-1);
    for (int s=0;s<stachelZahl;s++){
      x=xx+delate[height-1]+(int)((cos((double)s/ss +(rotation[height-1]/100.0))*ra))-1;
      p->drawLine(x,y,x,y);
    }
    y-=2;
  }

  //eventuell die Bluete darstellen
  if(hasFlower && flowers.count()>0){
    x = xx+delate[height-1]+flowerDelay - (flowers.at(flowerSize)->width() /2);
    y = yy-flowerHeight*2               - (flowers.at(flowerSize)->height()/2);
    p->drawImage(x,y, *(flowers.at(flowerSize)) );
  }

/*  //ggf. Easter-egg darstellen
  if (imageEasterEgg!=null && !easterEggFrontmost){
    g.drawImage(imageEasterEgg, xx + easterEggLocation.x,
                                yy + easterEggLocation.y, obs);
  }
*/
}	

bool CactusShoot::isFinishedGrowing(){
  if (finishedGrowing)
    return true;

  for (int i=0;i<noChildren;i++){
    if (!mpChildren[i]->isFinishedGrowing())
      return false;
  }

  if (noChildren<maxChildren)
    return false;
  if (height<maxHeight)
    return false;

  finishedGrowing=true;
  return true;
}


void CactusShoot::flower(){
  if (finishedFlowering) return;

	//nur die tiefste Rekursionsstufe darf bluehen
  if (noChildren>0){
    for (int i=0; i<noChildren; i++)
    	mpChildren[i]->flower();
    return;
  }

	//Blte wachsen lassen
  if (!hasFlower){
    hasFlower     = true;
  	maxFlowerSize = CactusTools::ranUpper(0,4);
  	flowerHeight  = CactusTools::ranUpper((height-1)*2/3,height-1);
  	flowerDelay   = CactusTools::ran(-radius[flowerHeight],radius[flowerHeight]);
  }else{
  	if (flowerSize < maxFlowerSize)
      flowerSize++;
  }
}

bool CactusShoot::isFinishedFlowering(){
  if (finishedFlowering)
    return true;

	if (noChildren>0){
    for (int i=0;i<noChildren;i++){
      if(!mpChildren[i]->isFinishedFlowering()) return false;
    }
		finishedFlowering=true;
    return true;
	}

	if (flowerSize==maxFlowerSize){
    finishedFlowering=true;
    return true;
	}
	return false;
}

int CactusShoot::grow(){
  if (finishedGrowing)
    return 0; //nichts mehr zum Wachsen da

  int result=0;

  //kinder wachsen lassen
  for (int i=0; i<noChildren; i++)
    result+=mpChildren[i]->grow();

  //selber wachsen
  if(height < maxHeight && CactusTools::ran(0,noChildren+1) < 15.0/height){
    //Radius der letzten Scheibe holen
    int old=radius[height-1];

    //Radius der neuen Scheibe bestimmen
    int r=CactusTools::ran(old-1,old+1);
    if (r<minRadius)
      r=minRadius;
    else
    if (r>maxRadius)
      r=maxRadius;
    radius[height]=r;

    //Drehung dieser Scheibe gegenber der vorherigen
    rotation[height]=(int)(25*rotate);
    deltaRotate= rand() % 100 - 50;
    rotate+=deltaRotate;

    delay+=deltaDelay;
    delay= (minRadius/2.0 <= delay ? minRadius/2.0 : delay); //min(minRadius/2.0, delay); ------------
    delate[height++]=(int)delay;

    deltaDelay*=CactusTools::ranLower(4,180)/100.0;

    result++;
  }

  //eventuell kinder kriegen
  if (height>maxRadius && (rand() % 10000)<15*height*(maxChildren-noChildren)){
    createChild();
    result++;
  }

  return result;
}

void CactusShoot::createChild(){
  int anknuepfHoehe =
    CactusTools::ran( CactusTools::max(maxRadius,(height-1)*2/3) ,    height-1);
  //Der neue maximale Radius einer Kaktusscheibe
  int newMax=
    CactusTools::ran(MINRADIUS,CactusTools::max(MINRADIUS+1,radius[anknuepfHoehe]-1));
  //Der neue minimale Radius einer Kaktusscheibe
  int newMin=
    CactusTools::max(MINRADIUS,newMax*3/4);
  int neurad=
    CactusTools::ran(newMin,newMax);

  //Winkel von oben gesehen, in welchem der neue Ast bzgl. dem alten w�hst
  double angle=0.0;

  //Wachstum in die Breite bevorzugen, weil schoener anzusehen (double angle=ran(0,360)/180.0*Math.PI; sonst)
  if (CactusTools::fifty())
    angle=CactusTools::ranUpper(90, 180) / 180.0*3.141593; //links
  else
    angle=CactusTools::ranLower( 0, 90 ) / 180.0*3.141593; //rechts

  int yy = start.y()-2*anknuepfHoehe;
  int xx = start.x()+delate[anknuepfHoehe]+(int)(cos(angle)* (radius[anknuepfHoehe]+neurad));

  double del=CactusTools::ranUpper(80,200)/100.0*cos(angle);


  //Neuen Nachfolger mit den erwrfelten Daten erzeugen
  mpChildren[noChildren++]=
       new CactusShoot(stammFarbe, //neue Farbe wird dann im Konstruktor aus dieser bestimmt
                       newMin,     //minimaler Radius
                       newMax,     //maximaler Radius
                       CactusTools::ranUpper(maxHeight-anknuepfHoehe,maxHeight), //Endh�e
                       neurad,     //Radius
                       xx, yy-1,   //Startpunkt
                       CactusTools::fifty(),   //davor/dahinter
                       del,        //Anfangsdelay
                       CactusTools::ranLower(0,maxChildren), //Anzahl der Kinder
                       depth+1,    //Rekursionstiefe
                       CactusTools::ran(MINSTACHELZAHL,stachelZahl));//Anzahl der Stacheln
}
