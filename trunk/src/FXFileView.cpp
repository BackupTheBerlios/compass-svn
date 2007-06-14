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
#include "FXFileListEx.h"
#include "FXImageViewEx.h"
#include "FXFileApplication.h"
#include "FXFileManager.h"
#include "FXFileView.h"

FXDEFMAP(FXFileView) FXFileViewMap[]={
  FXMAPFUNC(SEL_SELECTED,FXFileView::ID_FILELIST,FXFileView::onFileListSelected),
  FXMAPFUNC(SEL_DESELECTED,FXFileView::ID_FILELIST,FXFileView::onFileListDeselected),
  FXMAPFUNC(SEL_DOUBLECLICKED,FXFileView::ID_FILELIST,FXFileView::onFileListDoubleClick),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,FXFileView::ID_FILELIST,FXFileView::onFileListRightClick),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,FXFileView::ID_DIRLIST,FXFileView::onDirListRightClick),
  FXMAPFUNC(SEL_DELETED,FXFileView::ID_DIRLIST,FXFileView::onDirListDeleted),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,FXFileView::ID_IMAGEPREVIEW,FXFileView::onImagePreviewClick),
  FXMAPFUNC(SEL_COMMAND,FXFileView::ID_SHOWHIDDENFILES,FXFileView::onCmdShowHiddenFiles),
  FXMAPFUNC(SEL_UPDATE,FXFileView::ID_SHOWHIDDENFILES,FXFileView::onUpdShowHiddenFiles),
//  FXMAPFUNC(SEL_COMMAND,FXFileView::ID_DIRLIST,FXFileView::onCmdDirList),
  FXMAPFUNC(SEL_CHANGED,FXFileView::ID_DIRLIST,FXFileView::onCmdDirList),
  FXMAPFUNC(SEL_COMMAND,FXFileView::ID_SHOWDIRTREE,FXFileView::onCmdShowDirTree),
  FXMAPFUNC(SEL_UPDATE,FXFileView::ID_SHOWDIRTREE,FXFileView::onUpdShowDirTree),
  FXMAPFUNC(SEL_COMMAND,FXFileView::ID_SHOWPARENTDIR,FXFileView::onCmdShowParentDirectory),
  FXMAPFUNC(SEL_UPDATE,FXFileView::ID_SHOWPARENTDIR,FXFileView::onUpdShowParentDirectory),
  FXMAPFUNC(SEL_COMMAND,FXFileView::ID_SHOWIMAGES,FXFileView::onCmdShowImages),
  FXMAPFUNC(SEL_UPDATE,FXFileView::ID_SHOWIMAGES,FXFileView::onUpdShowImages),
  };

FXIMPLEMENT(FXFileView,FXSplitter,FXFileViewMap,ARRAYNUMBER(FXFileViewMap));

FXFileView::FXFileView(){
  current_icon=NULL;
  }

FXFileView::FXFileView(FXComposite *p,FXObject * tgt,FXSelector sel,FXuint opts) : FXSplitter(p,opts,0,0,0,0){
  current_icon=NULL;
  target = tgt;
  message = sel;
  FXVerticalFrame * sunkenframe = NULL;

  dirframe = new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0,0,0,0,0);
  dirlist = new FXDirList(dirframe,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_Y|TREELIST_SHOWS_BOXES|TREELIST_SHOWS_LINES|TREELIST_BROWSESELECT|DIRLIST_NO_OWN_ASSOC);
  dirlist->setAssociations(FXFileApplication::me->getAssociations());
  sunkenframe = new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0,0,0,0,0);
  switcher  = new FXSwitcher(sunkenframe,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0);
  filelist  = new FXFileListEx(switcher,this,ID_FILELIST,LAYOUT_FILL_X|LAYOUT_FILL_Y|FILELIST_NO_OWN_ASSOC);
  imageview = new FXImageViewEx(switcher,NULL,this,ID_IMAGEPREVIEW,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  filelist->setAssociations(FXFileApplication::me->getAssociations());

  dirlist->setTarget(this);
  dirlist->setSelector(FXFileView::ID_DIRLIST);
  num_selected=0;
  
  loadSettings("settings");
  }

FXFileView::~FXFileView(){
  switcher=NULL;
  filelist=NULL;
  imageview=NULL;
  }

void FXFileView::update(){
  filelist->scan(TRUE);
  }

void FXFileView::selectFile(const FXString & file){
  for (FXint i=0;i<filelist->getNumItems();i++){
    if (filelist->getItemFilename(i)==file){
      filelist->selectItem(i,TRUE);
      filelist->setCurrentItem(i);
      filelist->makeItemVisible(i);
      break;
      }
    }
  }


void FXFileView::showFile(const FXString & file){
  for (FXint i=0;i<filelist->getNumItems();i++){
    if (filelist->getItemFilename(i)==file){
      filelist->makeItemVisible(i);
      filelist->setCurrentItem(i);
      break;
      }
    }
  }


/// View a url
void FXFileView::view(const FXString & url,FXIcon * ic,bool preview){
  if (!preview) {

    /// Clean Up Preview
    if (switcher->getCurrent()!=0){
      if (imageview->getImage()){
        delete imageview->getImage();
        imageview->setImage(NULL);
        }
      switcher->setCurrent(0);
      }

    filelist->setDirectory(url);
    dirlist->setDirectory(url);
    }
  current_url =url;
  current_icon=ic;
  num_selected=0;
  }

void FXFileView::preview(FXImage * image){
  if (switcher->getCurrent()==1){
    if (imageview->getImage()){
      delete imageview->getImage();
      }
    }
  else {
    switcher->setCurrent(1);
    }
  if (image->hasAlpha())
    image->blend(imageview->getBackColor());
  image->create();
  imageview->setImage(image);
  }


/// Current Url
const FXString & FXFileView::url() const{
  return current_url;
  }


FXString FXFileView::selected() const {
  return filelist->getCurrentFile();
  }

FXString FXFileView::selectedDirectory() const {
  return dirlist->getItemPathname(dirlist->getCurrentItem());
  }

FXHistory & FXFileView::history() {
  return current_history;
  }

FXIcon * FXFileView::icon()  const {
  return current_icon;
  }

/// Are we in Preview Mode
bool FXFileView::previewShown() const {
  if (switcher->getCurrent()==0)
    return false;
  else
    return true;
  }




void FXFileView::saveSettings(const FXString & key) const {
#if FOX_MINOR == 6
  FXuint style = filelist->getListStyle();
  if (style&ICONLIST_MINI_ICONS)
    getApp()->reg().writeUnsignedEntry(key.text(),"filelist-view-style",1);
  else if (style&ICONLIST_BIG_ICONS)
    getApp()->reg().writeUnsignedEntry(key.text(),"filelist-view-style",2);
  else
    getApp()->reg().writeUnsignedEntry(key.text(),"filelist-view-style",0);

  if (style&ICONLIST_COLUMNS)
    getApp()->reg().writeUnsignedEntry(key.text(),"filelist-view-arrangement",1);
  else
    getApp()->reg().writeUnsignedEntry(key.text(),"filelist-view-arrangement",0);

  getApp()->reg().writeUnsignedEntry(key.text(),"show-directory-tree",dirframe->shown());
#else
  FXuint style = filelist->getListStyle();
  if (style&ICONLIST_MINI_ICONS)
    getApp()->reg().writeUIntEntry(key.text(),"filelist-view-style",1);
  else if (style&ICONLIST_BIG_ICONS)
    getApp()->reg().writeUIntEntry(key.text(),"filelist-view-style",2);
  else
    getApp()->reg().writeUIntEntry(key.text(),"filelist-view-style",0);

  if (style&ICONLIST_COLUMNS)
    getApp()->reg().writeUIntEntry(key.text(),"filelist-view-arrangement",1);
  else
    getApp()->reg().writeUIntEntry(key.text(),"filelist-view-arrangement",0);

  getApp()->reg().writeUIntEntry(key.text(),"show-directory-tree",dirframe->shown());
#endif
  getApp()->reg().writeIntEntry(key.text(),"directory-tree-width",getSplit(0));
  }

void FXFileView::loadSettings(const FXString & key) {
#if FOX_MINOR == 6
  FXuint style = getApp()->reg().readUnsignedEntry(key.text(),"filelist-view-style",1);
  if (style==1)
    filelist->setListStyle(filelist->getListStyle()|ICONLIST_MINI_ICONS);
  else if (style==2)
    filelist->setListStyle(filelist->getListStyle()|ICONLIST_BIG_ICONS);

  FXuint arrangement = getApp()->reg().readUnsignedEntry(key.text(),"filelist-view-arrangement",0);
  if (arrangement==1)
    filelist->setListStyle(filelist->getListStyle()|ICONLIST_COLUMNS);

#else
  FXuint style = getApp()->reg().readUIntEntry(key.text(),"filelist-view-style",1);
  if (style==1)
    filelist->setListStyle(filelist->getListStyle()|ICONLIST_MINI_ICONS);
  else if (style==2)
    filelist->setListStyle(filelist->getListStyle()|ICONLIST_BIG_ICONS);

  FXuint arrangement = getApp()->reg().readUIntEntry(key.text(),"filelist-view-arrangement",0);
  if (arrangement==1)
    filelist->setListStyle(filelist->getListStyle()|ICONLIST_COLUMNS);
#endif
  setSplit(0,getApp()->reg().readIntEntry(key.text(),"directory-tree-width",100));
  if (getApp()->reg().readIntEntry(key.text(),"show-directory-tree",0)==0){
    dirframe->hide();
    }

  showhiddenfiles = getApp()->reg().readBoolEntry(key.text(),"show-hidden-files",FALSE);   
  filelist->showHiddenFiles(showhiddenfiles);
  dirlist->showHiddenFiles(showhiddenfiles);
    


  }


FXint FXFileView::getNumSelected() const {
  int num=0;
  for (FXint i=0;i<filelist->getNumItems();i++)
    if (filelist->isItemSelected(i)) num++;
  return num;
//  return num_selected;
  }


/// By default send messages to filelist
long FXFileView::onDefault(FXObject* sender,FXSelector sel,void* ptr){
  return filelist->handle(sender,sel,ptr);
  }

long FXFileView::onFileListSelected(FXObject*,FXSelector,void*ptr){
  num_selected++;
  return 1;
  }

long FXFileView::onFileListDeselected(FXObject*,FXSelector,void*ptr){
  if (num_selected>0) num_selected--;
  return 1;
  }

long FXFileView::onFileListDoubleClick(FXObject*,FXSelector,void*ptr){
  FXint index=(FXint)(FXival)ptr;
  if(0<=index){
    FXString url = filelist->getItemPathname(index);
    if (target) target->handle(this,FXSEL(SEL_OPENED,message),(void*)&url);
    }
  return 1;
  }

long FXFileView::onCmdDirList(FXObject*,FXSelector sel,void* ptr){
  FXTreeItem *item=(FXTreeItem*)ptr;
  FXString url=dirlist->getItemPathname(item);
  if (target) target->handle(this,FXSEL(SEL_OPENED,message),(void*)&url);
  return 1;
  }


long FXFileView::onImagePreviewClick(FXObject*,FXSelector,void*){
  if (target) {
    FXString url = FXPath::directory(current_url);
    target->handle(this,FXSEL(SEL_OPENED,message),(void*)&url);
    }
  return 1;
  }

long FXFileView::onDirListRightClick(FXObject*,FXSelector,void*ptr){
  FXEvent *event=(FXEvent*)ptr;
  if(event->moved) return 1;
  FXString current = selectedDirectory();
  FXTreeItem * item = dirlist->getItemAt(event->win_x,event->win_y);
  if (item) dirlist->setCurrentItem(item);
  if (target) target->handle(this,FXSEL(SEL_COMMAND,FXFileManager::ID_DIR_CONTEXTMENU),ptr);
  dirlist->setDirectory(current,TRUE);
  return 1;
  }

long FXFileView::onDirListDeleted(FXObject*,FXSelector,void*ptr){
  FXTreeItem * item = reinterpret_cast<FXTreeItem*>(ptr);
  FXString url = dirlist->getItemPathname(item);
  //fxmessage("url got deleted: %s - %d \n",url.text(),item->isSelected());
  return 1;
  }


long FXFileView::onFileListRightClick(FXObject*,FXSelector,void*ptr){
  FXEvent *event=reinterpret_cast<FXEvent*>(ptr);
  if(event->moved) return 1;
  FXint index=filelist->getItemAt(event->win_x,event->win_y);
  if (index>=0 && !filelist->isItemSelected(index)){
    num_selected=0;
    filelist->killSelection();
    filelist->setCurrentItem(index,TRUE);
    filelist->selectItem(index,TRUE);
    }
  if (target) {
    target->handle(this,FXSEL(SEL_COMMAND,FXFileManager::ID_FILE_CONTEXTMENU),ptr);
    }
  return 1;

#if 0
  FXFileApplication * fileapp = dynamic_cast<FXFileApplication*>(getApp());
  FXEvent *event=(FXEvent*)ptr;
  FXint index=filelist->getItemAt(event->win_x,event->win_y);
  FXMenuPane pane(this);


  if (index>=0 && !filelist->isItemSelected(index))
    filelist->selectItem(index,TRUE);


  // We clicked in the background
  if(index<0){
/*
    new FXMenuCommand(&pane,"&Up\t\tChange up one level.",upicon,this,ID_UPDIRECTORY);
    new FXMenuCommand(&pane,"&Back\t\tChange to previous directory.",backicon,this,ID_GO_BACKWARD);
    new FXMenuCommand(&pane,"&Forward\t\tChange to next directory.",forwicon,this,ID_GO_FORWARD);
    new FXMenuCommand(&pane,"&Home\t\tChange to home directory.",homeicon,this,ID_GO_HOME);
    new FXMenuCommand(&pane,"&Work\t\tChange to current working directory.",workicon,this,ID_GO_WORK);
    new FXMenuCommand(&pane,"Bookmark...\t\tBookmark this directory.",setbookicon,this,ID_BOOKMARK);
    new FXMenuSeparator(&pane);
    new FXMenuCascade(&pane,"Arrange",NULL,arrangemenu);
    new FXMenuSeparator(&pane);
    new FXMenuCascade(&pane,"Sort by",NULL,sortmenu);
    new FXMenuSeparator(&pane);
    new FXMenuCommand(&pane,"Reload\t\tReload this directory.",NULL,filelist,FXFileList::ID_REFRESH);
*/

    new FXMenuCommand(&pane,"&Up\t\tChange up one level.",fileapp->icon_up_small,this,ID_GO_UP);
    new FXMenuCommand(&pane,"&Back\t\tChange to previous directory.",fileapp->icon_back_small,this,ID_GO_BACK);
    new FXMenuCommand(&pane,"&Forward\t\tChange to next directory.",fileapp->icon_forward_small,this,ID_GO_FORWARD);
    new FXMenuCommand(&pane,"&Home\t\tChange to home directory.",fileapp->icon_home_small,this,ID_GO_HOME);

    new FXMenuSeparator(&pane);
    if (num_selected_files>0) {
      new FXMenuCommand(&pane,"Copy\tCtrl-C",fileapp->icon_copy,this,ID_COPY);
      new FXMenuCommand(&pane,"Cut\tCtrl-X",fileapp->icon_cut,this,ID_CUT);
      new FXMenuCommand(&pane,"Paste\tCtrl-V",fileapp->icon_paste,this,ID_PASTE);
      new FXMenuCommand(&pane,"Delete\tDel",fileapp->icon_delete,this,ID_DELETE);
      }
    else {
      new FXMenuCommand(&pane,"Paste\tCtrl-V",fileapp->icon_paste,this,ID_PASTE);
      }
    }

  // We clicked on an item
  else{

    new FXMenuCommand(&pane,"Open...",NULL,NULL,0);
    new FXMenuCommand(&pane,"Open with...",NULL,NULL,0);
    new FXMenuCommand(&pane,"Open with editor",NULL,NULL,0);
    new FXMenuSeparator(&pane);
    new FXMenuCommand(&pane,"Copy\tCtrl-C",fileapp->icon_copy,this,ID_COPY);
    new FXMenuCommand(&pane,"Cut\tCtrl-X",fileapp->icon_cut,this,ID_CUT);
    new FXMenuCommand(&pane,"Paste\tCtrl-V",fileapp->icon_paste,this,ID_PASTE);
    new FXMenuCommand(&pane,"Delete\tDel",fileapp->icon_delete,this,ID_DELETE);
    if (num_selected_files==1) {
      new FXMenuSeparator(&pane);
      new FXMenuCommand(&pane,"Link",NULL,NULL,0);
      new FXMenuCommand(&pane,"Rename\tF2",NULL,this,ID_RENAME);
      }
    new FXMenuSeparator(&pane);
    new FXMenuCommand(&pane,"Properties...",NULL,this,ID_PROPERTIES);


    }
    pane.create();
    pane.popup(NULL,event->root_x,event->root_y);
    getApp()->runModalWhileShown(&pane);
#endif
  return 1;
  }



void FXFileView::getClipboardSelection(FXStringList & clipboard,bool fromdirlist){
  if (fromdirlist) {
    FXTreeItem * item = dirlist->getCurrentItem();
    clipboard.append(FXURL::fileToURL(dirlist->getItemPathname(item)));
    }
  else {
    for(int i=0; i<filelist->getNumItems(); i++){
      if(filelist->isItemSelected(i) && filelist->getItemFilename(i)!=".." && filelist->getItemFilename(i)!="."){
        clipboard.append(FXURL::fileToURL(filelist->getItemPathname(i)));
        }
      }
    }
  }

void FXFileView::getSelection(FXStringList & clipboard){
  for(int i=0; i<filelist->getNumItems(); i++){
    if(filelist->isItemSelected(i) && filelist->getItemFilename(i)!=".." && filelist->getItemFilename(i)!="."){
      clipboard.append(filelist->getItemPathname(i));
      }
    }
  }


long FXFileView::onCmdShowHiddenFiles(FXObject*,FXSelector,void*){
  showhiddenfiles=!showhiddenfiles;
  filelist->showHiddenFiles(showhiddenfiles);
  dirlist->showHiddenFiles(showhiddenfiles);
  return 1;
  }

long FXFileView::onUpdShowHiddenFiles(FXObject*sender,FXSelector,void*){
  if (showhiddenfiles)
    sender->handle(this,FXSEL(SEL_COMMAND,ID_CHECK),0);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_UNCHECK),0);
  return 1;
  }


long FXFileView::onCmdShowDirTree(FXObject*,FXSelector,void*){
  if (dirframe->shown())
    dirframe->hide();
  else
    dirframe->show();

  dirframe->recalc();
  return 1;
  }

long FXFileView::onUpdShowDirTree(FXObject*sender,FXSelector,void*){
  if (dirframe->shown())
    sender->handle(this,FXSEL(SEL_COMMAND,ID_CHECK),0);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_UNCHECK),0);
  return 1;
  }


long FXFileView::onCmdShowParentDirectory(FXObject*,FXSelector,void*){
  filelist->showParents(!filelist->showParents());
  return 1;
  }

long FXFileView::onUpdShowParentDirectory(FXObject*sender,FXSelector,void*){
  if (filelist->showParents())
    sender->handle(this,FXSEL(SEL_COMMAND,ID_CHECK),0);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_UNCHECK),0);
  return 1;
  }



long FXFileView::onCmdShowImages(FXObject*,FXSelector,void*){
  if (filelist->showImages()) {
    filelist->setItemSpace(128);
    }
  else {
    filelist->setItemSpace(134);
    filelist->setImageSize(128);
    }
  filelist->showImages(!filelist->showImages());
  return 1;
  }

long FXFileView::onUpdShowImages(FXObject*sender,FXSelector,void*){
  if (filelist->showImages())
    sender->handle(this,FXSEL(SEL_COMMAND,ID_CHECK),0);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_UNCHECK),0);
  return 1;
  }

