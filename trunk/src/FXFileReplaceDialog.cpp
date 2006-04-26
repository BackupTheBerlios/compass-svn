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
#include "FXFileApplication.h"
#include "FXFileReplaceDialog.h"

/*
// Define Message Map
FXDEFMAP(FXTaskbar) FXTaskbarMap[]={
  //____Message_Type______________ID_______________Message_Handler___
  };
*/

// Object implementation
FXIMPLEMENT(FXFileReplaceDialog,FXDialogBox,NULL,0)

FXFileReplaceDialog::FXFileReplaceDialog(FXApp * app) : FXDialogBox(app,"Rename Selected Files",DECOR_ALL,0,0,0,0,0,0,0,0) {
  setup();
  }

FXFileReplaceDialog::FXFileReplaceDialog(FXWindow* owner) : FXDialogBox(owner,"Rename Selected Files",DECOR_TITLE|DECOR_BORDER,0,0,0,0,0,0,0,0) {
  setup();
  }

FXFileReplaceDialog::~FXFileReplaceDialog(){
  }


void FXFileReplaceDialog::setup(){
  FXint spacing=15;

  FXLabel * label=NULL;
  FXVerticalFrame * main = new FXVerticalFrame(this,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0,0,0);

  FXHorizontalFrame * header =new FXHorizontalFrame(main,LAYOUT_FILL_X,0,0,0,0,0,0,0,0,0,0);
  label = new FXLabel(header,FXString::null,NULL,LABEL_NORMAL|LAYOUT_FILL_Y,0,0,0,0,10,10,0,0);
  label->setBackColor(FXRGB(255,255,255));
  FXVerticalFrame * textframe=new FXVerticalFrame(header,LAYOUT_FILL_X,0,0,0,0,0,0,0,0,0,0);

  label = new FXLabel(textframe,"Advanced File Rename",NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,0,spacing,spacing,0);
  label->setBackColor(FXRGB(255,255,255));
  label->setFont(FXFileApplication::me->font_title);

  label = new FXLabel(textframe,"Replace parts of filenames using expressions",NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,30,spacing,0,spacing);
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

  FXVerticalFrame * content = new FXVerticalFrame(main,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,10,10,10,10);
  FXLabel*searchlabel=new FXLabel(content,tr("S&earch for:"),NULL,JUSTIFY_LEFT|ICON_BEFORE_TEXT|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X);
  FXHorizontalFrame*searchbox=new FXHorizontalFrame(content,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_CENTER_Y,0,0,0,0, 0,0,0,0, 0,0);
  FXTextField*searchtext=new FXTextField(searchbox,26,/*this,ID_SEARCH_TEXT,*/NULL,0,TEXTFIELD_ENTER_ONLY|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 4,4,4,4);
  FXVerticalFrame* searcharrows=new FXVerticalFrame(searchbox,LAYOUT_RIGHT|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,0);
  FXArrowButton* ar1=new FXArrowButton(searcharrows,/*this,ID_SEARCH_UP,*/NULL,0,FRAME_RAISED|FRAME_THICK|ARROW_UP|ARROW_REPEAT|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0,0,16,0, 1,1,1,1);
  FXArrowButton* ar2=new FXArrowButton(searcharrows,/*this,ID_SEARCH_DN,*/NULL,0,FRAME_RAISED|FRAME_THICK|ARROW_DOWN|ARROW_REPEAT|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0,0,16,0, 1,1,1,1);
  ar1->setArrowSize(3);
  ar2->setArrowSize(3);
  FXLabel*replacelabel=new FXLabel(content,tr("Replace &with:"),NULL,LAYOUT_LEFT);
  FXHorizontalFrame* replacebox=new FXHorizontalFrame(content,FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_CENTER_Y,0,0,0,0, 0,0,0,0, 0,0);
  FXTextField* replacetext=new FXTextField(replacebox,26,/*this,ID_REPLACE_TEXT,*/NULL,0,TEXTFIELD_ENTER_ONLY|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0, 4,4,4,4);
  FXVerticalFrame* replacearrows=new FXVerticalFrame(replacebox,LAYOUT_RIGHT|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0, 0,0);
  FXArrowButton* ar3=new FXArrowButton(replacearrows,/*this,ID_REPLACE_UP,*/NULL,0,FRAME_RAISED|FRAME_THICK|ARROW_UP|ARROW_REPEAT|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0,0,16,0, 1,1,1,1);
  FXArrowButton* ar4=new FXArrowButton(replacearrows,/*this,ID_REPLACE_DN,*/NULL,0,FRAME_RAISED|FRAME_THICK|ARROW_DOWN|ARROW_REPEAT|LAYOUT_FILL_Y|LAYOUT_FIX_WIDTH, 0,0,16,0, 1,1,1,1);
  ar3->setArrowSize(3);
  ar4->setArrowSize(3);
  FXHorizontalFrame* options1=new FXHorizontalFrame(content,LAYOUT_FILL_X,0,0,0,0, 0,0,0,0);
  new FXRadioButton(options1,tr("Ex&act"),/*this,ID_MODE+SEARCH_EXACT,*/NULL,0,ICON_BEFORE_TEXT);
  new FXRadioButton(options1,tr("&Ignore Case"),/*this,ID_MODE+SEARCH_IGNORECASE,*/NULL,0,ICON_BEFORE_TEXT);
  new FXRadioButton(options1,tr("E&xpression"),/*this,ID_MODE+SEARCH_REGEX,*/NULL,0,ICON_BEFORE_TEXT);
//  new FXCheckButton(options1,tr("&Backward"),/*this,ID_DIR,*/NULL,0,ICON_BEFORE_TEXT|LAYOUT_CENTER_X);

  new FXSeparator(main,SEPARATOR_GROOVE|LAYOUT_FILL_X);
  FXHorizontalFrame *closebox=new FXHorizontalFrame(main,LAYOUT_BOTTOM|LAYOUT_FILL_X,0,0,0,0);
  new FXButton(closebox," &Rename ",NULL,this,FXDialogBox::ID_CANCEL,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
  new FXButton(closebox," &Cancel ",NULL,this,FXDialogBox::ID_CANCEL,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
  }







