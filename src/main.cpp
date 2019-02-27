/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Son Mar 26 23:04:15 CEST 2000
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

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include "./gui/CMainWindow.h"
#include "./CCommandLineOptions.h"



int main( int argc, char* argv[] )
{
   // (1)  Take care of potential command line options.
   if ( argc > 1 )
      CCommandLineOptions( argc, argv );


   // (2)  Initiate QApplication.
   QApplication app( argc, argv );


   // (3)  Takes care of translation - load them from resources instead of a local directory.
   QTranslator qtTranslator;
   //qtTranslator.load( "qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath) );
   qtTranslator.load( ":/qt_" + QLocale::system().name() );
   app.installTranslator( &qtTranslator );

   QTranslator myappTranslator;
   //myappTranslator.load( "/Users/alex/c++/Qt4/tuxcards/tuxcards_" + QLocale::system().name() );
   myappTranslator.load( ":/tuxcards_" + QLocale::system().name() );
   app.installTranslator( &myappTranslator );


   // (4)  Set up mainwindow and start the application.
   CMainWindow mainwindow( argc > 1 ? argv[1] : "" );
   return app.exec();
}
