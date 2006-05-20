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

FXCopyFiles::FXCopyFiles(FXObject * tgt,FXSelector sel,FXStringList & f,const FXString & d) : FXTransfer(tgt,sel),
  destination(d),
  overwrite(MAYBE) {
  files.adopt(f);
  }

void FXCopyFiles::send_filename(const FXString & filename,FXuint size){
  FXMutexLock lock(mutex_status);
  status_info.code   =STATUS_INFORMATION;
  status_info.srcfile=filename;
  status_info.dstfile=filename;
  status_info.total  =size;
  if (size<=65536)
    status_info.current=size;
  else
    status_info.current=0;
  status_info.num++;
  notify();
  }

void FXCopyFiles::send_progress(FXuint pos){
  FXMutexLock lock(mutex_status);
  status_info.code    =STATUS_INFORMATION;
  status_info.current+=pos;
  notify();
  }


void FXCopyFiles::respondOverwrite(FXbool write,FXbool mode,FXbool cancel){
  if (write)
    status_info.code=1;
  else
    status_info.code=0;

  if (cancel)
    running=FALSE;

  condition_feedback.signal();
  }

FXbool FXCopyFiles::queryOverwriteFile(const FXString & src,const FXString dest){
  mutex_status.lock();
  status_info.code     = STATUS_OVERWRITE;
  status_info.srcfile  = src;
  status_info.dstfile  = dest;
  mutex_status.unlock();
  feedback.signal();
  condition_feedback.wait(mutex_feedback);
  if (status_info.code==1)
    return TRUE;
  else
    return FALSE;
  }

FXbool FXCopyFiles::copy(const FXString& srcfile,const FXString& dstfile){
  if(srcfile!=dstfile && running){
    FXuchar buffer[4096]; FXival nwritten,nread,nprogress=0; FXStat stat;FXbool done=FALSE;
    FXFile src(srcfile,FXIO::Reading);
    if(src.isOpen()){
      if(FXStat::stat(src,stat)){
        if (FXStat::exists(dstfile)) {
          if (overwrite==FALSE) return TRUE; /// do not overwrite
          else if (overwrite==MAYBE) {
            if (!queryOverwriteFile(srcfile,dstfile))
              return TRUE; // do not overwrite
            }
          }
        FXFile dst(dstfile,FXIO::Writing,stat.mode());
        if(dst.isOpen()) {
          send_filename(srcfile,stat.size());
          while(running){
            nread=src.readBlock(buffer,sizeof(buffer));
            if(nread<0) break;
            if(nread==0) {done=TRUE; break;}
            nwritten=dst.writeBlock(buffer,nread);
            if(nwritten<0) break;
            nprogress+=nwritten;
            if (nprogress>65536) {
              send_progress(nprogress);
              nprogress=0;
              }
            }
          if (!done) {
            send_fatal("Write/Read error while copying file");
            FXFile::remove(dstfile);
            return FALSE;
            }
          return TRUE;
          }
        send_fatal(FXStringFormat("Unable to open file for writing:\n%s.",dstfile.text()));
        return FALSE;
        }
      send_fatal(FXStringFormat("Unable to open file for reading:\n%s.",srcfile.text()));
      return FALSE;
      }
    }
  return FALSE;
  }


FXbool FXCopyFiles::copyFiles(const FXString& srcfile,const FXString& dstfile){
  if(srcfile!=dstfile && running){
    FXString name,linkname,dirname;
    FXStat srcstat;
    FXStat dststat;
    FXTRACE((1,"FXFile::copyFiles(%s,%s)\n",srcfile.text(),dstfile.text()));
    if(FXStat::statLink(srcfile,srcstat)){

      // Destination is a directory?
      if(FXStat::statLink(dstfile,dststat)){
        if(!dststat.isDirectory()){
          if(!overwrite) return FALSE;
          FXTRACE((1,"FXFile::remove(%s)\n",dstfile.text()));
          if(!FXFile::remove(dstfile)) return FALSE;
          }
        }

      // Source is a directory
      if(srcstat.isDirectory()){

        dirname = dstfile+PATHSEP+FXPath::name(srcfile);

        if (!FXStat::isDirectory(dirname)) {

          fxmessage("Creating Directory: %s\n",dirname.text());

          // Make directory
          if(!FXDir::create(dirname,srcstat.mode()|FXIO::OwnerWrite))
            return FALSE;

          }

        // Open source directory
        FXDir dir(srcfile);

        // Copy source directory
        while(dir.next()){

          // Next name
          name=dir.name();

          // Skip '.' and '..'
          if(name[0]=='.' && (name[1]=='\0' || (name[1]=='.' && name[2]=='\0'))) continue;

          // Recurse
          if(!copyFiles(srcfile+PATHSEP+name,dirname)) return FALSE;
          }

        // OK
        return TRUE;
        }

      // Source is a file
      if(srcstat.isFile()){

        //fxmessage("FXFile::copyFile(%s,%s)\n",srcfile.text(),FXString(dstfile+PATHSEP+FXPath::name(srcfile)).text());

        if (srcfile==dstfile+PATHSEP+FXPath::name(srcfile))
          return TRUE;

        // Simply copy
        if(!copy(srcfile,dstfile+PATHSEP+FXPath::name(srcfile)))
          return FALSE;

        // OK
        return TRUE;
        }

      // Source is symbolic link: make a new one
      if(srcstat.isLink()){
        linkname=FXFile::symlink(srcfile);
        FXTRACE((100,"symlink(%s,%s)\n",srcfile.text(),dstfile.text()));

        // New symlink to whatever old one referred to
        if(!FXFile::symlink(srcfile,dstfile+PATHSEPSTRING+FXPath::name(srcfile))) return FALSE;

        // OK
        return TRUE;
        }

      // Source is fifo: make a new one
      if(srcstat.isFifo()){
        FXTRACE((1,"FXPipe::create(%s)\n",dstfile.text()));

        // Make named pipe
        if(!FXPipe::create(dstfile,srcstat.mode())) return FALSE;

        // OK
        return TRUE;
        }

     /*
       // Source is device: make a new one
       if(S_ISBLK(status1.st_mode) || S_ISCHR(status1.st_mode) || S_ISSOCK(status1.st_mode)){
         FXTRACE((100,"mknod(%s)\n",newfile.text()));
         return ::mknod(newfile.text(),status1.st_mode,status1.st_rdev)==0;
         }
     */

      }
    }
  return FALSE;
  }


FXint FXCopyFiles::run(){
  FXMutexLock lock(mutex_feedback);
  for (FXint i=0; (i<files.no()) && running ;i++ ) {
    if (files[i].find(destination)==0){
      fxmessage("URL in URL copy: %s %s\n",destination.text(),files[i].text());
      }
    else  {
      fxmessage("Valid Copy: %s %s\n",destination.text(),files[i].text());
      }

//    fxmessage("Check (%s - %s): %s",destination.text(),files[i].text(),FXPath::relative(destination,files[i]).text());

    copyFiles(files[i],destination);
    }

  send_done();
  return 1;
  }
