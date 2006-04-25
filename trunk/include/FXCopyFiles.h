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
#ifndef FXCOPYFILES_H
#define FXCOPYFILES_H

class FXTransfer;

class FXCopyFiles : public FXTransfer {
protected:
  FXStringList   files;
  FXString       destination;
  FXbool         overwrite;             /// FALSE = never overwrite, MAYBE = ask first, TRUE = always overwrite
protected:
  virtual FXint run();
protected:
  FXbool copy(const FXString& srcfile,const FXString& dstfile);
  FXbool copyFiles(const FXString& srcfile,const FXString& dstfile);
  FXbool queryOverwriteFile(const FXString & src,const FXString dest);
  void send_filename(const FXString & filename,FXuint size);
  void send_progress(FXuint pos);
public:
  FXCopyFiles(FXObject * tgt,FXSelector sel,const FXStringList & files,const FXString & destination);

  virtual void respondOverwrite(FXbool,FXbool,FXbool);

  };

#endif
