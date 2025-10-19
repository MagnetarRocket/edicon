/*
 * CDE - Common Desktop Environment
 *
 * Copyright (c) 1993-2012, The Open Group. All rights reserved.
 *
 * These libraries and programs are free software; you can
 * redistribute them and/or modify them under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * These libraries and programs are distributed in the hope that
 * they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with these libraries and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
 */
/* $XConsortium: NewCvt.c /main/5 1996/03/26 19:53:53 drk $
 *
 * (c) Copyright 1996 Digital Equipment Corporation.
 * (c) Copyright 1990,1996 Hewlett-Packard Company.
 * (c) Copyright 1996 International Business Machines Corp.
 * (c) Copyright 1996 Sun Microsystems, Inc.
 * (c) Copyright 1996 Novell, Inc. 
 * (c) Copyright 1996 FUJITSU LIMITED.
 * (c) Copyright 1996 Hitachi.
 */

/**---------------------------------------------------------------------
***	
***	file:		NewCvt.c
***
***	project:	Motif Widgets
***
***	description:	Source code for new MotifPlus resource converters.
***	
***-------------------------------------------------------------------*/


/*-------------------------------------------------------------
**	Include Files
*/

#include <X11/Intrinsic.h>
#include "DialogBox.h"
#include <ctype.h>

#define STR_WORK_AREA "work_area"
#define STR_SEPARATOR "separator"
#define STR_BUTTON "button"

extern void _DtRegisterNewConverters( void ) ;

/********    Static Function Declarations    ********/

static Boolean _StringsAreEqual( 
                        String in_str,
                        String test_str) ;

static void _DtCvtStringToChildType( 
                        XrmValuePtr args,
                        Cardinal *num_args,
                        XrmValue *from_val,
                        XrmValue *to_val) ;


/********    End Static Function Declarations    ********/


/*-------------------------------------------------------------
**	_DtRegisterNewConverters
**		Convert MotifPlus resource converters.
*/
void 
_DtRegisterNewConverters( void )
{
  static Boolean new_converters_registered = False;
  
  if (!new_converters_registered)
    {
      if (!new_converters_registered)
	{
	  /* Register new converters. */
	  XtAddConverter (XmRString, XmRChildType,
			  _DtCvtStringToChildType, NULL, 0);

	  new_converters_registered = True;
	}
    }
}



/*-------------------------------------------------------------
**	_StringsAreEqual
**		Compare possibly mixed case string with lowercase string.
*/
static Boolean 
_StringsAreEqual(
        String in_str,
        String test_str )
{
	int	i, j;

	for (;;)
	{
		i = *in_str;
		j = *test_str;

		if (isupper (i))
			i = tolower (i);
		if (i != j) return (False);
		if (i == 0) return (True);

		in_str++;
		test_str++;
	}
}


/*-------------------------------------------------------------
**	_DtCvtStringToChildType
**		Convert XmRString to XmRChildType.
*/
static void 
_DtCvtStringToChildType(
        XrmValuePtr args,
        Cardinal *num_args,
        XrmValue *from_val,
        XrmValue *to_val )
{
	String			string = (String) from_val->addr;
	static unsigned char	value;

	to_val->size = sizeof (unsigned char);
	to_val->addr = (XtPointer) &value;

	if (_StringsAreEqual (string, STR_WORK_AREA))
		value = XmWORK_AREA;
	else if (_StringsAreEqual (string, STR_SEPARATOR))
		value = XmSEPARATOR;
	else if (_StringsAreEqual (string, STR_BUTTON))
		value = XmBUTTON;
	else
	{
		to_val->size = 0;
		to_val->addr = NULL;
		XtStringConversionWarning ((String) from_val->addr,
				XmRChildType);
	}
}
