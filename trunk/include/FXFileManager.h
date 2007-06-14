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
#ifndef FILEMANAGER_H
#define FILEMANAGER_H


class FXFileApplication;
class FXURLBox;
class FXFileListEx;
class FXImageViewEx;
class FXFileView;

enum FXViewSource {
  FROM_FILELIST,
  FROM_APPLICATION,
  FROM_LOCATIONBAR,
  FROM_BOOKMARKS,
  FROM_HISTORY,
  FROM_NEWTAB,
  };

class FXFileManager : public FXMainWindow {
  FXDECLARE(FXFileManager)
protected:
  FXImageViewEx   * imageview;
  FXURLBox        * location;
  FXVerticalFrame * main;
  FXFileView      * fileview;
  FXDelegator       fileview_delegator;
  FXTabBook       * tabbook;
  FXTabItem       * tabitem;
  FXStatusBar     * statusbar;
  FXButton        * dirupbutton;
  FXButton        * homebutton;
  FXRecentFiles     bookmarks;
  FXMenuPane      * filemenu;
  FXMenuPane      * editmenu;
  FXMenuPane      * sortmenu;
  FXMenuPane      * bookmarkmenu;
  FXMenuPane      * viewmenu;
  FXMenuPane      * windowmenu;
  FXMenuPane      * helpmenu;
protected:
  FXbool       tabview;
  FXbool       showhiddenfiles;
  bool         attachfileview;
  bool         dirwritable;

  bool         clipboard_cut;
  FXStringList clipboard;

  FXString     orgdirectory;   // Directory
  FXString     dropdirectory;  // Drop directory
  FXDragAction dropaction;     // Drop action
  FXString     current_url;
  FXFileDict * associations;
protected:
  FXIcon * applogo;
protected:
  FXFileManager(){}

  void pasteFiles(const FXString & destination);
  void initUserInterface();

  bool isWritable(const FXString & url);

  void createIcons();
  void destroyIcons();

  void loadSettings();
  void saveSettings();

  bool preview(const FXString & filename);

  void setTabLabel(FXTabItem *item,const FXString & url,FXIcon * icon);
  void updateActiveTab(FXint p);
  void createTab(bool initialtabonly=false);
  void closeTab(FXint tab);
  void showTabs();
  void hideTabs();
public:
  long onCmdClose(FXObject*,FXSelector,void*);
  long onSessionNotify(FXObject*,FXSelector,void*);
  long onSessionClosed(FXObject*,FXSelector,void*);

  long onCmdEdit(FXObject*,FXSelector,void*);
  long onCmdProperties(FXObject*,FXSelector,void*);
  long onUpdHasSelection(FXObject*,FXSelector,void*);

  long onCmdTest(FXObject*,FXSelector,void*);
  long onCmdTest2(FXObject*,FXSelector,void*);


  long onCmdAbout(FXObject*,FXSelector,void*);

  long onDirListRightClick(FXObject*,FXSelector,void*);

  long onCmdFileView(FXObject*,FXSelector,void*);

  long onCmdFileViewFileContext(FXObject*,FXSelector,void*);
  long onCmdFileViewDirContext(FXObject*,FXSelector,void*);



  long onListDNDEnter(FXObject*,FXSelector,void*);
  long onListDNDLeave(FXObject*,FXSelector,void*);
  long onListDNDDrop(FXObject*,FXSelector,void*);
  long onListDNDMotion(FXObject*,FXSelector,void*);
  long onListDNDOpen(FXObject*,FXSelector,void*);


  long onImagePreviewClick(FXObject*,FXSelector,void*);

  long onCmdGoHome(FXObject*,FXSelector,void*);
  long onCmdGoUp(FXObject*,FXSelector,void*);
  long onCmdGoBookmark(FXObject*,FXSelector,void*);

  long onCmdAddBookmark(FXObject*,FXSelector,void*);

  long onCmdDuplicateWindow(FXObject*,FXSelector,void*);

  long onCmdForwardHistory(FXObject*,FXSelector,void*);
  long onUpdForwardHistory(FXObject*,FXSelector,void*);
  long onCmdBackHistory(FXObject*,FXSelector,void*);
  long onUpdBackHistory(FXObject*,FXSelector,void*);

  long onCmdLocation(FXObject*,FXSelector,void*);
  long onCmdClearLocation(FXObject*,FXSelector,void*);

  long onCmdCopy(FXObject*,FXSelector,void*);
  long onUpdCopy(FXObject*,FXSelector,void*);
  long onCmdCut(FXObject*,FXSelector,void*);
  long onUpdCut(FXObject*,FXSelector,void*);
  long onCmdPaste(FXObject*,FXSelector,void*);
  long onUpdPaste(FXObject*,FXSelector,void*);
  long onCmdDelete(FXObject*,FXSelector,void*);
  long onUpdDelete(FXObject*,FXSelector,void*);

  long onCmdRename(FXObject*,FXSelector,void*);
  long onUpdRename(FXObject*,FXSelector,void*);

  long onCmdReplace(FXObject*,FXSelector,void*);
  long onUpdReplace(FXObject*,FXSelector,void*);


  long onCmdNewFolder(FXObject*,FXSelector,void*);
  long onUpdNewFolder(FXObject*,FXSelector,void*);

  long onCmdCopyDirList(FXObject*,FXSelector,void*);
  long onUpdCopyDirList(FXObject*,FXSelector,void*);
  long onCmdCutDirList(FXObject*,FXSelector,void*);
  long onUpdCutDirList(FXObject*,FXSelector,void*);
  long onCmdPasteDirList(FXObject*,FXSelector,void*);
  long onUpdPasteDirList(FXObject*,FXSelector,void*);
  long onCmdDeleteDirList(FXObject*,FXSelector,void*);
  long onUpdDeleteDirList(FXObject*,FXSelector,void*);


  long onClipboardRequest(FXObject*,FXSelector,void*);
  long onClipboardGained(FXObject*,FXSelector,void*);
  long onClipboardLost(FXObject*,FXSelector,void*);


  long onCmdOpenTab(FXObject*,FXSelector,void*);
  long onCmdActiveTab(FXObject*,FXSelector,void*);
  long onCmdNewTab(FXObject*,FXSelector,void*);
  long onCmdCloseTab(FXObject*,FXSelector,void*);
  long onUpdHasTab(FXObject*,FXSelector,void*);
  long onCmdDetachTab(FXObject*,FXSelector,void*);

  long onCmdTabPress(FXObject*,FXSelector,void*);
  long onCmdTabRelease(FXObject*,FXSelector,void*);

public:
  enum {
    ID_ABOUT=FXMainWindow::ID_LAST,
    ID_FILEVIEW,
    ID_DIR_CONTEXTMENU,
    ID_FILE_CONTEXTMENU,
    ID_FILELIST,
    ID_DIRLIST,
    ID_IMAGEPREVIEW,
    ID_GO_HOME,
    ID_DUP_WINDOW,
    ID_GO_FORWARD,
    ID_GO_BACK,
    ID_GO_UP,
    ID_GO_BOOKMARK,
    ID_ADD_BOOKMARK,
    ID_LOCATION,
    ID_CLEAR_LOCATION,
    ID_COPY,
    ID_COPY_DIRLIST,
    ID_CUT,
    ID_CUT_DIRLIST,
    ID_PASTE,
    ID_PASTE_DIRLIST,
    ID_DELETE,
    ID_DELETE_DIRLIST,
    ID_RENAME,
    ID_RENAME_DIRLIST,
    ID_REPLACE,
    ID_NEW_FOLDER,
    ID_SHOW_HIDDEN_FILES,
    ID_PROPERTIES,
    ID_DND_OPEN,
    ID_TABVIEW,
    ID_TABITEM,
    ID_NEW_TAB,
    ID_CLOSE_TAB,
    ID_DETACH_TAB,
    ID_EDIT,
    ID_TEST
    };
public:
  FXFileManager(FXFileApplication* a);
  FXFileManager(FXFileApplication* a,FXFileView * view);

  virtual void create();

  void updateView(const FXString & url,FXIcon * icon,bool preview);

  void view(const FXString & url,FXViewSource src=FROM_FILELIST);

  void execute(const FXString & url);

  void executeBinding(const FXString & url);

  void duplicateTabBook(FXTabBook *);

  virtual ~FXFileManager();
  };

#endif


