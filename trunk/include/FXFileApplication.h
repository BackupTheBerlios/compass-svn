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
#ifndef FILEAPPLICATION_H
#define FILEAPPLICATION_H

enum FXOpenMode {
  OPEN_IN_CURRENT=0,
  OPEN_IN_WINDOW=1,
  OPEN_IN_TAB=2
  };


class FXFileView;

class FXFileApplication : public FXApp {
  FXDECLARE(FXFileApplication)
friend class FXFileManager;
protected:
  FXFileDict * associations;
protected:
  FXOpenMode diropenmode;
  FXOpenMode previewopenmode;
public:
  FXIcon * applogo;
  FXIcon * icon_about;
  FXIcon * icon_back;
  FXIcon * icon_forward;
  FXIcon * icon_home;
  FXIcon * icon_back_small;
  FXIcon * icon_forward_small;
  FXIcon * icon_home_small;
  FXIcon * icon_up_small;
  FXIcon * icon_settings;

  FXIcon * icon_exit;
  FXIcon * icon_clear;
  FXIcon * icon_document;
  FXIcon * icon_folder;
	FXIcon * icon_folder_big;
	FXIcon * icon_folder_open_big;

  FXIcon * icon_tab_new;
  FXIcon * icon_tab_close;
  FXIcon * icon_tab_detach;
  FXIcon * icon_tab_duplicate;

  FXIcon * icon_up;
  FXIcon * icon_new;
  FXIcon * icon_copy;
  FXIcon * icon_cut;
  FXIcon * icon_paste;
  FXIcon * icon_delete;

  FXIcon * icon_info;
  FXIcon * icon_critical;
  FXIcon * icon_warning;

  FXIcon * icon_lock;



  FXFont * font_title;
private:
  FXFile * dde;
public:
  FXDragType kde_clipboard;
  FXDragType gnome_clipboard;
  FXDragType gnome_dragndrop;
protected:
  void createIcons();
  void destroyIcons();
public:
  enum {
    ID_NEW_WINDOW=FXApp::ID_LAST,
    ID_CHILD,
    ID_DDE,
    ID_LAST
    };
public:
  long onCmdNewWindow(FXObject*,FXSelector,void*);
  long onCmdDuplicateWindow(FXObject*,FXSelector,void*);
  long onChildSignal(FXObject*,FXSelector,void*);
  long onDDE(FXObject*,FXSelector,void*);

public:
  static FXFileApplication * me;
public:
  FXFileApplication();

  void createWindow(FXbool checkargs=FALSE);

  void createWindow(FXString url);

  void createWindow(FXFileView * view);
  
  void createWindow(FXTabBook * tabbook);

  void registerDDE(FXFile * dde);

  FXIcon * getIconForDirectory(const FXString & url);

  FXIcon * getIconForFile(const FXString & url);

  FXIcon * getIconForUrl(const FXString & url);

  FXFileDict * getAssociations() const;


  virtual void init(int& argc,char** argv,FXbool connect=TRUE);

  virtual ~FXFileApplication();
  };

#endif


