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
/* $XConsortium: DialogBoxP.h /main/5 1996/03/27 20:16:24 drk $ */
/**---------------------------------------------------------------------
***	
***	file:		DialogBoxP.h
***
***	project:	Motif Widgets
***
***	description:	Private include file for DtDialogBox class.
***	
***	
***			(c) Copyright 1990 by Hewlett-Packard Company.
***
***
***-------------------------------------------------------------------*/


#ifndef _DtDialogBoxP_h
#define _DtDialogBoxP_h

#include <Xm/XmP.h>
#include <Xm/BulletinBP.h>
#include <Xm/FormP.h>
#include "DialogBox.h"

typedef void (*DlgGetSizeProc)(
	Widget,
	Dimension,
	Dimension,
	Dimension,
	Dimension,
	Dimension *,
	Dimension *
);
/*-------------------------------------------------------------
**	Class Structure
*/

/*	Class Part
*/
typedef struct _DtDialogBoxClassPart
{
	XtWidgetProc		create_children;
	XtWidgetProc		configure_children;
	DlgGetSizeProc		get_size;
	XtCallbackProc		button_callback;
	caddr_t			extension;
} DtDialogBoxClassPart;

/*	Full Class Record
*/
typedef struct _DtDialogBoxClassRec
{
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	XmBulletinBoardClassPart	bulletin_board_class;
	XmFormClassPart		form_class;
	DtDialogBoxClassPart	dialog_box_class;
} DtDialogBoxClassRec;

/*	Actual Class
*/
extern DtDialogBoxClassRec dtDialogBoxClassRec;


/*-------------------------------------------------------------
**	Instance Structure
*/

/*	Instance Part
*/
typedef struct _DtDialogBoxPart
{
	XtCallbackList	callback;
	Widget		work_area;
	Widget		separator;
	WidgetList	button;
	Cardinal	button_count;	
	Boolean		minimize_buttons;
	XmStringTable	button_label_strings;
} DtDialogBoxPart;

/*	Full Instance Record
*/
typedef struct _DtDialogBoxRec
{
	CorePart		core;
	CompositePart		composite;
	ConstraintPart		constraint;
	XmManagerPart		manager;
	XmBulletinBoardPart	bulletin_board;
	XmFormPart		form;
	DtDialogBoxPart		dialog_box;
} DtDialogBoxRec;


/*-------------------------------------------------------------
**	Constraint Structure
*/

/*	Constraint Part
*/
typedef struct _DtDialogBoxConstraintPart
{
	unsigned char		child_type;
} DtDialogBoxConstraintPart, * DtDialogBoxConstraint;

/*	Full Constraint Record
*/
typedef struct _DtDialogBoxConstraintRec
{
	XmManagerConstraintPart		manager_constraint;
	XmFormConstraintPart		form_constraint;
 	DtDialogBoxConstraintPart	dialog_box_constraint;
} DtDialogBoxConstraintRec, * DtDialogBoxConstraintPtr;



/*-------------------------------------------------------------
**	Class and Instance Macros
*/

/*	DtDialogBox Class Macros
*/	
/**** WARNING: These macros are not thread-safe! ****/
#define C_CreateChildren(mc)	((mc) -> dialog_box_class.create_children)
#define C_ConfigureChildren(mc)	((mc) -> dialog_box_class.configure_children)
#define C_GetSize(mc)		((mc) -> dialog_box_class.get_size)
#define C_ButtonCallback(mc)	((mc) -> dialog_box_class.button_callback)

/*	DtDialogBox Instance Macros
*/
#define M_DialogBoxConstraint(w) \
 	(&((DtDialogBoxConstraintPtr) (w)->core.constraints) \
		-> dialog_box_constraint)
#define M_WorkArea(m)		(m -> dialog_box.work_area)
#define M_Separator(m)		(m -> dialog_box.separator)
#define M_MinimizeButtons(m)	(m -> dialog_box.minimize_buttons)
#define M_Button(m)		(m -> dialog_box.button)
#define M_ButtonCount(m)	(m -> dialog_box.button_count)
#define M_ButtonLabelStrings(m)	(m -> dialog_box.button_label_strings)
#define M_Callback(m)		(m -> dialog_box.callback)

/*	Core Macros
*/
#define M_X(w)			(w -> core.x)
#define M_Y(w)			(w -> core.y)
#define M_Width(w)		(w -> core.width)
#define M_Height(w)		(w -> core.height)
#define M_BorderWidth(w)	(w -> core.border_width)
#define M_Background(w)		(w -> core.background_pixel)
#define M_Sensitive(w)		(w->core.sensitive && w->core.ancestor_sensitive) 
#define M_Name(w)		(w -> core.name)
#define M_Accelerators(w)	(w -> core.accelerators)
#define M_TmTranslations(w)	(w -> core.tm.translations)
#define M_PopupList(w)		(w -> core.popup_list)
#define M_NumPopups(w)		(w -> core.num_popups)

#define M_Foreground(m)		(m -> manager.foreground)
#define M_HighlightThickness(m)	(m -> manager.highlight_thickness)
#define M_ShadowThickness(m)	(m -> manager.shadow_thickness)
#define M_TopShadowGC(m)	(m -> manager.top_shadow_GC)
#define M_BottomShadowGC(m)	(m -> manager.bottom_shadow_GC)
#define M_BackgroundGC(m)	(m -> manager.background_GC)
#define M_SelectedGadget(m)	(m -> manager.selected_gadget)
#define M_HelpCB(m)		(m -> manager.help_callback)
#define M_UnitType(m)		(m -> manager.unit_type)

#define M_ButtonFontList(m)	(m -> bulletin_board.button_font_list)
#define M_LabelFontList(m)	(m -> bulletin_board.label_font_list)
#define M_TextFontList(m)	(m -> bulletin_board.text_font_list)
#define M_MarginWidth(m)	(m -> bulletin_board.margin_width)
#define M_MarginHeight(m)	(m -> bulletin_board.margin_height)

#define C_WidgetClass(wc)	(wc -> core_class.widget_class)
#define C_Superclass(wc)	(wc -> core_class.superclass)
#define C_ClassName(wc)		(wc -> core_class.class_name)
#define C_ClassInited(wc)	(wc -> core_class.class_inited)
#define C_NumResources(wc)	(wc -> core_class.num_resources)

#define P_X(w)			(w -> core.x)
#define P_Y(w)			(w -> core.y)
#define P_Width(w)		(w -> core.width)
#define P_Height(w)		(w -> core.height)
#define P_BorderWidth(w)	(w -> core.border_width)
#define P_XrmName(w)		(w -> core.xrm_name)
#define P_PopupList(w)		(w -> core.popup_list)
#define P_NumPopups(w)		(w -> core.num_popups)


#endif /* _DtDialogBoxP_h */

/* DON'T ADD ANYTHING AFTER THIS #endif */
