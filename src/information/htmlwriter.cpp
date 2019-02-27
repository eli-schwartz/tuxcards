/***************************************************************************
                          htmlwriter.cpp  -  description
                             -------------------
    begin                : Son Jul 28 2002
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

#include "htmlwriter.h"
#include "../version.h"

#include <qdir.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>
//Added by qt3to4:
#include <QPixmap>

#include "dot.xpm"
#include "space15.xpm"


const QString HTMLWriter::TUX_HTMLEXPORT_SUBDIR = "Notes";
const QString HTMLWriter::TUX_SPACER = "space15.png";
const QString HTMLWriter::TUX_DOT = "dot.png";
const QString HTMLWriter::USE_UTF8_CHARSET = "<meta content=\"text/html; charset=utf-8\" />";

QStringList HTMLWriter::static_List = QStringList();

/**
 * Exports a aollection to HTML.
 * Returns 'TRUE' if successful, otherwise 'FALSE'.
 */
// -------------------------------------------------------------------------------
bool HTMLWriter::writeCollectionToHTMLFile( CInformationCollection& collection,
                                            const QString& dirPath )
// -------------------------------------------------------------------------------
{
   QDir dir = QDir( dirPath );

   bool bSuccess = createSubDir( dir );
   if ( !bSuccess )
      return FALSE;

   bSuccess = createImages( dir );
   if ( !bSuccess )
      return FALSE;

   static_List.clear();

   // TODO: Check if files could be opened, also. If not -> return 'FALSE'.
   createTopFrame( dir );

   CInformationElement* pRootElem = collection.getRootElement();
   if ( NULLPTR != pRootElem )
   {
      createIndexFile( dir, pRootElem->getDescription() );
      createTreeFrame( collection, dir );
   }

   static_List.clear();
   return TRUE;
}


// -------------------------------------------------------------------------------
bool HTMLWriter::createSubDir( const QDir& dir )
// -------------------------------------------------------------------------------
{
   bool bSuccess = dir.mkdir( TUX_HTMLEXPORT_SUBDIR );
   if ( FALSE == bSuccess )
      return FALSE;

   return TRUE;
}

// -------------------------------------------------------------------------------
bool HTMLWriter::createImages( const QDir& dir )
// -------------------------------------------------------------------------------
{
   // create dot and space and write them to disk
   QPixmap dot( dot_xpm );
   bool bRetVal = dot.save( dir.absolutePath() + "/" + TUX_DOT, "PNG" );

   QPixmap space( space15_xpm );
   bRetVal = bRetVal | space.save( dir.absolutePath() + "/" + TUX_SPACER, "PNG" );

   return bRetVal;
}

// -------------------------------------------------------------------------------
void HTMLWriter::createIndexFile( const QDir& dir, const QString& sRootDescription )
// -------------------------------------------------------------------------------
{
   // "index.html" erstellen;
   // we could not do this in advance -> because we did not know the 'name' of the "root-page"
   QString index="<html> <head><title>TuxCards-HTMLExport</title>"+USE_UTF8_CHARSET+"</head>\n\n";
   index+="<!-- frames -->\n";
   index+="<frameset rows=\"100,*\" border=0>\n";
   index+="<frame name=\"nav\" src=\"top.html\" frameborder=\"0\">\n";
   index+="    <frameset cols=\"230,*\">\n";
   index+="            <frame name=\"tree\" src=\"./treeHTML.html\" frameborder=\"0\">\n";
   index+="            <frame name=\"main\" src=\"./"+TUX_HTMLEXPORT_SUBDIR+"/"+sRootDescription+".html\" frameborder=\"0\">\n";
   index+="    </frameset>\n";
   index+="<noframes>\n";
   index+="    <body bgcolor=\"#F0F0E1\">\n";
   index+="    Sie benötigen einen Framefähigen Browser.<br>\n";
   index+="		You do need a browser that supports frames.\n";
   index+="    </body>\n";
   index+="</noframes>\n";
   index+="</frameset>\n";
   index+="</html>";
   QFile f2( dir.absolutePath() + "/index.html" );
   if ( f2.open(QIODevice::WriteOnly) ) {        // file opened successfully
      QTextStream t( &f2 );                      // use a text stream
      t.setCodec( "UTF-8" );
      t<<index;
      f2.close();
   }
}


// -------------------------------------------------------------------------------
void HTMLWriter::createTreeFrame( CInformationCollection& collection, const QDir& dir )
// -------------------------------------------------------------------------------
{
   // generate "tree"
   QString sTreeHTML = "<html>\n<head>" + USE_UTF8_CHARSET + "</head>\n<body>\n";


   CInformationElement* pCollectionRootElement = collection.getRootElement();
   if ( NULLPTR != pCollectionRootElement )
   {
      convertInformationElementToHTML( *pCollectionRootElement, dir, sTreeHTML );
   }


   sTreeHTML.append("\n</body></html>");

   QFile f( dir.absolutePath()+"/"+"treeHTML.html" );
   if ( f.open(QIODevice::WriteOnly) )                        // file opened successfully
   {
      QTextStream t( &f );                                    // use a text stream
      t.setCodec( "UTF-8" );
      t<<sTreeHTML;
      f.close();
   }
}


// -------------------------------------------------------------------------------
void HTMLWriter::convertInformationElementToHTML( CInformationElement& element,
                                                  const QDir& dir, QString& sTreeHTML,
                                                  int iTabCount  )
// -------------------------------------------------------------------------------
{
   // create filename; escaping characters that are not allowed within a filename
   QString sFileName = element.getDescription();
   sFileName = sFileName.replace( '/', '_' );
   sFileName = sFileName.replace( '.', '_' );

   int i = 0;
   while ( static_List.contains(sFileName) )
   {
      sFileName = element.getDescription() + "_" + QString::number(i);
      i++;
   }
   static_List.append( sFileName );

   // create file
   QFile f( dir.absolutePath() + "/" + TUX_HTMLEXPORT_SUBDIR + "/"
            + sFileName + ".html" );
   if ( f.open(QIODevice::WriteOnly) )                     // file opened successfully
   {
      QTextStream t( &f );                                 // use a text stream
      t.setCodec( "UTF-8" );

      if ( element.getInformationFormat() == InformationFormat::getByString("ASCII") )
      {
         t<<"<html>\n<head> <title>"+element.getDescription()+"</title> </head>\n<body>";
         QString text = element.getInformation();
         t<< text.replace(QRegExp("\n"),"<br>\n");
         t<<"\n</body></html>";
      }
      else
      {
         QString sText = element.getInformation();
         sText.replace( QRegExp("<meta name=\"qrichtext\" content=\"1\" />"), USE_UTF8_CHARSET ); // set correct charset
         t<<sText;
      }
      f.close();
   }

   // create the tree
   for ( i = 0; i < iTabCount; i++ )
      sTreeHTML.append("<img src=\"" + TUX_SPACER + "\">");

   iTabCount++;
   sTreeHTML.append(QString("<img src=\"" + TUX_DOT + "\">")
                    + "<a href=\"./"+ TUX_HTMLEXPORT_SUBDIR + "/"
                    + sFileName +".html\" target=main>"
                    + element.getDescription()+"</a><br>\n");


   QList<CInformationElement*>* pList = element.getChildren();
   if ( pList )
   {
      CInformationElement* pX;
      for ( int i = 0; i < pList->count(); i++ )
      {
         pX = pList->at( i );
         if ( pX )
            convertInformationElementToHTML( *pX, dir, sTreeHTML, iTabCount );
      }
   }
}




// -------------------------------------------------------------------------------
void HTMLWriter::createTopFrame( const QDir& dir )
// -------------------------------------------------------------------------------
{
   // "top.html" erstellen
   QString top = "<html> <head><title>TuxCards-HTMLExport</title></head>\n<body>\n";
   top += "<center>\n  <h1>TuxCards-HTML<i>Export</i></h1>\n";
   top += "<font size=-1><i>";
   top += TUX_VERSION;
   top += " - written by Alexander Theel</i></font>\n";
   top +="</body></html>";

   QFile f( dir.absolutePath()+"/top.html" );
   if ( f.open(QIODevice::WriteOnly) )                         // file opened successfully
   {
      QTextStream t( &f );                                     // use a text stream
      t<<top;
      f.close();
  }
}





