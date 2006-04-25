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
#include <fx.h>
#include "FXFileDictEx.h"

FXIMPLEMENT(FXFileDictEx,FXFileDict,NULL,0);

FXFileDictEx::FXFileDictEx(FXApp * app) : FXFileDict(app) {
  }

void *FXFileDictEx::createData(const void* ptr) {
  FXFileAssoc * assoc =  reinterpret_cast<FXFileAssoc*>(FXFileDict::createData(ptr));

  if (assoc->bigicon)
    assoc->bigicon->blend(FXRGB(255,255,255));

  if (assoc->miniicon)
    assoc->miniicon->blend(FXRGB(255,255,255));

  if (assoc->bigiconopen)
    assoc->bigiconopen->blend(FXRGB(255,255,255));

  if (assoc->miniiconopen)
    assoc->miniiconopen->blend(FXRGB(255,255,255));

  return assoc;
  }
