/***************************************************************************
                          CCommandLineOptions.cpp  -  description
                             -------------------
    begin                : Don Sep 19 2002
    copyright            : (C) 2002 by alex
    email                : alex@rsh-318
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qapplication.h>
#include "CCommandLineOptions.h"
#include <iostream>

#include "CTuxCardsConfiguration.h"
#include "./information/xmlpersister.h"
#include "./information/htmlwriter.h"
#include "version.h"


// -------------------------------------------------------------------------------
CCommandLineOptions::CCommandLineOptions( int argc, char** argv )
// -------------------------------------------------------------------------------
{
   this->argc=argc;
   this->argv=argv;

   QString option = argv[1];
   if ( option == "-e" )
   {
      if ( (argc > 2) && (QString( argv[2] ) == "html") )
      {
         if ( argc <= 4 )
            htmlExport( argc==4 ? argv[3] : "" );
         else if ( argc == 5 )
            htmlExport( argv[3], argv[4] );
         else
            printUsage();
      }
      else
      {
         printUsage();
      }
      exit(0);

   }
   else if ( option == "-v" )
   {
      std::cout << "Tuxcards - The Notebook for Tuxfreaks" << std::endl
                << TUX_VERSION << std::endl
                << "www.tuxcards.de" << std::endl
                << "written by Alexander Theel" << std::endl;
      exit(0);

   }
   else if ( option == "-h" )
   {
      printUsage();
      exit(0);
   }

}


/**
 * export to default directory
 */
// -------------------------------------------------------------------------------
void CCommandLineOptions::htmlExport( QString fileName )
// -------------------------------------------------------------------------------
{
   // Verzeichnis erstellen
   QDir dir( QDir::homePath() );
   dir.mkdir( "tuxcardsHTML" );
   dir.cd( "tuxcardsHTML" );

   htmlExport( fileName, dir.absolutePath() );
}


// -------------------------------------------------------------------------------
void CCommandLineOptions::htmlExport( QString fileName, QString dirPath )
// -------------------------------------------------------------------------------
{
   QApplication app( argc, argv );
   if (fileName=="")
   {
      CTuxCardsConfiguration& refConfig = CTuxCardsConfiguration::getInstance();
      fileName = refConfig.getStringValue( CTuxCardsConfiguration::S_DATA_FILE_NAME );
   }
   if (!QFile::exists(fileName))
   {
      std::cout<<"No file specified or given file does not exist."<<std::endl;
      return;
   }
   if ( ! QDir(dirPath).exists() )
   {
      std::cout<<"The given directory '"<<dirPath.toStdString()<<"' does not exist."<<std::endl;
      return;
   }

   QFile file(fileName);
   CInformationCollection* collection = XMLPersister::createInformationCollection( file );
   HTMLWriter::writeCollectionToHTMLFile( *collection, dirPath );
   std::cout<<"File '"<<fileName.toStdString()<<"' exported to '"<<dirPath.toStdString()<<"'."<<std::endl;
}


// -------------------------------------------------------------------------------
void CCommandLineOptions::printUsage()
// -------------------------------------------------------------------------------
{
   std::cout<<"TuxCards is a hierarchical notebook. "
            <<"Every kind of notes and ideas may be "
            <<"managed and sorted within a tree structure.\n"
            <<"\n"
            <<"Usage:\ttuxcards [OPTION] [FILE]\n\n"
            <<"Examples:\n"
            <<"  tuxcards                           # starts the application\n"
            <<"  tuxcards <filename>                # starts the application using <filename>\n"
            <<"  tuxcards -e html                   # exports to html using the datafile found\n"
            <<"                                       in .tuxcards\n"
            <<"  tuxcards -e html <filename>        # exports to html using the <filename> as input\n"
            <<"  tuxcards -e html <filename> <dir>  # exports to html using the <filename> as input\n"
            <<"                                       and the specified <dir> as export directory\n"
            <<"\n"
            <<"Options:\n"
            <<"  -e html           HTML export; An optional file name and directory may be given.\n"
            <<"                    If no file is specified the file name is extracted from the\n"
            <<"                    configuration file '~/.tuxcards'. If no directory is given, the\n"
            <<"                    the html export will be done to the default export directory\n"
            <<"                    '~/tuxcardsHTML'.\n"
            <<"  -v                Print TuxCards version.\n"
            <<"  -h                Shows these usage information.\n"
            <<"\n"
            <<"For questions or bugs write to Alexander Theel <tuxcards@gmx.net>."
            <<std::endl;
}
