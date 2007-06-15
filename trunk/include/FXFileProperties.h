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
#ifndef FILEPROPERTIES_H
#define FILEPROPERTIES_H

class FXFileProperties : public FXDialogBox {
  FXDECLARE(FXFileProperties)
private:
  FXStringList  files;
  FXString      username;
  FXString      groupname;
  FXuint        type;
  FXuint        uflags;
  FXuint        gflags;
  FXuint        oflags;
  FXbool        userid;
  FXbool        groupid;
  FXbool        stickyid;
  bool          editpermissions;
  FXint         num_files;
  FXint         num_dir;
private:
  FXListBox * ownerlist;
  FXListBox * grouplist;
  FXListBox * otherlist;
  FXListBox * groupidlist;
  FXCheckButton * usercheck;
  FXCheckButton * groupcheck;
  FXCheckButton * stickycheck;
private:
  enum {
    SHOW_FOLDER,
    SHOW_FILE,
    SHOW_FOLDER_AND_FILE,
    };
private:
  void fileinfo(FXint no,FXuint & uflags,FXuint & gflags,FXuint & oflags,FXbool & user, FXbool & group, FXbool & sticky);
  void detectFileType();
  void initPermissions(FXbool advancedmode);
  void setup(const FXString & url,FXbool advancedmode);
private:
  FXFileProperties(){}
  FXFileProperties(const FXFileProperties&);
  FXFileProperties& operator=(const FXFileProperties&);
public:
  long onCmdAccept(FXObject*,FXSelector,void*);
public:
  FXFileProperties(FXApp* app,const FXString & url,const FXStringList & list,FXbool advancedmode);
  FXFileProperties(FXWindow* owner,const FXString & url,const FXStringList & list,FXbool advancedmode);

  void update(const FXString & url,const FXStringList & list,FXbool advancedmode);
  };

#endif

