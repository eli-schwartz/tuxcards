/***************************************************************************
                          cactusmanager.cpp  -  description
                             -------------------
    begin                : Mon Apr 10 2000
    copyright            : (C) 2000 by Markus Gebhard & Alexander Theel
    idea & java
       implementation		 : Markus Gebhard
    implementation c++   : Alexander Theel

    email                : ukgh@rz.uni-karlsruhe.de
                           alex.theel@gmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/**
	Der Hauptteil, der alles steuert und als Thread geringer Prioritaet
	die einzelnen Wachstumsschritte anstoest.
	Wacht ausserdem ber Maus-Aktivitaet.

	Das Auftreten von Easter-Eggs wird durch das Konfigurationsfile "cactus.egg" gesteuert.
	Das Format fr dieses Skript ist in jenem File erlaeutert.

 *-------------------------------------------------------------------------------------*
 * Verwendung der Idee, des Algorithmus und Teile des Quelltexts vorerst nur mit meiner
 * persoenlichen Genehmigung und unter der Bedingung, dass die Erweiterbarkeit um
 * zusaetzliche Buelten oder Easter-eggs gewaerleistet ist - nach Moeglichkeit in der
 * Form wie hier, also durch GIF-Bilder und durch einn kompatibles Skript.
 *
 * Known Bugs: -
 *
 * Missing Features:
 *             - Parameter zum Wachstum auch noch in ein Konfigurationsfile auslagern
 *
 * @version 0.5 04/09/00
 *
 * @author Markus Gebhard <a href="mailto:ukgh@rz.uni-karlsruhe.de">ukgh@rz.uni-karlsruhe.de</a>
 */

#include "../../global.h"
#include "cactus.h"

#include "CCactusBar.h"


// -------------------------------------------------------------------------------
Cactus::Cactus( QWidget* pDisplayComponent )
 : mpImageManager( NULLPTR )
 , mpTimer( NULLPTR )
 , mpDisplayComponent( pDisplayComponent )
 , mpCactusRootShoot( NULLPTR )
// -------------------------------------------------------------------------------
{
  //from 'CactusParameters'          wird in ImageManager noch mal deklariert
//  IMAGE_DIR=QString("./flowers/");
//  CACTUS_EGG_CONFIGURATION_FILE=QString("cactus.egg");

  //super();

//  mpDisplayComponent = pDisplayComponent;
  //mpDisplayComponent.addMouseListener(this);
  //mpDisplayComponent.addMouseMotionListener(this);

  //setPriority(MIN_PRIORITY);

  //ImageManager initialisieren
   mpImageManager = new ImageManager( mpDisplayComponent );
  //Nach Bildern fr die Blten suchen

  //Laden des EasterEgg description files
  //ImageManager.loadEasterEggConfiguration();
}

// -------------------------------------------------------------------------------
Cactus::~Cactus()
// -------------------------------------------------------------------------------
{
   DELETE( mpImageManager );
   DELETE( mpTimer );
   mpDisplayComponent = NULLPTR;
   DELETE( mpCactusRootShoot );
}

// -------------------------------------------------------------------------------
void Cactus::setFlowerDirectory( const QString& sDir )
// -------------------------------------------------------------------------------
{
   mpImageManager->setFlowerDirectory( sDir );
}

// -------------------------------------------------------------------------------
void Cactus::paint( QPainter* p, const QRect& region )
// -------------------------------------------------------------------------------
{
   if ( mpCactusRootShoot )
   {
      QPoint point( region.x()+region.width()/2, region.y()+region.height() );
      mpCactusRootShoot->paint( p, &point );
   }
}


/**
 * Der eigentliche Thread-ablauf.
 * Es werden kontinuierlich neue Kakteen erzeugt, Wachstumsimpulse gegeben, etc.
 * Bei Mausaktivitaet wird ggf. der wartende Thread unterbrochen, damit der
 * naechste vorgesehene Wachstumsschritt durchgefuehrt wird.
 */
// -------------------------------------------------------------------------------
void Cactus::run()
// -------------------------------------------------------------------------------
{
   createNewPlant();

   mpTimer = new QTimer( this );
   connect( mpTimer, SIGNAL(timeout()), SLOT(simulate()) );
   mpTimer->start( THREAD_SLEEP_TIME_GROWING );


//  while (!shallStop){
//    yield();------------------------------------------------------------------------


// /*    try{
//;//      sleep(sleepTime);------------------------------------------------
//    }catch (InterruptedException e) {}
//*/
//    //einen Wachstumsschritt simulieren
//    simulate();
//  }
}

/**
 * Ein nicht-notwendiges Feature ist das Easter-egg.
 * Hier wird zum Beispiel dem Kaktus ein Gesicht aufgesetzt oder aehnliches...
 */
// -------------------------------------------------------------------------------
void Cactus::drawEasterEgg()
// -------------------------------------------------------------------------------
{
   //schon vorhanden? dann nicht!
   if ( easterEgg )
      return;

//  boolean success=mpImageManager.applyEasterEgg(mpCactusRootShoot);-----------------------------

   //Wenn erfolgreich angebracht, dann neu darstellen
   if ( true/*success*/ ){//------------------------------------------------
      emit ( triggerPaint() );
      easterEgg=true;
   }

   //ansonsten halt kein Osterei diesmal :(
}




/**
 * Eine neue Pflanze erzeugen
 */
// -------------------------------------------------------------------------------
void Cactus::createNewPlant()
// -------------------------------------------------------------------------------
{
   if( mpCactusRootShoot != 0 )
      DELETE( mpCactusRootShoot );
   
   mpCactusRootShoot=new CactusShoot();
   mpImageManager->applyFlower( mpCactusRootShoot );
   
   easterEgg=false;
   state=GROWING;
}

/**
 * Ein einzelner Wachstumsschritt wird simuliert.
 */
// -------------------------------------------------------------------------------
void Cactus::simulate()
// -------------------------------------------------------------------------------
{
   if ( !mpCactusRootShoot )
      return;

   switch( state )
   {
      case GROWING:
         //solange versuchen zu wachsen, bis mindestens eine Stufe gewachsen
         while( mpCactusRootShoot->grow()==0 ){};
			
         emit ( triggerPaint() );

         //Fertig mit wachsen? Dann ab jetzt Zustand blhend
         if(mpCactusRootShoot->isFinishedGrowing())
            state=FLOWERING;
         break;
         
      case FLOWERING:
         mpCactusRootShoot->flower();
         emit ( triggerPaint() );

         //Blhen beendet? Dann fertig.
         if ( mpCactusRootShoot->isFinishedFlowering() )
            state=FINISHED;
         break;
         
      case FINISHED:
         createNewPlant();
         emit ( triggerPaint() );
         state = GROWING;
         break;
   }

   // Timer anpassen (diese Routine steht im Ursprnglichen Programm
   // in der Methode 'run()' )
   // -> aktuelle Pausenlaenge bestimmen
   int sleepTime = 0;
   switch( state )
   {
      case GROWING:
         sleepTime = THREAD_SLEEP_TIME_GROWING;
         break;
      case FLOWERING:
         sleepTime = THREAD_SLEEP_TIME_FLOWERING;
         break;
      case FINISHED:
         sleepTime = THREAD_SLEEP_TIME_FINISHED;
         break;
   }
   //Pause vor dem naechsten Simulationsschritt; Timer anpassen
   mpTimer->setInterval( sleepTime );
}
