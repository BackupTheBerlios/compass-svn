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
#include "aboutwindow.h"


/* created by reswrap from file GPL_TEXT */
const char GPL_LICENSE_SHORT[]=
  "\x54\x68\x69\x73\x20\x70\x72\x6f\x67\x72\x61\x6d\x20\x69\x73\x20\x66\x72\x65\x65"
  "\x20\x73\x6f\x66\x74\x77\x61\x72\x65\x3b\x20\x79\x6f\x75\x20\x63\x61\x6e\x20\x72"
  "\x65\x64\x69\x73\x74\x72\x69\x62\x75\x74\x65\x20\x69\x74\x20\x61\x6e\x64\x2f\x6f"
  "\x72\x0a\x6d\x6f\x64\x69\x66\x79\x20\x69\x74\x20\x75\x6e\x64\x65\x72\x20\x74\x68"
  "\x65\x20\x74\x65\x72\x6d\x73\x20\x6f\x66\x20\x74\x68\x65\x20\x47\x4e\x55\x20\x47"
  "\x65\x6e\x65\x72\x61\x6c\x20\x50\x75\x62\x6c\x69\x63\x20\x4c\x69\x63\x65\x6e\x73"
  "\x65\x0a\x61\x73\x20\x70\x75\x62\x6c\x69\x73\x68\x65\x64\x20\x62\x79\x20\x74\x68"
  "\x65\x20\x46\x72\x65\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65\x20\x46\x6f\x75\x6e"
  "\x64\x61\x74\x69\x6f\x6e\x3b\x20\x65\x69\x74\x68\x65\x72\x20\x76\x65\x72\x73\x69"
  "\x6f\x6e\x20\x32\x0a\x6f\x66\x20\x74\x68\x65\x20\x4c\x69\x63\x65\x6e\x73\x65\x2c"
  "\x20\x6f\x72\x20\x28\x61\x74\x20\x79\x6f\x75\x72\x20\x6f\x70\x74\x69\x6f\x6e\x29"
  "\x20\x61\x6e\x79\x20\x6c\x61\x74\x65\x72\x20\x76\x65\x72\x73\x69\x6f\x6e\x2e\x0a"
  "\x0a\x54\x68\x69\x73\x20\x70\x72\x6f\x67\x72\x61\x6d\x20\x69\x73\x20\x64\x69\x73"
  "\x74\x72\x69\x62\x75\x74\x65\x64\x20\x69\x6e\x20\x74\x68\x65\x20\x68\x6f\x70\x65"
  "\x20\x74\x68\x61\x74\x20\x69\x74\x20\x77\x69\x6c\x6c\x20\x62\x65\x20\x75\x73\x65"
  "\x66\x75\x6c\x2c\x0a\x62\x75\x74\x20\x57\x49\x54\x48\x4f\x55\x54\x20\x41\x4e\x59"
  "\x20\x57\x41\x52\x52\x41\x4e\x54\x59\x3b\x20\x77\x69\x74\x68\x6f\x75\x74\x20\x65"
  "\x76\x65\x6e\x20\x74\x68\x65\x20\x69\x6d\x70\x6c\x69\x65\x64\x20\x77\x61\x72\x72"
  "\x61\x6e\x74\x79\x20\x6f\x66\x0a\x4d\x45\x52\x43\x48\x41\x4e\x54\x41\x42\x49\x4c"
  "\x49\x54\x59\x20\x6f\x72\x20\x46\x49\x54\x4e\x45\x53\x53\x20\x46\x4f\x52\x20\x41"
  "\x20\x50\x41\x52\x54\x49\x43\x55\x4c\x41\x52\x20\x50\x55\x52\x50\x4f\x53\x45\x2e"
  "\x20\x53\x65\x65\x20\x74\x68\x65\x0a\x47\x4e\x55\x20\x47\x65\x6e\x65\x72\x61\x6c"
  "\x20\x50\x75\x62\x6c\x69\x63\x20\x4c\x69\x63\x65\x6e\x73\x65\x20\x66\x6f\x72\x20"
  "\x6d\x6f\x72\x65\x20\x64\x65\x74\x61\x69\x6c\x73\x2e\x0a\x0a\x59\x6f\x75\x20\x73"
  "\x68\x6f\x75\x6c\x64\x20\x68\x61\x76\x65\x20\x72\x65\x63\x65\x69\x76\x65\x64\x20"
  "\x61\x20\x63\x6f\x70\x79\x20\x6f\x66\x20\x74\x68\x65\x20\x47\x4e\x55\x20\x47\x65"
  "\x6e\x65\x72\x61\x6c\x20\x50\x75\x62\x6c\x69\x63\x20\x4c\x69\x63\x65\x6e\x73\x65"
  "\x0a\x61\x6c\x6f\x6e\x67\x20\x77\x69\x74\x68\x20\x74\x68\x69\x73\x20\x70\x72\x6f"
  "\x67\x72\x61\x6d\x3b\x20\x69\x66\x20\x6e\x6f\x74\x2c\x20\x77\x72\x69\x74\x65\x20"
  "\x74\x6f\x20\x74\x68\x65\x20\x46\x72\x65\x65\x20\x53\x6f\x66\x74\x77\x61\x72\x65"
  "\x0a\x46\x6f\x75\x6e\x64\x61\x74\x69\x6f\x6e\x2c\x20\x49\x6e\x63\x2e\x2c\x20\x35"
  "\x39\x20\x54\x65\x6d\x70\x6c\x65\x20\x50\x6c\x61\x63\x65\x20\x2d\x20\x53\x75\x69"
  "\x74\x65\x20\x33\x33\x30\x2c\x20\x42\x6f\x73\x74\x6f\x6e\x2c\x20\x4d\x41\x20\x30"
  "\x32\x31\x31\x31\x2d\x31\x33\x30\x37\x2c\x20\x55\x53\x41\x2e\x0a"
  ;


/*
// Define Message Map
FXDEFMAP(FXTaskbar) FXTaskbarMap[]={
  //____Message_Type______________ID_______________Message_Handler___
  };
*/

// Object implementation
FXIMPLEMENT(FXAboutDialog,FXDialogBox,NULL,0)

FXAboutDialog::FXAboutDialog(FXApp * app,const FXString & name, const FXString & version, const FXString & author, const FXString & year,FXIcon * icon) : FXDialogBox(app,"",DECOR_ALL,0,0,0,0,0,0,0,0) {
  setup(name,version,author,year,icon);
  setTitle("About "+name);
  }

FXAboutDialog::FXAboutDialog(FXWindow* owner,const FXString & name, const FXString & version, const FXString & author, const FXString & year,FXIcon * icon) : FXDialogBox(owner,"",DECOR_TITLE|DECOR_BORDER,0,0,0,0,0,0,0,0) {
  setup(name,version,author,year,icon);
  setTitle("About "+name);
  }

FXAboutDialog::~FXAboutDialog(){
  delete titlefont;
  }


void FXAboutDialog::setup(const FXString & name, const FXString & version, const FXString & author, const FXString & year,FXIcon * icon){
  /// Create Title Font
  FXFontDesc fontdescription;
  getApp()->getNormalFont()->getFontDesc(fontdescription);
  fontdescription.size = (FXuint) (((double)fontdescription.size) * 1.5);
  titlefont = new FXFont(getApp(),fontdescription);
  titlefont->create();
  FXint spacing=15;

  FXLabel * label=NULL;
  FXVerticalFrame * main = new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0,0,0);

  FXHorizontalFrame * header =new FXHorizontalFrame(main,LAYOUT_FILL_X,0,0,0,0,0,0,0,0,0,0);
  label = new FXLabel(header,FXString::null,icon,LABEL_NORMAL|LAYOUT_FILL_Y,0,0,0,0,10,10,0,0);
  label->setBackColor(FXRGB(255,255,255));
  FXVerticalFrame * textframe=new FXVerticalFrame(header,LAYOUT_FILL_X,0,0,0,0,0,0,0,0,0,0);

  label = new FXLabel(textframe,name,NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,0,spacing,spacing,0);
  label->setBackColor(FXRGB(255,255,255));
  label->setFont(titlefont);

  label = new FXLabel(textframe,"The filemanager for the FOX Desktop",NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,30,spacing,0,spacing);
  label->setBackColor(FXRGB(255,255,255));
//  new FXSeparator(p,SEPARATOR_GROOVE|LAYOUT_FILL_X);








  FXVerticalFrame * frame=NULL;

//  FXint spacing=15;

//  if (icon)
//  	spacing=5;
  
//  FXLabel * label = new FXLabel(main,name,icon,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,spacing,spacing,spacing,spacing);
//  label->setBackColor(FXRGB(255,255,255));
//  label->setFont(titlefont);

  new FXSeparator(main,SEPARATOR_GROOVE|LAYOUT_FILL_X);
  FXString title = name + " " + version + "\nCopyright (c) " + year + " "  + author;
  frame = new FXVerticalFrame(main,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,15,15,15,15);
  new FXLabel(frame,title,NULL,LAYOUT_FILL_X|JUSTIFY_LEFT);
  new FXLabel(frame,GPL_LICENSE_SHORT,NULL,LAYOUT_FILL_X|JUSTIFY_LEFT);
  new FXSeparator(main,SEPARATOR_GROOVE|LAYOUT_FILL_X);
  FXHorizontalFrame *closebox=new FXHorizontalFrame(main,LAYOUT_BOTTOM|LAYOUT_FILL_X,0,0,0,0);
  new FXButton(closebox," &Close ",NULL,this,FXDialogBox::ID_CANCEL,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
	label = new FXLabel(closebox,FXStringFormat("Build with fox-toolkit v%d.%d.%d",fxversion[0],fxversion[1],fxversion[2]),NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|LAYOUT_CENTER_Y,0,0,0,0,20);
	label->disable();
  }







