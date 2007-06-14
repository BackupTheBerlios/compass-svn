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
#include <sys/types.h>
#include <unistd.h>


#include "common.h"
#include "FXFileApplication.h"
#include "FXFileProperties.h"


#define UGO_FILE_PERMISSION "Forbidden\nCan Read\nCan Write\nCan Execute\nCan Read and Write\nCan Read and Execute\nCan Write and Execute\nCan Read, Write and Execute"
#define UGO_FOLDER_PERMISSION "Forbidden\nCan Read\nCan Write\nCan Enter\nCan Read and Write\nCan Read and Enter\nCan Write and Enter\nCan Read, Write and Enter"

#define UGO_SIMPLE_FILE_PERMISSION "Forbidden\nCan Read\nCan Write\nCan Read and Write"
#define UGO_SIMPLE_FOLDER_PERMISSION "Forbidden\nCan View Content\nCan Modify Content\nCan View and Modify Content"


extern FXString display_bytes(FXulong bytes);


// Define Message Map
FXDEFMAP(FXFileProperties) FXFilePropertiesMap[]={
  //____Message_Type______________ID_______________Message_Handler___
  FXMAPFUNC(SEL_COMMAND,FXDialogBox::ID_ACCEPT,FXFileProperties::onCmdAccept),
  };

// Object implementation
FXIMPLEMENT(FXFileProperties,FXDialogBox,FXFilePropertiesMap,ARRAYNUMBER(FXFilePropertiesMap))

FXFileProperties::FXFileProperties(FXWindow* owner,const FXStringList & list,FXbool advancedmode) : FXDialogBox(owner,"File Properties",DECOR_TITLE|DECOR_BORDER,0,0,0,0,0,0,0,0,0,0), files(list), type(SHOW_FILE),editpermissions(TRUE) {
  detectFileType();
  setup(advancedmode);
  initPermissions(advancedmode);





  }

/*
    show_header(main,"Properties","Modify file and folder properties",app->icon_lock);
  else
    show_header(main,"Properties","Only the owner may change properties",app->icon_lock);
*/

void FXFileProperties::fileinfo(FXint no,FXuint & uflags,FXuint & gflags,FXuint & oflags,FXbool & user, FXbool & group, FXbool & sticky) {
  FXStat info;
  FXStat::statFile(files[no],info);

  uflags=gflags=oflags=0;

  if (info.isOwnerExecutable()) {
    if (info.isOwnerReadable()) {
      if (info.isOwnerWritable())
        uflags=7;
      else
        uflags=5;
      }
    else if (info.isOwnerWritable())
      uflags=6;
    else
      uflags=3;
    }
  else if (info.isOwnerReadable()){
    if (info.isOwnerWritable())
      uflags=4;
    else
      uflags=1;
    }
  else if (info.isOwnerWritable()){
    uflags=2;
    }

  if (info.isGroupExecutable()) {
    if (info.isGroupReadable()) {
      if (info.isGroupWritable())
        gflags=7;
      else
        gflags=5;
      }
    else if (info.isGroupWritable())
      gflags=6;
    else
      gflags=3;
    }
  else if (info.isGroupReadable()){
    if (info.isGroupWritable())
      gflags=4;
    else
      gflags=1;
    }
  else if (info.isGroupWritable()){
    gflags=2;
    }

  if (info.isOtherExecutable()) {
    if (info.isOtherReadable()) {
      if (info.isOtherWritable())
        oflags=7;
      else
        oflags=5;
      }
    else if (info.isOtherWritable())
      oflags=6;
    else
      oflags=3;
    }
  else if (info.isOtherReadable()){
    if (info.isOtherWritable())
      oflags=4;
    else
      oflags=1;
    }
  else if (info.isOtherWritable()){
    oflags=2;
    }

  user = info.isSetUid();
  group = info.isSetGid();
  sticky = info.isSetSticky();


  }




void FXFileProperties::detectFileType() {
  num_dir=num_files=0;  
  FXStat stat;
  FXStat::statLink(files[0],stat);
  type = (stat.isDirectory(files[0]) ? SHOW_FOLDER : SHOW_FILE);
  if (type==SHOW_FOLDER)
    num_dir++;
  else
    num_files++;
  editpermissions = (FXSystem::user() == stat.user());
  FXuint group,user;
  user = stat.user();
  group = stat.group();
  username = FXSystem::userName(user);
  groupname = FXSystem::groupName(group);

  if (files.no()>1) {
    unsigned int filetype;
    for (FXint i=1;i<files.no();i++){
      FXStat::statLink(files[i],stat);
      if (FXSystem::user() != stat.user() )
        editpermissions=FALSE;

      if (user!=stat.user())
        username=FXString::null;

      if (group!=stat.group())
        groupname=FXString::null;

      filetype=(stat.isDirectory(files[i]) ? SHOW_FOLDER : SHOW_FILE);
      if (filetype==SHOW_FOLDER)
        num_dir++;
      else
        num_files++;
      }
    if (num_dir>0 && num_files>0)
      type=SHOW_FOLDER_AND_FILE;
    }
  }

void FXFileProperties::initPermissions(FXbool advanced){
  register FXint i=0;
  if (type == SHOW_FILE) {
    if (advanced) {
      ownerlist->fillItems(UGO_FILE_PERMISSION);
      grouplist->fillItems(UGO_FILE_PERMISSION);
      otherlist->fillItems(UGO_FILE_PERMISSION);
      }
    else {
      ownerlist->fillItems(UGO_SIMPLE_FILE_PERMISSION);
      grouplist->fillItems(UGO_SIMPLE_FILE_PERMISSION);
      otherlist->fillItems(UGO_SIMPLE_FILE_PERMISSION);
      }
    }
  else if (type == SHOW_FOLDER) {
    if (advanced) {
      ownerlist->fillItems(UGO_FOLDER_PERMISSION);
      grouplist->fillItems(UGO_FOLDER_PERMISSION);
      otherlist->fillItems(UGO_FOLDER_PERMISSION);
      }
    else {
      ownerlist->fillItems(UGO_SIMPLE_FOLDER_PERMISSION);
      grouplist->fillItems(UGO_SIMPLE_FOLDER_PERMISSION);
      otherlist->fillItems(UGO_SIMPLE_FOLDER_PERMISSION);
      }
    }
  else {
    }

  FXuint uflags2;
  FXuint gflags2;
  FXuint oflags2;
  FXbool userid2;
  FXbool groupid2;
  FXbool stickyid2;
  fileinfo(0,uflags,gflags,oflags,userid,groupid,stickyid);


  if (files.no()>1) {
    for (i=1;i<files.no();i++){
      fileinfo(i,uflags2,gflags2,oflags2,userid2,groupid2,stickyid2);

      if (uflags!=8 && uflags2!=uflags)
        uflags=8;

      if (gflags!=8 && gflags2!=gflags)
        gflags=8;

      if (oflags!=8 && oflags2!=oflags)
        oflags=8;

      if (userid!=MAYBE && userid2!=userid)
        userid=MAYBE;

      if (groupid!=MAYBE && groupid2!=groupid)
        groupid=MAYBE;

      if (stickyid!=MAYBE && stickyid2!=stickyid)
        stickyid=MAYBE;

      if (uflags==8 && gflags==8 && oflags==8 && userid==MAYBE && groupid==MAYBE && stickyid==MAYBE) break;
      }
    if (uflags==8)
      ownerlist->appendItem(" ");

    if (gflags==8)
      grouplist->appendItem(" ");

    if (oflags==8)
      otherlist->appendItem(" ");
    }
  ownerlist->setCurrentItem(FXMIN((unsigned)ownerlist->getNumItems()-1,uflags));
  grouplist->setCurrentItem(FXMIN((unsigned)grouplist->getNumItems()-1,gflags));
  otherlist->setCurrentItem(FXMIN((unsigned)otherlist->getNumItems()-1,oflags));
  ownerlist->setNumVisible(ownerlist->getNumItems());
  grouplist->setNumVisible(grouplist->getNumItems());
  otherlist->setNumVisible(otherlist->getNumItems());
  usercheck->setCheck(userid);
  groupcheck->setCheck(groupid);
  stickycheck->setCheck(stickyid);

  }


void FXFileProperties::setup(FXbool advanced){
  FXFileApplication * app = dynamic_cast<FXFileApplication*>(getApp());
  FXint spacing=15;
  FXLabel * label=NULL;
  FXHorizontalFrame * main =new FXHorizontalFrame(this,LAYOUT_FILL_X,0,0,0,0,0,0,0,0,0,0);
  label = new FXLabel(main,FXString::null,app->icon_lock,LABEL_NORMAL|LAYOUT_FILL_Y,0,0,0,0,10,10,0,0);
  label->setBackColor(FXRGB(255,255,255));
  FXVerticalFrame * textframe=new FXVerticalFrame(main,LAYOUT_FILL_X,0,0,0,0,0,0,0,0,0,0);
  label = new FXLabel(textframe,"Properties",NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,0,spacing,spacing,0);
  label->setBackColor(FXRGB(255,255,255));
  label->setFont(app->font_title);
  if (editpermissions)
    label = new FXLabel(textframe,"Modify file and folder properties",NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,30,spacing,0,spacing);
  else
    label = new FXLabel(textframe,"Only the owner may change properties",NULL,LAYOUT_FILL_X|JUSTIFY_LEFT|TEXT_AFTER_ICON,0,0,0,0,30,spacing,0,spacing);

  label->setBackColor(FXRGB(255,255,255));
  new FXSeparator(this,SEPARATOR_GROOVE|LAYOUT_FILL_X);

  FXHorizontalFrame *closebox=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0);
  new FXButton(closebox,tr("&Cancel"),NULL,this,FXDialogBox::ID_CANCEL,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0,20,20);
  new FXButton(closebox,tr("&Update"),NULL,this,FXDialogBox::ID_ACCEPT,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0,20,20);

  FXMatrix * matrix=NULL;

  FXTabBook * tabbook = new FXTabBook(this,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0);

  new FXTabItem(tabbook," General ");
  FXVerticalFrame * content = new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK,0,0,0,0);


  if (files.no()==1) {
    matrix = new FXMatrix(content,2,LAYOUT_FILL_X|LAYOUT_FILL_Y|MATRIX_BY_COLUMNS,0,0,0,0);
    new FXLabel(matrix,"Name",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
    new FXLabel(matrix,FXPath::name(files[0]),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);


    new FXLabel(matrix,"Size",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
    new FXLabel(matrix,display_bytes(FXStat::size(files[0])),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);

    new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
    new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);

    new FXLabel(matrix,"Modified",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
#if FOX_MINOR == 6
    new FXLabel(matrix,FXSystem::time(FXStat::modified(files[0])),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
#else
    new FXLabel(matrix,FXSystem::localTime(FXStat::modified(files[0])),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
#endif

    new FXLabel(matrix,"Accessed",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
#if FOX_MINOR == 6
    new FXLabel(matrix,FXSystem::time(FXStat::accessed(files[0])),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
#else
    new FXLabel(matrix,FXSystem::localTime(FXStat::accessed(files[0])),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
#endif

    new FXLabel(matrix,"Created",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
#if FOX_MINOR == 6
    new FXLabel(matrix,FXSystem::time(FXStat::created(files[0])),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
#else
    new FXLabel(matrix,FXSystem::localTime(FXStat::created(files[0])),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
#endif

#if FOX_MINOR == 6
    new FXLabel(matrix,"Touched",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
    new FXLabel(matrix,FXSystem::time(FXStat::touched(files[0])),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
#endif

    new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
    new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);
    }
  else {
    FXlong totalsize=0;
    for (FXint i=0;i<files.no();i++){
      totalsize+=FXStat::size(files[i]);
      }

    matrix = new FXMatrix(content,2,LAYOUT_FILL_X|LAYOUT_FILL_Y|MATRIX_BY_COLUMNS,0,0,0,0);
    new FXLabel(matrix,"Selected",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
    if (num_files>0 && num_dir>0)
      new FXLabel(matrix,FXStringFormat("%d File(s), %d Folder(s)",num_files,num_dir),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
    else if (num_files>0)
      new FXLabel(matrix,FXStringFormat("%d Files",num_files),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
    else
      new FXLabel(matrix,FXStringFormat("%d Folders",num_dir),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);

    new FXLabel(matrix,"Size",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_RIGHT|LAYOUT_FILL_X);
    new FXLabel(matrix,FXStringFormat("%llu Bytes",totalsize),NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);

    new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
    new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);

    }

  new FXTabItem(tabbook," Permissions ");
  content = new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK,0,0,0,0);

  matrix = new FXMatrix(content,2,LAYOUT_FILL_X|LAYOUT_FILL_Y|MATRIX_BY_COLUMNS,0,0,0,0);

  new FXLabel(matrix,"Owner",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  ownerlist = new FXListBox(matrix,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|FRAME_THICK|FRAME_SUNKEN);

  new FXLabel(matrix,"Group",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  grouplist = new FXListBox(matrix,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|FRAME_THICK|FRAME_SUNKEN);

  new FXLabel(matrix,"Others",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y);
  otherlist = new FXListBox(matrix,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|FRAME_THICK|FRAME_SUNKEN);

  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);

  if (advanced) {
    new FXFrame(matrix,FRAME_NONE);
    usercheck = new FXCheckButton(matrix,"Set User Id",NULL,0,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
    new FXFrame(matrix,FRAME_NONE);
    groupcheck = new FXCheckButton(matrix,"Set Group Id",NULL,0,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
    new FXFrame(matrix,FRAME_NONE);
    stickycheck = new FXCheckButton(matrix,"Set Sticky Bit",NULL,0,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
    }
  else {



    }
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE);
  new FXSeparator(matrix,LAYOUT_FILL_X|SEPARATOR_GROOVE|LAYOUT_FILL_COLUMN);
  new FXLabel(matrix,"Owner",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_LEFT);
  new FXLabel(matrix,username,NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN);
  new FXLabel(matrix,"Group",NULL,LABEL_NORMAL|LAYOUT_CENTER_Y|JUSTIFY_LEFT);
  new FXLabel(matrix,groupname,NULL,LABEL_NORMAL|JUSTIFY_LEFT|LAYOUT_FILL_COLUMN); 
 //groupidlist = new FXListBox(matrix,NULL,0,LAYOUT_FILL_X|LAYOUT_FILL_COLUMN|FRAME_THICK|FRAME_SUNKEN);
/*
  gid_t groups[100];
  int num = getgroups(100,groups);
  int current=-1;
  for (int i=0;i<num;i++){
    groupidlist->appendItem(FXSystem::groupName(groups[i]),NULL,(void*)(FXival)groups[i]);
    }
  groupidlist->setNumVisible(FXMIN(num,9));

*/
 

/*
  if (type) {
    new FXFrame(matrix,FRAME_NONE);
    new FXCheckButton(matrix,"Only owner can rename or delete\n folder content",&dt_sticky,FXDataTarget::ID_VALUE,JUSTIFY_LEFT|JUSTIFY_TOP|ICON_BEFORE_TEXT|LAYOUT_FILL_COLUMN);
    }
  else {
    new FXFrame(matrix,FRAME_NONE);
    new FXCheckButton(matrix,"Executable",&dt_execute,FXDataTarget::ID_VALUE,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
    new FXFrame(matrix,FRAME_NONE);
    new FXCheckButton(matrix,"Set User ID on Execution",&dt_userid,FXDataTarget::ID_VALUE,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
    new FXFrame(matrix,FRAME_NONE);
    new FXCheckButton(matrix,"Set Group ID on Execution",&dt_groupid,FXDataTarget::ID_VALUE,CHECKBUTTON_NORMAL|LAYOUT_FILL_COLUMN);
    }
*/

  if (!editpermissions) {
    ownerlist->disable();
    grouplist->disable();
    otherlist->disable();
    usercheck->disable();
    groupcheck->disable();
    stickycheck->disable();
    }

//  new FXTabItem(tabbook," Binding ");
//  content = new FXVerticalFrame(tabbook,LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_RAISED|FRAME_THICK,0,0,0,0);
  }

long FXFileProperties::onCmdAccept(FXObject*sender,FXSelector sel,void*ptr){
  bool ownerchanged=(uflags!=(unsigned)ownerlist->getCurrentItem());
  bool groupchanged=(gflags!=(unsigned)grouplist->getCurrentItem());
  bool otherchanged=(oflags!=(unsigned)otherlist->getCurrentItem());
  bool groupidchanged=(groupid!=groupcheck->getCheck());
  bool useridchanged=(userid!=usercheck->getCheck());
  bool stickyidchanged=(stickyid!=stickycheck->getCheck());

  FXuint perm=0;
  FXuint mode=0;
  register FXint i=0;

  if (ownerchanged || groupchanged || otherchanged || useridchanged || groupidchanged || stickyidchanged ) {

    switch(ownerlist->getCurrentItem()){
      case  7: perm|=FXIO::OwnerFull;                     break;
      case  6: perm|=(FXIO::OwnerExec|FXIO::OwnerWrite);  break;
      case  5: perm|=(FXIO::OwnerExec|FXIO::OwnerRead);   break;
      case  4: perm|=(FXIO::OwnerWrite|FXIO::OwnerRead);  break;
      case  3: perm|=FXIO::OwnerExec;                     break;
      case  2: perm|=FXIO::OwnerWrite;                    break;
      case  1: perm|=FXIO::OwnerRead;                     break;
      default: break;
      }

    switch(grouplist->getCurrentItem()){
      case  7: perm|=FXIO::GroupFull;                     break;
      case  6: perm|=(FXIO::GroupExec|FXIO::GroupWrite);  break;
      case  5: perm|=(FXIO::GroupExec|FXIO::GroupRead);   break;
      case  4: perm|=(FXIO::GroupWrite|FXIO::GroupRead);  break;
      case  3: perm|=FXIO::GroupExec;                     break;
      case  2: perm|=FXIO::GroupWrite;                    break;
      case  1: perm|=FXIO::GroupRead;                     break;
      default: break;
      }

    switch(otherlist->getCurrentItem()){
      case  7: perm|=FXIO::OtherFull;                     break;
      case  6: perm|=(FXIO::OtherExec|FXIO::OtherWrite);  break;
      case  5: perm|=(FXIO::OtherExec|FXIO::OtherRead);   break;
      case  4: perm|=(FXIO::OtherWrite|FXIO::OtherRead);  break;
      case  3: perm|=FXIO::OtherExec;                     break;
      case  2: perm|=FXIO::OtherWrite;                    break;
      case  1: perm|=FXIO::OtherRead;                     break;
      default: break;
      }

    for (i=0;i<files.no();i++) {
      mode = FXStat::mode(files[i]);
      if (ownerchanged) {
        mode&=~FXIO::OwnerFull;
        }
      if (groupchanged) {
        mode&=~FXIO::GroupFull;
        }
      if (otherchanged) {
        mode&=~FXIO::OtherFull;
        }
      if (groupidchanged){
        mode&=~FXIO::SetGroup;
        if (groupcheck->getCheck()) mode|=FXIO::SetGroup;
        }
      if (useridchanged){
        mode&=~FXIO::SetUser;
        if (usercheck->getCheck()) mode|=FXIO::SetUser;
        }
      if (stickyidchanged){
        mode&=~FXIO::Sticky;
        if (stickycheck->getCheck()) mode|=FXIO::Sticky;
        }      
      mode|=perm;

      if (!FXStat::mode(files[i],mode)) {
        fxmessage("Update Failed\n");
        }
      }

    }
  getApp()->stopModal(this,TRUE);
  hide();
  return 1;
  }





