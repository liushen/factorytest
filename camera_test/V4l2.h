/*
 * File:    V4l2.h
 * Author:  Wu Song <wusong@broncho.cn>
 * Brief:   A Interface for V4l2
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
 * 2009年04月23日 Wu Song <wusong@broncho.cn> created
 */
#ifndef __V4l2__H
#define __V4l2__H
//#include <ui/CameraHardwareInterface.h>


#define __USE_BSD 

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <errno.h>
#include <linux/videodev.h>

enum
{
	V4l2_RGB565,
	V4l2_YUV422P
};

struct buffer_s
{
	void *start[3];
};

namespace android {
class V4l2
{
private:
	int mFd;
	char* mDevPath;
	int mPreviewFmt;
	struct pollfd ufds;
	struct buffer_s *preview_map;
	struct  v4l2_buffer mBuffer;
	struct  v4l2_format mFormat;
	size_t mMapLen;
	size_t mDropFrame;
	size_t mWidth;
	size_t mHeight;
	size_t mBrightNess;
	size_t mSaturation;
	size_t mContrast;
	bool   mStart;

public:

    V4l2(char* DevPath, size_t w, size_t h);
    ~V4l2();

	bool v4l2_device_open();
	bool v4l2_device_close();
	int  v4l2_grab_sync();
	bool v4l2_mmap_init(struct buffer_s **private_map, size_t& map_len);
	void v4l2_munmap(struct buffer_s * private_map, size_t maplen);

	bool v4l2_preview_config();
	bool v4l2_preview_start();
	bool v4l2_preview_stop();
	bool v4l2_config(int width , int height, int format);
	bool v4l2_capture_config();
	bool v4l2_set_preview_size(int x, int y);
	bool v4l2_get_preview_size(int &x , int &y);
	bool v4l2_set_preview_format(int Fmt);
	bool v4l2_get_preview_format(int &Fmt);

	bool v4l2_get_brightness(int & Val);
	bool v4l2_set_brightness(int Val);

	bool v4l2_get_contrast(int & Val);
	bool v4l2_set_contrast(int Val);

	bool v4l2_get_saturation(int & Val);
	bool v4l2_set_saturation(int Val);

	bool v4l2_get_hue(int & Val);
	bool v4l2_set_hue(int Val);
	
	bool getNextFrameAsYuv422(unsigned char *psrc[3]);
	bool v4l2_frame_data_release();
	bool v4l2_get_preview_data_real(unsigned char *psrc[3]);
	bool v4l2_take_picture(uint8_t *buffer);
//	status_t dump(int fd, const Vector<String16>& args);

	
};

};

#endif /*__V4l2__H*/
