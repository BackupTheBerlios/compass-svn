/*******************************************************************************
*                              Process Viewer                                  *
********************************************************************************
*            Copyright (C) 2005 Sander Jansen. All Rights Reserved             *
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
/*
$Id$
*/

#include "common.h"
#include "FXFileListEx.h"
#include "FXTransferManager.h"


FXDEFMAP(FXFileListEx) FXFileListExMap[]={
  FXMAPFUNC(SEL_DND_ENTER,0,FXFileListEx::onDNDEnter),
  FXMAPFUNC(SEL_DND_LEAVE,0,FXFileListEx::onDNDLeave),
  FXMAPFUNC(SEL_DND_DROP,0,FXFileListEx::onDNDDrop),
  FXMAPFUNC(SEL_DND_MOTION,0,FXFileListEx::onDNDMotion),
  };

FXIMPLEMENT(FXFileListEx,FXFileList,FXFileListExMap,ARRAYNUMBER(FXFileListExMap));


// For serialization
FXFileListEx::FXFileListEx(){
  }


FXFileListEx::FXFileListEx(FXComposite *p,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h): FXFileList(p,tgt,sel,opts,x,y,w,h){
    }


// Handle drag-and-drop enter, remember current directory
long FXFileListEx::onDNDEnter(FXObject* sender,FXSelector sel,void* ptr){
  FXIconList::onDNDEnter(sender,sel,ptr);
  orgdirectory=getDirectory();
  return 1;
  }


// Handle drag-and-drop leave, restore current directory prior to drag
long FXFileListEx::onDNDLeave(FXObject* sender,FXSelector sel,void* ptr){
  FXIconList::onDNDLeave(sender,sel,ptr);
  getApp()->removeTimeout(this,ID_OPENTIMER);
  stopAutoScroll();
  setDirectory(orgdirectory);
  return 1;
  }


// Handle drag-and-drop motion
long FXFileListEx::onDNDMotion(FXObject* sender,FXSelector sel,void* ptr){
  FXEvent *event=(FXEvent*)ptr;
  FXint index=-1;

  // Cancel open up timer
  getApp()->removeTimeout(this,ID_OPENTIMER);

  // Start autoscrolling
  if(startAutoScroll(event,FALSE)) return 1;

  // Give base class a shot
  if(FXIconList::onDNDMotion(sender,sel,ptr)) return 1;

  // Dropping list of filenames
  if(offeredDNDType(FROM_DRAGNDROP,urilistType)){

    // Drop in the background
    dropdirectory=getDirectory();

    // What is being done (move,copy,link)
    dropaction=inquireDNDAction();

    // We will open up a folder if we can hover over it for a while
    index=getItemAt(event->win_x,event->win_y);
    if(0<=index && isItemDirectory(index)){

      // Set open up timer
      getApp()->addTimeout(this,ID_OPENTIMER,700);

      // Directory where to drop, or directory to open up
      dropdirectory=getItemPathname(index);
      }

    // See if dropdirectory is writable
    if(FXStat::isWritable(dropdirectory)){
      FXTRACE((100,"accepting drop on %s\n",dropdirectory.text()));
      acceptDrop(DRAG_ACCEPT);
      }
    return 1;
    }
  return 0;
  }


// Handle drag-and-drop drop
long FXFileListEx::onDNDDrop(FXObject* sender,FXSelector sel,void* ptr){
  FXStringList files;
  FXString dropfiles,filename,url;
  FXint beg,end;

  // Cancel open up timer
  getApp()->removeTimeout(this,ID_OPENTIMER);

  // Stop scrolling
  stopAutoScroll();

  // Restore original directory
  setDirectory(orgdirectory);

  // Perhaps target wants to deal with it
  if(FXIconList::onDNDDrop(sender,sel,ptr)) return 1;

  // Get uri-list of files being dropped
  if(getDNDData(FROM_DRAGNDROP,urilistType,dropfiles)){

    // Tell drag source we got it
    dropFinished(DRAG_ACCEPT);

    // Loop over urls
    for(beg=0; beg<dropfiles.length(); beg=end+2){
      if((end=dropfiles.find_first_of("\r\n",beg))<0) end=dropfiles.length();

      // File url
      url=dropfiles.mid(beg,end-beg);

      // Source filename
      filename=FXURL::fileFromURL(url);

      if (filename.empty()) continue;

      /// Append to Files
      files.append(filename);
      }

    fxmessage("Files:\n");
    for (FXint i=0;i<files.no();i++){
      fxmessage("%d - %s\n",i,files[i].text());
      }

    if (dropaction==DRAG_COPY) {
      fxmessage("Copy Files...\n");
      FXTransferManager * manager = new FXTransferManager(this,files,dropdirectory,FXTransferManager::OP_COPY);
      manager->show();
      fxmessage("Done\n");
      }
    else if (dropaction==DRAG_MOVE) {
      fxmessage("Move Files...\n");
      FXTransferManager * manager = new FXTransferManager(this,files,dropdirectory,FXTransferManager::OP_MOVE);
      manager->show();
      
      //FXTransferManager manager(this,files,dropdirectory,FXTransferManager::OP_MOVE);
//      manager.execute();
      }
    else if (dropaction==DRAG_LINK) {
      fxmessage("DRAG_LINK not implemented");
      }
    fxmessage("Return 1\n");
    return 1;

#if 0
    // Loop over urls
    for(beg=0; beg<dropfiles.length(); beg=end+2){
      if((end=dropfiles.find_first_of("\r\n",beg))<0) end=dropfiles.length();

      // File url
      url=dropfiles.mid(beg,end-beg);

      // Source filename
      filesrc=FXURL::fileFromURL(url);

      // Destination filename
      filedst=dropdirectory+PATHSEPSTRING+FXPath::name(filesrc);

      // Move, Copy, or Link as appropriate
      if(dropaction==DRAG_MOVE){
        FXTRACE((1,"Moving file: %s to %s\n",filesrc.text(),filedst.text()));
        if(!FXFile::moveFiles(filesrc,filedst)) getApp()->beep();
        }
      else if(dropaction==DRAG_COPY){
        FXTRACE((1,"Copying file: %s to %s\n",filesrc.text(),filedst.text()));
        if(!FXFile::copyFiles(filesrc,filedst)) getApp()->beep();
        }
      else if(dropaction==DRAG_LINK){
        FXTRACE((1,"Linking file: %s to %s\n",filesrc.text(),filedst.text()));
        if(!FXFile::symlink(filesrc,filedst)) getApp()->beep();
        }
      }
    return 1;
#endif
    }
  return 0;
  }

