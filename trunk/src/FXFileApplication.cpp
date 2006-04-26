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

/// For waitpid
#include <sys/types.h>
#include <sys/wait.h>

#include "xincs.h"
#include "common.h"
#include "FXFileManager.h"
#include "FXFileApplication.h"
#include "FXFileDictEx.h"
#include "FXFileView.h"
#include "icons.h"

FXDEFMAP(FXFileApplication) FXFileApplicationMap[]={
  FXMAPFUNC(SEL_COMMAND,FXFileApplication::ID_NEW_WINDOW,FXFileApplication::onCmdNewWindow),
  FXMAPFUNC(SEL_SIGNAL,FXFileApplication::ID_CHILD,FXFileApplication::onChildSignal),
  FXMAPFUNC(SEL_IO_READ,FXFileApplication::ID_DDE,FXFileApplication::onDDE),
  };


FXIMPLEMENT(FXFileApplication,FXApp,FXFileApplicationMap,ARRAYNUMBER(FXFileApplicationMap));

FXFileApplication * FXFileApplication::me=NULL;


FXFileApplication::FXFileApplication() : FXApp("fxfilemanager") {
  me=this;
  }

void FXFileApplication::init(int& argc,char** argv,FXbool connect){
  FXApp::init(argc,argv,TRUE);
  createIcons();

  new FXToolTip(this);

  associations = new FXFileDictEx(this);

  kde_clipboard=registerDragType("application/x-kde-cutselection");
  gnome_clipboard=registerDragType("x-special/gnome-copied-files");
  gnome_dragndrop=registerDragType("x-special/gnome-icon-list");

  /// Create Title Font
  FXFontDesc fontdescription;
  getNormalFont()->getFontDesc(fontdescription);
  fontdescription.weight = FXFont::Bold;
  font_title = new FXFont(this,fontdescription);
  font_title->create();

  /// Close Connection on Exec
  fcntl(ConnectionNumber((Display*)getDisplay()),F_SETFD,FD_CLOEXEC);

  /// Register SIGNAL handler
  addSignal(SIGCHLD,this,ID_CHILD);

  previewopenmode = (FXOpenMode) reg().readUnsignedEntry("settings","preview-open-mode",OPEN_IN_CURRENT);
  diropenmode     = (FXOpenMode) reg().readUnsignedEntry("settings","directory-open-mode",OPEN_IN_CURRENT);
  }


FXFileApplication::~FXFileApplication(){
  destroyIcons();
  removeSignal(SIGCHLD);
  }

void FXFileApplication::registerDDE(FXFile * in) {
  dde = in;
  addInput(dde->handle(),INPUT_READ,this,ID_DDE);
  }


void FXFileApplication::createIcons(){
	icon_back = new FXPNGIcon(this,back_png);
	icon_back->blend(getBaseColor());
	icon_forward = new FXPNGIcon(this,forward_png);
	icon_forward->blend(getBaseColor());
	icon_home = new FXPNGIcon(this,gohome_png);
	icon_home->blend(getBaseColor());
	icon_up = new FXPNGIcon(this,up_png);
	icon_up->blend(getBaseColor());


  icon_settings = new FXPNGIcon(this,settings_png);
  icon_settings->blend(getBaseColor());

	icon_back_small = new FXPNGIcon(this,back_small_png);
	icon_back_small->blend(getBaseColor());
  icon_back_small->create();
	icon_forward_small = new FXPNGIcon(this,forward_small_png);
	icon_forward_small->blend(getBaseColor());
  icon_forward_small->create();
	icon_home_small = new FXPNGIcon(this,gohome_small_png);
	icon_home_small->blend(getBaseColor());
  icon_home_small->create();
	icon_up_small = new FXPNGIcon(this,up_small_png);
	icon_up_small->blend(getBaseColor());
  icon_up_small->create();


  icon_tab_new = new FXPNGIcon(this,tab_new_png);
 	icon_tab_new->blend(getBaseColor());
  icon_tab_new->create();

  icon_tab_duplicate = new FXPNGIcon(this,tab_duplicate_png);
 	icon_tab_duplicate->blend(getBaseColor());
  icon_tab_duplicate->create();

  icon_tab_close  = new FXPNGIcon(this,tab_remove_png);
	icon_tab_close->blend(getBaseColor());
  icon_tab_close->create();

  icon_tab_detach = new FXPNGIcon(this,tab_breakoff_png);
	icon_tab_detach->blend(getBaseColor());
  icon_tab_detach->create();


  icon_about = new FXPNGIcon(this,compass_png);
	icon_about->blend(FXRGB(255,255,255));
  icon_about->create();

	icon_exit = new FXPNGIcon(this,exit_png);
	icon_exit->blend(getBaseColor());
	icon_new = new FXPNGIcon(this,window_new_png);
	icon_new->blend(getBaseColor());

	icon_clear = new FXPNGIcon(this,clear_right_png);
	icon_clear->blend(getBaseColor());

	icon_document = new FXGIFIcon(this,minidoc_gif);
	icon_folder = new FXGIFIcon(this,minifolder_gif);
	icon_folder_big = new FXGIFIcon(this,bigfolder_gif);
  icon_folder_open_big = new FXGIFIcon(this,bigfolderopen_gif);
	icon_document->create();
	icon_folder->create();
  icon_folder_open_big->create();

  icon_info = new FXPNGIcon(this,info_png);
	icon_info->blend(FXRGB(255,255,255));
  icon_info->create();

  icon_critical = new FXPNGIcon(this,critical_png);
	icon_critical->blend(FXRGB(255,255,255));
  icon_critical->create();

  icon_warning  = new FXPNGIcon(this,warning_png);
	icon_warning->blend(FXRGB(255,255,255));
  icon_warning->create();

  icon_lock = new FXPNGIcon(this,lock_png);
	icon_lock->blend(FXRGB(255,255,255));
  icon_warning->create();

	icon_copy = new FXGIFIcon(this,copy_gif,0,IMAGE_ALPHAGUESS);
	icon_cut = new FXGIFIcon(this,cut_gif,0,IMAGE_ALPHAGUESS);
	icon_paste = new FXGIFIcon(this,paste_gif,0,IMAGE_ALPHAGUESS);
  icon_delete = new FXGIFIcon(this,delete_gif,0,IMAGE_ALPHAGUESS);

  
  }

void FXFileApplication::destroyIcons(){
  delete icon_back;
  delete icon_forward;
  delete icon_home;
  delete icon_exit;
  delete icon_clear;
  delete icon_document;
  delete icon_folder;
	delete icon_folder_big;
  delete icon_folder_open_big;
  delete icon_up;
  delete icon_new;
  delete icon_copy;
  delete icon_cut;
  delete icon_paste;
  delete icon_delete;
  delete icon_back_small;
  delete icon_forward_small;
  delete icon_home_small;
  delete icon_up_small;
  delete icon_settings;
  delete icon_lock;
  }

void FXFileApplication::createWindow(FXbool checkargs){
  FXString url=FXSystem::getCurrentDirectory();
  FXFileManager * filemanager = new FXFileManager(this);
  filemanager->create();
  url = FXSystem::getCurrentDirectory();
  if (checkargs && getArgc()>1) {
    FXString path = getArgv()[1];
    path=FXPath::expand(path);
    path=FXPath::absolute(path);

    if (FXStat::exists(path)){
      url = path;
      }
    }
  filemanager->view(url,FROM_APPLICATION);
  filemanager->show();
  }


void FXFileApplication::createWindow(FXString url){
  FXFileManager * filemanager = new FXFileManager(this);
  filemanager->create();
  if (url.empty()) {
    url = FXSystem::getCurrentDirectory();
    }
  filemanager->view(url,FROM_APPLICATION);
  filemanager->show();
  }


void FXFileApplication::createWindow(FXFileView * fileview){
  FXFileManager * filemanager = new FXFileManager(this,fileview);
  filemanager->create();
  filemanager->show();
  }


void FXFileApplication::createWindow(FXTabBook * tabbook){
  FXFileManager * filemanager = new FXFileManager(this);
  filemanager->create();
  filemanager->duplicateTabBook(tabbook);
  filemanager->show();
  }



long FXFileApplication::onDDE(FXObject*,FXSelector,void*){
  FXString url;
  FXchar buffer[255];
  int nread=dde->readBlock(buffer,255);
  if (nread>0) {
    url=FXString(buffer,nread);
    fxmessage("compass.dde: %s\n",url.text());  
    if (FXStat::exists(url)){
      createWindow(url);
      } 
    else {
      createWindow(FXString::null);  
      }
    }
  return 1;
  }



FXIcon * FXFileApplication::getIconForDirectory(const FXString & url){
  FXFileAssoc * assoc = NULL;
  assoc = associations->findDirBinding(url.text());
  if (assoc) {
    if (assoc->miniicon)
      return assoc->miniicon;
    else
      return assoc->miniiconopen;
      }
  return icon_folder;
  }

FXIcon * FXFileApplication::getIconForFile(const FXString & url){
  FXFileAssoc * assoc = NULL;
  assoc = associations->findFileBinding(url.text());
  if (assoc) {
    if (assoc->miniicon)
      return assoc->miniicon;
    else
      return assoc->miniiconopen;
      }
  return icon_document;
  }

FXIcon * FXFileApplication::getIconForUrl(const FXString & url){
  if (FXStat::isDirectory(url))
    return getIconForDirectory(url);
  else
    return getIconForFile(url);
  }


FXFileDict * FXFileApplication::getAssociations() const{
  return associations;
  }



long FXFileApplication::onCmdNewWindow(FXObject*,FXSelector,void*){
  createWindow();
  return 1;
  }

long FXFileApplication::onChildSignal(FXObject*,FXSelector,void*){
  FXint pid;
  FXint status;
  while(1) {
    pid = waitpid(-1,&status,WNOHANG);
    if (pid>0) {
      continue;
      }
    break;
    }
  return 1;
  }





