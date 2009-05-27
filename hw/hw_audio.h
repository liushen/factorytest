#ifndef _HW_AUDIO_H_
#define _HW_AUDIO_H_

enum 
{
    HA_DEVICE_SPEAKER = 0,
    HA_DEVICE_RECEIVER
};

void hw_audio_play(int device, const char *file);

void hw_audio_stop(int device);

#endif/*_HW_AUDIO_H_*/
