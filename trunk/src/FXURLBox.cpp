/********************************************************************************
*                                                                               *
*                       C o m b o   B o x   O b j e c t                         *
*                                                                               *
*********************************************************************************
* Copyright (C) 1998,2005 by Jeroen van der Zijp.   All Rights Reserved.        *
*********************************************************************************
* This library is free software; you can redistribute it and/or                 *
* modify it under the terms of the GNU Lesser General Public                    *
* License as published by the Free Software Foundation; either                  *
* version 2.1 of the License, or (at your option) any later version.            *
*                                                                               *
* This library is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
* Lesser General Public License for more details.                               *
*                                                                               *
* You should have received a copy of the GNU Lesser General Public              *
* License along with this library; if not, write to the Free Software           *
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.    *
*********************************************************************************
* $Id: FXURLBox.cpp,v 1.64 2005/07/20 03:03:20 fox Exp $                      *
********************************************************************************/
#include <fx.h>
#include "FXURLBox.h"

/*
  Notes:
  - Handling typed text:
    a) Pass string to target only.
    b) Pass string to target & add to list [begin, after/before current, or end].
    c) Pass string to target & replace current item's label.
  - FXURLBox is a text field which may be filled from an FXList.
  - FXURLBox is a text field which in turn may fill an FXList also.
  - In most other respects, it behaves like a FXTextField.
  - Need to catch up/down arrow keys.
  - Combobox turns OFF GUI Updating while being manipulated.
  - Need some way to size the FXList automatically to the number of items.
  - If you leave the list then getCurrentItem() returns the last item under
    cursor which is not the same item shown in FXURLBox.
  - FXURLBox::getItem() and FXURLBox::getItemText() are the same; this
    should probably change.
  - No reaction to up and down arrow while disabled.
*/

#define URLBOX_INS_MASK   (URLBOX_REPLACE|URLBOX_INSERT_BEFORE|URLBOX_INSERT_AFTER|URLBOX_INSERT_FIRST|URLBOX_INSERT_LAST)
#define URLBOX_MASK       (URLBOX_STATIC|URLBOX_INS_MASK)


/*******************************************************************************/


// Map
FXDEFMAP(FXURLBox) FXURLBoxMap[]={
  FXMAPFUNC(SEL_FOCUS_UP,0,FXURLBox::onFocusUp),
  FXMAPFUNC(SEL_FOCUS_DOWN,0,FXURLBox::onFocusDown),
  FXMAPFUNC(SEL_FOCUS_SELF,0,FXURLBox::onFocusSelf),
  FXMAPFUNC(SEL_UPDATE,FXURLBox::ID_BUTTON,FXURLBox::onUpdButton),
  FXMAPFUNC(SEL_UPDATE,FXURLBox::ID_TEXT,FXURLBox::onUpdFmText),
  FXMAPFUNC(SEL_CLICKED,FXURLBox::ID_LIST,FXURLBox::onListClicked),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,FXURLBox::ID_TEXT,FXURLBox::onTextButton),
  FXMAPFUNC(SEL_MOUSEWHEEL,FXURLBox::ID_TEXT,FXURLBox::onMouseWheel),
  FXMAPFUNC(SEL_CHANGED,FXURLBox::ID_TEXT,FXURLBox::onTextChanged),
  FXMAPFUNC(SEL_COMMAND,FXURLBox::ID_TEXT,FXURLBox::onTextCommand),
  FXMAPFUNC(SEL_COMMAND,FXWindow::ID_SETVALUE,FXURLBox::onFwdToText),
  FXMAPFUNC(SEL_COMMAND,FXWindow::ID_SETINTVALUE,FXURLBox::onFwdToText),
  FXMAPFUNC(SEL_COMMAND,FXWindow::ID_SETREALVALUE,FXURLBox::onFwdToText),
  FXMAPFUNC(SEL_COMMAND,FXWindow::ID_SETSTRINGVALUE,FXURLBox::onFwdToText),
  FXMAPFUNC(SEL_COMMAND,FXWindow::ID_GETINTVALUE,FXURLBox::onFwdToText),
  FXMAPFUNC(SEL_COMMAND,FXWindow::ID_GETREALVALUE,FXURLBox::onFwdToText),
  FXMAPFUNC(SEL_COMMAND,FXWindow::ID_GETSTRINGVALUE,FXURLBox::onFwdToText),
  };


// Object implementation
FXIMPLEMENT(FXURLBox,FXPacker,FXURLBoxMap,ARRAYNUMBER(FXURLBoxMap))


// Combo box
FXURLBox::FXURLBox(FXComposite *p,FXint cols,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb):
  FXPacker(p,opts,x,y,w,h, 0,0,0,0, 0,0){
  flags|=FLAG_ENABLED;
  target=tgt;
  message=sel;
  label=new FXLabel(this,FXString::null,NULL);
  field=new FXTextField(this,cols,this,FXURLBox::ID_TEXT,0, 0,0,0,0, pl,pr,pt,pb);
  label->setBackColor(field->getBackColor());
  if(options&URLBOX_STATIC) field->setEditable(FALSE);
  pane=new FXPopup(this,FRAME_LINE);
  list=new FXList(pane,this,FXURLBox::ID_LIST,LIST_BROWSESELECT|LIST_AUTOSELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y|SCROLLERS_TRACK|HSCROLLER_NEVER);
  if(options&URLBOX_STATIC) list->setScrollStyle(SCROLLERS_TRACK|HSCROLLING_OFF);
  button=new FXMenuButton(this,FXString::null,NULL,pane,FRAME_RAISED|FRAME_THICK|MENUBUTTON_DOWN|MENUBUTTON_ATTACH_RIGHT, 0,0,0,0, 0,0,0,0);
  button->setXOffset(border);
  button->setYOffset(border);
  button->setTarget(this);
  button->setSelector(FXURLBox::ID_BUTTON);
  flags&=~FLAG_UPDATE;  // Never GUI update
  }


// Create window
void FXURLBox::create(){
  FXPacker::create();
  pane->create();
  }


// Detach window
void FXURLBox::detach(){
  FXPacker::detach();
  pane->detach();
  }


// Destroy window
void FXURLBox::destroy(){
  pane->destroy();
  FXPacker::destroy();
  }


// Enable the window
void FXURLBox::enable(){
  if(!isEnabled()){
    FXPacker::enable();
    field->enable();
    button->enable();
    }
  }


// Disable the window
void FXURLBox::disable(){
  if(isEnabled()){
    FXPacker::disable();
    field->disable();
    button->disable();
    }
  }


// Get default width
FXint FXURLBox::getDefaultWidth(){
  FXint ww,pw;
  ww=field->getDefaultWidth()+button->getDefaultWidth()+(border<<1);
  pw=pane->getDefaultWidth();
  return FXMAX(ww,pw);
  }


// Get default height
FXint FXURLBox::getDefaultHeight(){
  FXint th,bh;
  th=field->getDefaultHeight();
  bh=button->getDefaultHeight();
  return FXMAX(th,bh)+(border<<1);
  }


// Recalculate layout
void FXURLBox::layout(){
  FXint buttonWidth,textWidth,itemHeight,labelWidth;
  itemHeight=height-(border<<1);
  buttonWidth=button->getDefaultWidth();
  labelWidth=label->getDefaultWidth();
  textWidth=width-buttonWidth-(border<<1)-labelWidth;
  label->position(border,border,labelWidth,itemHeight);
  field->position(labelWidth+border,border,textWidth,itemHeight);
  button->position(border+textWidth+labelWidth,border,buttonWidth,itemHeight);
  pane->resize(width,pane->getDefaultHeight());
  flags&=~FLAG_DIRTY;
  }


void FXURLBox::setDelimiters(const FXString & l){
  field->setDelimiters(l.text());
  }

void FXURLBox::setFocusOnField(){
  field->setFocus();
  }


// Forward GUI update of text field to target; but only if pane is not popped
long FXURLBox::onUpdFmText(FXObject*,FXSelector,void*){
  return target && !isPaneShown() && target->tryHandle(this,FXSEL(SEL_UPDATE,message),NULL);
  }


// Forward GUI update of text field to target; but only if pane is not popped
long FXURLBox::onUpdButton(FXObject*sender,FXSelector,void*){
  if (isPaneShown() || list->getNumItems()>1)
    sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),NULL);
  else
    sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),NULL);
  return 1;
  }


// Command handled in the text field
long FXURLBox::onFwdToText(FXObject* sender,FXSelector sel,void* ptr){
  return field->handle(sender,sel,ptr);
  }


// Forward clicked message from list to target
long FXURLBox::onListClicked(FXObject*,FXSelector,void* ptr){
  button->handle(this,FXSEL(SEL_COMMAND,ID_UNPOST),NULL);    // Unpost the list
  if(0<=((FXint)(FXival)ptr)){
    field->setText(list->getItemText((FXint)(FXival)ptr));
    field->selectAll();
    if(target) target->tryHandle(this,FXSEL(SEL_COMMAND,message),(void*)getText().text());
    }
  return 1;
  }


// Pressed left button in text field
long FXURLBox::onTextButton(FXObject*,FXSelector,void*){
  if(options&URLBOX_STATIC){
    button->handle(this,FXSEL(SEL_COMMAND,ID_POST),NULL);    // Post the list
    return 1;
    }
  return 0;
  }


// Text has changed
long FXURLBox::onTextChanged(FXObject*,FXSelector,void* ptr){
  return target && target->tryHandle(this,FXSEL(SEL_CHANGED,message),ptr);
  }


// Text has changed
long FXURLBox::onTextCommand(FXObject*,FXSelector,void* ptr){
  FXint index=list->getCurrentItem();
  if(!(options&URLBOX_STATIC)){
    switch(options&URLBOX_INS_MASK){
      case URLBOX_REPLACE:
        if(0<=index) setItem(index,(FXchar*)ptr,getItemIcon(index),getItemData(index));
        break;
      case URLBOX_INSERT_BEFORE:
        if(0<=index) insertItem(index,(FXchar*)ptr);
        break;
      case URLBOX_INSERT_AFTER:
        if(0<=index) insertItem(index+1,(FXchar*)ptr);
        break;
      case URLBOX_INSERT_FIRST:
        insertItem(0,(FXchar*)ptr);
        break;
      case URLBOX_INSERT_LAST:
        appendItem((FXchar*)ptr);
        break;
      }
    }
  return target && target->tryHandle(this,FXSEL(SEL_COMMAND,message),ptr);
  }


// Bounce focus to the text field
long FXURLBox::onFocusSelf(FXObject* sender,FXSelector,void* ptr){
  return field->handle(sender,FXSEL(SEL_FOCUS_SELF,0),ptr);
  }


// Select upper item
long FXURLBox::onFocusUp(FXObject*,FXSelector,void*){
  if(isEnabled()){
    FXint index=getCurrentItem();
    if(index<0) index=getNumItems()-1;
    else if(0<index) index--;
    if(0<=index && index<getNumItems()){
      setCurrentItem(index,TRUE);
      }
    return 1;
    }
  return 0;
  }


// Select lower item
long FXURLBox::onFocusDown(FXObject*,FXSelector,void*){
  if(isEnabled()){
    FXint index=getCurrentItem();
    if(index<0) index=0;
    else if(index<getNumItems()-1) index++;
    if(0<=index && index<getNumItems()){
      setCurrentItem(index,TRUE);
      }
    return 1;
    }
  return 0;
  }


// Mouse wheel
long FXURLBox::onMouseWheel(FXObject*,FXSelector,void* ptr){
  FXEvent* event=(FXEvent*)ptr;
  if(isEnabled()){
    FXint index=getCurrentItem();
    if(event->code<0){
      if(index<0) index=0;
      else if(index<getNumItems()-1) index++;
      }
    else if(event->code>0){
      if(index<0) index=getNumItems()-1;
      else if(0<index) index--;
      }
    if(0<=index && index<getNumItems()){
      setCurrentItem(index,TRUE);
      }
    return 1;
    }
  return 0;
  }


// Return true if editable
FXbool FXURLBox::isEditable() const {
  return field->isEditable();
  }


// Set widget is editable or not
void FXURLBox::setEditable(FXbool edit){
  field->setEditable(edit);
  }


// Set text
void FXURLBox::setIcon(FXIcon * icon){
  label->setIcon(icon);
  }

// Set text
void FXURLBox::setText(const FXString& text){
  field->setText(text);
  }


// Obtain text
FXString FXURLBox::getText() const {
  return field->getText();
  }


// Set number of text columns
void FXURLBox::setNumColumns(FXint cols){
  field->setNumColumns(cols);
  }


// Get number of text columns
FXint FXURLBox::getNumColumns() const {
  return field->getNumColumns();
  }


// Get number of items
FXint FXURLBox::getNumItems() const {
  return list->getNumItems();
  }


// Get number of visible items
FXint FXURLBox::getNumVisible() const {
  return list->getNumVisible();
  }


// Set number of visible items
void FXURLBox::setNumVisible(FXint nvis){
  list->setNumVisible(nvis);
  }


// Is item current
FXbool FXURLBox::isItemCurrent(FXint index) const {
  return list->isItemCurrent(index);
  }


// Change current item
void FXURLBox::setCurrentItem(FXint index,FXbool notify){
  FXint current=list->getCurrentItem();
  if(current!=index){
    list->setCurrentItem(index);
    list->makeItemVisible(index);
    if(0<=index){
      setText(list->getItemText(index));
			setIcon(list->getItemIcon(index));
      }
    else{
      setText(FXString::null);
			setIcon(NULL);
      }
    if(notify && target){target->tryHandle(this,FXSEL(SEL_COMMAND,message),(void*)getText().text());}
    }
  }


// Get current item
FXint FXURLBox::getCurrentItem() const {
  return list->getCurrentItem();
  }


// Retrieve item
FXString FXURLBox::getItem(FXint index) const {
  return list->getItem(index)->getText();
  }


// Replace text of item at index
FXint FXURLBox::setItem(FXint index,const FXString& text,FXIcon * icon,void* ptr){
  if(index<0 || list->getNumItems()<=index){ fxerror("%s::setItem: index out of range.\n",getClassName()); }
  list->setItem(index,text,icon,ptr);
  if(isItemCurrent(index)){
    field->setText(text);
		label->setIcon(icon);
    }
  recalc();
  return index;
  }


// Fill list by appending items from array of strings
FXint FXURLBox::fillItems(const FXchar** strings){
  register FXint numberofitems=list->getNumItems();
  register FXint n=list->fillItems(strings);
  if(numberofitems<=list->getCurrentItem()){
    field->setText(list->getItemText(list->getCurrentItem()));
		label->setIcon(NULL);
    }
  recalc();
  return n;
  }


// Fill list by appending items from newline separated strings
FXint FXURLBox::fillItems(const FXString& strings){
  register FXint numberofitems=list->getNumItems();
  register FXint n=list->fillItems(strings);
  if(numberofitems<=list->getCurrentItem()){
    field->setText(list->getItemText(list->getCurrentItem()));
		label->setIcon(NULL);
    }
  recalc();
  return n;
  }


// Insert item at index
FXint FXURLBox::insertItem(FXint index,const FXString& text,FXIcon * icon,void* ptr){
  if(index<0 || list->getNumItems()<index){ fxerror("%s::insertItem: index out of range.\n",getClassName()); }
  list->insertItem(index,text,icon,ptr);
  if(isItemCurrent(index)){
    field->setText(text);
		label->setIcon(icon);
    }
  recalc();
  return index;
  }


// Append item
FXint FXURLBox::appendItem(const FXString& text,FXIcon * icon,void* ptr){
  FXint index=list->appendItem(text,icon,ptr);
  if(isItemCurrent(getNumItems()-1)){
    field->setText(text);
		label->setIcon(icon);
    }
  recalc();
  return index;
  }


// Prepend item
FXint FXURLBox::prependItem(const FXString& text,FXIcon * icon,void* ptr){
  FXint index=list->prependItem(text,icon,ptr);
  if(isItemCurrent(0)){
    field->setText(text);
		label->setIcon(icon);
    }
  recalc();
  return index;
  }


// Move item from oldindex to newindex
FXint FXURLBox::moveItem(FXint newindex,FXint oldindex){
  if(newindex<0 || list->getNumItems()<=newindex || oldindex<0 || list->getNumItems()<=oldindex){ fxerror("%s::moveItem: index out of range.\n",getClassName()); }
  FXint current=list->getCurrentItem();
  list->moveItem(newindex,oldindex);
  if(current!=list->getCurrentItem()){
    current=list->getCurrentItem();
    if(0<=current){
      field->setText(list->getItemText(current));
			label->setIcon(list->getItemIcon(current));
      }
    else{
      field->setText(" ");
			label->setIcon(NULL);
      }
    }
  recalc();
  return newindex;
  }


// Remove given item
void FXURLBox::removeItem(FXint index){
  FXint current=list->getCurrentItem();
  list->removeItem(index);
  if(index==current){
    current=list->getCurrentItem();
    if(0<=current){
      field->setText(list->getItemText(current));
			label->setIcon(list->getItemIcon(current));
      }
    else{
      field->setText(FXString::null);
			label->setIcon(NULL);
      }
    }
  recalc();
  }


// Remove all items
void FXURLBox::clearItems(){
  field->setText(FXString::null);
	label->setIcon(NULL);
  list->clearItems();
  recalc();
  }


// Get item by name
FXint FXURLBox::findItem(const FXString& text,FXint start,FXuint flgs) const {
  return list->findItem(text,start,flgs);
  }


// Get item by data
FXint FXURLBox::findItemByData(const void *ptr,FXint start,FXuint flgs) const {
  return list->findItemByData(ptr,start,flgs);
  }


// Set item text
void FXURLBox::setItemText(FXint index,const FXString& txt){
  if(isItemCurrent(index)) setText(txt);
  list->setItemText(index,txt);
  recalc();
  }


// Get item text
FXString FXURLBox::getItemText(FXint index) const {
  return list->getItemText(index);
  }


// Set item data
void FXURLBox::setItemData(FXint index,void* ptr) const {
  list->setItemData(index,ptr);
  }


// Get item data
void* FXURLBox::getItemData(FXint index) const {
  return list->getItemData(index);
  }


// Set item text
void FXURLBox::setItemIcon(FXint index,FXIcon * icon){
  if(isItemCurrent(index)) setIcon(icon);
  list->setItemIcon(index,icon);
  recalc();
  }

// Get item data
FXIcon* FXURLBox::getItemIcon(FXint index) const {
  return list->getItemIcon(index);
  }


// Is the pane shown
FXbool FXURLBox::isPaneShown() const {
  return pane->shown();
  }


// Set font
void FXURLBox::setFont(FXFont* fnt){
  if(!fnt){ fxerror("%s::setFont: NULL font specified.\n",getClassName()); }
  field->setFont(fnt);
  list->setFont(fnt);
  recalc();
  }


// Obtain font
FXFont* FXURLBox::getFont() const {
  return field->getFont();
  }


// Change combobox style
void FXURLBox::setComboStyle(FXuint mode){
  FXuint opts=(options&~URLBOX_MASK)|(mode&URLBOX_MASK);
  if(opts!=options){
    options=opts;
    if(options&URLBOX_STATIC){
      field->setEditable(FALSE);                                // Non-editable
      list->setScrollStyle(SCROLLERS_TRACK|HSCROLLING_OFF);     // No scrolling
      }
    else{
      field->setEditable(TRUE);                                 // Editable
      list->setScrollStyle(SCROLLERS_TRACK|HSCROLLER_NEVER);    // Scrollable, but no scrollbar
      }
    recalc();
    }
  }


// Get combobox style
FXuint FXURLBox::getComboStyle() const {
  return (options&URLBOX_MASK);
  }


// Set window background color
void FXURLBox::setBackColor(FXColor clr){
  field->setBackColor(clr);
  list->setBackColor(clr);
	label->setBackColor(clr);
  }


// Get background color
FXColor FXURLBox::getBackColor() const {
  return field->getBackColor();
  }


// Set text color
void FXURLBox::setTextColor(FXColor clr){
  field->setTextColor(clr);
  list->setTextColor(clr);
  }


// Return text color
FXColor FXURLBox::getTextColor() const {
  return field->getTextColor();
  }


// Set select background color
void FXURLBox::setSelBackColor(FXColor clr){
  field->setSelBackColor(clr);
  list->setSelBackColor(clr);
  }


// Return selected background color
FXColor FXURLBox::getSelBackColor() const {
  return field->getSelBackColor();
  }


// Set selected text color
void FXURLBox::setSelTextColor(FXColor clr){
  field->setSelTextColor(clr);
  list->setSelTextColor(clr);
  }


// Return selected text color
FXColor FXURLBox::getSelTextColor() const {
  return field->getSelTextColor();
  }


// Sort items using current sort function
void FXURLBox::sortItems(){
  list->sortItems();
  }


// Return sort function
FXListSortFunc FXURLBox::getSortFunc() const {
  return list->getSortFunc();
  }


// Change sort function
void FXURLBox::setSortFunc(FXListSortFunc func){
  list->setSortFunc(func);
  }


// Set help text
void FXURLBox::setHelpText(const FXString& txt){
  field->setHelpText(txt);
  }


// Get help text
const FXString& FXURLBox::getHelpText() const {
  return field->getHelpText();
  }


// Set tip text
void FXURLBox::setTipText(const FXString& txt){
  field->setTipText(txt);
  }


// Get tip text
const FXString& FXURLBox::getTipText() const {
  return field->getTipText();
  }


// Save object to stream
void FXURLBox::save(FXStream& store) const {
  FXPacker::save(store);
  store << field;
  store << button;
  store << list;
  store << pane;
  }


// Load object from stream
void FXURLBox::load(FXStream& store){
  FXPacker::load(store);
  store >> field;
  store >> button;
  store >> list;
  store >> pane;
  }


// Delete it
FXURLBox::~FXURLBox(){
  delete pane;
  pane=(FXPopup*)-1L;
  field=(FXTextField*)-1L;
  button=(FXMenuButton*)-1L;
  list=(FXList*)-1L;
	label=(FXLabel*)-1L;
  }

