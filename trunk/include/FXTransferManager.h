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
#ifndef FXTRANSFERMANAGER_H
#define FXTRANSFERMANAGER_H

class FXTransfer;

class FXTransferManager : public FXMainWindow {
FXDECLARE(FXTransferManager)
private:
  FXFont 				* titlefont;
	FXTextField  	* text_file;
	FXTextField  	* text_count;
	FXProgressBar	* progress_bar;
  FXTransfer 		* transfer;
private:
  FXTransferManager(){}
  FXTransferManager(const FXTransferManager&);
  FXTransferManager& operator=(const FXTransferManager&);
protected:
  void setup();
public:
  enum {
    ID_THREAD = FXDialogBox::ID_LAST,
    ID_CANCEL,
    };
	enum {
		OP_COPY,
		OP_MOVE,
    OP_DELETE,
		};
public:
	long onCmdShow(FXObject*,FXSelector,void*);
	long onCmdCancel(FXObject*,FXSelector,void*);
	long onProgress(FXObject*,FXSelector,void*);
	long onSessionNotify(FXObject*,FXSelector,void*);
	long onSessionClosed(FXObject*,FXSelector,void*);
public:
  FXTransferManager(FXWindow * owner,const FXStringList & files,const FXString & destination,FXuint operation);

  virtual void show(FXuint placement=PLACEMENT_OWNER);

  virtual FXuint execute(FXuint placement=PLACEMENT_CURSOR);

  virtual ~FXTransferManager();
  };
#endif
