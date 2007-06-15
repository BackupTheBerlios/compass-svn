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
#ifndef FILEVIEW_H
#define FILEVIEW_H

#include "FXHistory.h"

class FXFileListEx;
class FXImageViewEx;

class FXFileView : public FXSplitter {
FXDECLARE(FXFileView)
private:
  FXString  current_url;         // Current url
  FXIcon *  current_icon;        // Current icon
  FXint     num_selected;        // Number of selected files
  FXHistory current_history;     // Current History
  FXbool    showhiddenfiles;     // Show Hidden Files
protected:
  FXFileListEx      * filelist;
  FXDirList         * dirlist;
  FXSplitter        * splitter;
  FXImageViewEx     * imageview;
  FXSwitcher        * switcher;
  FXVerticalFrame   * dirframe;
protected:
  FXFileView();
private:
  FXFileView(const FXFileView&);
  FXFileView& operator=(const FXFileView&);
public:
  enum {
    ID_FILELIST=FXFileList::ID_LAST,
    ID_DIRLIST,
    ID_IMAGEPREVIEW,
    ID_SHOWHIDDENFILES,
    ID_SHOWDIRTREE,
    ID_SHOWPARENTDIR,
    ID_SHOWIMAGES
    };
public:
  long onDefault(FXObject*,FXSelector,void*);
  long onFileListDoubleClick(FXObject*,FXSelector,void*);
  long onFileListRightClick(FXObject*,FXSelector,void*);
  long onFileListSelected(FXObject*,FXSelector,void*);
  long onFileListDeselected(FXObject*,FXSelector,void*);
  long onFileListSelection(FXObject*,FXSelector,void*);
  long onDirListRightClick(FXObject*,FXSelector,void*);
  long onDirListDeleted(FXObject*,FXSelector,void*);
  long onImagePreviewClick(FXObject*,FXSelector,void*);
  long onCmdDirList(FXObject*,FXSelector,void*);
  long onCmdShowHiddenFiles(FXObject*,FXSelector,void*);
  long onUpdShowHiddenFiles(FXObject*,FXSelector,void*);
  long onCmdShowDirTree(FXObject*,FXSelector,void*);
  long onUpdShowDirTree(FXObject*,FXSelector,void*);
  long onCmdShowParentDirectory(FXObject*,FXSelector,void*);
  long onUpdShowParentDirectory(FXObject*,FXSelector,void*);
  long onCmdShowImages(FXObject*,FXSelector,void*);
  long onUpdShowImages(FXObject*,FXSelector,void*);
public:
  FXFileView(FXComposite *p,FXObject * tgt=NULL,FXSelector sel=0,FXuint opts=0);

  /// View a url
  void view(const FXString & url,FXIcon * ic,bool preview=false);

  /// Select a file
  void selectFile(const FXString & file);

  /// Show File
  void showFile(const FXString & file);

  /// Preview a image
  void preview(FXImage *);

  /// Current Url
  const FXString & url() const;

  /// Current Selected File
  FXString selected() const;

  /// Current Selected Directory
  FXString selectedDirectory() const;

  /// Browse History
  FXHistory & history();

  /// Curent Icon
  FXIcon * icon() const;

  /// Refresh the filelist
  void update(FXIcon * icon);

  /// Are we in Preview Mode
  bool previewShown() const;

  /// Get Clipboard Selection
  void getClipboardSelection(FXStringList &,bool fromdirlist=false);

  /// Get Selection
  void getSelection(FXStringList &);

    /// Load Settings from Registry
  void loadSettings(const FXString & key);

  /// Save Settings to Registry
  void saveSettings(const FXString & key) const;

  /// Return Number of Selected Files
  FXint getNumSelected() const;

  /// Destructor
  ~FXFileView();
  };

#endif






