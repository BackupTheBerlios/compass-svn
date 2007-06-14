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
#include <fx.h>
#include "FXFileDictEx.h"

FXIMPLEMENT(FXFileDictEx,FXFileDict,NULL,0);


FXIMPLEMENT(FXIconSourceEx,FXIconSource,NULL,0);

FXIconSourceEx::FXIconSourceEx(FXApp *a) : FXIconSource(a) {
  }

FXIcon *FXIconSourceEx::loadIconFile(const FXString& filename,const FXString& type) const{
  FXIcon * icon = FXIconSource::loadIconFile(filename,type);
  if (icon && icon->hasAlpha()) icon->blend(app->getBackColor());
  return icon;
  }
FXIcon *FXIconSourceEx::loadIconData(const void *pixels,const FXString& type) const{
  FXIcon * icon = FXIconSource::loadIconData(pixels,type);
  if (icon && icon->hasAlpha()) icon->blend(app->getBackColor());
  return icon;
  }
FXIcon *FXIconSourceEx::loadIconStream(FXStream& store,const FXString& type) const{
  FXIcon * icon = FXIconSource::loadIconStream(store,type);
  if (icon && icon->hasAlpha()) icon->blend(app->getBackColor());
  return icon;
  }
FXImage *FXIconSourceEx::loadImageFile(const FXString& filename,const FXString& type) const{
  FXImage * image = FXIconSource::loadImageFile(filename,type);
  if (image && image->hasAlpha()) image->blend(app->getBackColor());
  return image;
  }
FXImage *FXIconSourceEx::loadImageData(const void *pixels,const FXString& type) const{
  FXImage * image = FXIconSource::loadImageData(pixels,type);
  if (image && image->hasAlpha()) image->blend(app->getBackColor());
  return image;
  }
FXImage *FXIconSourceEx::loadImageStream(FXStream& store,const FXString& type) const{
  FXImage * image = FXIconSource::loadImageStream(store,type);
  if (image && image->hasAlpha()) image->blend(app->getBackColor());
  return image;
  }
FXIcon *FXIconSourceEx::loadScaledIconFile(const FXString& filename,FXint size,FXint qual,const FXString& type) const{
  FXIcon * icon = FXIconSource::loadScaledIconFile(filename,size,qual,type);
  if (icon && icon->hasAlpha()) icon->blend(app->getBackColor());
  return icon;
  }
FXIcon *FXIconSourceEx::loadScaledIconData(const void *pixels,FXint size,FXint qual,const FXString& type) const{
  FXIcon * icon = FXIconSource::loadScaledIconData(pixels,size,qual,type);
  if (icon && icon->hasAlpha()) icon->blend(app->getBackColor());
  return icon;
  }
FXIcon *FXIconSourceEx::loadScaledIconStream(FXStream& store,FXint size,FXint qual,const FXString& type) const{
  FXIcon * icon = FXIconSource::loadScaledIconStream(store,size,qual,type);
  if (icon && icon->hasAlpha()) icon->blend(app->getBackColor());
  return icon;
  }
FXImage *FXIconSourceEx::loadScaledImageFile(const FXString& filename,FXint size,FXint qual,const FXString& type) const{
  FXImage * image = FXIconSource::loadScaledImageFile(filename,size,qual,type);
  if (image && image->hasAlpha()) image->blend(app->getBackColor());
  return image;
  }
FXImage *FXIconSourceEx::loadScaledImageData(const void *pixels,FXint size,FXint qual,const FXString& type) const{
  FXImage * image = FXIconSource::loadScaledImageData(pixels,size,qual,type);
  if (image && image->hasAlpha()) image->blend(app->getBackColor());
  return image;
  }
FXImage *FXIconSourceEx::loadScaledImageStream(FXStream& store,FXint size,FXint qual,const FXString& type) const{
  FXImage * image = FXIconSource::loadScaledImageStream(store,size,qual,type);
  if (image && image->hasAlpha()) image->blend(app->getBackColor());
  return image;
  }





FXFileDictEx::FXFileDictEx(FXApp * app) : FXFileDict(app) {
  delete getIconDict()->getIconSource();
  getIconDict()->setIconSource(new FXIconSourceEx(app));
  }

#if FOX_MINOR == 6
void *FXFileDictEx::createData(const void* ptr) {
#else
void *FXFileDictEx::createData(void* ptr) {
#endif
  FXFileAssoc * assoc =  reinterpret_cast<FXFileAssoc*>(FXFileDict::createData(ptr));
/*
  fxmessage("CHECK\n");

  if (assoc->bigicon && assoc->bigicon->hasAlpha())
    assoc->bigicon->blend(FXRGB(255,255,255));

  if (assoc->miniicon && assoc->miniicon->hasAlpha())
    assoc->miniicon->blend(FXRGB(255,255,255));

  if (assoc->bigiconopen && assoc->bigiconopen->hasAlpha())
    assoc->bigiconopen->blend(FXRGB(255,255,255));

  if (assoc->miniiconopen && assoc->miniiconopen->hasAlpha())
    assoc->miniiconopen->blend(FXRGB(255,255,255));
*/
  return assoc;
  }
