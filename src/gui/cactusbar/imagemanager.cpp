
/**
Sorgt dafr, dass die vorhandenen Blten bei Bedarf aus den Dateien
geladen werden. Ausserdem beinhaltet sie Methoden zum Laden von Bildern.
Zudem: Laden des Easter-egg-Konfigurationsfiles und Anbringen eines
Eaestereggs bei Bedarf.

Denkbar ist auch, dass man einen Satz an Blten fest codiert und weitere
Blten optional dazul�t.

Wo die Blten sind, steht in CactusParameters.java.
*/

#include "../../global.h"
#include "imagemanager.h"
#include "cactustools.h"

#include <iostream>
#include <qdir.h>
#include <qimage.h>
#include <QList>

QString ImageManager::IMAGE_DIR="/opt/tuxcards/flowers/";
const QString ImageManager::CACTUS_EGG_CONFIGURATION_FILE="cactus.egg";


ImageManager::ImageManager(QWidget* component)
 : mpFlowerNames ( NULLPTR )
 , mpComponent( NULLPTR )
{

	mpComponent = component;

	initFlowers();
//	loadEasterEggConfiguration();
}

ImageManager::~ImageManager(){
	DELETE( mpFlowerNames );
}

void ImageManager::setFlowerDirectory(QString dir){
	IMAGE_DIR=dir;
	initFlowers();
}


// not needed anymore -> constructor does this
//void ImageManager::setDisplayComponent(QWidget* component){
//  mpComponent=component;
//}


//int ImageManager::getWidth(Image image){
//  return image.getWidth(mpComponent);
//}
//
//int ImageManager::getHeight(Image image){
//  return image.getHeight(mpComponent);
//}

/**
* L�t alle das File "cactus.egg", welches die Beschreibung aller Easter-eggs
* beinhaltet.
*/
//void ImageManager::loadEasterEggConfiguration(){
//  easterEggs=new Vector(10);
//
//  try{
//    File file=new File(CactusParameters.IMAGE_DIR+CactusParameters.CACTUS_EGG_CONFIGURATION_FILE);
//    BufferedReader br=new BufferedReader(new FileReader(file));
//
//    String s=br.readLine().trim();
//
//    while(s!=null){
//      while(s!=null && !s.startsWith("egg "))
//        s=br.readLine();
//
//      if (s==null)
//        break;
//
//      //Wir sind jetzt in der ersten Zeile einer Egg-Beschreibung
//      //Name holen (wird derzeit eigentlich nicht ben�igt, aber kann nie schaden...
//      String name=s.substring(4,s.indexOf('{',4));
//
//      //Name des GIF-Files holen
//      s=br.readLine().trim();
//      int index1=s.indexOf('"');
//      int index2=s.indexOf('"',index1+1);
//      String imageName=s.substring(index1+1,index2);
//
//      //Erster Parameter: propability factor
//      s=br.readLine();
//      int i1=s.indexOf('#');
//      if (i1!=-1)
//        s=s.substring(0,i1);
//      s=s.trim();
//      int propabilityFactor=Integer.parseInt(s);
//
//      //Zweiter Parameter: min/max depth
//      s=br.readLine();
//      i1=s.indexOf('#');
//      if (i1!=-1)
//        s=s.substring(0,i1);
//      s=s.trim();
//
//      i1=s.indexOf(',');
//
//      int minDepth=Integer.parseInt(s.substring(0,i1));
//      int maxDepth=Integer.parseInt(s.substring(i1+1).trim());
//
//      //3. Parameter: min/max height
//      s=br.readLine();
//      i1=s.indexOf('#');
//      if (i1!=-1)
//        s=s.substring(0,i1);
//      s=s.trim();
//
//      i1=s.indexOf(',');
//
//      int minHeight=Integer.parseInt(s.substring(0,i1));
//      int maxHeight=Integer.parseInt(s.substring(i1+1).trim());
//
//      //4. Parameter: min/max translation in x-direction
//      s=br.readLine();
//      i1=s.indexOf('#');
//      if (i1!=-1)
//        s=s.substring(0,i1);
//      s=s.trim();
//
//      i1=s.indexOf(',');
//
//      int minXTranslation=Integer.parseInt(s.substring(0,i1));
//      int maxXTranslation=Integer.parseInt(s.substring(i1+1).trim());
//
//      //5. Parameter: 1/0 fr frontMost
//      s=br.readLine();
//      i1=s.indexOf('#');
//      if (i1!=-1)
//        s=s.substring(0,i1);
//      s=s.trim();
//
//      boolean frontMost=true;
//      int f=Integer.parseInt(s);
//      if (f==0)
//        frontMost=false;
//
//      //Neues Easteregg generieren
//      EasterEgg egg=new EasterEgg(name, imageName, propabilityFactor,
//                                  minDepth,        maxDepth,
//                                  minHeight,       maxHeight,
//                                  minXTranslation, maxXTranslation,
//                                  frontMost);
//
//      //und gem� der Wahrscheinlichkeitsangabe entsprechend
//      //oft in dem Vector mit allen Eggs referenzieren
//      for (int i=0;i<propabilityFactor;++i)
//        easterEggs.addElement(egg);
//
//      //weiterlesen bis geschweifte Klammer zu
//      while(s!=null && !s.trim().startsWith("}"))
//        s=br.readLine();
//
//      //Und wieder von vorne, eventuell weitere eggs
//    }
//
//    br.close();
//  }catch (NumberFormatException e){
//    System.err.println("Cactus Error: Wrong number format in file "+CactusParameters.IMAGE_DIR+CactusParameters.CACTUS_EGG_CONFIGURATION_FILE+" :"+e);
//    System.err.println(" Some Features for the cactus will be disabled.");
//  }catch (Exception e){
//    System.err.println("Cactus Error: In file "+CactusParameters.IMAGE_DIR+CactusParameters.CACTUS_EGG_CONFIGURATION_FILE+" :"+e);
//    System.err.println(" Some Features for the cactus will be disabled.");
//  }
//}
//
//
//
//boolean ImageManager::applyEasterEgg(CactusShoot root){
//  if (easterEggs==null || easterEggs.size()==0)
//    return false;
//
//  //Wrfeln, welches Easteregg...
//  int index=CactusTools.ran(0,easterEggs.size()-1);
//
//  //Und versuchen, es anzubringen
//  EasterEgg egg=(EasterEgg)easterEggs.elementAt(index);
//  return egg.applyOn(root);
//}

//Image ImageManager::loadImage(QString fileName){
//  Toolkit toolkit = Toolkit.getDefaultToolkit();
//  MediaTracker tracker = new MediaTracker(mpComponent);
//  Image image = toolkit.getImage(fileName);
//
//  tracker.addImage(image, 0);
//  try{
//    tracker.waitForID(0);
//  }catch (InterruptedException e){
//System.err.println(e);
//  }
//
//  return image;
//}

/**
* Initialisiert die Bltennamen. Im Verzeichnis CactusParameters.IMAGE_DIR
* wird dazu nach passenden Dateien gesucht.
*/
void ImageManager::initFlowers(){
  QDir dir(IMAGE_DIR);
  if (!dir.exists()){
    //cerr<<"Cactus Error: no such directory for flower-files: "<<IMAGE_DIR<<"."<<endl
    //		<<" Some Features for the cactus will be disabled."<<endl;
    return;
  }

  mpFlowerNames=new QList<QString>;

  //Gesamtes Verzeichnis nach Blten durchsuchen
  dir.setFilter(QDir::Files | QDir::NoSymLinks);
	
  QFileInfoList files=dir.entryInfoList();
  if( !files.isEmpty() ){
//    QFileInfoListIterator it(*files);									// create iterator
    QFileInfo f;

		// for all files/components
      for ( int i = 0; i < files.count(); i++ )
      {
		//while( (f=it.current())!=0 ){
			
//			++it;
         f = files.at(i);
			if( f.isFile() && (f.suffix()=="gif") && (f.baseName().right(2)=="_0") ){
				QString name=f.baseName();
				name=name.left(name.length()-2);
				//cout<<name<<endl;
				
				mpFlowerNames->append( QString(name) );
			}
			
		}//while/for
	}//if(files)
}

/**
 * setzt einen Index, der die erste(kleinste) Blte enth�t; dabei
 * wird zuf�lig eine Bltenart herausgesucht
 */
void ImageManager::applyFlower(CactusShoot* c){
  if (mpFlowerNames==0) return;

  // delete the previous flowers
  while ( ! c->flowers.isEmpty() )
     delete c->flowers.takeFirst();

  //Zufällig einen Blütennamen heraussuchen
  //um die Bilder für die Blüten kümmern; alle Blüten einlesen
	QString flowerName = mpFlowerNames->at( CactusTools::ran(0,mpFlowerNames->count()-1) );
  for (int i=0; i<5; i++){
  	c->flowers.insert(i, new QImage(IMAGE_DIR+flowerName+"_"+QString::number(i)+".gif"));
 	}
}
