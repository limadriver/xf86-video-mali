/*
 * Copyright (C) 2010 ARM Limited. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/ioctl.h>
#include "xf86.h"
#include "xf86Crtc.h"

#include "mali_fbdev.h"
#define IGNORE(a) (a=a)

static void fbdev_lcd_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
	IGNORE(crtc);
	IGNORE(mode);
}

static Bool fbdev_lcd_crtc_lock(xf86CrtcPtr crtc)
{
	IGNORE( crtc );

	return TRUE;
}

static void fbdev_lcd_crtc_unlock(xf86CrtcPtr crtc)
{
	IGNORE( crtc );
}

static Bool fbdev_lcd_crtc_mode_fixup(xf86CrtcPtr crtc, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
	IGNORE( crtc );
	IGNORE( mode );
	IGNORE( adjusted_mode );

	return TRUE;
}

static void fbdev_lcd_crtc_prepare(xf86CrtcPtr crtc)
{
	IGNORE( crtc );
}

static void fbdev_lcd_crtc_mode_set(xf86CrtcPtr crtc, DisplayModePtr mode, DisplayModePtr adjusted_mode, int x, int y)
{
	IGNORE( crtc );
	IGNORE( mode );
	IGNORE( adjusted_mode );
	IGNORE( x );
	IGNORE( y );
}

static void fbdev_lcd_crtc_commit(xf86CrtcPtr crtc)
{
	IGNORE( crtc );
}

static void fbdev_lcd_crtc_gamma_set(xf86CrtcPtr crtc, CARD16 *red, CARD16 *green, CARD16 *blue, int size)
{
	IGNORE( crtc );
	IGNORE( red );
	IGNORE( green );
	IGNORE( blue );
	IGNORE( size );
}

static void fbdev_lcd_crtc_set_origin(xf86CrtcPtr crtc, int x, int y)
{
	IGNORE( crtc );
	IGNORE( x );
	IGNORE( y );
}

static const xf86CrtcFuncsRec fbdev_lcd_crtc_funcs = 
{
	.dpms = fbdev_lcd_crtc_dpms,
	.save = NULL, 
	.restore = NULL,
	.lock = fbdev_lcd_crtc_lock,
	.unlock = fbdev_lcd_crtc_unlock,
	.mode_fixup = fbdev_lcd_crtc_mode_fixup,
	.prepare = fbdev_lcd_crtc_prepare,
	.mode_set = fbdev_lcd_crtc_mode_set,
	.commit = fbdev_lcd_crtc_commit,
	.gamma_set = fbdev_lcd_crtc_gamma_set,
	.shadow_allocate = NULL, 
	.shadow_create = NULL,
	.shadow_destroy = NULL,
	.set_cursor_colors = NULL,
	.set_cursor_position = NULL,
	.show_cursor = NULL,
	.hide_cursor = NULL,
	.load_cursor_image = NULL,
	.load_cursor_argb = NULL,
	.destroy = NULL,
	.set_mode_major = NULL,
	.set_origin = fbdev_lcd_crtc_set_origin,
};

static void fbdev_lcd_output_dpms(xf86OutputPtr output, int mode)
{
	MaliPtr fPtr = MALIPTR(output->scrn);

	if ( mode == DPMSModeOn ) 
	{
		ioctl(fPtr->fb_lcd_fd, FBIOBLANK, FB_BLANK_UNBLANK);
	}
	else if( mode == DPMSModeOff )
	{
		ioctl(fPtr->fb_lcd_fd, FBIOBLANK, FB_BLANK_POWERDOWN);
	}
}

static void fbdev_lcd_output_save(xf86OutputPtr output)
{
	IGNORE( output );
}

static void fbdev_lcd_output_restore(xf86OutputPtr output)
{
	IGNORE( output );
}

static int fbdev_lcd_output_mode_valid(xf86OutputPtr output, DisplayModePtr pMode)
{
	MaliPtr fPtr = MALIPTR(output->scrn);

	if( (pMode->HDisplay == (int)fPtr->fb_lcd_var.xres) && (pMode->VDisplay == (int)fPtr->fb_lcd_var.yres) ) return MODE_OK;

	return MODE_ERROR;
}

static Bool fbdev_lcd_output_mode_fixup(xf86OutputPtr output, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
	IGNORE( output );
	IGNORE( mode );
	IGNORE( adjusted_mode );

	return TRUE;
}

static void fbdev_lcd_output_prepare(xf86OutputPtr output)
{
	IGNORE( output );
}

static void fbdev_lcd_output_commit(xf86OutputPtr output)
{
	output->funcs->dpms(output, DPMSModeOn);
}

static void fbdev_lcd_output_mode_set(xf86OutputPtr output, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
	IGNORE( output );
	IGNORE( mode );
	IGNORE( adjusted_mode );
}

static xf86OutputStatus fbdev_lcd_output_detect(xf86OutputPtr output)
{
	IGNORE( output );

	return XF86OutputStatusConnected;
}

static DisplayModePtr fbdev_lcd_output_get_modes(xf86OutputPtr output)
{
	MaliPtr fPtr = MALIPTR(output->scrn);
	DisplayModePtr mode_ptr;

	unsigned int hactive_s = fPtr->fb_lcd_var.xres;
	unsigned int vactive_s = fPtr->fb_lcd_var.yres;

	mode_ptr = xnfcalloc(1, sizeof(DisplayModeRec));

	mode_ptr->HDisplay = hactive_s;
	mode_ptr->HSyncStart = hactive_s + 20;
	mode_ptr->HSyncEnd = hactive_s + 40;
	mode_ptr->HTotal = hactive_s + 80;

	mode_ptr->VDisplay = vactive_s;
	mode_ptr->VSyncStart = vactive_s + 20;
	mode_ptr->VSyncEnd = vactive_s + 40;
	mode_ptr->VTotal = vactive_s + 80;

	mode_ptr->VRefresh = 60.0;

	mode_ptr->Clock = (int) (mode_ptr->VRefresh * mode_ptr->VTotal * mode_ptr->HTotal / 1000.0);

	mode_ptr->type = M_T_DRIVER;

	xf86SetModeDefaultName(mode_ptr);
		
	mode_ptr->next = NULL;
	mode_ptr->prev = NULL;

	return mode_ptr;
}

#ifdef RANDR_GET_CRTC_INTERFACE
static xf86CrtcPtr fbdev_lcd_output_get_crtc(xf86OutputPtr output)
{
	return output->crtc;
}
#endif

static void fbdev_lcd_output_destroy(xf86OutputPtr output)
{
	IGNORE( output );
}

static const xf86OutputFuncsRec fbdev_lcd_output_funcs = 
{
	.create_resources = NULL,
	.dpms = fbdev_lcd_output_dpms,
	.save = fbdev_lcd_output_save,
	.restore = fbdev_lcd_output_restore,
	.mode_valid = fbdev_lcd_output_mode_valid,
	.mode_fixup = fbdev_lcd_output_mode_fixup,
	.prepare = fbdev_lcd_output_prepare,
	.commit = fbdev_lcd_output_commit,
	.mode_set = fbdev_lcd_output_mode_set,
	.detect = fbdev_lcd_output_detect,
	.get_modes = fbdev_lcd_output_get_modes,
#ifdef RANDR_12_INTERFACE
	.set_property = NULL,
#endif
#ifdef RANDR_13_INTERFACE
	.get_property = NULL,
#endif
#ifdef RANDR_GET_CRTC_INTERFACE
	.get_crtc = fbdev_lcd_output_get_crtc,
#endif
	.destroy = fbdev_lcd_output_destroy,
};


Bool FBDEV_lcd_init(ScrnInfoPtr pScrn)
{
	xf86CrtcPtr crtc;
	xf86OutputPtr output;

	crtc = xf86CrtcCreate(pScrn, &fbdev_lcd_crtc_funcs);

	if(crtc == NULL) return FALSE;

	output = xf86OutputCreate(pScrn, &fbdev_lcd_output_funcs, "LCD");

	if(output == NULL) return FALSE;

	output->possible_crtcs = (1 << 0);

	return TRUE;
}
