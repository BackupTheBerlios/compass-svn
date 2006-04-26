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
#include "FXRemoveFiles.h"
#include "FXTransferManager.h"

// Define Message Map
FXDEFMAP(FXTransferManager) FXTransferManagerMap[]={
  //____Message_Type______________ID_______________Message_Handler___
  FXMAPFUNC(SEL_IO_READ,FXTransferManager::ID_THREAD,FXTransferManager::onProgress),
  FXMAPFUNC(SEL_CLOSE,0,FXTransferManager::onCmdCancel),
  FXMAPFUNC(SEL_COMMAND,FXTransferManager::ID_CANCEL,FXTransferManager::onCmdCancel),
  FXMAPFUNC(SEL_SESSION_NOTIFY,0,FXTransferManager::onSessionNotify),
  FXMAPFUNC(SEL_SESSION_CLOSED,0,FXTransferManager::onSessionClosed),
  };

// Object implementation
FXIMPLEMENT(FXTransferManager,FXMainWindow,FXTransferManagerMap,ARRAYNUMBER(FXTransferManagerMap))


FXTransferManager::FXTransferManager(FXWindow* owner,const FXStringList & files,const FXString & destination,FXuint operation) :
  FXMainWindow(owner->getApp(),"Copying File(s)...",NULL,NULL,DECOR_TITLE|DECOR_BORDER,0,0,0,0,0,0,0,0), progress_bar(NULL){

  FXFontDesc 				fontdescription;
  FXLabel  				* label=NULL;
  FXMatrix 				* matrix=NULL;
  FXVerticalFrame * main=NULL;
  FXint 						spacing=15;

//  if (icon)
//  	spacing=5;

  if (operation==OP_COPY) {
    setTitle("Copying File(s)...");
    transfer = new FXCopyFiles(this,FXTransferManager::ID_THREAD,files,destination);
    }
  else if (operation==OP_MOVE){
    setTitle("Moving File(s)...");
    transfer = new FXMoveFiles(this,FXTransferManager::ID_THREAD,files,destination);
    }
  else if (operation==OP_DELETE){
    setTitle("Deleting File(s)...");
    transfer = new FXRemoveFiles(this,FXTransferManager::ID_THREAD,files);
    }


  /// Create Title Font
  getApp()->getNormalFont()->getFontDesc(fontdescription);
  fontdescription.weight = FXFont::Bold;
  titlefont = new FXFont(getApp(),fontdescription);
  titlefont->create();

  main = new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0,0,0);

  if (operation==OP_COPY)
    label = new FXLabel(main,"Copying File(s)...",NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,spacing,spacing,spacing,0);
  else if (operation==OP_MOVE)
    label = new FXLabel(main,"Moving File(s)...",NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,spacing,spacing,spacing,0);
  else if (operation==OP_DELETE)
    label = new FXLabel(main,"Deleting File(s)...",NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,spacing,spacing,spacing,0);

  label->setBackColor(FXRGB(255,255,255));
  label->setFont(titlefont);

  label = new FXLabel(main,"Please wait. This may take a while.",NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,spacing+30,spacing,0,spacing);
  label->setBackColor(FXRGB(255,255,255));

  new FXSeparator(main,SEPARATOR_GROOVE|LAYOUT_FILL_X);

  matrix = new FXMatrix(main,2,MATRIX_BY_COLUMNS|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,15,15,15,15);

  if (operation==OP_COPY)
    new FXLabel(matrix,"Copied:",NULL,LABEL_NORMAL|LAYOUT_FILL_X|JUSTIFY_RIGHT|LAYOUT_CENTER_Y);
  else if (operation==OP_MOVE)
    new FXLabel(matrix,"Moved:",NULL,LABEL_NORMAL|LAYOUT_FILL_X|JUSTIFY_RIGHT|LAYOUT_CENTER_Y);
  else if (operation==OP_DELETE)
    new FXLabel(matrix,"Delete:",NULL,LABEL_NORMAL|LAYOUT_FILL_X|JUSTIFY_RIGHT|LAYOUT_CENTER_Y);

  text_count = new FXTextField(matrix,10,NULL,0,TEXTFIELD_READONLY|TEXTFIELD_INTEGER|LAYOUT_FILL_X|JUSTIFY_LEFT);
  text_count->disable();
  text_count->setTextColor(FXRGB(0,0,255));
  text_count->setText("0");

  new FXLabel(matrix,"File:",NULL,LABEL_NORMAL|LAYOUT_FILL_X|JUSTIFY_RIGHT|LAYOUT_CENTER_Y);
  text_file = new FXTextField(matrix,40,NULL,0,TEXTFIELD_READONLY|LAYOUT_FILL_X);
  text_file->disable();


  if (operation!=OP_DELETE) {
    new FXLabel(matrix,"File Progress:",NULL,LABEL_NORMAL|LAYOUT_FILL_X|JUSTIFY_RIGHT|LAYOUT_CENTER_Y);
    progress_bar = new FXProgressBar(matrix,NULL,0,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|PROGRESSBAR_PERCENTAGE);
    }



  new FXSeparator(main,SEPARATOR_GROOVE|LAYOUT_FILL_X);
  FXHorizontalFrame *closebox=new FXHorizontalFrame(main,LAYOUT_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0);
  new FXButton(closebox,tr(" &Cancel "),NULL,this,FXDialogBox::ID_CANCEL,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_CENTER_X|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
  }

FXTransferManager::~FXTransferManager(){
  fxmessage("FXTransferManager::~FXTransferManager()\n");
  getApp()->removeTimeout(this,FXWindow::ID_SHOW);
  delete transfer;
  delete titlefont;
  }


// Show and raise window, placed properly on the screen
void FXTransferManager::show(FXuint placement){
  fxmessage("FXTransferManager::show(FXuint placement)\n");
  create();
  FXMainWindow::show(PLACEMENT_OWNER);
  getApp()->addTimeout(this,FXWindow::ID_SHOW,500);
  getApp()->refresh();
  transfer->start();
  }


/// Run modal invocation of the dialog
FXuint FXTransferManager::execute(FXuint placement){
  create();
  place(PLACEMENT_SCREEN);
  getApp()->addTimeout(this,FXWindow::ID_SHOW,500);
  getApp()->refresh();
  transfer->start();
  return getApp()->runModalFor(this);
  }

// Close dialog with a cancel
long FXTransferManager::onCmdCancel(FXObject*,FXSelector,void*){
  getApp()->removeTimeout(this,FXWindow::ID_SHOW);
  transfer->dispose_and_join();
  getApp()->stopModal(this,FALSE);
  hide();
  return 1;
  }

long FXTransferManager::onProgress(FXObject*,FXSelector,void*){
  bool addtimeout=false;
  FXint timeout=0;
  FXint response;
  FXTransferStatus progress;
  transfer->status(progress);

  if (getApp()->hasTimeout(this,FXWindow::ID_SHOW)) {
    timeout = getApp()->remainingTimeout(this,FXWindow::ID_SHOW);
    getApp()->removeTimeout(this,FXWindow::ID_SHOW);
    addtimeout=true;
    }

  switch(progress.code) {
    case STATUS_DONE:
      transfer->join();
      getApp()->stopModal(this,FALSE);
      hide();
      fxmessage("Joining...\n");
      close(FALSE);
      break;
    case STATUS_FATAL:
      transfer->join();
      FXMessageBox::error(this,MBOX_OK,"Fatal Error","%s",progress.error.text());
      //getApp()->stopModal(this,FALSE);
      hide();
      close(FALSE);
      break;
    case STATUS_OVERWRITE:
      response = FXMessageBox::question(this,MBOX_YES_NO_CANCEL,"Overwrite File?","File already exists. Overwrite?\nCopying file: %s\nto:%s",progress.srcfile.text(),progress.dstfile.text());
      if (response==MBOX_CLICKED_CANCEL){
        transfer->respondOverwrite(FALSE,MAYBE,TRUE);
        transfer->join();
        //getApp()->stopModal(this,FALSE);
        hide();
        close(FALSE);
        }
      else if (response==MBOX_CLICKED_YES){
        transfer->respondOverwrite(TRUE,MAYBE,FALSE);
        if (addtimeout)
          getApp()->addTimeout(this,FXWindow::ID_SHOW,timeout);
        }
      else {
        transfer->respondOverwrite(FALSE,MAYBE,FALSE);
        if (addtimeout)
          getApp()->addTimeout(this,FXWindow::ID_SHOW,timeout);
        }
      break;
    case STATUS_DELETE:
      response = FXMessageBox::question(this,MBOX_YES_NO,"Error Deleting File","Unable to remove file:\n%s\nContinue deleting remaining files?",progress.srcfile.text());
      if (response==MBOX_CLICKED_NO){
        transfer->respondDelete(FALSE);
        transfer->join();
        //getApp()->stopModal(this,FALSE);
        hide();
        close(FALSE);
        }
      else {
        transfer->respondDelete(TRUE);
        if (addtimeout)
          getApp()->addTimeout(this,FXWindow::ID_SHOW,timeout);
        }
      break;
    default:
      text_count->setText(FXStringVal(progress.num));
      text_file->setText(progress.srcfile);
      if (progress_bar) {
        progress_bar->setTotal(progress.total);
        progress_bar->setProgress(progress.current);
        }
      if (addtimeout)
        getApp()->addTimeout(this,FXWindow::ID_SHOW,timeout);
      break;
    }
  return 1;
  }

long FXTransferManager::onSessionNotify(FXObject*,FXSelector,void* ptr){
  return 1; /// Object to closing
  }

long FXTransferManager::onSessionClosed(FXObject*,FXSelector,void* ptr){
//  close(FALSE);
  return 1;
  }


