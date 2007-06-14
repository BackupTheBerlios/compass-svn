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
#ifndef FXFILEDICTEX_H
#define FXFILEDICTEX_H


class FXAPI FXIconSourceEx : public FXIconSource {
  FXDECLARE(FXIconSourceEx)
protected:
  FXIconSourceEx(){}
private:
  FXIconSourceEx(const FXIconSourceEx&);
  FXIconSourceEx &operator=(const FXIconSourceEx&);
public:
  FXIconSourceEx(FXApp* a);
  virtual FXIcon *loadIconFile(const FXString& filename,const FXString& type=FXString::null) const;
  virtual FXIcon *loadIconData(const void *pixels,const FXString& type=FXString::null) const;
  virtual FXIcon *loadIconStream(FXStream& store,const FXString& type=FXString::null) const;
  virtual FXImage *loadImageFile(const FXString& filename,const FXString& type=FXString::null) const;
  virtual FXImage *loadImageData(const void *pixels,const FXString& type=FXString::null) const;
  virtual FXImage *loadImageStream(FXStream& store,const FXString& type=FXString::null) const;
  virtual FXIcon *loadScaledIconFile(const FXString& filename,FXint size=32,FXint qual=0,const FXString& type=FXString::null) const;
  virtual FXIcon *loadScaledIconData(const void *pixels,FXint size=32,FXint qual=0,const FXString& type=FXString::null) const;
  virtual FXIcon *loadScaledIconStream(FXStream& store,FXint size=32,FXint qual=0,const FXString& type=FXString::null) const;
  virtual FXImage *loadScaledImageFile(const FXString& filename,FXint size=32,FXint qual=0,const FXString& type=FXString::null) const;
  virtual FXImage *loadScaledImageData(const void *pixels,FXint size=32,FXint qual=0,const FXString& type=FXString::null) const;
  virtual FXImage *loadScaledImageStream(FXStream& store,FXint size=32,FXint qual=0,const FXString& type=FXString::null) const;
  };


class FXFileDictEx : public FXFileDict {
  FXDECLARE(FXFileDictEx)
protected:
  FXFileDictEx(){}

#if FOX_MINOR == 6
  virtual void *createData(const void*);
#else
  virtual void *createData(void*);
#endif
private:
  FXFileDictEx(const FXFileDictEx&);
  FXFileDictEx &operator=(const FXFileDictEx&);
public:
  FXFileDictEx(FXApp *);
  };
#endif
