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
#ifndef FXREMOVEFILES_H
#define FXREMOVEFILES_H

class FXTransfer;

class FXRemoveFiles : public FXTransfer {
protected:
  FXStringList   files;
  FXbool         ignore;
protected:
  virtual FXint run();
protected:
  void send_filename(const FXString & filename);
  void queryDelete(const FXString &);
  FXbool remove(const FXString &);
public:
  FXRemoveFiles(FXObject * tgt,FXSelector sel,FXStringList & files);

  void respondDelete(FXbool ignore);
  };

#endif
