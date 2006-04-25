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
#ifndef FXIMAGEVIEWEX_H
#define FXIMAGEVIEWEX_H

class FXImageViewEx : public FXImageView {
  FXDECLARE(FXImageViewEx)
protected:
  FXImageViewEx(){}
private:
  FXImageViewEx(const FXImageViewEx&);
  FXImageViewEx &operator=(const FXImageViewEx&);
public:
  FXImageViewEx(FXComposite* p,FXImage* img=NULL,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);
  ~FXImageViewEx();
  };
#endif
