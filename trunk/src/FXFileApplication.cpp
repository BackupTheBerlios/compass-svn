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

/// For waitpid
#include <sys/types.h>
#include <sys/wait.h>

#include "xincs.h"
#include "common.h"
#include "FXFileManager.h"
#include "FXFileApplication.h"
#include "FXFileDictEx.h"
#include "FXFileView.h"
#include "FXFileProperties.h"
#include "icons.h"

/*

#-------------------------------------
# FOX Desktop Standard File Bindings
#-------------------------------------

[SETTINGS]
iconpath="/usr/share/fox-desktop/icons/default"

[FILETYPES]

#-------------------------
# File and Folders
#-------------------------
defaultfilebinding=";;big/empty.png;small/empty.png"

#-------------------------
# Standard Files
#-------------------------
README="adie;ReadMe File;big/info.png;small/info.png;text/plain"
INSTALL="adie;Installation Instructions;big/info.png;small/info.png;text/plain"
AUTHORS="adie;Author Information;big/info.png;small/info.png;text/plain"
LICENSE="adie;License Information;big/info.png;small/info.png;text/plain"

#-------------------------
# Archives
#-------------------------
tar.bz2=";Bzip2-ed Tar Archive;big/tgz.png;small/tgz.png;application/x-tbz"
tar.gz=";Gzipped Tar Archive;big/tgz.png;small/tgz.png;application/x-tgz"
tgz=";Gzipped Tar Archive;big/tgz.png;small/tgz.png;application/x-tgz"
gz=";Gzip File;big/tgz.png;small/tgz.png;application/x-gzip"
bz2=";Bzip2 File;big/tgz.png;small/tgz.png;application/x-bzip2"
zip=";Zip Archive;big/tgz.png;small/tgz.png;application/x-zip"

#-------------------------
# System Related
#-------------------------
o=";Object Code;big/binary.png;small/binary.png;application/x-object"

#-------------------------
# Text
#-------------------------
txt="adie;Plain Text Document;big/txt.png;small/txt.png;text/plain"
html="firefox;HTML Document;big/html.png;small/html.png;text/html"
htm="firefox;HTML Document;big/html.png;small/html.png;text/html"

cpp="adie;C++ Source File;big/source_cpp.png;small/source_cpp.png;text/x-c++src"
cxx="adie;C++ Source File;big/source_cpp.png;small/source_cpp.png;text/x-c++src"
cc="adie;C++ Source File;big/source_cpp.png;small/source_cpp.png;text/x-c++src"
C="adie;C++ Source File;big/source_cpp.png;small/source_cpp.png;text/x-c++src"

c="adie;C Source File;big/source_c.png;small/source_c.png;text/x-csrc"
h="adie;C/C++ Header File;big/source_h.png;small/source_h.png;text/x-chdr"

#-------------------------
# Audio Types
#-------------------------
aac=";AAC Sound;big/sound.png;small/sound.png;audio/aac"
ac3=";Dolby AC3 Sound;big/sound.png;small/sound.png;audio/ac3"
flac="gmm;FLAC Audio;big/sound.png;small/sound.png;audio/x-flac"
mp3="gmm;MPEG Layer 3 Audio;big/sound.png;small/sound.png;audio/x-mp3"
ogg="gmm;Ogg Vorbis Audio;big/sound.png;small/sound.png;application/ogg"
wav=";WAV Audio;big/sound.png;small/sound.png;audio/x-wav"

#-------------------------
# Image Types
#-------------------------
bmp=";BMP Image;big/image.png;small/image.png;image/x-bmp"
gif=";GIF Image;big/image.png;small/image.png;image/gif"
ico=";Windows Icon;big/image.png;small/image.png;image/x-ico"
jpeg=";JPEG Image;big/image.png;small/image.png;image/jpeg"
jpg=";JPEG Image;big/image.png;small/image.png;image/jpg"
pcx=";PCX Image;big/image.png;small/image.png;image/x-pcx"
png=";PNG Image;big/image.png;small/image.png;image/png"
rgb=";SGI Image;big/image.png;small/image.png;image/x-rgb"
tga=";TGA Image;big/image.png;small/image.png;image/x-targa"
tiff=";TIFF Image;big/image.png;small/image.png;image/tiff"
xbm=";X Bitmap Image;big/image.png;small/image.png;image/x-xbm"

#-------------------------
# Video Types
#-------------------------
avi=";Microsoft AVI Video;big/video.png;small/video.png;video/avi"
wmv="mplayer;Windows Media Video;big/video.png;small/video.png;video/wmv"
mp4="mplayer;MPEG-4 Video;big/video.png;small/video.png;video/mp4"
mpeg="mplayer;MPEG Video;big/video.png;small/video.png;video/mpeg"
mpg="mplayer;MPEG Video;big/video.png;small/video.png;video/mpeg"
mov="mplayer;Quicktime Video;big/video.png;small/video.png;video/quicktime"
qt="mplayer;Quicktime Video;big/video.png;small/video.png;video/quicktime"

#-------------------------
# Office Documents
#-------------------------
doc=";Microsoft Word Document;big/wordprocessing.png;small/wordprocessing.png;application/vnd.ms-word"
xls=";Microsoft Excel Spreadsheet;big/spreadsheet.png;small/spreadsheet.png;application/vnd.ms-excel"
ppt=";Microsoft PowerPoint Presentation;big/presentation.png;small/presentation.png;application/vnd.ms-powerpoint"
wpd=";WordPerfect Document;big/wordprocessing.png;small/wordprocessing.png;application/wordperfect"

odc=";OpenDocument Chart;;;application/vnd.oasis.opendocument.chart"
odf=";OpenDocument Formula;;;application/vnd.oasis.opendocument.formula"
odg=";OpenDocument Graphics;;;application/vnd.oasis.opendocument.graphics"
odi=";OpenDocument Image;;;application/vnd.oasis.opendocument.image"
odp=";OpenDocument Presentation;;;application/vnd.oasis.opendocument.presentation"
ods=";OpenDocument SpreadSheet;;;application/vnd.oasis.opendocument.spreadsheet"
odt=";OpenDocument Text;;;application/vnd.oasis.opendocument.text"
otg=";OpenDocument Graphics Template;;;application/vnd.oasis.opendocument.graphics-template"
otp=";OpenDocument Presentation Template;;;application/vnd.oasis.opendocument.presentation-template"
ots=";OpenDocument SpreadSheet Template;;;application/vnd.oasis.opendocument.spreadsheet-template"
ott=";OpenDocument Text Template;;;application/vnd.oasis.opendocument.text-template"

odb=";OpenOffice.org Database;;;application/vnd.sun.xml.base"
sxc=";OpenOffice.org Spreadsheet;;;application/vnd.sun.xml.calc"
stc=";OpenOffice.org Spreadsheet Template;;;application/vnd.sun.xml.calc.template"
sxd=";OpenOffice.org Drawing;;;application/vnd.sun.xml.draw"
std=";OpenOffice.org Drawing Template;;;application/vnd.sun.xml.draw.template"
sxi=";OpenOffice.org Presentation;;;application/vnd.sun.xml.impress"
sti=";OpenOffice.org Presentation Template;;;application/vnd.sun.xml.impress.template"
sxw=";OpenOffice.org Text Document;;;application/vnd.sun.xml.writer"
sxg=";OpenOffice.org Master Text Document;;;application/vnd.sun.xml.writer.master"
stw=";OpenOffice.org Text Document Template;;;application/vnd.sun.xml.writer.template"

pdf="xpdf;PDF Document;big/pdf.png;small/pdf.png;application/pdf"
ps=";PostScript Document;big/postscript.png;small/postscript.png;application/ps"
*/


struct IconTheme {
  const char * themename;
  const char * iconpath;     
  const char * big;         
  const char * small;       
  const char * executable;  
  const char * folder;      
  const char * folder_open; 
  const char * folder_home;   
  const char * folder_desktop; 
  const char * file;
  const char * audio;
  const char * archive;
  const char * image;
  const char * video;
  const char * document;
  const char * spreadsheet;
  const char * presentation;
  const char * text;
  const char * www;
  FXbool       available;
  };        


static IconTheme iconthemes[]={ 

  // The Default Theme
  { 
    "Default", /// Theme Name
    NULL, /// Icon Path
    NULL, /// Big
    NULL, /// Small
    NULL, /// Executable
    NULL, /// Folder 
    NULL, /// Folder Open
    NULL, /// Folder Home
    NULL, /// Folder Desktop
    NULL, /// File
    NULL, /// Audio Type
    NULL, /// Archive
    NULL, /// Image
    NULL, /// Video
    NULL, /// Office Document
    NULL, /// Office Spreadsheet
    NULL, /// Office Presentation
    NULL, /// Text Files
    NULL, /// WWW  
    true,
  },

  // The GNOME theme  
  {
    
    "Gnome FC5", /// Theme Name
    "/usr/share/icons/gnome", /// Icon Path
    "48x48/", /// Big
    "16x16/", /// Small
    "mimetypes/gnome-mime-application-x-executable.png", /// Executable
    "filesystems/gnome-fs-directory.png", /// Folder 
    NULL, /// Folder Open
    "filesystems/gnome-fs-home.png", /// Folder Home
    "filesystems/gnome-fs-directory.png", /// Folder Desktop
    "filesystems/gnome-fs-regular.png", /// File
    NULL, /// Audio File
    NULL, /// Archive
    NULL, /// Image
    NULL, /// Video
    NULL, /// Office Document
    NULL, /// Office Spreadsheet
    NULL, /// Office Presentation
    NULL, /// Text Files
    NULL, /// WWW   
    false,
  },


  // The GNOME theme  
  {
    
    "Gnome", /// Theme Name
    "/usr/share/icons/gnome", /// Icon Path
    "32x32/", /// Big
    "16x16/", /// Small
    "mimetypes/gnome-mime-application-x-executable.png", /// Executable
    "places/gnome-fs-directory.png", /// Folder 
    NULL, /// Folder Open
    "places/gnome-fs-home.png", /// Folder Home
    "places/gnome-fs-directory.png", /// Folder Desktop
    "mimetypes/text-x-generic.png", /// File
    NULL, /// Audio File
    NULL, /// Archive
    NULL, /// Image
    NULL, /// Video
    NULL, /// Office Document
    NULL, /// Office Spreadsheet
    NULL, /// Office Presentation
    NULL, /// Text Files
    NULL, /// WWW
    false,   
  },


  // Tango
  {
    "Tango", /// Theme Name
    "/usr/share/icons/Tango", /// Icon Path
    "32x32/", /// Big
    "16x16/", /// Small
    "mimetypes/application-x-executable.png", /// Executable
    "places/folder.png", /// Folder 
    "status/folder-open.png", /// Folder Open
    "places/user-home.png", /// Folder Home
    "places/user-desktop.png", /// Folder Desktop
    "mimetypes/text-x-generic-template.png", /// File
    "mimetypes/audio-x-generic.png", /// Audio File
    "mimetypes/package-x-generic.png", /// Archive File 
    "mimetypes/image-x-generic.png", /// Image
    "mimetypes/video-x-generic.png", /// Video
    "mimetypes/x-office-document.png", /// Office Document
    "mimetypes/x-office-spreadsheet.png", /// Office Spreadsheet
    "mimetypes/x-office-presentation.png", /// Office Presentation
    "mimetypes/text-x-generic.png", /// Text Files
    "mimetypes/text-html.png", /// WWW

    false,   
  },


  // KDE Classic
  {
    "KDE Classic", /// Theme Name
    "/usr/share/icons/kdeclassic", /// Icon Path
    "32x32/", /// Big
    "16x16/", /// Small
    "filesystems/exec.png", /// Executable
    "filesystems/folder.png", /// Folder 
    "filesystems/folder_open.png", /// Folder Open
    "filesystems/folder_home.png", /// Folder Home
    "filesystems/desktop.png", /// Folder Desktop
    "mimetypes/empty.png", /// File
    NULL, /// Audio File
    NULL, /// Archive
    NULL, /// Image
    NULL, /// Video
    NULL, /// Office Document
    NULL, /// Office Spreadsheet
    NULL, /// Office Presentation
    NULL, /// Text Files
    NULL, /// WWW
    false,   
  },

  // Crystal SVG
  {
    "Crystal SVG", /// Theme Name
    "/usr/share/icons/crystalsvg", /// Icon Path
    "32x32/", /// Big
    "16x16/", /// Small
    "filesystems/exec.png", /// Executable
    "filesystems/folder.png", /// Folder 
    "filesystems/folder_open.png", /// Folder Open
    "filesystems/folder_home.png", /// Folder Home
    "filesystems/desktop.png", /// Folder Desktop
    "mimetypes/empty.png", /// File
    NULL, /// Audio File
    NULL, /// Archive
    NULL, /// Image
    NULL, /// Video
    NULL, /// Office Document
    NULL, /// Office Spreadsheet
    NULL, /// Office Presentation
    NULL, /// Text Files
    NULL, /// WWW
    false, 
  },

  // KIDS
  {
    "Kids", /// Theme Name
    "/usr/share/icons/kids", /// Icon Path
    "32x32/", /// Big
    "16x16/", /// Small
    "filesystems/exec.png", /// Executable
    "filesystems/folder.png", /// Folder 
    NULL, /// Folder Open
    "filesystems/folder_home.png", /// Folder Home
    "filesystems/desktop.png", /// Folder Desktop
    "mimetypes/empty.png",/// File   
    NULL, /// Audio File
    NULL, /// Archive
    NULL, /// Image
    NULL, /// Video
    NULL, /// Office Document
    NULL, /// Office Spreadsheet
    NULL, /// Office Presentation
    NULL, /// Text Files
    NULL, /// WWW
    false, 
  }

};


inline static void assoc_exact(FXFileDict * dict,const IconTheme & theme,const char * key,const char * description,const char * mime,const char * icon) {
  FXString value;
  if (icon)
    value = FXStringFormat(";%s;%s%s;%s%s;%s",description,theme.big,icon,theme.small,icon,mime);
  else
    value = FXStringFormat(";%s;;;%s",description,mime);
  dict->replace(key,value.text());
  }

inline static void assoc_exact(FXFileDict * dict,const IconTheme & theme,const char * key,const char * description,const char * mime,const char * icon,const char * icon_open) {
  FXString value;
  if (icon && icon_open)
    value = FXStringFormat(";%s;%s%s:%s%s;%s%s:%s%s;%s",description,theme.big,icon,theme.big,icon_open,theme.small,icon,theme.small,icon_open,mime);
  else if (icon)
    value = FXStringFormat(";%s;%s%s;%s%s;%s",description,theme.big,icon,theme.small,icon,mime);
  else if (icon_open)
    value = FXStringFormat(";%s;%s%s;%s%s;%s",description,theme.big,icon_open,theme.small,icon_open,mime);
  else
    value = FXStringFormat(";%s;;;%s",description,mime);
  dict->replace(key,value.text());
  }

inline static void assoc(FXFileDict * dict,const IconTheme & theme,const char * key,const char * description,const char * mime,const char * icon) {
  FXString value;
  if (icon)
    value = FXStringFormat(";%s;%s%s;%s%s;%s",description,theme.big,icon,theme.small,icon,mime);
  else
    value = FXStringFormat(";%s;;;%s",description,mime);
  dict->replace(key,value.text());
  dict->replace(FXString(key).upper().text(),value.text());
  }

inline static void assoc(FXFileDict * dict,const IconTheme & theme,const char * key,const char * description,const char * mime,const char * icon,const char * icon_open) {
  FXString value;
  if (icon && icon_open)
    value = FXStringFormat(";%s;%s%s:%s%s;%s%s:%s%s;%s",description,theme.big,icon,theme.big,icon_open,theme.small,icon,theme.small,icon_open,mime);
  else if (icon)
    value = FXStringFormat(";%s;%s%s;%s%s;%s",description,theme.big,icon,theme.small,icon,mime);
  else if (icon_open)
    value = FXStringFormat(";%s;%s%s;%s%s;%s",description,theme.big,icon_open,theme.small,icon_open,mime);
  else
    value = FXStringFormat(";%s;;;%s",description,mime);
  dict->replace(key,value.text());
  dict->replace(FXString(key).upper().text(),value.text());
  }


void initAssociations(FXFileDict * dict, const IconTheme & theme) {
  const char * icon;

  dict->getIconDict()->clear();

  dict->setIconPath(theme.iconpath);

  assoc_exact(dict,theme,FXFileDict::defaultExecBinding,"Executable","",theme.executable);
  if (theme.folder_open)
    assoc_exact(dict,theme,FXFileDict::defaultDirBinding,"Folder","",theme.folder,theme.folder_open);
  else
    assoc_exact(dict,theme,FXFileDict::defaultDirBinding,"Folder","",theme.folder);

  assoc_exact(dict,theme,FXFileDict::defaultFileBinding,"","",theme.file);
  assoc_exact(dict,theme,FXSystem::getHomeDirectory().text(),"","",theme.folder_home);
  assoc_exact(dict,theme,FXString(FXSystem::getHomeDirectory()+PATHSEPSTRING+"Desktop").text(),"","",theme.folder_desktop);

  /// Audio Files
  icon = (theme.audio) ? theme.audio : theme.file;
  assoc(dict,theme,"ogg","Ogg Vorbis","",theme.audio);
  assoc(dict,theme,"mp3","Mpeg Layer 3","",theme.audio);
  assoc(dict,theme,"mp4","Audio","",theme.audio);
  assoc(dict,theme,"m4a","Audio","",theme.audio);
  assoc(dict,theme,"aac","Audio","",theme.audio);
  assoc(dict,theme,"ac3","Audio","",theme.audio);
  assoc(dict,theme,"mpc","MusePack","",theme.audio);
  assoc(dict,theme,"flac","Lossless Audio","",theme.audio);
  assoc(dict,theme,"wav","Wav Audio","",theme.audio);

  /// Archive Files
  icon = (theme.archive) ? theme.archive : theme.file;
  assoc(dict,theme,"tar","Archive","",icon);
  assoc(dict,theme,"tar.gz","Archive (gzip)","application/x-tgz",icon);
  assoc(dict,theme,"tar.bz2","Archive (bzip2)","application/x-tbz",icon);
  assoc(dict,theme,"tgz","Archive (gzip)","application/x-tgz",icon);
  assoc(dict,theme,"bz2","Archive (bzip2)","application/x-tbz",icon);
  assoc(dict,theme,"zip","Archive (zip)","application/x-zip",icon);

  /// Image Files
  icon = (theme.image) ? theme.image : theme.file;
  assoc(dict,theme,"bmp","Image","image/x-bmp",icon);
  assoc(dict,theme,"gif","Image","image/gif",icon);
  assoc(dict,theme,"ico","Icon","image/x-ico",icon);
  assoc(dict,theme,"jpeg","Image","image/jpeg",icon);
  assoc(dict,theme,"jpg","Image","image/jpg",icon);
  assoc(dict,theme,"pcx","Image","image/x-pcx",icon);
  assoc(dict,theme,"png","Image","image/png",icon);
  assoc(dict,theme,"rgb","Image","image/x-rgb",icon);
  assoc(dict,theme,"tga","Image","image/x-targa",icon);
  assoc(dict,theme,"tiff","Image","image/tiff",icon);
  assoc(dict,theme,"xbm","Image","image/x-xbm",icon);

  /// Video Files
  icon = (theme.video) ? theme.video : theme.file;
  assoc(dict,theme,"avi","Video","video/avi",icon);
  assoc(dict,theme,"qt","Video","video/quicktime",icon);
  assoc(dict,theme,"mov","Video","video/quicktime",icon);
  assoc(dict,theme,"mpg","Video","video/mpeg",icon);
  assoc(dict,theme,"mpeg","Video","video/mpeg",icon);
  assoc(dict,theme,"m4v","Video","video/mp4",icon);
  assoc(dict,theme,"wmv","Video","video/wmv",icon);

  /// Office Documents
  icon = (theme.document) ? theme.document : theme.file;
  assoc(dict,theme,"doc","Word Document","application/vnd.ms-word",icon);
  assoc(dict,theme,"pdf","PDF Document","application/pdf",icon);
  assoc(dict,theme,"ps","Post Script Document","application/ps",icon);
  assoc(dict,theme,"wpd","Word Perfect Document","application/wordperfect",icon);
  assoc(dict,theme,"sxw","Open Office Text","application/vnd.sun.xml.write",icon);
  assoc(dict,theme,"odt","Open Document Text","application/vnd.oasis.opendocument.text",icon);

  /// Office Spreadsheet
  icon = (theme.spreadsheet) ? theme.spreadsheet : theme.file;
  assoc(dict,theme,"xls","Excel Spreadsheet","application/vnd.ms-excel",icon);
  assoc(dict,theme,"sxc","Open Office Spreadsheet","application/vnd.sun.xml.calc",icon);
  assoc(dict,theme,"ods","Open Document Spreadsheet","application/vnd.oasis.opendocument.spreadsheet",icon);

  /// Office Presentation
  icon = (theme.presentation) ? theme.presentation : theme.file;
  assoc(dict,theme,"ppt","PowerPoint Presentation","application/vnd.ms-powerpoint",icon);
  assoc(dict,theme,"sxi","Open Office Presentation","application/vnd.sun.xml.impress",icon);
  assoc(dict,theme,"otp","Open Document Presentation","application/vnd.oasis.opendocument.presentation",icon);

  /// Text Files
  icon = (theme.text) ? theme.text : theme.file;
  assoc(dict,theme,"readme","Read Me","text/plain",icon);
  assoc(dict,theme,"cpp","C++ Source","text/x-c++src",icon);
  assoc(dict,theme,"cxx","C++ Source","text/x-c++src",icon);
  assoc(dict,theme,"cc","C++ Source","text/x-c++src",icon);
  assoc(dict,theme,"c","C Source","text/x-c++src",icon);
  assoc(dict,theme,"h","C/C++ Header","text/x-chdr",icon);
  assoc(dict,theme,"hxx","C/C++ Header","text/x-chdr",icon);


  assoc(dict,theme,"o","Object","",icon);
  assoc(dict,theme,"obj","Object","",icon);


/*
README="adie;ReadMe File;big/info.png;small/info.png;text/plain"
INSTALL="adie;Installation Instructions;big/info.png;small/info.png;text/plain"
AUTHORS="adie;Author Information;big/info.png;small/info.png;text/plain"
LICENSE="adie;License Information;big/info.png;small/info.png;text/plain"
*/
  /// Internet Related
  icon = (theme.www) ? theme.www : theme.file;
  assoc(dict,theme,"html","HTML Document","text/html",icon);
  assoc(dict,theme,"htm","HTML Document","text/html",icon);







  }

/*

void init_theme(IconTheme & themes[]) {
  for (FXint i=0;i<ARRAYNUMBER(themes);i++){
    if (themes[i].folder && themes[i].file && themes[i].big && themes[i].small && themes[i].iconpath) {
      if (FXStat::exists(themes[i].iconpath) && FXStat::exists(FXStri



      }
    }
  }
*/














FXDEFMAP(FXFileApplication) FXFileApplicationMap[]={
  FXMAPFUNCS(SEL_COMMAND,FXFileApplication::ID_ICON_THEME_1,FXFileApplication::ID_ICON_THEME_7,FXFileApplication::onCmdIconTheme),
  FXMAPFUNCS(SEL_UPDATE, FXFileApplication::ID_ICON_THEME_1,FXFileApplication::ID_ICON_THEME_7,FXFileApplication::onUpdIconTheme),
  FXMAPFUNC(SEL_COMMAND, FXFileApplication::ID_NEW_WINDOW,FXFileApplication::onCmdNewWindow),
  FXMAPFUNC(SEL_SIGNAL,  FXFileApplication::ID_CHILD,FXFileApplication::onChildSignal),
  FXMAPFUNC(SEL_IO_READ, FXFileApplication::ID_DDE,FXFileApplication::onDDE),
  };


FXIMPLEMENT(FXFileApplication,FXApp,FXFileApplicationMap,ARRAYNUMBER(FXFileApplicationMap));

FXFileApplication * FXFileApplication::me=NULL;


FXFileApplication::FXFileApplication() : FXApp("fxfilemanager"), icontheme(0),fileproperties(NULL) {
  me=this;
  }

void FXFileApplication::init(int& argc,char** argv,FXbool connect){
  FXApp::init(argc,argv,TRUE);
  createIcons();

  new FXToolTip(this);

  associations = new FXFileDictEx(this);

  kde_clipboard=registerDragType("application/x-kde-cutselection");
  gnome_clipboard=registerDragType("x-special/gnome-copied-files");
  gnome_dragndrop=registerDragType("x-special/gnome-icon-list");

  /// Create Title Font
  FXFontDesc fontdescription;
  getNormalFont()->getFontDesc(fontdescription);
  fontdescription.weight = FXFont::Bold;
  font_title = new FXFont(this,fontdescription);
  font_title->create();

  /// Close Connection on Exec
  fcntl(ConnectionNumber((Display*)getDisplay()),F_SETFD,FD_CLOEXEC);

  /// Register SIGNAL handler
  addSignal(SIGCHLD,this,ID_CHILD);

#if FOX_MINOR == 6
  previewopenmode = (FXOpenMode) reg().readUnsignedEntry("settings","preview-open-mode",OPEN_IN_CURRENT);
  diropenmode     = (FXOpenMode) reg().readUnsignedEntry("settings","directory-open-mode",OPEN_IN_CURRENT);
#else
  previewopenmode = (FXOpenMode) reg().readUIntEntry("settings","preview-open-mode",OPEN_IN_CURRENT);
  diropenmode     = (FXOpenMode) reg().readUIntEntry("settings","directory-open-mode",OPEN_IN_CURRENT);
#endif

  for (FXint i=1;i<ARRAYNUMBER(iconthemes);i++){
    if (FXStat::exists(FXString(iconthemes[i].iconpath)+PATHSEPSTRING+FXString(iconthemes[i].big)+FXString(iconthemes[i].folder)))
      iconthemes[i].available=true;
    } 
  //initAssociations(associations,iconthemes[2]);
  }


FXFileApplication::~FXFileApplication(){
  destroyIcons();
  removeSignal(SIGCHLD);
  }

    




/*
 GNOME
  const FXchar * big            = "48x48/mimetypes/";
  const FXchar * small          = "16x16/mimetypes/";
  const FXchar * bigfs          = "48x48/filesystems/";
  const FXchar * smallfs        = "16x16/filesystems/";

  const FXchar * mime_image     = "gnome-mime-image.png";
  const FXchar * mime_text      = "gnome-mime-text.png";
  const FXchar * mime_text_html = "gnome-mime-text-html.png";

  const FXchar * mime_text_c      = "gnome-mime-text-x-csrc.png";
  const FXchar * mime_text_cpp    = "gnome-mime-text-x-c++src.png";
  const FXchar * mime_text_h      = "gnome-mime-text-x-c-header.png";
  const FXchar * mime_text_copy   = "gnome-mime-text-x-copying.png";
  const FXchar * mime_text_readme = "gnome-mime-text-x-readme.png";
  const FXchar * mime_app_object  = "gnome-mime-application-x-object.png";

  const FXchar * default_exec     = "gnome-mime-application-x-executable.png";
  const FXchar * default_file     = "gnome-fs-regular.png";
  const FXchar * default_dir      = "gnome-fs-directory.png";
  const FXchar * home_dir         = "gnome-fs-home.png";
  const FXchar * desktop_dir      = "gnome-fs-desktop.png";


  associations->setIconPath("/usr/share/icons/gnome");
*/




  /* TANGO */
/*
  const FXchar * big            = "32x32/";
  const FXchar * small          = "16x16/";

  const FXchar * mime_image     = "mimetypes/image-x-generic.png";
  const FXchar * mime_text      = "mimetypes/text-x-generic.png";
  const FXchar * mime_text_html = "mimetypes/text-html.png";

  const FXchar * mime_text_c      = "mimetypes/text-x-generic.png";
  const FXchar * mime_text_cpp    = "mimetypes/text-x-generic.png";
  const FXchar * mime_text_h      = "mimetypes/text-x-generic.png";
  const FXchar * mime_text_copy   = "mimetypes/text-x-generic.png";
  const FXchar * mime_text_readme = "mimetypes/text-x-generic.png";
  const FXchar * mime_app_object  = "mimetypes/text-x-generic.png";

  const FXchar * default_exec     = "mimetypes/application-x-executable.png";
  const FXchar * default_file     = "mimetypes/text-x-generic.png";
  const FXchar * default_dir      = "places/folder.png";
  const FXchar * home_dir         = "places/user-home.png";
  const FXchar * desktop_dir      = "places/user-desktop.png";


  associations->setIconPath("/usr/share/icons/Tango");
*/

/* KDE classic */
/*
  const FXchar * big            = "32x32/";
  const FXchar * small          = "16x16/";

  const FXchar * mime_image     = "mimetypes/image.png";
  const FXchar * mime_text      = "mimetypes/txt.png";
  const FXchar * mime_text_html = "mimetypes/html.png";

  const FXchar * mime_text_c      = "mimetypes/source_c.png";
  const FXchar * mime_text_cpp    = "mimetypes/source_cpp.png";
  const FXchar * mime_text_h      = "mimetypes/source_h.png";
  const FXchar * mime_text_copy   = "mimetypes/info.png";
  const FXchar * mime_text_readme = "mimetypes/readme.png";
  const FXchar * mime_app_object  = "mimetypes/source_o.png";

  const FXchar * default_exec     = "filesystems/exec.png";
  const FXchar * default_file     = "mimetypes/empty.png";
  const FXchar * default_dir      = "filesystems/folder.png";
  const FXchar * default_dir_open = "filesystems/folder_open.png";
  const FXchar * home_dir         = "filesystems/folder_home.png";
  const FXchar * desktop_dir      = "filesystems/desktop.png";


  associations->setIconPath("/usr/share/icons/kdeclassic");
*/
#if 0

  const FXchar * big            = "32x32/";
  const FXchar * small          = "16x16/";

  const FXchar * mime_image     = "mimetypes/image.png";
  const FXchar * mime_text      = "mimetypes/txt.png";
  const FXchar * mime_text_html = "mimetypes/html.png";

  const FXchar * mime_text_c      = "mimetypes/source_c.png";
  const FXchar * mime_text_cpp    = "mimetypes/source_cpp.png";
  const FXchar * mime_text_h      = "mimetypes/source_h.png";
  const FXchar * mime_text_copy   = "mimetypes/info.png";
  const FXchar * mime_text_readme = "mimetypes/readme.png";
  const FXchar * mime_app_object  = "mimetypes/source_o.png";

  const FXchar * default_exec     = "filesystems/exec.png";
  const FXchar * default_file     = "mimetypes/empty.png";
  const FXchar * default_dir      = "filesystems/folder.png";
  const FXchar * default_dir_open = "filesystems/folder_open.png";
  const FXchar * home_dir         = "filesystems/folder_home.png";
  const FXchar * desktop_dir      = "filesystems/desktop.png";

  associations->setIconPath("/usr/share/icons/crystalsvg");



  makeAssociation(FXFileDict::defaultExecBinding,"","",default_exec,big,small);
  makeAssociation(FXFileDict::defaultDirBinding,"","",default_dir,default_dir_open,big,small);
  makeAssociation(FXFileDict::defaultFileBinding,"","",default_file,big,small);
  makeAssociation(FXSystem::getHomeDirectory().text(),"","",home_dir,big,small);
  makeAssociation(FXString(FXSystem::getHomeDirectory()+PATHSEPSTRING+"Desktop").text(),"","",desktop_dir,big,small);


  makeAssociation("htm","HTML File","text/html",mime_text_html,big,small);
  makeAssociation("html","HTML File","text/html",mime_text_html,big,small);
  makeAssociation("txt","Text File","text/plain",mime_text,big,small);

  makeAssociation("c","C Source","text/plain",mime_text_c,big,small);
  makeAssociation("cpp","C++ Source","text/plain",mime_text_cpp,big,small);
  makeAssociation("cc","C++ Source","text/plain",mime_text_cpp,big,small);
  makeAssociation("h","Header File","text/plain",mime_text_h,big,small);

  makeAssociation("copying","License","text/plain",mime_text_copy,big,small);
  makeAssociation("readme","Read Me","text/plain",mime_text_copy,big,small);

  makeAssociation("o","Object File","",mime_app_object,big,small);
  makeAssociation("obj","Object File","",mime_app_object,big,small);

  makeAssociation("bmp","BMP Image","image/bmp",mime_image,big,small);
  makeAssociation("gif","BMP Image","image/gif",mime_image,big,small);
  makeAssociation("jpg","JPEG Image","image/jpeg",mime_image,big,small);
  makeAssociation("jpeg","JPEG Image","image/jpeg",mime_image,big,small);
  makeAssociation("jpe","JPEG Image","image/jpeg",mime_image,big,small);
  makeAssociation("pbm","PBM Image","image/x-portable-bitmap",big,big,small);
  makeAssociation("pgm","PGM Image","image/x-portable-graymap",big,big,small);
  makeAssociation("png","PNG Image","image/png",mime_image,big,small);
  makeAssociation("pnm","PNM Image","image/x-portable-anymap",mime_image,big,small);
  makeAssociation("ppm","PPM Image","image/x-portable-pixmap",mime_image,big,small);
  makeAssociation("rgb","RGB Image","image/x-rgb",mime_image,big,small);
  makeAssociation("svg","SVG Image","image/png",mime_image,big,small);
  makeAssociation("tiff","TIFF Image","image/x-portable-anymap",mime_image,big,small);
  makeAssociation("xbm","XBM Image","image/x-xbitmap",mime_image,big,small);
  makeAssociation("xpm","XPM Image","image/x-xpixmap",mime_image,big,small);








  }

#endif

void FXFileApplication::registerDDE(FXFile * in) {
  dde = in;
  addInput(dde->handle(),INPUT_READ,this,ID_DDE);
  }


void FXFileApplication::createIcons(){
	icon_back = new FXPNGIcon(this,back_png);
	icon_back->blend(getBaseColor());
	icon_forward = new FXPNGIcon(this,forward_png);
	icon_forward->blend(getBaseColor());
	icon_home = new FXPNGIcon(this,gohome_png);
	icon_home->blend(getBaseColor());
	icon_up = new FXPNGIcon(this,up_png);
	icon_up->blend(getBaseColor());


  icon_settings = new FXPNGIcon(this,settings_png);
  icon_settings->blend(getBaseColor());

	icon_back_small = new FXPNGIcon(this,back_small_png);
	icon_back_small->blend(getBaseColor());
  icon_back_small->create();
	icon_forward_small = new FXPNGIcon(this,forward_small_png);
	icon_forward_small->blend(getBaseColor());
  icon_forward_small->create();
	icon_home_small = new FXPNGIcon(this,gohome_small_png);
	icon_home_small->blend(getBaseColor());
  icon_home_small->create();
	icon_up_small = new FXPNGIcon(this,up_small_png);
	icon_up_small->blend(getBaseColor());
  icon_up_small->create();


  icon_tab_new = new FXPNGIcon(this,tab_new_png);
 	icon_tab_new->blend(getBaseColor());
  icon_tab_new->create();

  icon_tab_duplicate = new FXPNGIcon(this,tab_duplicate_png);
 	icon_tab_duplicate->blend(getBaseColor());
  icon_tab_duplicate->create();

  icon_tab_close  = new FXPNGIcon(this,tab_remove_png);
	icon_tab_close->blend(getBaseColor());
  icon_tab_close->create();

  icon_tab_detach = new FXPNGIcon(this,tab_breakoff_png);
	icon_tab_detach->blend(getBaseColor());
  icon_tab_detach->create();


  icon_about = new FXPNGIcon(this,compass_png);
	icon_about->blend(FXRGB(255,255,255));
  icon_about->create();

	icon_exit = new FXPNGIcon(this,exit_png);
	icon_exit->blend(getBaseColor());
	icon_new = new FXPNGIcon(this,window_new_png);
	icon_new->blend(getBaseColor());

	icon_clear = new FXPNGIcon(this,clear_right_png);
	icon_clear->blend(getBaseColor());

	icon_document = new FXGIFIcon(this,minidoc_gif);
	icon_folder = new FXGIFIcon(this,minifolder_gif);
	icon_folder_big = new FXGIFIcon(this,bigfolder_gif);
  icon_folder_open_big = new FXGIFIcon(this,bigfolderopen_gif);
	icon_document->create();
	icon_folder->create();
  icon_folder_open_big->create();

  icon_info = new FXPNGIcon(this,info_png);
	icon_info->blend(FXRGB(255,255,255));
  icon_info->create();

  icon_critical = new FXPNGIcon(this,critical_png);
	icon_critical->blend(FXRGB(255,255,255));
  icon_critical->create();

  icon_warning  = new FXPNGIcon(this,warning_png);
	icon_warning->blend(FXRGB(255,255,255));
  icon_warning->create();

  icon_lock = new FXPNGIcon(this,lock_png);
	icon_lock->blend(FXRGB(255,255,255));
  icon_warning->create();

	icon_copy = new FXGIFIcon(this,copy_gif,0,IMAGE_ALPHAGUESS);
	icon_cut = new FXGIFIcon(this,cut_gif,0,IMAGE_ALPHAGUESS);
	icon_paste = new FXGIFIcon(this,paste_gif,0,IMAGE_ALPHAGUESS);
  icon_delete = new FXGIFIcon(this,delete_gif,0,IMAGE_ALPHAGUESS);

  
  }

void FXFileApplication::destroyIcons(){
  delete icon_back;
  delete icon_forward;
  delete icon_home;
  delete icon_exit;
  delete icon_clear;
  delete icon_document;
  delete icon_folder;
	delete icon_folder_big;
  delete icon_folder_open_big;
  delete icon_up;
  delete icon_new;
  delete icon_copy;
  delete icon_cut;
  delete icon_paste;
  delete icon_delete;
  delete icon_back_small;
  delete icon_forward_small;
  delete icon_home_small;
  delete icon_up_small;
  delete icon_settings;
  delete icon_lock;
  }

void FXFileApplication::createWindow(FXbool checkargs){
  FXString url=FXSystem::getCurrentDirectory();
  FXFileManager * filemanager = new FXFileManager(this);
  filemanager->create();
  url = FXSystem::getCurrentDirectory();
  if (checkargs && getArgc()>1) {
    FXString path = getArgv()[1];
    path=FXPath::expand(path);
    path=FXPath::absolute(path);

    if (FXStat::exists(path)){
      url = path;
      }
    }
  filemanager->view(url,FROM_APPLICATION);
  filemanager->show();
  }


void FXFileApplication::createWindow(FXString url){
  FXFileManager * filemanager = new FXFileManager(this);
  filemanager->create();
  if (url.empty()) {
    url = FXSystem::getCurrentDirectory();
    }
  filemanager->view(url,FROM_APPLICATION);
  filemanager->show();
  }


void FXFileApplication::createWindow(FXFileView * fileview){
  FXFileManager * filemanager = new FXFileManager(this,fileview);
  filemanager->create();
  filemanager->show();
  }


void FXFileApplication::createWindow(FXTabBook * tabbook){
  FXFileManager * filemanager = new FXFileManager(this);
  filemanager->create();
  filemanager->duplicateTabBook(tabbook);
  filemanager->show();
  }


void FXFileApplication::showFileProperties(const FXString & url,const FXStringList & files) {
  if (fileproperties==NULL) {
    fileproperties = new FXFileProperties(this,url,files,true);
    }
  fileproperties->update(url,files,true);
  fileproperties->create();
  fileproperties->recalc();
  if (!fileproperties->shown()) fileproperties->show(PLACEMENT_SCREEN);
  }

FXbool FXFileApplication::filePropertiesShown() const {
  return (fileproperties && fileproperties->shown());
  }

long FXFileApplication::onDDE(FXObject*,FXSelector,void*){
  FXString url;
  FXchar buffer[255];
  int nread=dde->readBlock(buffer,255);
  if (nread>0) {
    url=FXString(buffer,nread);
    fxmessage("compass.dde: %s\n",url.text());  
    if (FXStat::exists(url)){
      createWindow(url);
      } 
    else {
      createWindow(FXString::null);  
      }
    }
  return 1;
  }



FXIcon * FXFileApplication::getIconForDirectory(const FXString & url){
  FXFileAssoc * assoc = NULL;
  assoc = associations->findDirBinding(url.text());
  if (assoc) {
    if (assoc->miniicon)
      return assoc->miniicon;
    else
      return assoc->miniiconopen;
      }
  return icon_folder;
  }

FXIcon * FXFileApplication::getIconForFile(const FXString & url){
  FXFileAssoc * assoc = NULL;
  assoc = associations->findFileBinding(url.text());
  if (assoc) {
    if (assoc->miniicon)
      return assoc->miniicon;
    else
      return assoc->miniiconopen;
      }
  return icon_document;
  }

FXIcon * FXFileApplication::getIconForUrl(const FXString & url){
  if (FXStat::isDirectory(url))
    return getIconForDirectory(url);
  else
    return getIconForFile(url);
  }


FXFileDict * FXFileApplication::getAssociations() const{
  return associations;
  }



long FXFileApplication::onCmdNewWindow(FXObject*,FXSelector,void*){
  createWindow();
  return 1;
  }

long FXFileApplication::onChildSignal(FXObject*,FXSelector,void*){
  FXint pid;
  FXint status;
  while(1) {
    pid = waitpid(-1,&status,WNOHANG);
    if (pid>0) {
      continue;
      }
    break;
    }
  return 1;
  }


long FXFileApplication::onCmdIconTheme(FXObject*,FXSelector sel,void*){
  icontheme = FXSELID(sel)-ID_ICON_THEME_1; 
  fxmessage("theme: %d/%d\n",icontheme,ARRAYNUMBER(iconthemes));
  initAssociations(associations,iconthemes[icontheme]);
  FXWindow * window = getRootWindow()->getFirst();
  FXFileManager * manager;
  while(window) {
    manager = dynamic_cast<FXFileManager*>(window);
    if (manager) manager->scan();
    window = window->getNext();
    }
  return 1;
  }


long FXFileApplication::onUpdIconTheme(FXObject*sender,FXSelector sel,void*){
  if ((FXSELID(sel)-ID_ICON_THEME_1) >= ARRAYNUMBER(iconthemes) || !iconthemes[FXSELID(sel)-ID_ICON_THEME_1].available)
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_HIDE),NULL);
  else {
    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SHOW),NULL);
    if (( FXSELID(sel)-ID_ICON_THEME_1) == icontheme)
      sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_CHECK),NULL);
    else
      sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_UNCHECK),NULL);

    sender->handle(this,FXSEL(SEL_COMMAND,FXWindow::ID_SETSTRINGVALUE),(void*)&FXString(iconthemes[FXSELID(sel)-ID_ICON_THEME_1].themename));
    }
  return 1;
  }



