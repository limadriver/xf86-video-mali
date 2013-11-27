/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2010 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#ifndef _MALI_FBDEV_DRIVER_H_
#define _MALI_FBDEV_DRIVER_H_

#include <linux/videodev2.h>
#include <linux/fb.h>
#include "exa.h"

#define DPMSModeOn	0
#define DPMSModeStandby	1
#define DPMSModeSuspend	2
#define DPMSModeOff	3

enum dri_type
{
	DRI_DISABLED,
	DRI_NONE,
	DRI_2,
};

typedef struct {
	unsigned char  *fbstart;
	unsigned char  *fbmem;
	int             fboff;
	int             lineLength;
	CreateScreenResourcesProcPtr CreateScreenResources;
	void (*PointerMoved)(int index, int x, int y);
	CloseScreenProcPtr  CloseScreen;
	EntityInfoPtr       pEnt;
	OptionInfoPtr       Options;
	int    fb_lcd_fd;
	struct fb_fix_screeninfo fb_lcd_fix;
	struct fb_var_screeninfo fb_lcd_var;
	ExaDriverPtr exa;
	int  dri_render;
	Bool dri_open;
	int  drm_fd;
	char deviceName[64];
	Bool use_pageflipping;
	Bool use_pageflipping_vsync;
} MaliRec, *MaliPtr;

typedef struct {
	char *device;
	int   fd;
	void *fbmem;
	unsigned int   fbmem_len;
	unsigned int   fboff;
	struct fb_fix_screeninfo fix;
	struct fb_var_screeninfo var;
	struct fb_var_screeninfo saved_var;
	DisplayModeRec buildin;
	int xres;
	int yres;
} MaliHWRec, *MaliHWPtr;

#define MALIPTR(p) ((MaliPtr)((p)->driverPrivate))
#define MALIHWPTRLVAL(p) (p)->privates[malihwPrivateIndex].ptr
#define MALIHWPTR(p) ((MaliHWPtr)(MALIHWPTRLVAL(p)))

//#define MALI_DEBUG_MSG_ENABLE

#ifdef MALI_DEBUG_MSG_ENABLE
#define MALIDBGMSG(type, format, args...)       xf86Msg(type, format, args)
#else
#define MALIDBGMSG(type, format, args...)
#endif


Bool FBDEV_lcd_init(ScrnInfoPtr pScrn);

Bool MaliDRI2ScreenInit( ScreenPtr pScreen );
void MaliDRI2CloseScreen( ScreenPtr pScreen );

#endif /* _MALI_FBDEV_DRIVER_H_ */

