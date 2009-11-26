/*
 * File:    camera.c
 * Author:  Wu Song <wusong@broncho.cn>
 * Brief:   a camera test example
 *
 * Copyright (c) 2009 Broncho
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * Histroy:
 * =============================================================
 * 2009年07月28日 Wu Song <wusong@broncho.cn> created
 */

#include <V4l2.h>
#include <display.h>

namespace android {

extern "C" int main(int argc , char** argv)
{
	int preview_width = 640;
	int preview_height = 480;

	V4l2 * camera = new V4l2(NULL, preview_width, preview_height);
	camera->v4l2_config(preview_width, preview_height, V4l2_YUV422P);

	display_cfg * disconf = NULL;

	int ret = display_open(&disconf);
	if(ret != 0)
	{
		printf("display_open is error %d\n", ret);
	}
	
	disconf->bpp = 16;
	disconf->format = FORMAT_PLANAR_422;
	disconf->screen_width = 320;
	disconf->screen_height = 480;
	disconf->screen_pos_x = 0;
	disconf->screen_pos_y = 0;
	disconf->step = 640;
	disconf->height = 480;

    ret = display_config(disconf);
	if( ret != 0 )
	{
		display_close(&disconf);
		printf("overlay2 config error... close");
	}

	unsigned char * src[3] = {0};
	while(1)
	{
		if(!camera->getNextFrameAsYuv422(src))
		{
			printf("getNextFrameAsYuv422 is error\n");

			goto  camera_stop;
			break;
		}

		ret = copy_to_overlay(disconf, src);
		if(ret != 0)
		{
			printf("copy_to_overlay is error %d\n", ret);
			
			goto camera_stop;
			break;
		}
	
	};

camera_stop:
	
		delete camera;
		display_close(&disconf);

		return 0;
};

};
