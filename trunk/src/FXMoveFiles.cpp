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
#include "common.h"
#include "FXCopyFiles.h"
#include "FXMoveFiles.h"


FXMoveFiles::FXMoveFiles(FXObject * tgt,FXSelector sel,FXStringList & f,const FXString & d) : FXCopyFiles(tgt,sel,f,d){
  }

FXbool FXMoveFiles::moveFiles(const FXString& srcfile,const FXString& dstfile){
  if(srcfile!=dstfile){
    if(FXStat::exists(srcfile)){
      if(FXStat::exists(dstfile)){
        if(!overwrite) return FALSE;
        if(!FXFile::removeFiles(dstfile,TRUE)) return FALSE;
        }
      if(FXDir::rename(srcfile,dstfile)) return TRUE;
//      if(copyFiles(srcfile,dstfile)){
//        return FXFile::removeFiles(srcfile,TRUE);
//        }
      }
    }
  return FALSE;
  }

FXint FXMoveFiles::run(){
  FXMutexLock lock(mutex_feedback);
  for (FXint i=0; (i<files.no()) && running ;i++ ) {
    send_filename(files[i],0);
    moveFiles(files[i],destination+PATHSEPSTRING+FXPath::name(files[i]));
    }
  send_done();
  return 1;
  }



