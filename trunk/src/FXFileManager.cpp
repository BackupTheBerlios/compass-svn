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
#include <signal.h>
#include <unistd.h>

#include "common.h"
#include <fxkeys.h>
#include "FXFileApplication.h"
#include "FXFileManager.h"
#include "FXFileProperties.h"
#include "FXHistory.h"
#include "FXURLBox.h"
#include "FXFileView.h"
#include "FXFileListEx.h"
#include "FXImageViewEx.h"
#include "FXFileReplaceDialog.h"

#include "FXTransferManager.h"
#include "aboutwindow.h"
#include "version.h"
#include "icons.h"

#include <sys/statvfs.h>

FXulong getTotalDiskSpace(const FXString & path) {
    struct statvfs64 info;
    if (statvfs64(path.text(),&info)==0){
        if (info.f_frsize)
            return info.f_blocks*info.f_frsize;
        else
            return info.f_blocks*info.f_bsize;
        }
    return 0;
    }

FXulong getAvailableDiskSpace(const FXString & path) {
    struct statvfs64 info;
    if (statvfs64(path.text(),&info)==0){
        if (info.f_frsize)
            return info.f_bavail*info.f_frsize;
        else
            return info.f_bavail*info.f_bsize;
        }
    return 0;
    }

FXString display_bytes(FXulong bytes) {
    static const FXchar * units[] = {NULL,"kilo","mega","giga","terra","peta","exa","zetta","yotta"};
    //static FXchar units[]={0,'K','M','G','T','P','E','Z','Y'};
    double num = bytes;
    FXint u=0;
    while(num>1024){
        num/=1024;
        u++;
        }
    if (u==0 || u > 8)
        return FXStringFormat("%ld bytes",bytes);
    else
        return FXStringFormat("%.1f %sbytes",num,units[u]);
    }



void show_header(FXComposite *p,const FXString & title,const FXString & subtitle,FXIcon * icon){
  FXFileApplication * app = dynamic_cast<FXFileApplication*>(FXApp::instance());
  FXint spacing=15;
  FXLabel * label=NULL;
  FXHorizontalFrame * main =new FXHorizontalFrame(p,LAYOUT_FILL_X,0,0,0,0,0,0,0,0,0,0);
  label = new FXLabel(main,FXString::null,icon,LABEL_NORMAL|LAYOUT_FILL_Y,0,0,0,0,10,10,0,0);
  label->setBackColor(FXRGB(255,255,255));
  FXVerticalFrame * textframe=new FXVerticalFrame(main,LAYOUT_FILL_X,0,0,0,0,0,0,0,0,0,0);

  label = new FXLabel(textframe,title,NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,0,spacing,spacing,0);
  label->setBackColor(FXRGB(255,255,255));
  label->setFont(app->font_title);

  label = new FXLabel(textframe,subtitle,NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,30,spacing,0,spacing);
  label->setBackColor(FXRGB(255,255,255));
  new FXSeparator(p,SEPARATOR_GROOVE|LAYOUT_FILL_X);
  }

void show_critical(FXComposite *p,const FXString & title,const FXString & subtitle){
  FXFileApplication * app = dynamic_cast<FXFileApplication*>(FXApp::instance());
  show_header(p,title,subtitle,app->icon_critical);
  }


const FXString ApplicationTitle = "Compass";


FXDEFMAP(FXFileManager) FXFileManagerMap[]={
  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_GO_FORWARD,FXFileManager::onUpdForwardHistory),
  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_GO_BACK,FXFileManager::onUpdBackHistory),

  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_DELETE,FXFileManager::onUpdHasSelection),
  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_PROPERTIES,FXFileManager::onUpdHasSelection),
  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_RENAME,FXFileManager::onUpdRename),
  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_REPLACE,FXFileManager::onUpdReplace),

  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_COPY,FXFileManager::onUpdCopy),
  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_CUT,FXFileManager::onUpdCut),
  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_PASTE,FXFileManager::onUpdPaste),

  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_NEW_FOLDER,FXFileManager::onUpdNewFolder),

//  FXMAPFUNC(SEL_TIMEOUT,FXFileManager::ID_DND_OPEN,FXFileManager::onListDNDOpen),


  FXMAPFUNC(SEL_OPENED,FXFileManager::ID_FILEVIEW,FXFileManager::onCmdFileView),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_DIR_CONTEXTMENU,FXFileManager::onCmdFileViewDirContext),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_FILE_CONTEXTMENU,FXFileManager::onCmdFileViewFileContext),

  FXMAPFUNC(SEL_CLIPBOARD_LOST,0,FXFileManager::onClipboardLost),
  FXMAPFUNC(SEL_CLIPBOARD_GAINED,0,FXFileManager::onClipboardGained),
  FXMAPFUNC(SEL_CLIPBOARD_REQUEST,0,FXFileManager::onClipboardRequest),

  FXMAPFUNC(SEL_CLOSE,0,FXFileManager::onCmdClose),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_CLOSE,FXFileManager::onCmdClose),
  FXMAPFUNC(SEL_SESSION_NOTIFY,0,FXFileManager::onSessionNotify),
  FXMAPFUNC(SEL_SESSION_CLOSED,0,FXFileManager::onSessionClosed),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_ABOUT,FXFileManager::onCmdAbout),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_LOCATION,FXFileManager::onCmdLocation),


  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_CLEAR_LOCATION,FXFileManager::onCmdClearLocation),

  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_GO_HOME,FXFileManager::onCmdGoHome),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_GO_FORWARD,FXFileManager::onCmdForwardHistory),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_GO_BACK,FXFileManager::onCmdBackHistory),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_GO_UP,FXFileManager::onCmdGoUp),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_GO_BOOKMARK,FXFileManager::onCmdGoBookmark),

  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_ADD_BOOKMARK,FXFileManager::onCmdAddBookmark),

  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_PROPERTIES,FXFileManager::onCmdProperties),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_DUP_WINDOW,FXFileManager::onCmdDuplicateWindow),

  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_COPY,FXFileManager::onCmdCopy),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_CUT,FXFileManager::onCmdCut),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_PASTE,FXFileManager::onCmdPaste),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_DELETE,FXFileManager::onCmdDelete),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_NEW_FOLDER,FXFileManager::onCmdNewFolder),

  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_RENAME,FXFileManager::onCmdRename),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_RENAME_DIRLIST,FXFileManager::onCmdRename),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_REPLACE,FXFileManager::onCmdReplace),


  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_TABVIEW,FXFileManager::onCmdActiveTab),

  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,FXFileManager::ID_TABITEM,FXFileManager::onCmdTabPress),
  FXMAPFUNC(SEL_RIGHTBUTTONRELEASE,FXFileManager::ID_TABITEM,FXFileManager::onCmdTabRelease),

  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_TEST,FXFileManager::onCmdTest),

  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_COPY_DIRLIST,FXFileManager::onCmdCopyDirList),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_CUT_DIRLIST,FXFileManager::onCmdCutDirList),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_PASTE_DIRLIST,FXFileManager::onCmdPasteDirList),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_DELETE_DIRLIST,FXFileManager::onCmdDeleteDirList),

  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_NEW_TAB,FXFileManager::onCmdNewTab),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_CLOSE_TAB,FXFileManager::onCmdCloseTab),
  FXMAPFUNC(SEL_COMMAND,FXFileManager::ID_DETACH_TAB,FXFileManager::onCmdDetachTab),
  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_CLOSE_TAB,FXFileManager::onUpdHasTab),
  FXMAPFUNC(SEL_UPDATE,FXFileManager::ID_DETACH_TAB,FXFileManager::onUpdHasTab),


  };

FXIMPLEMENT(FXFileManager,FXMainWindow,FXFileManagerMap,ARRAYNUMBER(FXFileManagerMap))



FXFileManager::FXFileManager(FXFileApplication * app) : FXMainWindow(app,ApplicationTitle,app->icon_folder,app->icon_folder_big,DECOR_ALL,0,0,500,400){
  fileview=NULL;
  attachfileview=false;
  bookmarks.setTarget(this);
  bookmarks.setSelector(ID_GO_BOOKMARK);
  bookmarks.setMaxFiles(10);
  bookmarks.setGroupName("bookmarks");
  initUserInterface();
  }


FXFileManager::FXFileManager(FXFileApplication * app,FXFileView * v) : FXMainWindow(app,ApplicationTitle,app->icon_folder,app->icon_folder_big,DECOR_ALL,0,0,500,400){
  fileview=v;
  fileview_delegator.setDelegate(fileview);
  bookmarks.setTarget(this);
  bookmarks.setSelector(ID_GO_BOOKMARK);
  bookmarks.setMaxFiles(10);
  bookmarks.setGroupName("bookmarks");
  attachfileview=true;
  initUserInterface();
  }


void FXFileManager::initUserInterface() {
  associations = FXFileApplication::me->associations;
  showhiddenfiles=FALSE;

  applogo = new FXGIFIcon(getApp(),logo_gif);

  tabbook=NULL;
  tabitem=NULL;
  dirwritable=false;

  FXMenuBar * menubar = new FXMenuBar(this,LAYOUT_FILL_X|LAYOUT_SIDE_TOP);
  new FXSeparator(this,LAYOUT_FILL_X|LAYOUT_SIDE_TOP|SEPARATOR_GROOVE);
  FXHorizontalFrame * toolframe = new FXHorizontalFrame(this,LAYOUT_FILL_X|LAYOUT_SIDE_TOP,0,0,0,0,2,2,2,2);
  new FXButton(toolframe,"\tBack\tBack",FXFileApplication::me->icon_back,this,FXFileManager::ID_GO_BACK,FRAME_RAISED|BUTTON_TOOLBAR);
  new FXButton(toolframe,"\tForward\tForward",FXFileApplication::me->icon_forward,this,FXFileManager::ID_GO_FORWARD,FRAME_RAISED|BUTTON_TOOLBAR);
  dirupbutton=new FXButton(toolframe,"\tUp\tUp",FXFileApplication::me->icon_up,this,FXFileManager::ID_GO_UP,FRAME_RAISED|BUTTON_TOOLBAR);
  homebutton=new FXButton(toolframe,"\tHome\tHome",FXFileApplication::me->icon_home,this,FXFileManager::ID_GO_HOME,FRAME_RAISED|BUTTON_TOOLBAR);
  new FXSeparator(toolframe,SEPARATOR_GROOVE|LAYOUT_FILL_Y);
  new FXButton(toolframe,"\tClear Path\tClear Path",FXFileApplication::me->icon_clear,this,FXFileManager::ID_CLEAR_LOCATION,FRAME_RAISED|BUTTON_TOOLBAR|LAYOUT_CENTER_Y);
  location = new FXURLBox(toolframe,20,this,FXFileManager::ID_LOCATION,LAYOUT_FILL_X|LAYOUT_CENTER_Y|FRAME_SUNKEN|FRAME_THICK);

  statusbar = new FXStatusBar(this,LAYOUT_FILL_X|LAYOUT_SIDE_BOTTOM|STATUSBAR_WITH_DRAGCORNER,0,0,0,0,1,1,1,1);

  main = new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0);


  // File menu
  filemenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&File",NULL,filemenu);
//  new FXMenuCommand(filemenu,"&New Window\tCtrl-N\tOpen New Window.",app->icon_new,getApp(),FXFileApplication::ID_NEW_WINDOW);
//  new FXMenuCommand(filemenu,"&Duplicate Window\tCtrl-D\tDuplicate Window.",app->icon_new,this,FXFileManager::ID_DUP_WINDOW);
//  new FXMenuCommand(filemenu,"&New Tab\tCtrl-T\tNew Tab.",app->icon_tab_new,this,FXFileManager::ID_NEW_TAB);
//  new FXMenuCommand(filemenu,"&Close Tab\tCtrl-W\tClose Tab.",app->icon_tab_close,this,FXFileManager::ID_CLOSE_TAB);
//  new FXMenuSeparator(filemenu);
  new FXMenuCommand(filemenu,"New Folder...\tCtrl-F\tCreate a new folder.",FXFileApplication::me->icon_folder,this,FXFileManager::ID_NEW_FOLDER);
  new FXMenuSeparator(filemenu);
  new FXMenuCommand(filemenu,"&Quit\tCtrl-Q\tQuit PathFinder",FXFileApplication::me->icon_exit,this,FXFileManager::ID_CLOSE);

  // View Menu
  editmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&Edit",NULL,editmenu);
  new FXMenuCommand(editmenu,"Copy\tCtrl-C\tCopy selected files",FXFileApplication::me->icon_copy,this,FXFileManager::ID_COPY);
  new FXMenuCommand(editmenu,"Cut\tCtrl-X\tCut selected files",FXFileApplication::me->icon_cut,this,FXFileManager::ID_CUT);
  new FXMenuCommand(editmenu,"Paste\tCtrl-V\tPaste files from clipboard to current directory.",FXFileApplication::me->icon_paste,this,FXFileManager::ID_PASTE);
  new FXMenuCommand(editmenu,"Delete\tDel\tDelete Selected Files",FXFileApplication::me->icon_delete,this,FXFileManager::ID_DELETE);
  new FXMenuSeparator(editmenu);
  new FXMenuCommand(editmenu,"Rename\tF2\tRename selected file or directory",NULL,this,FXFileManager::ID_RENAME);
  new FXMenuCommand(editmenu,"Replace\tCtrl-R\tReplace selected file or directory names using a expression",NULL,this,FXFileManager::ID_REPLACE);

  new FXMenuCommand(editmenu,"Select All\tCtrl-A\tSelect All Files",NULL,&fileview_delegator,FXIconList::ID_SELECT_ALL);
  new FXMenuSeparator(editmenu);
  new FXMenuCommand(editmenu,"Properties...\t\tEdit File and Folder permission for selected files.",NULL,this,FXFileManager::ID_PROPERTIES);
//  new FXMenuSeparator(editmenu);
//  new FXMenuCommand(editmenu,"Preferences",app->icon_settings);

  // View Menu
  bookmarkmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&Bookmarks",NULL,bookmarkmenu);
  new FXMenuCommand(bookmarkmenu,"Add Bookmark\tCtrl-B\tAdd bookmark for the current directory.",NULL,this,ID_ADD_BOOKMARK);
  FXMenuSeparator * sep = new FXMenuSeparator(bookmarkmenu);
  sep->setTarget(&bookmarks);
  sep->setSelector(FXRecentFiles::ID_ANYFILES);
  new FXMenuCommand(bookmarkmenu,FXString::null,NULL,&bookmarks,FXRecentFiles::ID_FILE_1);
  new FXMenuCommand(bookmarkmenu,FXString::null,NULL,&bookmarks,FXRecentFiles::ID_FILE_2);
  new FXMenuCommand(bookmarkmenu,FXString::null,NULL,&bookmarks,FXRecentFiles::ID_FILE_3);
  new FXMenuCommand(bookmarkmenu,FXString::null,NULL,&bookmarks,FXRecentFiles::ID_FILE_4);
  new FXMenuCommand(bookmarkmenu,FXString::null,NULL,&bookmarks,FXRecentFiles::ID_FILE_5);
  new FXMenuCommand(bookmarkmenu,FXString::null,NULL,&bookmarks,FXRecentFiles::ID_FILE_6);
  new FXMenuCommand(bookmarkmenu,FXString::null,NULL,&bookmarks,FXRecentFiles::ID_FILE_7);
  new FXMenuCommand(bookmarkmenu,FXString::null,NULL,&bookmarks,FXRecentFiles::ID_FILE_8);
  new FXMenuCommand(bookmarkmenu,FXString::null,NULL,&bookmarks,FXRecentFiles::ID_FILE_9);
  new FXMenuCommand(bookmarkmenu,FXString::null,NULL,&bookmarks,FXRecentFiles::ID_FILE_10);


  sortmenu=new FXMenuPane(this);
  new FXMenuRadio(sortmenu,"By Name",&fileview_delegator,FXFileList::ID_SORT_BY_NAME);
  new FXMenuRadio(sortmenu,"By Type",&fileview_delegator,FXFileList::ID_SORT_BY_TYPE);
  new FXMenuRadio(sortmenu,"By Size",&fileview_delegator,FXFileList::ID_SORT_BY_SIZE);
  new FXMenuRadio(sortmenu,"By Time",&fileview_delegator,FXFileList::ID_SORT_BY_TIME);
  new FXMenuRadio(sortmenu,"By User",&fileview_delegator,FXFileList::ID_SORT_BY_USER);
  new FXMenuRadio(sortmenu,"By Group",&fileview_delegator,FXFileList::ID_SORT_BY_GROUP);
  new FXSeparator(sortmenu);
  new FXMenuCheck(sortmenu,"Reverse",&fileview_delegator,FXFileList::ID_SORT_REVERSE);
  new FXMenuCheck(sortmenu,"Case Sensitive",&fileview_delegator,FXFileList::ID_SORT_CASE);

  // View Menu
  viewmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&View",NULL,viewmenu);
  new FXMenuCheck(viewmenu,"Show Hidden Files",&fileview_delegator,FXFileView::ID_SHOWHIDDENFILES);
  new FXSeparator(viewmenu);
  new FXMenuRadio(viewmenu,"Detailed View",&fileview_delegator,FXFileList::ID_SHOW_DETAILS);
  new FXMenuRadio(viewmenu,"Icon View",&fileview_delegator,FXFileList::ID_SHOW_MINI_ICONS);
  new FXMenuRadio(viewmenu,"Large Icon View",&fileview_delegator,FXFileList::ID_SHOW_BIG_ICONS);
  new FXSeparator(viewmenu);
  new FXMenuRadio(viewmenu,"Arrange by Columns",&fileview_delegator,FXFileList::ID_ARRANGE_BY_COLUMNS);
  new FXMenuRadio(viewmenu,"Arrange by Rows",&fileview_delegator,FXFileList::ID_ARRANGE_BY_ROWS);
  new FXSeparator(viewmenu);
  new FXMenuCascade(viewmenu,"Sort Files",NULL,sortmenu);
  new FXSeparator(viewmenu);
  new FXMenuCheck(viewmenu,"Show Directory Tree",&fileview_delegator,FXFileView::ID_SHOWDIRTREE);

  // View Menu
  windowmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&Window",NULL,windowmenu);
  new FXMenuCommand(windowmenu,"&New Window\tCtrl-N\tOpen New Window.",FXFileApplication::me->icon_new,getApp(),FXFileApplication::ID_NEW_WINDOW);
  new FXMenuCommand(windowmenu,"&Duplicate Window\tCtrl-D\tDuplicate Window.",FXFileApplication::me->icon_new,this,FXFileManager::ID_DUP_WINDOW);
  new FXSeparator(windowmenu);
  new FXMenuCommand(windowmenu,"&New Tab\tCtrl-T\tNew Tab.",FXFileApplication::me->icon_tab_new,this,FXFileManager::ID_NEW_TAB);
  new FXMenuCommand(windowmenu,"&Duplicate Tab\tCtrl-Y\tDuplicate Tab.",FXFileApplication::me->icon_tab_duplicate,this,FXFileManager::ID_NEW_TAB);
  new FXMenuCommand(windowmenu,"&Detach Tab\tCtrl-U\tClose Tab.",FXFileApplication::me->icon_tab_detach,this,FXFileManager::ID_DETACH_TAB);
  new FXMenuCommand(windowmenu,"&Close Tab\tCtrl-W\tClose Tab.",FXFileApplication::me->icon_tab_close,this,FXFileManager::ID_CLOSE_TAB);

  // Help menu
  helpmenu=new FXMenuPane(this);
  new FXMenuTitle(menubar,"&Help",NULL,helpmenu,LAYOUT_RIGHT);
  new FXMenuCommand(helpmenu,"About "+ApplicationTitle,NULL,this,ID_ABOUT);


  getAccelTable()->addAccel(KEY_BackSpace,this,FXSEL(SEL_COMMAND,FXFileManager::ID_GO_UP));
  getAccelTable()->addAccel(KEY_Home,this,FXSEL(SEL_COMMAND,FXFileManager::ID_GO_HOME));
  getAccelTable()->addAccel(KEY_KP_Home,this,FXSEL(SEL_COMMAND,FXFileManager::ID_GO_HOME));
  getAccelTable()->addAccel(KEY_Page_Up,this,FXSEL(SEL_COMMAND,FXFileManager::ID_GO_BACK));
  getAccelTable()->addAccel(KEY_Page_Down,this,FXSEL(SEL_COMMAND,FXFileManager::ID_GO_FORWARD));
  getAccelTable()->addAccel(KEY_KP_Page_Up,this,FXSEL(SEL_COMMAND,FXFileManager::ID_GO_BACK));
  getAccelTable()->addAccel(KEY_KP_Page_Down,this,FXSEL(SEL_COMMAND,FXFileManager::ID_GO_FORWARD));
  }

FXFileManager::~FXFileManager(){
  delete filemenu;
  delete editmenu;
  delete sortmenu;
  delete viewmenu;
  delete bookmarkmenu;
  delete windowmenu;
  delete helpmenu;
  delete applogo;
  }

void FXFileManager::create(){
  FXMainWindow::create();
  loadSettings();
  if (attachfileview){

    /// Attach Window to our tree
    fileview->reparent(main);

    /// Make it point to our window instead
    fileview->setTarget(this);

    /// Add to our history browser
    fileview->history().append(fileview->url());

    /// Set the Window Title
    setTitle(FXPath::name(fileview->url())+" - "+ApplicationTitle);

    /// Set the Location Bar
    location->setText(fileview->url());
    location->setIcon(fileview->icon());

    /// Disable or enable directory up button
    if (FXPath::isTopDirectory(fileview->url()))
      dirupbutton->disable();
    else
      dirupbutton->enable();

    /// Disable or enable home directory button
    if (FXSystem::getHomeDirectory()==fileview->url())
      homebutton->disable();
    else
      homebutton->enable();

    /// Add Url to visited locations list
    FXbool found=FALSE;
    for (FXint i=0;i<location->getNumItems();i++){
      if (location->getItemText(i)==fileview->url()){
        found=TRUE;
        location->moveItem(0,i);
        break;
        }
      }
    if (!found) {
      location->prependItem(fileview->url(),fileview->icon());
      location->setNumVisible(FXMIN(location->getNumItems(),9));
      }
    }
  else {
    fileview = new FXFileView(main,this,ID_FILEVIEW,LAYOUT_FILL_X|LAYOUT_FILL_Y);
    fileview_delegator.setDelegate(fileview);
    fileview->loadSettings("settings");
    fileview->create();
    }

  }



void FXFileManager::loadSettings(){
  FXint xx,yy,ww,hh;
//  splitter->setSplit(0,getApp()->reg().readIntEntry("settings","directory-tree-width",100));

  xx = getApp()->reg().readIntEntry("settings","window-x",0);
  yy = getApp()->reg().readIntEntry("settings","window-y",0);
  ww = getApp()->reg().readIntEntry("settings","window-width",500);
  hh = getApp()->reg().readIntEntry("settings","window-height",400);

  FXString delimiters = getApp()->reg().readStringEntry("settings","location-delimiters","!\"#$%&'()*+,-./:;<=>?@[\\]^`{|}~");
  location->setDelimiters(delimiters);


//  if (getApp()->reg().readIntEntry("settings","show-directory-tree",0)==0){
//    dirframe->hide();
//    }

/*
  FXint tabs = getApp()->reg().readUnsignedEntry("settings","tab-count",0);
  if (tabs>0) {
    FXString entry;
    FXString url;
    for (FXint i=0;i<tabs;i++){
      entry="tab-"+FXStringVal(i)+"-url";
      url = getApp()->reg().readStringEntry("settings",entry.text(),NULL);
      if (url.empty()) break;
      if (i==0)
        createTab(true);
      else
        createTab();

      setTabLabel(tabitem,url,myapp->getIconForUrl(url));
      }
    }
*/
  position(xx,yy,ww,hh);
  }

void FXFileManager::saveSettings(){
//  getApp()->reg().writeUnsignedEntry("settings","show-directory-tree",dirframe->shown());
//  getApp()->reg().writeIntEntry("settings","directory-tree-width",splitter->getSplit(0));
  getApp()->reg().writeIntEntry("settings","window-width",getWidth());
  getApp()->reg().writeIntEntry("settings","window-height",getHeight());
  getApp()->reg().writeIntEntry("settings","window-x",getX());
  getApp()->reg().writeIntEntry("settings","window-y",getY());

/*
  if (tabbook) {
    getApp()->reg().writeUnsignedEntry("settings","tab-count",tabbook->numChildren()/2);
    for (int i=0;i<(tabbook->numChildren()/2);i++){
      FXFileView * view = dynamic_cast<FXFileView*>(tabbook->childAtIndex((i*2)+1)->getFirst());
      getApp()->reg().writeStringEntry("settings",FXStringFormat("tab-%d-url",i).text(),view->url().text());
      }
    }
  else {
    getApp()->reg().writeUnsignedEntry("settings","tab-count",0);
    }
*/
  }



void FXFileManager::duplicateTabBook(FXTabBook * book){
  FXASSERT(book);
  for (int i=0;i<(book->numChildren()/2);i++){
    FXFileView * v = dynamic_cast<FXFileView*>(book->childAtIndex((i*2)+1)->getFirst());
    FXASSERT(v);
    if (i==0)
      createTab(true);
    else
      createTab();
//
    //setTabLabel(tabitem,v->url(),v->icon());
    view(v->url(),FROM_NEWTAB);
    }
//  tabbook->setCurrent(book->getCurrent(),TRUE);
  }




void FXFileManager::createTab(bool initialtabonly) {
  FXVerticalFrame * frame=NULL;
  if (!tabbook)  {
    // Create Tab Book And First Tab Item
    tabbook = new FXTabBook(main,this,ID_TABVIEW,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,3,3,0,2);
    tabitem = new FXTabItem(tabbook,FXString::null);
    frame   = new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK,0,0,0,0);
    FXASSERT(fileview);
    setTabLabel(tabitem,fileview->url(),fileview->icon());
    tabbook->create();
    fileview->reparent(frame);
    tabitem->setTarget(this);
    tabitem->setSelector(ID_TABITEM);
    }

  if (initialtabonly) return;

  tabitem      = new FXTabItem(tabbook,FXString::null);
  frame        = new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK,0,0,0,0);
  fileview     = new FXFileView(frame,this,ID_FILEVIEW,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  fileview_delegator.setDelegate(fileview);
  tabitem->setTarget(this);
  tabitem->setSelector(ID_TABITEM);

  tabbook->setCurrent(tabbook->numChildren()/2);

  frame->create();
  tabitem->create();
  }

void FXFileManager::closeTab(FXint tab){
  if (!tabbook) return;

  // Delete Tab Item and Frame
  delete tabbook->childAtIndex((tab*2)+1);
  delete tabbook->childAtIndex(tab*2);

  // Delete Tabbook if only one tab left
  if (tabbook->numChildren()==2) {

    fileview = dynamic_cast<FXFileView*>(tabbook->childAtIndex(1)->getFirst());
    fileview_delegator.setDelegate(fileview);
    fileview->reparent(main);

    /// Set the Window Title
    setTitle(FXPath::name(fileview->url())+" - "+ApplicationTitle);

    /// Set the Location Bar
    location->setText(fileview->url());
    location->setIcon(fileview->icon());

    delete tabbook;
    tabbook=NULL;
    tabitem=NULL;
    }
  else {
    // Make previous tab active
    updateActiveTab((tab==0)?1:tab-1);
    }
  }

void FXFileManager::showTabs() {
  if (tabbook) return;

  FXVerticalFrame * frame=NULL;

  }



void FXFileManager::updateActiveTab(FXint tab) {
  tabitem  = dynamic_cast<FXTabItem*>(tabbook->childAtIndex(tab*2));
  fileview = dynamic_cast<FXFileView*>(tabbook->childAtIndex((tab*2)+1)->getFirst());
  fileview_delegator.setDelegate(fileview);

  /// Set the Window Title
  setTitle(FXPath::name(fileview->url())+" - "+ApplicationTitle);

  /// Set the Location Bar
  location->setText(fileview->url());
  location->setIcon(fileview->icon());
  }


void FXFileManager::setTabLabel(FXTabItem * item,const FXString & url,FXIcon * icon){
  if (item){
    FXString name = FXPath::name(url);
    if (name.empty()) name = FXPath::directory(url);
    item->setText(name);
    item->setIcon(icon);
    }
  }


/*
  Update All GUI Items based on the new URL
*/
void FXFileManager::updateView(const FXString & url,FXIcon * icon,bool do_preview) {

  fileview->view(url,icon,do_preview);

  /// Disable or enable directory up button
  if (FXPath::isTopDirectory(url))
    dirupbutton->disable();
  else
    dirupbutton->enable();


  /// Disable or enable home directory button
  if (FXSystem::getHomeDirectory()==url)
    homebutton->disable();
  else
    homebutton->enable();

  /// Add Url to visited locations list
  FXbool found=FALSE;
  for (FXint i=0;i<location->getNumItems();i++){
    if (location->getItemText(i)==url){
      found=TRUE;
      location->moveItem(0,i);
      break;
      }
    }
  if (!found) {
    location->prependItem(url,icon);
    location->setNumVisible(FXMIN(location->getNumItems(),9));
    }

  /// Set the Location Text and Icon
  location->setText(url);
  location->setIcon(icon);

  /// Set Tab and Window Title
  FXString title = FXPath::name(url);
  if (title.empty()) title = FXPath::directory(url);
  if (tabitem) {
    tabitem->setIcon(icon);
    tabitem->setText(title);
    }
  setTitle(title+" - "+ApplicationTitle);

  /// Update Tab Text and Icon
//  setTabLabel(tabitem,url,icon);

  /// Update the Window Title
  //setTitle(FXPath::name(url)+" - "+ApplicationTitle);



  statusbar->getStatusLine()->setNormalText(FXStringFormat("Available Disk Space: %s",display_bytes(getAvailableDiskSpace(url)).text()));
  }


void FXFileManager::view(const FXString & url,FXViewSource src) {
  bool do_preview=false;

  /// Directory
  if (FXStat::isDirectory(url)) {
    if (src==FROM_FILELIST) {
      if (FXFileApplication::me->diropenmode==OPEN_IN_TAB) {
        createTab();
        }
      else if (FXFileApplication::me->diropenmode==OPEN_IN_WINDOW) {
        FXFileApplication::me->createWindow(url);
        return; // We're done here. New Window should take care of things
        }
      }
    icon = FXFileApplication::me->getIconForDirectory(url);
    dirwritable = isWritable(url);
    }
  else if (FXStat::isFile(url)){
    /// If it is a executable, execute it.
    if (FXStat::isExecutable(url)) {
      execute(url);
      return;
      }
    else if (preview(url)) {
      do_preview=true;
      if (FXFileApplication::me->previewopenmode==OPEN_IN_WINDOW)
        return;
      }
    else {
      /// Last chance... Try Executing the filebinding instead. No matter what happens, we're done.
      executeBinding(url);
      location->setText(fileview->url());
      return;
      }
    icon = FXFileApplication::me->getIconForFile(url);

    dirwritable = isWritable(FXPath::directory(url));
    }
  else {    
    location->setText(fileview->url());
    return;
    }  

  if (src!=FROM_HISTORY)
    fileview->history().append(url);

  /// Update the rest of the GUI
  updateView(url,icon,do_preview);
  }

bool FXFileManager::preview(const FXString & url){
  FXIconSource iconsource(getApp());
  FXImage * old   = NULL;
  FXImage * image = iconsource.loadImageFile(url);
  if (image) {
    if (FXFileApplication::me->previewopenmode==OPEN_IN_TAB) {
      createTab();
      }
    else if (FXFileApplication::me->previewopenmode==OPEN_IN_WINDOW){
      FXFileApplication::me->createWindow(url);
      return true; // We're done here. New Window should take care of things
      }
    fileview->preview(image);
    return true;
    }
  return false;
  }



void FXFileManager::executeBinding(const FXString & url) {
  FXFileAssoc * assoc = associations->findFileBinding(url.text());
  if (assoc && !assoc->command.empty()){
    execute(assoc->command + " " + FXPath::enquote(url));
    }
  }

void FXFileManager::execute(const FXString & url) {
  pid_t pid;
  pid=fork();
  if (pid==-1){ /// Failure delivered to Parent Process
    return;
    }
  else if (pid==0) { /// Child Process
    execlp("/bin/sh", "sh", "-c",url.text(), (char *)0);
    exit(EXIT_FAILURE);
    }
  else { /// Parent Process
    return;
    }
  }

bool FXFileManager::isWritable(const FXString & url) {
  if (access(url.text(),W_OK)==0)
    return true;
  else
    return false;
  }



long FXFileManager::onCmdActiveTab(FXObject*,FXSelector,void*ptr){
  FXint tab = (FXint)(FXival)ptr;
  updateActiveTab(tab);
  return 1;
  }


long FXFileManager::onCmdTabPress(FXObject*,FXSelector,void*ptr){
  return 1;
  }

long FXFileManager::onCmdTabRelease(FXObject*sender,FXSelector,void*ptr){
  FXEvent * event = reinterpret_cast<FXEvent*>(ptr);
  if (event->moved) return 1;
  tabbook->setCurrent(tabbook->indexOfChild((FXWindow*)sender)/2,TRUE);
  FXMenuPane pane(this);
  new FXMenuCommand(&pane,"New Tab",FXFileApplication::me->icon_tab_duplicate,this,ID_NEW_TAB);
  new FXMenuCommand(&pane,"Duplicate Tab",FXFileApplication::me->icon_tab_new,this,ID_NEW_TAB);
  new FXMenuCommand(&pane,"Detach Tab",FXFileApplication::me->icon_tab_detach,this,ID_DETACH_TAB);
  new FXMenuCommand(&pane,"Close Tab",FXFileApplication::me->icon_tab_close,this,ID_CLOSE_TAB);
  pane.create();
  pane.popup(NULL,event->root_x,event->root_y);
  getApp()->runModalWhileShown(&pane);
  return 1;
  }

long FXFileManager::onCmdNewTab(FXObject*,FXSelector,void*ptr){
  FXString url = fileview->url();
  if (fileview->previewShown()) { // Do not replicate previews
    url = FXPath::directory(url);
    }
  createTab();
  view(url,FROM_NEWTAB);
  return 1;
  }

long FXFileManager::onCmdCloseTab(FXObject*,FXSelector,void*ptr){
  if (!tabbook) return 1;
  closeTab(tabbook->getCurrent());
  return 1;
  }

long FXFileManager::onCmdDetachTab(FXObject*,FXSelector,void*ptr){
  if (!tabbook) return 1;
  FXFileApplication::me->createWindow(fileview);
  closeTab(tabbook->getCurrent());
  return 1;
  }


long FXFileManager::onUpdHasTab(FXObject*sender,FXSelector,void*ptr){
  if (tabbook && tabbook->numChildren()>2)
    sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),NULL);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),NULL);
  return 1;
  }



// Change the directory from the FXDirList
long FXFileManager::onCmdLocation(FXObject*,FXSelector,void* ptr){
  view(location->getText(),FROM_LOCATIONBAR);
  return 1;
  }

long FXFileManager::onImagePreviewClick(FXObject*,FXSelector,void* ptr){
//  if (tabbook) {
//    closeTab(tabbook->getCurrent());
//    }
//  else {
    view(fileview->history().back(),FROM_HISTORY);
//    }
  return 1;
  }

long FXFileManager::onSessionNotify(FXObject*,FXSelector,void* ptr){
  saveSettings();
  return 0;
  }

long FXFileManager::onSessionClosed(FXObject*,FXSelector,void* ptr){
  close(FALSE);
  return 1;
  }

long FXFileManager::onCmdClose(FXObject*,FXSelector,void*){
  saveSettings();
  fileview->saveSettings("settings");
  close(FALSE);
  return 1;
  }

long FXFileManager::onCmdAbout(FXObject*,FXSelector,void*){
  FXAboutDialog dialog(this,"Compass",APPLICATION_VERSION_STRING,"Sander Jansen","2005-2006",FXFileApplication::me->icon_about);
  dialog.execute(PLACEMENT_SCREEN);
  return 1;
  }


long FXFileManager::onCmdGoUp(FXObject*,FXSelector,void*){
  FXString name=FXPath::name(fileview->url());
  FXString url;
  if (FXStat::isDirectory(fileview->url()))
    url = FXPath::upLevel(fileview->url());
  else
    url = FXPath::directory(fileview->url());

  view(url,FROM_BOOKMARKS);

  fileview->selectFile(name);
  return 1;
  }

long FXFileManager::onCmdGoHome(FXObject*,FXSelector,void*){
  view(FXSystem::getHomeDirectory(),FROM_BOOKMARKS);
  return 1;
  }

long FXFileManager::onCmdGoBookmark(FXObject*,FXSelector,void*filename){
  if (!FXStat::exists((const char*)filename)){
    if (FXMessageBox::question(this,MBOX_YES_NO,"Bookmark Error","The bookmark \"%s\" doesn't appear to exist. Would you like to remove it?",(const char*)filename)==MBOX_CLICKED_YES)
      bookmarks.removeFile((const char*)filename);
    return 1;
    }

  view((const char*)filename,FROM_BOOKMARKS);     
  return 1;
  }


long FXFileManager::onCmdAddBookmark(FXObject*,FXSelector,void*){
  if (FXStat::isDirectory(fileview->url())) {
    bookmarks.appendFile(fileview->url());
    }
  else {
    getApp()->beep();
    }
  return 1;
  }


long FXFileManager::onCmdClearLocation(FXObject*,FXSelector,void*){
  location->setText(FXString::null);
  location->setIcon(NULL);
  location->setFocusOnField();
  return 1;
  }



long FXFileManager::onCmdDuplicateWindow(FXObject*,FXSelector,void*){
  FXFileApplication * app = dynamic_cast<FXFileApplication*>(getApp());
  if (tabbook)
    app->createWindow(tabbook);
  else
    app->createWindow(fileview->url());
  return 1;
  }


long FXFileManager::onCmdForwardHistory(FXObject*,FXSelector,void*){
  if (fileview->history().canForward()) view(fileview->history().forward(),FROM_HISTORY);
  return 1;
  }

long FXFileManager::onUpdForwardHistory(FXObject*sender,FXSelector,void*){
  sender->handle(this,fileview->history().canForward() ? FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),NULL);
  return 1;
  }

long FXFileManager::onCmdBackHistory(FXObject*,FXSelector,void*){
  if (fileview->history().canBack()) view(fileview->history().back(),FROM_HISTORY);
  return 1;
  }

long FXFileManager::onUpdBackHistory(FXObject*sender,FXSelector,void*){
  sender->handle(this, fileview->history().canBack() ?FXSEL(SEL_COMMAND,ID_ENABLE):FXSEL(SEL_COMMAND,ID_DISABLE),NULL);
  return 1;
  }

long FXFileManager::onUpdHasSelection(FXObject*sender,FXSelector,void*){
  if (fileview->getNumSelected())
    sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),0);
  return 1;
  }



long FXFileManager::onCmdCopyDirList(FXObject*,FXSelector,void*){
  FXDragType types[3]={FXFileApplication::me->kde_clipboard,FXFileApplication::me->gnome_clipboard,urilistType};
  if (acquireClipboard(types,3)){
    fileview->getClipboardSelection(clipboard,true);
    clipboard_cut=FALSE;
    }
  else {
    getApp()->beep();
    }
  return 1;
  }

long FXFileManager::onCmdCopy(FXObject*,FXSelector,void*){
  FXDragType types[3]={FXFileApplication::me->kde_clipboard,FXFileApplication::me->gnome_clipboard,urilistType};
  if (acquireClipboard(types,3)){
    fileview->getClipboardSelection(clipboard);
    clipboard_cut=FALSE;
    }
  else {
    getApp()->beep();
    }
  return 1;
  }

long FXFileManager::onUpdCopy(FXObject*sender,FXSelector,void*){
  if (fileview->getNumSelected())
    sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),0);
  return 1;
  }

long FXFileManager::onCmdCutDirList(FXObject*,FXSelector,void*){
  FXDragType types[3]={FXFileApplication::me->kde_clipboard,FXFileApplication::me->gnome_clipboard,urilistType};
  if (acquireClipboard(types,3)){
    fileview->getClipboardSelection(clipboard,true);
    clipboard_cut=true;
    }
  else {
    getApp()->beep();
    }
  return 1;
  }


long FXFileManager::onCmdCut(FXObject*,FXSelector,void*){
  FXFileApplication * fileapp = dynamic_cast<FXFileApplication*>(getApp());
  FXDragType types[3]={FXFileApplication::me->kde_clipboard,FXFileApplication::me->gnome_clipboard,urilistType};
  if (acquireClipboard(types,3)){
    fileview->getClipboardSelection(clipboard);
    clipboard_cut=true;
    }
  else {
    getApp()->beep();
    }
  return 1;
  }

long FXFileManager::onUpdCut(FXObject*sender,FXSelector,void*){
  if (fileview->getNumSelected())
    sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),0);
  return 1;
  }

void FXFileManager::pasteFiles(const FXString & destination){
  FXStringList files;
  FXString dropfiles,url,filesrc,data;
  FXString cutselection;
  FXint beg,end;

  FXbool from_kde=FALSE;
  FXbool from_gnome=FALSE;
  FXbool do_copy=TRUE;
  FXbool first=TRUE;

  FXuint num;
  FXDragType * types;

  if (!inquireDNDTypes(FROM_CLIPBOARD,types,num)) return;

  for (FXuint i=0;i<num;i++){
    if (types[i]==FXFileApplication::me->kde_clipboard)
      from_kde=TRUE;
    else if (types[i]==FXFileApplication::me->gnome_clipboard)
      from_gnome=TRUE;
    }
  FXFREE(&types);

  if (from_gnome && getDNDData(FROM_CLIPBOARD,FXFileApplication::me->gnome_clipboard,dropfiles)){

    for(beg=0; beg<dropfiles.length(); beg=end+1){

      if((end=dropfiles.find_first_of("\n",beg))<0) end=dropfiles.length();

      // File url
      url=dropfiles.mid(beg,end-beg);

      if (first) {
        if (url=="cut")
          do_copy=FALSE;
        first=FALSE;
        }
      else {
        files.append(FXURL::fileFromURL(url));
        }
      }
    }
  else if (from_kde) {

    //// Are we copying or moving...
    if (getDNDData(FROM_CLIPBOARD,FXFileApplication::me->kde_clipboard,data)) {
      if (data=="1")
        do_copy=FALSE;
      }

    // Get uri-list of files being dropped
    if (getDNDData(FROM_CLIPBOARD,urilistType,dropfiles)) {

      // Loop over urls
      for(beg=0; beg<dropfiles.length(); beg=end+2){

        if((end=dropfiles.find_first_of("\r\n",beg))<0) end=dropfiles.length();

        // File url
        url=dropfiles.mid(beg,end-beg);

        if (url.length()>1)
          files.append(FXURL::fileFromURL(url));
        }
      }
    }
  else {
    getApp()->beep();
    return;
    }

  fxmessage("Files:\n");
  for (FXint i=0;i<files.no();i++){
    fxmessage("%d - %s\n",i,files[i].text());
    }

  if (do_copy) {
    FXTransferManager * manager = new FXTransferManager(this,files,destination,FXTransferManager::OP_COPY);
    manager->show();
    }
  else {
    FXTransferManager * manager = new FXTransferManager(this,files,destination,FXTransferManager::OP_MOVE);
    manager->show();
    }



  }


long FXFileManager::onCmdPasteDirList(FXObject*,FXSelector,void*){
  pasteFiles(fileview->selectedDirectory());
  return 1;
  }

long FXFileManager::onCmdPaste(FXObject*,FXSelector,void*){
  pasteFiles(fileview->url());
  return 1;
  }

long FXFileManager::onUpdPaste(FXObject*,FXSelector,void*){
  return 1;
  }



// We now really do have the clipboard
long FXFileManager::onClipboardGained(FXObject* sender,FXSelector sel,void* ptr){
  FXMainWindow::onClipboardGained(sender,sel,ptr);
  return 1;
  }

// We lost the clipboard, free clipped text
long FXFileManager::onClipboardLost(FXObject* sender,FXSelector sel,void* ptr){
  FXMainWindow::onClipboardLost(sender,sel,ptr);
  clipboard.clear();
  fxmessage("ClipBoard Lost");
  return 1;
  }


// Somebody wants our clipped text
long FXFileManager::onClipboardRequest(FXObject* sender,FXSelector sel,void* ptr){
  FXFileApplication * fileapp = dynamic_cast<FXFileApplication*>(getApp());
  FXEvent *event=(FXEvent*)ptr;
  FXString string;

  // Perhaps the target wants to supply its own data for the clipboard
  if(FXMainWindow::onClipboardRequest(sender,sel,ptr)) return 1;

  if (event->target==fileapp->kde_clipboard) {
    fxmessage("KDE CLIPBOARD REQUEST\n");
    setDNDData(FROM_CLIPBOARD,event->target,clipboard_cut ? "1" : "0");
    }
  else if (event->target==urilistType){
    fxmessage("URL LIST REQUEST\n");
    FXString list;
    list+=clipboard[0];
    for (int i=1;i<clipboard.no();i++){
      list+="\r\n";
      list+=clipboard[i];
      }
    fxmessage("Clipboard:\n%s\n",list.text());
    setDNDData(FROM_CLIPBOARD,event->target,list);
    }
  else if (event->target==fileapp->gnome_clipboard){
    fxmessage("GNOME CLIPBOARD REQUEST\n");
    FXString list;

    if (clipboard_cut)
      list+="cut\n";
    else
      list+="copy\n";

    list+=clipboard[0];
    for (int i=1;i<clipboard.no();i++){
      list+="\n";
      list+=clipboard[i];
      }
    fxmessage("Clipboard:\n%s\n",list.text());
    setDNDData(FROM_CLIPBOARD,event->target,list);
    }
  else {
    fxmessage("Not handled: %s\n",getApp()->getDragTypeName(event->target).text());
    return 0;
    }
  return 1;
  }

long FXFileManager::onCmdDeleteDirList(FXObject*,FXSelector,void*){
  FXStringList files;
  files.append(fileview->selectedDirectory());
  FXDialogBox dialog(this,"Delete File(s)?",DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE,0,0,0,300,0,0,0,0,0);
  FXVerticalFrame * main = new FXVerticalFrame(&dialog,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0,0,0);
  show_critical(main,"Delete File(s)?","Caution: deleted files cannot be recovered.");
  FXVerticalFrame * content = new FXVerticalFrame(main,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0);
  FXVerticalFrame * sunken = new FXVerticalFrame(content,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0,0,0,0,0);
  FXList * list = new FXList(sunken,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  for(int i=0; i<files.no();i++){
    list->appendItem(files[i]);
    }  
  new FXSeparator(main,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  FXHorizontalFrame *closebox=new FXHorizontalFrame(main,LAYOUT_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0);
  new FXButton(closebox,tr(" &Cancel "),NULL,&dialog,FXDialogBox::ID_CANCEL,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
  new FXButton(closebox,tr(" &Delete Files "),NULL,&dialog,FXDialogBox::ID_ACCEPT,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
  if (dialog.execute(PLACEMENT_OWNER)) {
    FXTransferManager *  manager =  new FXTransferManager(this,files,FXString::null,FXTransferManager::OP_DELETE);
    manager->show();
    }
  return 1;
  }





long FXFileManager::onCmdDelete(FXObject*,FXSelector,void*){
  FXStringList files;
  fileview->getSelection(files);
  FXDialogBox dialog(this,"Delete File(s)?",DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE,0,0,0,300,0,0,0,0,0);
  FXVerticalFrame * main = new FXVerticalFrame(&dialog,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0,0,0);
  show_critical(main,"Delete File(s)?","Caution: deleted files cannot be recovered.");
  FXVerticalFrame * content = new FXVerticalFrame(main,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0);
  FXVerticalFrame * sunken = new FXVerticalFrame(content,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0,0,0,0,0);
  FXList * list = new FXList(sunken,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_Y);
  for(int i=0; i<files.no();i++){
    list->appendItem(files[i]);
//    if (filelist->isItemSelected(i) && filelist->getItemFilename(i)!=".." && filelist->getItemFilename(i)!="."){
//      list->appendItem(filelist->getItemFilename(i),filelist->getItemMiniIcon(i));
//      files.append(filelist->getItemPathname(i));
//      }
    }
  new FXSeparator(main,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  FXHorizontalFrame *closebox=new FXHorizontalFrame(main,LAYOUT_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0);
  new FXButton(closebox,tr(" &Cancel "),NULL,&dialog,FXDialogBox::ID_CANCEL,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
  new FXButton(closebox,tr(" &Delete Files "),NULL,&dialog,FXDialogBox::ID_ACCEPT,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
  if (dialog.execute(PLACEMENT_OWNER)) {
    FXTransferManager *  manager =  new FXTransferManager(this,files,FXString::null,FXTransferManager::OP_DELETE);
    manager->show();
    }
  return 1;
  }

long FXFileManager::onUpdDelete(FXObject*sender,FXSelector,void*){
  if (fileview->getNumSelected())
    sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),0);
  return 1;
  }


long FXFileManager::onCmdRename(FXObject*,FXSelector sel,void*){
  FXDialogBox dialog(this,"Rename File or Folder",DECOR_TITLE|DECOR_BORDER,0,0,0,0,0,0,0,0,0);
  FXVerticalFrame * main = new FXVerticalFrame(&dialog,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0,0,0);
  show_header(main,"Rename File or Folder","Please specify new file or folder name",FXFileApplication::me->icon_folder_open_big);
  FXVerticalFrame * content = new FXVerticalFrame(main,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);
  FXString selected;
  if (FXSELID(sel)==ID_RENAME_DIRLIST)
    selected = fileview->selectedDirectory();
  else
    selected = fileview->selected(); 
  new FXLabel(content,"Rename " + FXPath::name(selected) + " to");
  FXTextField * field = new FXTextField(content,20,&dialog,FXDialogBox::ID_ACCEPT,TEXTFIELD_ENTER_ONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X);
  field->setText(FXPath::name(selected));
  new FXSeparator(main,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  FXHorizontalFrame *closebox=new FXHorizontalFrame(main,LAYOUT_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0);
  new FXButton(closebox,tr("&Cancel"),NULL,&dialog,FXDialogBox::ID_CANCEL,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0,20,20);
  new FXButton(closebox,tr("&Rename"),NULL,&dialog,FXDialogBox::ID_ACCEPT,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0,20,20);
  dialog.create();
  field->setFocus();
//  fxmessage("title: %s - %d\n",FXPath::title(filelist->getCurrentFile()).text(),FXPath::title(filelist->getCurrentFile()).count());
  field->setSelection(0,FXPath::title(fileview->selected()).count());
  //field->selectAll();
  if (dialog.execute(PLACEMENT_OWNER)) {
    if (field->getText().empty()) return 1;
    FXString filefolder = FXPath::directory(selected) + PATHSEP + field->getText();
    bool success=false;

    if (FXStat::exists(filefolder))
      FXMessageBox::error(this,MBOX_OK,"Folder or File Exists","File or folder %s already exists.",field->getText().text());
    else if (FXStat::isDirectory(selected)){
      if (!FXDir::rename(selected,filefolder))
        FXMessageBox::error(this,MBOX_OK,"Folder Error","Unable to move folder %s\nto\n%s.",selected.text(),filefolder.text());
      else
        success=true;
      }
    else {
      if (!FXFile::rename(selected,filefolder))
        FXMessageBox::error(this,MBOX_OK,"File Error","Unable to rename file %s\nto\n%s.",selected.text(),filefolder.text());
      else
        success=true;
      }
    if (success) {
      fileview->update();
      if (FXSELID(sel)==ID_RENAME)
        fileview->selectFile(field->getText());
      }

/*
    if (field->getText().empty()) return 1;
    FXString folder = current_url + PATHSEP + field->getText();
    if (FXStat::exists(folder))
      FXMessageBox::error(this,MBOX_OK,"Folder Exists","File or folder %s already exists.",field->getText().text());
    else if (!FXDir::create(folder))
      FXMessageBox::error(this,MBOX_OK,"Folder Error","Cannot create folder %s.",field->getText().text());
*/  }
  return 1;
  }

long FXFileManager::onUpdRename(FXObject*sender,FXSelector,void*){
  if (fileview->getNumSelected()==1)
    sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),0);
  return 1;
  }

long FXFileManager::onCmdReplace(FXObject*,FXSelector,void*){
  FXFileReplaceDialog dialog(this);
  dialog.execute();
  return 1;
  }
long FXFileManager::onUpdReplace(FXObject*sender,FXSelector,void*){
  if (fileview->getNumSelected())
    sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),0);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),0);
  return 1;
  }


long FXFileManager::onCmdNewFolder(FXObject*,FXSelector,void*){
  FXDialogBox dialog(this,"New Folder",DECOR_TITLE|DECOR_BORDER,0,0,0,0,0,0,0,0,0);
  FXVerticalFrame * main = new FXVerticalFrame(&dialog,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0,0,0);
  show_header(main,"New Folder","Please specify folder name",FXFileApplication::me->icon_folder_open_big);
  FXVerticalFrame * content = new FXVerticalFrame(main,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);
  FXTextField * field = new FXTextField(content,20,&dialog,FXDialogBox::ID_ACCEPT,TEXTFIELD_ENTER_ONLY|FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X);
  field->setText("New Folder");
  new FXSeparator(main,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  FXHorizontalFrame *closebox=new FXHorizontalFrame(main,LAYOUT_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0);
  new FXButton(closebox,tr("&Cancel"),NULL,&dialog,FXDialogBox::ID_CANCEL,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0,20,20);
  new FXButton(closebox,tr("&Create"),NULL,&dialog,FXDialogBox::ID_ACCEPT,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0,20,20);
  dialog.create();
  field->setFocus();
  field->selectAll();
  if (dialog.execute(PLACEMENT_OWNER)) {
    if (field->getText().empty()) return 1;
    FXString folder = fileview->url() + PATHSEP + field->getText();
    if (FXStat::exists(folder))
      FXMessageBox::error(this,MBOX_OK,"Folder Exists","File or folder %s already exists.",field->getText().text());
    else if (!FXDir::create(folder))
      FXMessageBox::error(this,MBOX_OK,"Folder Error","Cannot create folder %s.",field->getText().text());
    }
  return 1;
  }

long FXFileManager::onUpdNewFolder(FXObject*sender,FXSelector,void*){
  if (dirwritable)
    sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),NULL);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),NULL);
  return 1;
  }



#define UGO_FILE_PERMISSION "Forbidden\nCan Read\nCan Write\nCan Execute\nCan Read and Write\nCan Read and Execute\nCan Write and Execute\nCan Read, Write and Execute"
#define UGO_FOLDER_PERMISSION "Forbidden\nCan Read\nCan Write\nCan Enter\nCan Read and Write\nCan Read and Enter\nCan Write and Enter\nCan Read, Write and Enter"

long FXFileManager::onCmdProperties(FXObject*,FXSelector,void*){
  FXStringList list;
  fileview->getSelection(list);
  FXFileProperties dialog(this,list,TRUE);
  if (dialog.execute(PLACEMENT_OWNER)) {
    fileview->update();
    }
  return 1;
  }


long FXFileManager::onCmdTest(FXObject*,FXSelector,void*){
#if 0
  FXStringList list;
  for (int i=0;i<filelist->getNumItems();i++){
    if (filelist->isItemSelected(i) && filelist->getItemFilename(i)!=".." && filelist->getItemFilename(i)!="."){
      list.append(filelist->getItemPathname(i));
      }
    }
  FXFileProperties dialog(this,list,TRUE);
  dialog.execute(PLACEMENT_OWNER);
  return 1;



  FXbool type=FALSE;          /// FALSE = files, TRUE folders, MAYBE mixed
//  FXbool unknown[3]={FALSE};
  FXStat dstat;
  FXuint uflags=0;
  FXuint gflags=0;
  FXuint oflags=0;
  FXbool isowner=FALSE;
  FXbool issticky=MAYBE;
  FXbool isexecute=MAYBE;
  FXbool isuserid=MAYBE;
  FXbool isgroupid=MAYBE;

  FXString username;
  FXString groupname;
  FXString filename;

  FXDataTarget dt_sticky(issticky);
  FXDataTarget dt_execute(isexecute);
  FXDataTarget dt_userid(isuserid);
  FXDataTarget dt_groupid(isgroupid);
  FXDataTarget dt_owner(uflags);
  FXDataTarget dt_group(gflags);
  FXDataTarget dt_other(oflags);


  if (num_selected_files>1) return 1;


  for (int i=0;i<filelist->getNumItems();i++){
    if (filelist->isItemSelected(i) && filelist->getItemFilename(i)!=".." && filelist->getItemFilename(i)!="."){
      filename=filelist->getItemPathname(i);
      FXStat::statFile(filename,dstat);
      type=filelist->isItemDirectory(i);

      if (dstat.isOwnerExecutable()) {
        if (dstat.isOwnerReadable()) {
          if (dstat.isOwnerWritable())
            uflags=7;
          else
            uflags=5;
          }
        else if (dstat.isOwnerWritable())
          uflags=6;
        else
          uflags=3;
        }
      else if (dstat.isOwnerReadable()){
        if (dstat.isOwnerWritable())
          uflags=4;
        else
          uflags=1;
        }
      else if (dstat.isOwnerWritable()){
        uflags=2;
        }

      if (dstat.isGroupExecutable()) {
        if (dstat.isGroupReadable()) {
          if (dstat.isGroupWritable())
            gflags=7;
          else
            gflags=5;
          }
        else if (dstat.isGroupWritable())
          gflags=6;
        else
          gflags=3;
        }
      else if (dstat.isGroupReadable()){
        if (dstat.isGroupWritable())
          gflags=4;
        else
          gflags=1;
        }
      else if (dstat.isGroupWritable()){
        gflags=2;
        }

      if (dstat.isOtherExecutable()) {
        if (dstat.isOtherReadable()) {
          if (dstat.isOtherWritable())
            oflags=7;
          else
            oflags=5;
          }
        else if (dstat.isOtherWritable())
          oflags=6;
        else
          oflags=3;
        }
      else if (dstat.isOtherReadable()){
        if (dstat.isOtherWritable())
          oflags=4;
        else
          oflags=1;
        }
      else if (dstat.isOtherWritable()){
        oflags=2;
        }

      if (FXSystem::user() && dstat.user() )
        isowner=TRUE;

      issticky = dstat.isSetSticky();
      isexecute = dstat.isExecutable();
      isuserid = dstat.isSetUid();
      isgroupid = dstat.isSetGid();

      username = FXSystem::userName(dstat.user());
      groupname = FXSystem::groupName(dstat.group());

      break;
      }
    }




  FXFileApplication * app = dynamic_cast<FXFileApplication*>(FXApp::instance());
  FXDialogBox dialog(this,"File and Folder Permissions",DECOR_TITLE|DECOR_BORDER,0,0,0,0,0,0,0,0,0);
  FXVerticalFrame * main = new FXVerticalFrame(&dialog,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0,0,0);
  if (isowner)
    show_header(main,"Properties","Modify file and folder properties",app->icon_lock);
  else
    show_header(main,"Properties","Only the owner may change properties",app->icon_lock);


  FXTabBook * tabbook = new FXTabBook(main,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0);

  new FXTabItem(tabbook," General ");
  FXVerticalFrame * content = new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK,0,0,0,0);

  FXMatrix * matrix = new FXMatrix(content,2,LAYOUT_FILL_X|LAYOUT_FILL_Y|MATRIX_BY_COLUMNS,0,0,0,0);


  new FXLabel(matrix,"Size",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
  new FXLabel(matrix,FXStringFormat("%llu Bytes",FXStat::size(filename)),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);

  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);

  new FXLabel(matrix,"Modified",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
  new FXLabel(matrix,FXSystem::time(FXStat::modified(filename)),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);

  new FXLabel(matrix,"Accessed",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
  new FXLabel(matrix,FXSystem::time(FXStat::accessed(filename)),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);

  new FXLabel(matrix,"Created",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
  new FXLabel(matrix,FXSystem::time(FXStat::created(filename)),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);

  new FXLabel(matrix,"Touched",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
  new FXLabel(matrix,FXSystem::time(FXStat::touched(filename)),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);

  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);

  new FXTabItem(tabbook," Permissions ");
  content = new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK,0,0,0,0);

  matrix = new FXMatrix(content,2,LAYOUT_FILL_X|LAYOUT_FILL_Y|MATRIX_BY_COLUMNS,0,0,0,0);
  FXListBox * listbox;

  new FXLabel(matrix,"Owner",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  listbox = new FXListBox(matrix,&dt_owner,FXDataTarget::ID_VALUE,LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|FRAME_THICK|FRAME_SUNKEN);
  if (type)
    listbox->fillItems(UGO_FOLDER_PERMISSION);
  else
    listbox->fillItems(UGO_FILE_PERMISSION);
  listbox->setNumVisible(listbox->getNumItems());
  listbox->setCurrentItem(uflags);
  if (!isowner) listbox->disable();

  new FXLabel(matrix,"Group",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  listbox = new FXListBox(matrix,&dt_group,FXDataTarget::ID_VALUE,LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|FRAME_THICK|FRAME_SUNKEN);
  if (type)
    listbox->fillItems(UGO_FOLDER_PERMISSION);
  else
    listbox->fillItems(UGO_FILE_PERMISSION);
  listbox->setNumVisible(listbox->getNumItems());
  listbox->setCurrentItem(gflags);
  if (!isowner) listbox->disable();

  new FXLabel(matrix,"Others",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  listbox = new FXListBox(matrix,&dt_other,FXDataTarget::ID_VALUE,LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|FRAME_THICK|FRAME_SUNKEN);
  if (type)
    listbox->fillItems(UGO_FOLDER_PERMISSION);
  else
    listbox->fillItems(UGO_FILE_PERMISSION);
  listbox->setNumVisible(listbox->getNumItems());
  listbox->setCurrentItem(oflags);
  if (!isowner) listbox->disable();
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);

  new FXFrame(matrix,FRAME_NONE);
  new FXCheckButton(matrix,"Set User Id",&dt_userid,FXDataTarget::ID_VALUE,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
  new FXFrame(matrix,FRAME_NONE);
  new FXCheckButton(matrix,"Set Group Id",&dt_groupid,FXDataTarget::ID_VALUE,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
  new FXFrame(matrix,FRAME_NONE);
  new FXCheckButton(matrix,"Set Sticky Bit",&dt_sticky,FXDataTarget::ID_VALUE,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);

  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);

  new FXLabel(matrix,"Owner",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_LEFT);
  new FXLabel(matrix,username,NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
  new FXLabel(matrix,"Group",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_LEFT);
  new FXLabel(matrix,groupname,NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);

  //new FXSeparator(main,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  FXHorizontalFrame *closebox=new FXHorizontalFrame(main,LAYOUT_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0);
  new FXButton(closebox,tr("&Cancel"),NULL,&dialog,FXDialogBox::ID_CANCEL,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0,20,20);
  new FXButton(closebox,tr("&Update"),NULL,&dialog,FXDialogBox::ID_ACCEPT,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0,20,20);
  if (dialog.execute(PLACEMENT_OWNER)) {

    FXuint perm=0;

    switch(uflags){
      case  7: perm|=FXIO::OwnerFull;                     break;
      case  6: perm|=(FXIO::OwnerExec|FXIO::OwnerWrite);  break;
      case  5: perm|=(FXIO::OwnerExec|FXIO::OwnerRead);   break;
      case  4: perm|=(FXIO::OwnerWrite|FXIO::OwnerRead);  break;
      case  3: perm|=FXIO::OwnerExec;                     break;
      case  2: perm|=FXIO::OwnerWrite;                    break;
      case  1: perm|=FXIO::OwnerRead;                     break;
      default: break;
      }

    switch(gflags){
      case  7: perm|=FXIO::GroupFull;                     break;
      case  6: perm|=(FXIO::GroupExec|FXIO::GroupWrite);  break;
      case  5: perm|=(FXIO::GroupExec|FXIO::GroupRead);   break;
      case  4: perm|=(FXIO::GroupWrite|FXIO::GroupRead);  break;
      case  3: perm|=FXIO::GroupExec;                     break;
      case  2: perm|=FXIO::GroupWrite;                    break;
      case  1: perm|=FXIO::GroupRead;                     break;
      default: break;
      }

    switch(oflags){
      case  7: perm|=FXIO::OtherFull;                     break;
      case  6: perm|=(FXIO::OtherExec|FXIO::OtherWrite);  break;
      case  5: perm|=(FXIO::OtherExec|FXIO::OtherRead);   break;
      case  4: perm|=(FXIO::OtherWrite|FXIO::OtherRead);  break;
      case  3: perm|=FXIO::OtherExec;                     break;
      case  2: perm|=FXIO::OtherWrite;                    break;
      case  1: perm|=FXIO::OtherRead;                     break;
      default: break;
      }

    if (issticky)
      perm|=FXIO::Sticky;
    if (isuserid)
      perm|=FXIO::SetUser;
    if (isgroupid)
      perm|=FXIO::SetGroup;

    if (!FXStat::mode(filename,perm)) {
      fxmessage("Failed\n");
      }

    }
  return 1;
#endif
  }










long FXFileManager::onCmdTest2(FXObject*,FXSelector,void*){
#if 0
  FXbool type=FALSE;          /// FALSE = files, TRUE folders, MAYBE mixed
//  FXbool unknown[3]={FALSE};
  FXStat dstat;
  FXuint uflags=0;
  FXuint gflags=0;
  FXuint oflags=0;
  FXbool isowner=FALSE;
  FXbool issticky=MAYBE;
  FXbool isexecute=MAYBE;
  FXbool isuserid=MAYBE;
  FXbool isgroupid=MAYBE;

  FXString username;
  FXString groupname;

  FXDataTarget dt_sticky(issticky);
  FXDataTarget dt_execute(isexecute);
  FXDataTarget dt_userid(isuserid);
  FXDataTarget dt_groupid(isgroupid);


  if (fileview->getNumSelected()>1) return 1;


  for (int i=0;i<filelist->getNumItems();i++){
    if (filelist->isItemSelected(i) && filelist->getItemFilename(i)!=".." && filelist->getItemFilename(i)!="."){
      FXStat::statFile(filelist->getItemPathname(i),dstat);
      if (filelist->isItemDirectory(i)) {
        type=TRUE;
        if (dstat.isOwnerReadWriteExecute())
          uflags=2;
        else if (dstat.isOwnerExecutable() && dstat.isOwnerReadable() && !dstat.isOwnerWritable())
          uflags=1;
        else
          uflags=0;

        if (dstat.isGroupReadWriteExecute())
          gflags=2;
        else if (dstat.isGroupExecutable() && dstat.isGroupReadable() && !dstat.isGroupWritable())
          gflags=1;
        else
          gflags=0;

        if (dstat.isOtherReadWriteExecute())
          oflags=2;
        else if (dstat.isOtherExecutable() && dstat.isOtherReadable() && !dstat.isOtherWritable())
          oflags=1;
        else
          oflags=0;

        }
      else {
        type=FALSE;
        if (dstat.isOwnerWritable() && dstat.isOwnerReadable())
          uflags=3;
        else if (dstat.isOwnerWritable() && !dstat.isOwnerReadable())
          uflags=2;
        else if (!dstat.isOwnerWritable() && dstat.isOwnerReadable())
          uflags=1;
        else
          uflags=0;

        if (dstat.isGroupWritable() && dstat.isGroupReadable())
          gflags=3;
        else if (dstat.isGroupWritable() && !dstat.isGroupReadable())
          gflags=2;
        else if (!dstat.isGroupWritable() && dstat.isGroupReadable())
          gflags=1;
        else
          gflags=0;

        if (dstat.isOtherWritable() && dstat.isOtherReadable())
          oflags=3;
        else if (dstat.isOtherWritable() && !dstat.isOtherReadable())
          oflags=2;
        else if (!dstat.isOtherWritable() && dstat.isOtherReadable())
          oflags=1;
        else
          oflags=0;
        }

      if (FXSystem::user() && dstat.user() )
        isowner=TRUE;

      issticky = dstat.isSetSticky();
      isexecute = dstat.isExecutable();
      isuserid = dstat.isSetUid();
      isgroupid = dstat.isSetGid();

      username = FXSystem::userName(dstat.user());
      groupname = FXSystem::groupName(dstat.group());

      break;
      }
    }


  FXFileApplication * app = dynamic_cast<FXFileApplication*>(FXApp::instance());
  FXDialogBox dialog(this,"File and Folder Permissions",DECOR_TITLE|DECOR_BORDER,0,0,0,0,0,0,0,0,0);
  FXVerticalFrame * main = new FXVerticalFrame(&dialog,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0,0,0);
  if (isowner)
    show_header(main,"Permissions","Modify file and folder permissions",app->icon_lock);
  else
    show_header(main,"Permissions","Only the owner may change permissions",app->icon_lock);

  FXVerticalFrame * content = new FXVerticalFrame(main,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);
  FXMatrix * matrix = new FXMatrix(content,4,LAYOUT_FILL_X|LAYOUT_FILL_Y|MATRIX_BY_COLUMNS|PACK_UNIFORM_WIDTH,0,0,0,0,0,0,0,0,0,0);

  new FXFrame(matrix,FRAME_NONE);
  new FXLabel(matrix,"Read",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  new FXLabel(matrix,"Write",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  new FXLabel(matrix,"Execute",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);

  new FXLabel(matrix,"Owner",NULL,LABEL_NORMAL|JUSTIFY_RIGHT);
  new FXCheckButton(matrix,FXString::null,NULL,0,LAYOUT_CENTER_Y|LAYOUT_CENTER_X);
  new FXCheckButton(matrix,FXString::null,NULL,0,LAYOUT_CENTER_Y|LAYOUT_CENTER_X);
  new FXCheckButton(matrix,FXString::null,NULL,0,LAYOUT_CENTER_Y|LAYOUT_CENTER_X);

  new FXLabel(matrix,"Group",NULL,LABEL_NORMAL|JUSTIFY_RIGHT);
  new FXCheckButton(matrix,FXString::null,NULL,0,LAYOUT_CENTER_Y|LAYOUT_CENTER_X);
  new FXCheckButton(matrix,FXString::null,NULL,0,LAYOUT_CENTER_Y|LAYOUT_CENTER_X);
  new FXCheckButton(matrix,FXString::null,NULL,0,LAYOUT_CENTER_Y|LAYOUT_CENTER_X);

  new FXLabel(matrix,"Others",NULL,LABEL_NORMAL|JUSTIFY_RIGHT);
  new FXCheckButton(matrix,FXString::null,NULL,0,LAYOUT_CENTER_Y|LAYOUT_CENTER_X);
  new FXCheckButton(matrix,FXString::null,NULL,0,LAYOUT_CENTER_Y|LAYOUT_CENTER_X);
  new FXCheckButton(matrix,FXString::null,NULL,0,LAYOUT_CENTER_Y|LAYOUT_CENTER_X);

/*
  new FXLabel(matrix,"Owner",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  listbox = new FXListBox(matrix,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|FRAME_THICK|FRAME_SUNKEN);
  if (type)
    listbox->fillItems(UGO_FOLDER_PERMISSION);
  else
    listbox->fillItems(UGO_FILE_PERMISSION);
  listbox->setNumVisible(listbox->getNumItems());
  listbox->setCurrentItem(uflags);
  if (!isowner) listbox->disable();

  new FXLabel(matrix,"Group",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  listbox = new FXListBox(matrix,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|FRAME_THICK|FRAME_SUNKEN);
  if (type)
    listbox->fillItems(UGO_FOLDER_PERMISSION);
  else
    listbox->fillItems(UGO_FILE_PERMISSION);
  listbox->setNumVisible(listbox->getNumItems());
  listbox->setCurrentItem(gflags);
  if (!isowner) listbox->disable();

  new FXLabel(matrix,"Others",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  listbox = new FXListBox(matrix,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|FRAME_THICK|FRAME_SUNKEN);
  if (type)
    listbox->fillItems(UGO_FOLDER_PERMISSION);
  else
    listbox->fillItems(UGO_FILE_PERMISSION);
  listbox->setNumVisible(listbox->getNumItems());
  listbox->setCurrentItem(oflags);
  if (!isowner) listbox->disable();
*/
/*
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);


  if (type) {
    new FXFrame(matrix,FRAME_NONE);
    new FXCheckButton(matrix,"Only owner can rename or delete\n folder content",&dt_sticky,FXDataTarget::ID_VALUE,JUSTIFY_LEFT|JUSTIFY_TOP|ICON_BEFORE_TEXT|LAYOUT_FILL_COLUMN);
    }
  else {
    new FXFrame(matrix,FRAME_NONE);
    new FXCheckButton(matrix,"Executable",&dt_execute,FXDataTarget::ID_VALUE,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
    new FXFrame(matrix,FRAME_NONE);
    new FXCheckButton(matrix,"Set User ID on Execution",&dt_userid,FXDataTarget::ID_VALUE,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
    new FXFrame(matrix,FRAME_NONE);
    new FXCheckButton(matrix,"Set Group ID on Execution",&dt_groupid,FXDataTarget::ID_VALUE,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
    }

  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);

//  new FXSeparator(content,LAYOUT_FILL_X|SEPARATOR_GROOVE);
//  matrix = new FXMatrix(content,2,LAYOUT_FILL_X|LAYOUT_FILL_Y|MATRIX_BY_COLUMNS,0,0,0,0,0,0,0,0);
  new FXLabel(matrix,"Owner",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_LEFT);
  new FXLabel(matrix,username,NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
  new FXLabel(matrix,"Group",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_LEFT);
  new FXLabel(matrix,groupname,NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);


*/

  new FXSeparator(main,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  FXHorizontalFrame *closebox=new FXHorizontalFrame(main,LAYOUT_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0);
  new FXButton(closebox,tr("&Cancel"),NULL,&dialog,FXDialogBox::ID_CANCEL,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0,20,20);
  new FXButton(closebox,tr("&Update"),NULL,&dialog,FXDialogBox::ID_ACCEPT,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0,20,20);
  if (dialog.execute(PLACEMENT_OWNER)) {
    }
#endif
  return 1;
  }







long FXFileManager::onCmdFileView(FXObject*,FXSelector,void*ptr){
  const FXString * url = reinterpret_cast<const FXString*>(ptr);
  view(*url,FROM_FILELIST);
  return 1;
  }

long FXFileManager::onCmdFileViewFileContext(FXObject*,FXSelector,void*ptr){
  FXEvent *event=reinterpret_cast<FXEvent*>(ptr);
  FXMenuPane pane(this);
  if (fileview->getNumSelected()==0){
    new FXMenuCommand(&pane,"&Up\t\tChange up one level.",FXFileApplication::me->icon_up_small,this,ID_GO_UP);
    new FXMenuCommand(&pane,"&Back\t\tChange to previous directory.",FXFileApplication::me->icon_back_small,this,ID_GO_BACK);
    new FXMenuCommand(&pane,"&Forward\t\tChange to next directory.",FXFileApplication::me->icon_forward_small,this,ID_GO_FORWARD);
    new FXMenuCommand(&pane,"&Home\t\tChange to home directory.",FXFileApplication::me->icon_home_small,this,ID_GO_HOME);
    new FXMenuSeparator(&pane);
    new FXMenuCommand(&pane,"Paste\tCtrl-V",FXFileApplication::me->icon_paste,this,ID_PASTE);
    }
  else {
    new FXMenuCommand(&pane,"Open...",NULL,NULL,0);
    new FXMenuCommand(&pane,"Open with...",NULL,NULL,0);
    new FXMenuCommand(&pane,"Open with editor",NULL,NULL,0);
    new FXMenuSeparator(&pane);
    new FXMenuCommand(&pane,"Copy\tCtrl-C",FXFileApplication::me->icon_copy,this,ID_COPY);
    new FXMenuCommand(&pane,"Cut\tCtrl-X",FXFileApplication::me->icon_cut,this,ID_CUT);
    new FXMenuCommand(&pane,"Paste\tCtrl-V",FXFileApplication::me->icon_paste,this,ID_PASTE);
    new FXMenuCommand(&pane,"Delete\tDel",FXFileApplication::me->icon_delete,this,ID_DELETE);
    if (fileview->getNumSelected()==1) {
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
  return 1;
  }

long FXFileManager::onCmdFileViewDirContext(FXObject*,FXSelector,void*ptr){
  FXEvent *event=reinterpret_cast<FXEvent*>(ptr);
  FXMenuPane pane(this);
  new FXMenuCommand(&pane,"Copy",FXFileApplication::me->icon_copy,this,ID_COPY_DIRLIST);
  new FXMenuCommand(&pane,"Cut",FXFileApplication::me->icon_cut,this,ID_CUT_DIRLIST);
  new FXMenuCommand(&pane,"Paste",FXFileApplication::me->icon_paste,this,ID_PASTE_DIRLIST);
  new FXMenuCommand(&pane,"Delete",FXFileApplication::me->icon_delete,this,ID_DELETE_DIRLIST);
  new FXMenuSeparator(&pane);
  new FXMenuCommand(&pane,"Rename",NULL,this,ID_RENAME_DIRLIST);
  pane.create();
  pane.popup(NULL,event->root_x,event->root_y);
  getApp()->runModalWhileShown(&pane);  
  return 1;
  }
  



