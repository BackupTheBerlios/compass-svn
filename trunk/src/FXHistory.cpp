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
#include "FXHistory.h"

FXHistory::FXHistory() : visiting(0) {
  }

void FXHistory::append(const FXString & directory) {
  FXint i;
  if(visiting==0){
    for(i=MAX_HISTORY-1; i; i--) history[i]=history[i-1];
    }
  else {
    for(i=1; i+visiting-1<=(MAX_HISTORY-1); i++) history[i]=history[i+visiting-1];
    for( ; i<MAX_HISTORY; i++) history[i]=FXString::null;
    }
  history[0]=directory;
  visiting=0;
  }


/// Forward
FXString FXHistory::forward(){
  return history[--visiting];
  }

/// Back
FXString FXHistory::back(){
  return history[++visiting];
  }

FXbool FXHistory::canForward(){
  return (0<visiting);
  }

FXbool FXHistory::canBack(){
  return (visiting<(MAX_HISTORY-1) && !history[visiting+1].empty());
  }







