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
/* $XConsortium: globals.c /main/3 1995/11/02 14:04:57 rswiston $ */
/*********************************************************************
*  (c) Copyright 1993, 1994 Hewlett-Packard Company
*  (c) Copyright 1993, 1994 International Business Machines Corp.
*  (c) Copyright 1993, 1994 Sun Microsystems, Inc.
*  (c) Copyright 1993, 1994 Unix System Labs, Inc., a subsidiary of
*      Novell, Inc.
**********************************************************************/
#include <Xm/Xm.h>
#include <X11/xpm.h>
#include "constants.h"

char *black_string="black";
char *white_string="white";
char *none_string="none";

HelpStruct   *help_head;                /* head of the Help dialog list */
int      query_state, error_state;      /* current state of query/error
                                           dialogs */

Display *dpy;                           /* default display */
Window   root;                          /* default root window */
Window   tablet_win;                    /* window ID for graphics tablet */
Widget   tablet_wid;                    /* widget ID for graphics tablet */
int      screen;                        /* default screen index */
Screen  *screen_ptr;                    /* ptr to default screen struct. */
Colormap Cmap;                          /* default colormap */
Cursor   cursor;                        /* Custom cursor for grab operations */
GC       Color_gc;                      /* GC for fat bits & 1:1 color icon */
GC       Mono_gc;                       /* GC for 1:1 monochrome icon */
GC       Flicker_gc;                    /* GC for flicker lines on tablet */
GC       Grid_gc;                       /* GC for grid */
GC       Erase_gc;                      /* GC for the ERASE tool */
int      cmap_size;                     /* number of cells in colormap */
int      MagFactor;                     /* current enlargement value */
int      GraphicsOp;                    /* current graphics tool selected */
int      Backup_G_Op;                   /* previous graphics tool selected */
int      CurrentColor;                  /* current pen color selected */
int      ColorBlock;                    /* static color or dynamic color */
int      tx, ty, last_tx, last_ty;      /* x & y values on tablet */
int      ix, iy, last_ix, last_iy;      /* x & y values in icons */
int      ClickCount = 0;                /* ...for multi-click events */
int      format = ZPixmap;              /* XImage format for 2ndary ops. */
int      multiClickTime;                /* system default multi-click time */
XRectangle current_box;                 /* portion of the tablet currently
                                           visible */
XRectangle select_box;                  /* currently selected rect. area */
int      icon_width=32, icon_height=32; /* current icon width & height */
int      backup_width=0,
         backup_height=0;               /* current backup icon width & height */
Pixmap   color_icon, prev_color_icon;   /* current and 'undo' color pixmaps */
Pixmap   mono_icon, prev_mono_icon;     /* current and 'undo' 2-tone pixmaps */
Boolean  status, Dirty;                 /* function return & icon modified
                                           flags */
int      X_Hot, Y_Hot;                  /* hotspot coords. for XPM/XBM file */
Boolean  hotSpot=False;                 /* is a hot spot currently set? */
Boolean  GridEnabled=True;              /* should the tablet grid be visible? */
Boolean  FloodToBorder=False;           /* mono or multi-color flood? */
Boolean  FirstRigid=False;              /* is this the 1st rigid box drawed? */
Boolean  Selected=False;                /* is an area selected? */
Boolean  Anchored=False;                /* state flag for poly{line/gon} Ops. */
int      pointCount;                    /* num pts. for poly{line/gon} Ops. */
int      fileIOMode;                    /* are we reading or writing a file? */
int      fileFormat=FORMAT_XPM;         /* default file output format */
char     last_fname[MAX_FNAME];         /* last file read or written */
XPoint   pointList[MAX_PTS];            /* pt. list for poly{line/gon} Ops. */
XImage  *CutCopy=NULL;                  /* XImage from last cut/copy op. */
XImage  *CutCopy_mono=NULL;             /* XImage from last cut/copy op. */
XImage  *Rotate=NULL;                   /* XImage from last rotation op. */
XImage  *Rotate_mono=NULL;              /* XImage from last rotation op. */
XImage  *Scale=NULL;                    /* XImage from last scale op. */
XImage  *Scale_mono=NULL;               /* XImage from last scale op. */
Pixel    black_pixel, white_pixel;      /* default black & white pixels */
Pixel    Foreground, Background,
         TopShadow, BottomShadow,
         Select, Transparent;           /* the 6 dynamic colors */
Pixel    StaticPen[NUM_STATICS],
         DynamicPen[NUM_DYNAMICS];      /* 22 pens (pixel values) */
Pixel    StaticMono[NUM_STATICS],
         DynamicMono[NUM_DYNAMICS];     /* 22 monochrome fallbacks */
Widget   StaticWid[NUM_STATICS],
         DynamicWid[NUM_DYNAMICS];      /* 22 color button widgets */
Widget   GraphicOpsWid[NUM_GFX_OPS];    /* 10 'Graphics Ops.' button widgets */

Boolean  DialogFlag;                    /* state flag for pop-up dialogs */
Boolean  FillSolids;                    /* state flag for polygon fills */
Boolean  param_flag[NUM_PARAMS];        /* what cmd. line params got used? */
Boolean  argsNeedProcessed;             /* cmd. line args need to be resolved */
Boolean  UndoFlag=False;                /* Can we undo the last graphics op.? */
XpmAttributes xpm_ReadAttribs;          /* XPM read file attributes */
XpmAttributes xpm_WriteAttribs;         /* XPM write file attributes */
/* int      numSymbols=0;                  number of color symbols loaded */
XpmColorSymbol *colorSymbols;           /* color symbols list */
ApplicationData  xrdb;                  /* application specific resources  */
SessionData  session;                   /* session specific resources  */
PixelTable pixelTable;                  /* used for getting mono equiv icon */

char dynamic_c_str[NUM_DYNAMICS][30];   /* RGB value str. for dynamic colors */

char *color_table[NUM_STATICS+NUM_DYNAMICS][6] = {
       { "0",  "Color1",         "black",   NULL,  NULL,  "black" },
       { "1",  "Color2",         "white",   NULL,  NULL,  "white" },
       { "2",  "Color3",         "black",   NULL,  NULL,  "darkred" },
       { "3",  "Color4",         "white",   NULL,  NULL,  "red" },
       { "4",  "Color5",         "black",   NULL,  NULL,  "darkgreen" },
       { "5",  "Color6",         "white",   NULL,  NULL,  "green" },
       { "6",  "Color7",         "black",   NULL,  NULL,  "darkblue" },
       { "7",  "Color8",         "white",   NULL,  NULL,  "blue" },
       { "8",  "Color9",         "black",   NULL,  NULL,  "darkcyan" },
       { "9",  "Color10",        "white",   NULL,  NULL,  "cyan" },
       { "a",  "Color11",        "black",   NULL,  NULL,  "darkkhaki" },
       { "b",  "Color12",        "white",   NULL,  NULL,  "yellow" },
       { "c",  "Color13",        "black",   NULL,  NULL,  "darkmagenta" },
       { "d",  "Color14",        "white",   NULL,  NULL,  "magenta" },
       { "e",  "Color15",        "black",   NULL,  NULL,  "slateblue" },
       { "f",  "Color16",        "white",   NULL,  NULL,  "skyblue" },

       { "A",  "Gray1",          "white",   NULL,  NULL,  "#dededededede" },
       { "B",  "Gray2",          "white",   NULL,  NULL,  "#bdbdbdbdbdbd" },
       { "C",  "Gray3",          "white",   NULL,  NULL,  "#adadadadadad" },
       { "D",  "Gray4",          "white",   NULL,  NULL,  "#949494949494" },
       { "E",  "Gray5",          "black",   NULL,  NULL,  "#737373737373" },
       { "F",  "Gray6",          "black",   NULL,  NULL,  "#636363636363" },
       { "G",  "Gray7",          "black",   NULL,  NULL,  "#424242424242" },
       { "H",  "Gray8",          "black",   NULL,  NULL,  "#212121212121" },
       { "I",  "background",         "black",   NULL,  NULL,  "#949494949494" },
       { "K",  "foreground",         "white",   NULL,  NULL,  "white" },
       { "L",  "topShadowColor",     "white",   NULL,  NULL,  "#bdbdbdbdbdbd" },
       { "M",  "bottomShadowColor",  "black",   NULL,  NULL,  "#636363636363" },
       { "N",  "selectColor",        "white",   NULL,  NULL,  "#737373737373" },
       { "O",  "none",               "none",    NULL,  NULL,  "none" }
};

char *hints_cmt = { " width height ncolors cpp [x_hot y_hot] " };
char *colors_cmt = { " colors " };
char *pixels_cmt = { " pixels " };

/****** STUB **********/
#ifdef DEBUG
int           debug=True;
int           debug_status;
XImage       *debug_image, *debug_shape;
#endif
/**********************/
