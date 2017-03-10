/*
  Outlet

  Created by Skyler Brandt on August 2016

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/
*/

#ifndef OUTLET_H
#define OUTLET_H

#include <stdint.h>

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/
/*******Errors List************************************************************/
#define ERR_NOERROR 0 // no error has occured
#define ERR_UNKNOWN -1 // unknown error
#define ERR_NOMEM   -2 // out of memory

/*******User Defined***********************************************************/

typedef enum outletState {
    
}outletState_t;

/******************************************************************************/
/* Variable Definitions                                                       */
/******************************************************************************/
struct outletStruct {
    
};

typedef struct outletStruct* outletObj;

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
/*****Initialize***************************************************************/
int8_t outlet_init (outletObj inst);

/*****Run Time*****************************************************************/
void outlet_run (outletObj inst);


#endif /* OUTLET_H */