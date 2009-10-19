#ifndef _HW_AUDIO_H_
#define _HW_AUDIO_H_

enum 
{
    HA_DEVICE_SPEAKER = 0,
    HA_DEVICE_RECEIVER,
    HA_DEVICE_HANDSET,
};

enum
{
    HA_ROUTE_HS = 0,
    HA_ROUTE_SPK,
    HA_ROUTE_CALL_HS,
    HA_ROUTE_CALL_SPK,
    HA_ROUTE_CALL_RCV,
    HA_ROUTE_FM_HS,
    HA_ROUTE_FM_SPK,
    HA_ROUTE_ECHOLOOP_HS,
    HA_ROUTE_ECHOLOOP_SPK,
};

void hw_audio_play(int device, const char *file);

void hw_audio_stop(int device);

void hw_audio_echoloop_set(int device, int status);

void hw_audio_set_route(int route);

#endif/*_HW_AUDIO_H_*/
