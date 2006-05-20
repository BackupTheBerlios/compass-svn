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
#include "FXRemoveFiles.h"

FXRemoveFiles::FXRemoveFiles(FXObject * tgt,FXSelector sel,FXStringList & f) : FXTransfer(tgt,sel),files(f),ignore(FALSE) {
  }

FXbool FXRemoveFiles::remove(const FXString& path){
  FXbool result;
  FXStat stat;
  result = FXStat::statLink(path,stat);
  if (!result)
    result = FXStat::statFile(path,stat);

  if (result) {
    if(stat.isDirectory()){
      FXDir dir(path);
      FXString name;
      while(dir.next() && running){
        name=dir.name();
        if(name[0]=='.' && (name[1]=='\0' || (name[1]=='.' && name[2]=='\0'))) continue;
        if(!remove(path+PATHSEP+name) && !ignore) return FALSE;
        }
      if (ignore) {
        FXDir::remove(path);
        return TRUE;
        }
      else if (FXDir::remove(path)==FALSE){
        queryDelete(path);
        if (ignore==FALSE) return FALSE;
        }
      return TRUE;
      }
    if (ignore) {
      FXFile::remove(path);
      return TRUE;
      }
    if (!FXFile::remove(path)) {
      queryDelete(path);
      if (ignore==FALSE) return FALSE;
      }
    return TRUE;
    }
  return FALSE;
  }

void FXRemoveFiles::queryDelete(const FXString & path) {
  mutex_status.lock();
  status_info.code=STATUS_DELETE;
  status_info.srcfile=path;
  mutex_status.unlock();
  notify();
  condition_feedback.wait(mutex_feedback);
  }

void FXRemoveFiles::respondDelete(FXbool b){
  ignore=b;
  condition_feedback.signal();
  }

FXint FXRemoveFiles::run(){
  mutex_feedback.lock();
  status_info.num=0;
  for (FXint i=0; (i<files.no()) && running ;i++ ) {
    if (!remove(files[i])) {
      running=FALSE;
      }
    }
  if (running) {
    mutex_status.lock();
    status_info.code=-1;
    mutex_status.unlock();
    feedback.signal();
    }
  mutex_feedback.unlock();
  return 1;
  }
