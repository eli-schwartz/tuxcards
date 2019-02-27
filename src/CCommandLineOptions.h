/***************************************************************************
                          CCommandLineOptions.h  -  description
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

#ifndef COMMANDLINEOPTIONS_H
#define COMMANDLINEOPTIONS_H

#include <qstring.h>

/**
  *@author Alexander Theel
  */

class CCommandLineOptions {
public: 
  CCommandLineOptions(int argc, char** argv);

  static void printUsage();
  void htmlExport(QString fileName);
  void htmlExport(QString fileName, QString dirPath);

public:
  int argc;
  char** argv;
};

#endif
