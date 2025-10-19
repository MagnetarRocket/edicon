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
/* $TOG: main.c /main/11 1999/09/17 13:25:57 mgreess $ */
/*********************************************************************
*  (c) Copyright 1993, 1994 Hewlett-Packard Company
*  (c) Copyright 1993, 1994 International Business Machines Corp.
*  (c) Copyright 1993, 1994 Sun Microsystems, Inc.
*  (c) Copyright 1993, 1994 Unix System Labs, Inc., a subsidiary of
*      Novell, Inc.
**********************************************************************/
/*-----------------------------------------------------------*/
/* This is the project main program file.                    */
/* You may add application dependent source code             */
/* at the appropriate places.                                */
/*-----------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <locale.h>
#include <sys/param.h>

#include <X11/Intrinsic.h>
#include <Xm/Protocols.h>
#include <Xm/DialogS.h>
#include <Xm/MessageB.h>
#include <Xm/MwmUtil.h>
#include "externals.h"
#include "main.h"
#include "event.h"
#include "utils.h"
#include "process.h"


#define ERROR           -1
#define NO_ERROR        0

static XtResource resources[] = {
  {"useMessaging", "UseMessaging", XmRBoolean, sizeof (Boolean),
    XtOffset (ApplicationDataPtr, useBMS), XmRImmediate, (XtPointer)False,
  },
  {"bmSuffix", "BmSuffix", XmRString, sizeof (char *),
    XtOffset (ApplicationDataPtr, bmSuffix), XmRImmediate, (XtPointer)".xbm",
  },
  {"pmSuffix", "PmSuffix", XmRString, sizeof (char *),
    XtOffset (ApplicationDataPtr, pmSuffix), XmRImmediate, (XtPointer)".xpm",
  },
  {"maxIconWidth", "MaxIconWidth", XmRInt, sizeof (int),
    XtOffset (ApplicationDataPtr, maxIconWidth), XmRImmediate,
    (caddr_t)MAX_ICON_WIDTH,
  },
  {"maxIconHeight", "MaxIconHeight", XmRInt, sizeof (int),
    XtOffset (ApplicationDataPtr, maxIconHeight), XmRImmediate,
    (caddr_t)MAX_ICON_HEIGHT,
  },
  {"useFileFilter", "UseFileFilter", XmRBoolean, sizeof (Boolean),
    XtOffset (ApplicationDataPtr, useFileFilter), XmRImmediate, (XtPointer)True,
  },
  {"useFileLists", "UseFileLists", XmRBoolean, sizeof (Boolean),
    XtOffset (ApplicationDataPtr, useFileLists), XmRImmediate, (XtPointer)True,
  },
  {"transparentColor", "TransparentColor", XmRPixel, sizeof(Pixel),
  	XtOffset (ApplicationDataPtr, transparentPixel), XmRString,
	(XtPointer) "lightsteelblue"
  }
};

static XrmOptionDescRec option_list[] =
{
   {"-bmSuffix", "bmSuffix", XrmoptionSepArg, NULL},
   {"-pmSuffix", "pmSuffix", XrmoptionSepArg, NULL},
   {"-maxIconWidth", "maxIconWidth", XrmoptionSepArg, (caddr_t)MAX_ICON_WIDTH},
   {"-maxIconHeight", "maxIconHeight",XrmoptionSepArg,(caddr_t)MAX_ICON_HEIGHT},
   {"+fileFilter", "useFileFilter", XrmoptionNoArg, (caddr_t)"FALSE"},
   {"+fileLists", "useFileLists", XrmoptionNoArg, (caddr_t)"FALSE"},
   {"-fileFilter", "useFileFilter", XrmoptionNoArg, (caddr_t)"TRUE"},
   {"-fileLists", "useFileLists", XrmoptionNoArg, (caddr_t)"TRUE"},
   {"-transparent", "transparentColor", XrmoptionSepArg, (caddr_t) NULL}
};

static String fallback_res[] = {
	"*fileMenu.exit.acceleratorText: Alt+F4",
	"*fileMenu.exit.accelerator: Alt<Key>F4",
	"*editMenu.undo.acceleratorText: Ctrl+Z",
	"*editMenu.undo.accelerator: Ctrl<Key>z",
	"*editMenu.cut.acceleratorText: Ctrl+X",
	"*editMenu.cut.accelerator: Ctrl<Key>x",
	"*editMenu.copy.acceleratorText: Ctrl+C",
	"*editMenu.copy.accelerator: Ctrl<Key>c",
	"*editMenu.paste.acceleratorText: Ctrl+V",
	"*editMenu.paste.accelerator: Ctrl<Key>v",
	"*fileMenu.new.acceleratorText: Ctrl+N",
	"*fileMenu.new.accelerator: Ctrl<Key>n",
	"*fileMenu.open.acceleratorText: Ctrl+O",
	"*fileMenu.open.accelerator: Ctrl<Key>o",
	"*fileMenu.save.acceleratorText: Ctrl+S",
	"*fileMenu.save.accelerator: Ctrl<Key>s",
	"*instructionText.alignment: alignment_beginning",
	"*useFileFilter: False",
	NULL
};

extern Widget mainForm, iconForm, fileMenu_quit_pb;
extern char start_file[];
extern char dummy[];


/*-----------------------------------------------------------*/
/* Insert application global declarations here               */
/*-----------------------------------------------------------*/

XtAppContext AppContext;
Widget       TopLevel;
Widget       dticonShell;
char        *execName, *progName;
char msgID[120];




/*-----------------------------------------------------------*/
/* User has selected window manager close button... verify   */
/* close desired if data has not been saved.                 */
/*-----------------------------------------------------------*/
static void
CloseCB (
        Widget w,
        XtPointer clientData,
        XtPointer callbackArg )
{
    DialogFlag = QUIT;
    XtCallCallbacks (fileMenu_quit_pb, XmNactivateCallback, NULL);
}


/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
/* main                                                      */
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
int
main(
        int argc,
        char *argv[] )
{
        /*-----------------------------------------------------------*/
        /* Declarations.                                             */
        /*-----------------------------------------------------------*/
        Widget  w, d;
        int n;
        Arg args[20];
        Atom xa_WM_DELETE_WINDOW, xa_WM_SAVE_YOURSELF;
        char *tmpPtr=NULL;
        static char *untitledStr = NULL;
        char *p=NULL;
        char *tmpp=NULL;

        /*-----------------------------------------------------------*/
        /* Interface function declaration                            */
        /*-----------------------------------------------------------*/
        Widget  create_dtIconShell();
        Widget  create_newIconDialog();
        Widget  create_queryDialog();
        Widget  create_stdErrDialog();
        Widget  create_fileIODialog();
        void    GetMarginData();


        /* ------------------------*/
        /*      Initialize program      */
        /*------------------------------*/

        XtSetLanguageProc(NULL, NULL, NULL);

        execName = argv[0];
        if (progName=strrchr(argv[0], '/'))
            progName++;
        else
            progName = argv[0];


        TopLevel = XtAppInitialize(&AppContext, CLASS_NAME,
                                option_list, XtNumber(option_list),
                                &argc, argv, fallback_res, NULL, 0);

        XtGetApplicationResources(TopLevel, &xrdb, resources,
                                      XtNumber(resources), NULL, 0);

        /*------------------------------------------------------------*/
        /*    Insert initialization code for your application here    */
        /*------------------------------------------------------------*/


        /*------------------------------------------------------------------*/
        /*  Create and popup the first window of the interface.  The        */
        /*  return value can be used in the popdown or destroy functions.   */
        /*------------------------------------------------------------------*/
        w = create_dtIconShell();
	dticonShell = w;
        d = create_fileIODialog();
        d = create_newIconDialog();
        d = create_queryDialog();
        d = create_stdErrDialog();
        Create_Gfx_Labels(Foreground, Background);

        XtRealizeWidget (TopLevel);
        XtMapWidget (TopLevel);

        GetMarginData();

        /* */
        /*  Since I can't change the .msg files I'll have to force
            it here.
        */
    last_fname[0] = '\0';
    if (!untitledStr)
      untitledStr = GETSTR(2,20, "UNTITLED");
    strcpy(last_fname, untitledStr);
    strcat(last_fname, ".m.pm");
    ChangeTitle();
        /* */

        ParseAppArgs(argc, argv);
        if (start_file[0] != '\0')
        {
           /* strip off path portion of name if there is one */
           tmpPtr = strrchr(start_file,'/');
           if (tmpPtr != NULL)
	       XtSetArg(args[0], XmNiconName, tmpPtr+1);
           else
	       XtSetArg(args[0], XmNiconName, start_file);
	   XtSetValues(dtIconShell, args, 1);
        }
        n = 0;
        XtSetArg(args[n], XmNresizePolicy, XmRESIZE_NONE); n++;
        XtSetValues(mainForm, args, n);
        XtSetValues(iconForm, args, n);

        /*------------------------------------------------------------------*/
        /*   Catch window close event and call "File...Exit" callback.      */
        /*------------------------------------------------------------------*/
        xa_WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
        XmAddWMProtocolCallback(dtIconShell, xa_WM_DELETE_WINDOW, CloseCB,
                                NULL);

        /*-------------------------*/
        /*   Enter the event loop  */
        /*-------------------------*/
        {
            XEvent event;

            for (;;) {
                XtAppNextEvent(AppContext, &event);
                if ((GraphicsOp == S_GRAB) &&
                    ((event.type == ButtonPress)  ||
                     (event.type == MotionNotify) ||
                     (event.type == ButtonRelease)))
                  Do_ButtonOp(&event);
                else
                {
                    if ((GraphicsOp == POLYGON || GraphicsOp == POLYLINE) &&
                          (event.type == ButtonPress))
                    {
                        if (event.xbutton.window != tablet_win)
                            EndPolyOp();
                    }
                    XtDispatchEvent(&event);
                }
            }
        }
}


/******************************************************************************
NAME:           handle_dialog_child( wgt, manage_func )

INPUT:          Widget  wgt                     - the dialogShellWidget
                void    (*manage_func)()        - either XtManageChild
                                                  or XtUnmanageChild

RETURN:         int                             - NO_ERROR if successfull
                                                  ERROR otherwise

DESCRIPTION:    Handles the popping up or popping down of dialog shells
                by managing or unmanaging their children.

CREATION:       Visual Edge Software            Sept 19/91
-----------------------------------------------------------------------------*/
static  int     handle_dialog_child( Widget wgt, void (*manage_func)() )
{
        int      i;
        XtArgVal num_children;
        Widget  *children;

        XtVaGetValues( wgt,
                       XmNnumChildren, &num_children,
                       XmNchildren, &children,
                       NULL );

        /* We manage/unmanage the first rectObj child in the list.
         * Note that the check for rectObjClass is necessary since some
         * implementations of Motif add protocol children to the dialogShell.
         */

        for (i = 0; i < (int)num_children; i++)
        {
                if ( XtIsSubclass( children[i], rectObjClass ) )
                {
                        (*manage_func)(children[i]);
                        return ( NO_ERROR );
                }
        }

        return ( ERROR );
}

/******************************************************************************
NAME:           PopupInterface( wgt, grab_flag )

INPUT:          Widget          wgt             - Widget to popup
                XtGrabKind      grab_flag       - grab flag

RETURN:         int                     ERROR or NO_ERROR

DESCRIPTION:    Popups up an interface. The widget should be a toplevel widget.
                Note that special handling is required for dialogShells since
                those are popped up by managing their children if they have
                some.
                The grab_flag could be any of:
                                no_grab (XtGrabNone)
                                nonexclusive_grab (XtGrabNonexclusive)
                                exclusive_grab (XtGrabExclusive)

CREATION:       Visual Edge Software            April 6 1991
-----------------------------------------------------------------------------*/
int     PopupInterface( Widget wgt, XtGrabKind grab_flag )
{

        if ( XtIsSubclass( wgt, xmDialogShellWidgetClass ) )
        {
                if ( handle_dialog_child( wgt, XtManageChild ) == ERROR )
                        XtPopup( wgt, grab_flag );
        }
        else
        {
                XtPopup( wgt, grab_flag );
        }

        return ( NO_ERROR );
}
