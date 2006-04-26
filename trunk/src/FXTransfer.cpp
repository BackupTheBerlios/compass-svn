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

FXTransfer::FXTransfer(FXObject*tgt,FXSelector sel) : feedback(FXApp::instance(),tgt,sel),signalled(FALSE),running(TRUE) {
  }

FXTransfer::~FXTransfer(){
  }

void FXTransfer::dispose_and_join(){
  dispose();
  join();
  }

void FXTransfer::dispose() {
  running=FALSE;
  }

void FXTransfer::notify(){
  if (!signalled) {
    signalled=TRUE;
    feedback.signal();
    }
  }


void FXTransfer::send_fatal(const FXString & error){
  FXMutexLock lock(mutex_status);
  status_info.code = STATUS_FATAL;
  status_info.error = error;
  notify();
  running=FALSE;
  }

void FXTransfer::send_done(){
  if (!running) return;
  FXMutexLock lock(mutex_status);
  status_info.code = STATUS_DONE;
  notify();
  }

void FXTransfer::status(FXTransferStatus & p){
  FXMutexLock lock(mutex_status);
  p.srcfile=status_info.srcfile;
  p.dstfile=status_info.dstfile;
  p.total=status_info.total;
  p.current=status_info.current;
  p.code=status_info.code;
  p.num=status_info.num;
  p.error=status_info.error;
  signalled=FALSE;
  }
