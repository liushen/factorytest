#ifndef _HW_AUDIO_H_
#define _HW_AUDIO_H_

enum 
{
    HA_DEVICE_SPEAKER = 0,
    HA_DEVICE_RECEIVER,
    HA_DEVICE_HANDSET
};

void hw_audio_play(int device, const char *file);

void hw_audio_stop(int device);

void hw_audio_echoloop_set(int device, int status);

#endif/*_HW_AUDIO_H_*/
