/*******************************************************************************
*                 Compass - Filemanager for the FOX Desktop                    *
********************************************************************************
*          Copyright (C) 2005-2006 Sander Jansen. All Rights Reserved          *
*                               ---                                            *
* This program is free software; you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by         *
* the Free Software Foundation; either version 2 of the License, or            *
* (at your option) any later version.                                          *
*                                                                              *
* This program is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of               *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
* GNU General Public License for more details.                                 *
* You should have received a copy of the GNU General Public License            *
* along with this program; if not, write to the Free Software                  *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    *
*******************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include "common.h"
#include "FXFileApplication.h"
#include "FXFileManager.h"

const char * fifo = "/tmp/compass.dde";

bool initDDE(int argc,char * argv[],FXFile & dde){
  FXStat info;

  FXString cmd = argv[1];
  if (cmd.empty()) cmd = FXSystem::getHomeDirectory();

  if (FXStat::statFile(fifo,info)) {

    if (!info.isFifo() && !FXFile::remove(fifo)) {           
      /// DDE mechanism not available. Lets just start normally.
      fxmessage("Unable to remove normal file...\n");
      return false;
      }

    FXFile fp;
    if (fp.open(fifo,FXIO::WriteOnly|FXIO::NonBlocking)){

      if (!cmd.empty()) {     
          /// Try writing cmd. If it fails, we know we have a stale handle
          if (fp.writeBlock(cmd.text(),cmd.length())){
            //fxmessage("GMM Already running...\n");
            return true;
            }
          }
        else {
          return true;
          }

      }    
    fxmessage("Removing Dead Pipe...\n");
    FXFile::remove(fifo);
    }

  /// Create Fifo if we're the only process
  if (mkfifo(fifo,S_IWUSR|S_IRUSR)!=0)
      return false;

  /// Open the Fifo
  if (!dde.open(fifo,FXIO::Reading|FXIO::WriteOnly,FXIO::OwnerWrite))
    fxmessage("Failed to open /tmp/gmm.dde\n");

  return false;
  }



int main(int argc,char **argv){
  FXFile dde;

  if (fxversion[0]==1 && (fxversion[1]==6) ) { /// Test for Stable Version of FOX 1.6
    if (FOX_MAJOR!=fxversion[0] || FOX_MINOR!=fxversion[1]){
      fxwarning("FOX Header (v%d.%d.%d) / Library (v%d.%d.%d) mismatch!  -\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL,fxversion[0],fxversion[1],fxversion[2]);
      return 1;
      }
    }   
  else if (fxversion[0]==1 && ( fxversion[1]==7)) { /// Test for Development version of FOX 1.7 
    if (FOX_MAJOR!=fxversion[0] || FOX_MINOR!=fxversion[1] || FOX_LEVEL!=fxversion[2]) {
      fxwarning("FOX Header (v%d.%d.%d) / Library (v%d.%d.%d) mismatch!  -\n",FOX_MAJOR,FOX_MINOR,FOX_LEVEL,fxversion[0],fxversion[1],fxversion[2]);
      return 1;
      }
    }
  else {
    fxwarning("Compass was linked to a unknown/unsupported version of the FOX Library (v%d.%d.%d)",fxversion[0],fxversion[1],fxversion[2]);
    }

  if (initDDE(argc,argv,dde))
    return 0;

  FXFileApplication application;

  application.init(argc,argv);

  application.create();

  application.registerDDE(&dde);

  application.createWindow(TRUE);

  FXint result = application.run();

  /// Remove DDE
  dde.close();
  FXFile::remove(fifo);

  return result;
  }
