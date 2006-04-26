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
#ifndef FXTHREADEX_H
#define FXTHREADEX_H


enum {
	STATUS_INFORMATION = 0,
	STATUS_DONE = -1,
	STATUS_FATAL = -2,
	STATUS_OVERWRITE = -3,
  STATUS_DELETE = -4,
	};

struct FXTransferStatus {
  FXint    code;
  FXString error;
  FXString srcfile;
  FXString dstfile;
  FXint    total;   /// Total Bytes in File
  FXint    current; /// Current Bytes
  FXint    num;
  FXTransferStatus() : code(0),total(0),current(0),num(0) {}
  };

class FXTransfer : public FXThread {
protected:
	FXGUISignal         feedback;
  FXTransferStatus    status_info;
  FXMutex             mutex_status;
  FXMutex             mutex_feedback;
  FXCondition         condition_feedback;
  FXbool              signalled;
protected:
  volatile FXbool running;
protected:
  void notify();
  void send_done();
  void send_fatal(const FXString & e);
protected:
  virtual FXint run()=0;
public:
  FXTransfer(FXObject*,FXSelector);

  virtual void respondDelete(FXbool ignore) {}

  virtual void respondOverwrite(FXbool,FXbool,FXbool) {}

  void status(FXTransferStatus & p);

  /// Cleanup
  void dispose();

  /// Cleanup
  void dispose_and_join();

  virtual ~FXTransfer();
  };

#endif



