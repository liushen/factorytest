/*
 * File:    V4l2.cpp
 * Author:  Wu Song <wusong@broncho.cn>
 * Brief:   An Interface For V4l2
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
#define MAP_BUFFER_NUM 4
#define LIGHT_DEV_PATH  "/sys/class/flashlight/broncho-flashlight/power"
#define DEFAULT_DEVICE "/dev/video0"

#include <V4l2.h>
#define LOG_TAG "V4l2Camera"
#include <utils/Log.h>

namespace android {

V4l2::V4l2(char* DevPath , size_t w, size_t h)
{
	if(DevPath == NULL)
	{
		mDevPath = strdup(DEFAULT_DEVICE);
	}
	else
	{
		mDevPath = strdup(DevPath);
	}
	
	mWidth = w;
	mHeight = h;
	mFd = -10;
	mStart = false;
	preview_map = NULL;
	
	mDropFrame = 0;

	int desiredcamera = 0;
    mFd = open("/dev/video0", O_RDWR);
    if(mFd < 0)
    {
    	int nError = errno;
    	LOGD("open----v4l2_open : mFd=%d devpath=%s errnor = %d\n",mFd ,mDevPath, nError);
   	}

    ufds.fd  = mFd;
    ufds.events = POLLIN;

    if(-1 == ioctl(mFd, VIDIOC_S_INPUT, &desiredcamera))
    {
        LOGD("Error: can't set input%d \n", desiredcamera);
    }

    	/*FIX : In here we should set parmpaters for camera*/
}

V4l2::~V4l2()
{
	delete mDevPath;

	if(mFd > 0)
	{
		if(preview_map != NULL)
		{
			v4l2_munmap(preview_map, mMapLen);
		}

	v4l2_preview_stop();
	int ret = close(mFd);
	mFd = 0;
	LOGD("%s  mFd = %d  ret =%d\n",__func__, mFd, ret);
	}
}

int V4l2::v4l2_grab_sync()
{
	int ret = 0;
	int count = 0;

	do {
		count ++;
		ret = poll(&ufds, 1, 1000);
		if(count >= 10)
		{
			LOGD("%s error \n", __func__);
			break;
		}
	} while (!ret);

	return ret;
}

bool V4l2::v4l2_mmap_init(struct buffer_s **private_map, size_t& map_len)
{
	struct  v4l2_requestbuffers requestbuffers;
	unsigned int n_buffers = 0;
	unsigned int i = 0;
	struct  v4l2_buffer         buffer;
	int ret;
	
    /* request buffers */
    memset(&requestbuffers, 0, sizeof(struct  v4l2_requestbuffers));
    requestbuffers.count    = MAP_BUFFER_NUM;
    requestbuffers.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestbuffers.memory   = V4L2_MEMORY_MMAP;
    requestbuffers.reserved[0] = 1;
    if(-1 == ioctl(mFd, VIDIOC_REQBUFS, &requestbuffers))
    {
        LOGD("Error: can't require buffer\n");
        return false;
    }

    if (requestbuffers.count < 2)
    {
        LOGD ("Error: Insufficient buffer allocated\n");
        return false;
    }

	if (!*private_map)
    {
        *private_map= (struct buffer_s* )calloc (requestbuffers.count, sizeof (**private_map));
    }

    /* do memory map */
    for (n_buffers = 0; n_buffers < requestbuffers.count; n_buffers++)
    {
        memset (&buffer, 0, sizeof(buffer));
        buffer.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory      = V4L2_MEMORY_MMAP;
        buffer.index       = n_buffers;

        if (-1 == ioctl (mFd, VIDIOC_QUERYBUF, &(buffer)))
        {
        	LOGD("VIDIOC_QUERYBUF is error\n");
            return false;
        }


        (*private_map)[n_buffers].start[0] =
                            mmap (NULL, buffer.length, PROT_READ,MAP_SHARED, mFd, buffer.m.offset);

        if (MAP_FAILED == (*private_map)[n_buffers].start[0])
        {
            LOGD("mmap");
            return false;
        }

        if(mFormat.fmt.pix.pixelformat == V4L2_PIX_FMT_YUV422P)
        {
           int cb_offset = mWidth * mHeight ;
           int cr_offset = mWidth * mHeight*3/2;
           (*private_map)[n_buffers].start[1] = (unsigned char*)(*private_map)[n_buffers].start[0]+cb_offset;
           //(*private_map)[n_buffers].start[1] = (*private_map)[n_buffers].start[0]
           // + ((mWidth * mHeight + 4096 - 1) / 4096) * 4096;
            (*private_map)[n_buffers].start[2] = (unsigned char*)(*private_map)[n_buffers].start[0]+cr_offset;
           // (*private_map)[n_buffers].start[2] = (*private_map)[n_buffers].start[1]
           // +((mWidth * mHeight/2 + 4096 - 1) / 4096) * 4096;
        }

        mMapLen = buffer.length;
    }

    for(i= 0; i<n_buffers; i++)
    {
        memset (&buffer, 0, sizeof(buffer));
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        buffer.index = i;

        ret = ioctl(mFd,  VIDIOC_QBUF, &buffer);
        if(ret == -1)
        {
            LOGD("VIDIOC_QBUF:");
            return false;
        }
    }

    return true;
}

void V4l2::v4l2_munmap(struct buffer_s * private_map, size_t maplen)
{
    int i = 0;

	for(i = 0; i< MAP_BUFFER_NUM; i++)
	{
		if (private_map[i].start[0] && (-1 == munmap (private_map[i].start[0], maplen)))
		{
			LOGD ("Error: can't unmap preview_map %d memory successfully\n", i);
		}
		private_map[i].start[0] = NULL;
	}
}

bool V4l2::v4l2_device_open()
{
	return false;
}

bool V4l2::v4l2_config(int width , int height, int format)
{
	
	mWidth = width;
	mHeight = height;

	if(mStart)
	{
		if(v4l2_preview_stop())
		{
			LOGD("v4l2_preview_stop is error! %s\n", __func__);
			return false;
		}
		mStart = false;
	}
	struct  v4l2_streamparm parm ;

    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    parm.parm.capture.timeperframe.numerator = 0;
    parm.parm.capture.timeperframe.denominator = 0;
   // parm.parm.capture.capturemode = 1 ;          /* set as cap mode */

    if(-1 == ioctl(mFd, VIDIOC_S_PARM, &parm))
    {
        LOGD("Error: can't set input_still\n");
        return false;
    }

	mFormat.type                    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//	mFormat.fmt.pix.pixelformat      = V4L2_PIX_FMT_YUV422P;
	switch (format)
	{
		case V4l2_RGB565:
		{
			mFormat.fmt.pix.pixelformat      = V4L2_PIX_FMT_RGB565X;
			break;
		}
		case V4l2_YUV422P:
		{
		   mFormat.fmt.pix.pixelformat      = V4L2_PIX_FMT_YUV422P;
		   break;
		}
		default:
		{
			mFormat.fmt.pix.pixelformat      = V4L2_PIX_FMT_RGB565X;
			break;
		}
	}
	mFormat.fmt.pix.width            = width;
	mFormat.fmt.pix.height           = height;
	
    if(-1 == ioctl(mFd, VIDIOC_S_FMT, &mFormat))
    {
    	LOGD("Error: VIDIOC_S_FMT is error\n");
        return false;
    }

    if(preview_map != NULL)
    {
        v4l2_munmap(preview_map, mMapLen);
    	LOGD(" v4l2_munmap\n");
    }
	bool ret = true;
    ret=v4l2_mmap_init(&preview_map, mMapLen);
    if(!ret)
    {
    	LOGD("Error: v4l2_mmap_init is error %d\n", ret);
        return false;
    }
	
	if(!mStart)
	{
		if(!v4l2_preview_start())
		{
			LOGD("v4l2_preview_start() is error %s\n",__func__);
		}
	}

	if(!v4l2_set_brightness(4))
	{
		LOGD("v4l2_set_brightness is error\n");
	}
	if(!v4l2_set_contrast(4))
	{
		LOGD("v4l2_set_contrast is error\n");
	}
	if(!v4l2_set_saturation(4))
	{
		LOGD("v4l2_set_staturation is error\n");
	}

	return true;
}

bool V4l2::v4l2_capture_config()
{
	struct  v4l2_streamparm parm ;

    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    parm.parm.capture.timeperframe.numerator = 0;
    parm.parm.capture.timeperframe.denominator = 0;
    parm.parm.capture.capturemode = 1 ;          /* set as cap mode */

    if(-1 == ioctl(mFd, VIDIOC_S_PARM, &parm))
    {
        LOGD("Error: can't set input_still\n");
        return false;
    }

	mFormat.type                    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	mFormat.fmt.pix.pixelformat      = V4L2_PIX_FMT_YUV422P;
	mFormat.fmt.pix.width            = mWidth;
	mFormat.fmt.pix.height           = mHeight;
	
    if(-1 == ioctl(mFd, VIDIOC_S_FMT, &mFormat))
    {
    	LOGD("Error: VIDIOC_S_FMT is error\n");
        return false;
    }

    if(preview_map != NULL)
    {
    	LOGD("Error: v4l2_munmap\n");
        v4l2_munmap(preview_map, mMapLen);
    }
	bool ret = true;
    ret=v4l2_mmap_init(&preview_map, mMapLen);
    if(!ret)
    {
    	LOGD("Error: v4l2_mmap_init is error %d\n", ret);
        return false;
    }

	return true;
}

bool V4l2::v4l2_preview_start()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
/*
    v4l2_set_brightness_real();
    v4l2_set_contrast_real();
    v4l2_set_saturation_real();
*/
    if (-1 == ioctl (mFd, VIDIOC_STREAMON, &type)) //start capturing
    {
        LOGD("Failed(MMAP):VIDIOC_STREAMON\n");
        return false;
    }

	return true;
}

bool V4l2::v4l2_preview_stop()
{
    enum  v4l2_buf_type buftype = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(!v4l2_grab_sync())
    {
        LOGD("%s error\n", __func__);
    }
		
    if (-1 == ioctl(mFd, VIDIOC_STREAMOFF, &buftype))
    {
        LOGD("Error: can't stop streaming mFd = %d\n", mFd);
        return false;
    }

    return true;
}

bool V4l2::v4l2_set_preview_size(int w, int h)
{
	if (w >=0 && h >=0)
	{
		mWidth = w ; 
		mHeight = h;

		return true;
	}

	return false;
}

bool V4l2::v4l2_get_preview_size(int &w , int &h)
{
	w = mWidth;
	h = mHeight;

	return true;
}

bool V4l2::v4l2_set_preview_format(int Fmt)
{
	return true;
}

bool V4l2::v4l2_get_preview_format(int &Fmt)
{
	return true;
}

bool V4l2::v4l2_frame_data_release()
{
	if(ioctl(mFd,  VIDIOC_QBUF, &mBuffer) != 0)
	{
		LOGD("VIDIOC_QBUF:");
		return false;
	}

	return true;
}

bool V4l2:: getNextFrameAsYuv422(unsigned char *pSrc[3])
{
	int ret = 0;

    memset(&(mBuffer), 0, sizeof(struct  v4l2_buffer));
    mBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    mBuffer.memory = V4L2_MEMORY_MMAP;

    while(mDropFrame > 0)
    {
    	LOGD("drop frame %d\n",mDropFrame);
        if(!v4l2_grab_sync())
        {
            return false;
        }
        ret = ioctl (mFd, VIDIOC_DQBUF, &(mBuffer));
        if(ret == -1)
        {
            LOGD("VIDIOC_DQBUF");
            return false;
        }

        ret = ioctl(mFd,  VIDIOC_QBUF, &(mBuffer));
        if(ret == -1)
        {
            LOGD("VIDIOC_QBUF:");
            return false;
        }
        mDropFrame--;
    }

    if(!v4l2_grab_sync())
    {
        return false;
    }
    ret = ioctl (mFd, VIDIOC_DQBUF, &(mBuffer));
    if(ret == -1)
    {
        LOGD("VIDIOC_DQBUF");
        return false;
    }
    if(mBuffer.index > MAP_BUFFER_NUM)
    {
        LOGD("error buffer.index < MAP_BUFFER_NUM\n");
        return false;
    }
    switch(mFormat.fmt.pix.pixelformat)
    {
        case V4L2_PIX_FMT_YUV422P:
        {
            pSrc[0] = (unsigned char*)preview_map[mBuffer.index].start[0];
            pSrc[1] = (unsigned char*)preview_map[mBuffer.index].start[1];
            pSrc[2] = (unsigned char*)preview_map[mBuffer.index].start[2];
           // memcpy(buffer, t, mWidth*mHeight*2);
            break;
        }
        case V4L2_PIX_FMT_RGB565X:
        {
			uint8_t * t = (uint8_t*)preview_map[mBuffer.index].start[0];
			//	memcpy(buffer, t, mWidth*mHeight*2);

			break;
        }
        case V4L2_PIX_FMT_RGB24:
        {
            break;
        }
        default:
        {
            return false;
        }
    }
	
	if(!v4l2_frame_data_release())
	{
		LOGD("frame data release is wrong\n");
	};
	return true;
}

bool V4l2:: v4l2_take_picture(uint8_t *buffer)
{
	int ret = 0;
    memset(&(mBuffer), 0, sizeof(struct  v4l2_buffer));
    mBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    mBuffer.memory = V4L2_MEMORY_MMAP;
    while(mDropFrame > 0)
    {
    	LOGD("drop frame %d\n",mDropFrame);
        if(!v4l2_grab_sync())
        {
            return false;
        }
         ret = ioctl (mFd, VIDIOC_DQBUF, &(mBuffer));
        if(ret == -1)
        {
            LOGD("VIDIOC_DQBUF");
            return false;
        }

        ret = ioctl(mFd,  VIDIOC_QBUF, &(mBuffer));
        if(ret == -1)
        {
            LOGD("VIDIOC_QBUF:");
            return false;
        }
        mDropFrame--;
    }

    if(!v4l2_grab_sync())
    {
        return false;
    }
    ret = ioctl (mFd, VIDIOC_DQBUF, &(mBuffer));
    if(ret == -1)
    {
        LOGD("VIDIOC_DQBUF");
        return false;
    }
    if(mBuffer.index > MAP_BUFFER_NUM)
    {
        LOGD("error buffer.index < MAP_BUFFER_NUM\n");
        return false;
    }
    switch(mFormat.fmt.pix.pixelformat)
    {
        case V4L2_PIX_FMT_YUV422P:
        {
            uint8_t * t = (uint8_t*)preview_map[mBuffer.index].start[0];
            memcpy(buffer, t, mWidth*mHeight*2);
            break;
        }
        case V4L2_PIX_FMT_RGB565X:
        {
			uint8_t * t = (uint8_t*)preview_map[mBuffer.index].start[0];
			memcpy(buffer, t, mWidth*mHeight*2);

			break;
        }
        case V4L2_PIX_FMT_RGB24:
        {
            break;
        }
        default:
        {
            return false;
        }
    }
	
	if(!v4l2_frame_data_release())
	{
		LOGD("frame data release is wrong\n");
	};
	return true;
}
bool V4l2:: v4l2_set_brightness(int brightness)
{
    struct v4l2_control control = { 0 };

    control.id = V4L2_CID_BRIGHTNESS;
    control.value = brightness ; //0-4
    if(-1 == ioctl(mFd, VIDIOC_S_CTRL, &control))
    {
        LOGD("VIDIOC_S_CTRL");
        return false;
    }

    return true;
}

bool V4l2::v4l2_set_contrast(int Val)
{
    struct v4l2_control control = { 0 };

    control.id = V4L2_CID_CONTRAST;
    control.value = Val; //0-4
    if(-1 == ioctl(mFd, VIDIOC_S_CTRL, &control))
    {
        LOGD("VIDIOC_S_CTRL");
        return false;
    }

    return true;

}

bool V4l2::v4l2_set_saturation(int Val)
{
    struct v4l2_control control = { 0 };

    control.id = V4L2_CID_SATURATION;
    control.value = Val ; //0-5

    if(-1 == ioctl(mFd, VIDIOC_S_CTRL, &control))
    {
        LOGD("VIDIOC_S_CTRL");
        return false;
    }

    return true;
}

/*
status_t V4l2::dump(int fd, const Vector<String16>& args)
{
	const size_t SIZE = 256;
	char buffer[SIZE];
	String8 result;
	snprintf(buffer, 255, " width x height (%d x %d)\n", mWidth, mHeight);
	result.append(buffer);
	::write(fd, result.string(), result.size());
	return NO_ERROR;
}
*/

};
