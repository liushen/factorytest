#define LOG_TAG "Factory"

#include "hw_audio.h"
#include "hw_comm.h"
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <cutils/log.h>
#include <alsa/asoundlib.h>

#define HA_PLAY_EXEC    "alsa_aplay"

int hw_headset_get_state()
{
    int state = 0;

    char *text = hw_file_read("/sys/class/switch/h2w/state", 8);

    if (text && atoi(text))
    {
        state = 1;
        free(text);
    }

    return state;
}

int hw_headset_get_key()
{
    int state = 0;

    char *text = hw_file_read("/sys/class/switch/hook/state", 8);

    if (text && atoi(text))
    {
        state = 1;
        free(text);
    }

    return state;
}

void hw_audio_play(int device, const char *file)
{
    if (device == HA_DEVICE_HEADSET)
    {
        hw_audio_set_route(HA_ROUTE_HS);
    }
    else /* HA_DEVICE_SPEAKER */
    {
        hw_audio_set_route(HA_ROUTE_SPK);
    }

    if (fork() == 0)
    {
        execlp(HA_PLAY_EXEC, HA_PLAY_EXEC, file, NULL);
        exit(0);
    }
}

void hw_audio_stop(int device)
{
    system("killall -KILL "HA_PLAY_EXEC);
}

void hw_audio_echoloop_set(int device, int status)
{
    if (status)
    {
        if (fork() == 0)
        {
            execlp(HA_PLAY_EXEC, HA_PLAY_EXEC, "echoloop", NULL);
            exit(0);
        }

        usleep(100 * 1000);

        if (device == HA_DEVICE_HEADSET)
        {
            hw_audio_set_route(HA_ROUTE_ECHOLOOP_HS);
        }
        else /* HA_DEVICE_SPEAKER */
        {
            hw_audio_set_route(HA_ROUTE_ECHOLOOP_SPK);
        }
    }
    else
    {
        system("killall -KILL "HA_PLAY_EXEC);
    }
}

void hw_audio_set_route(int route)
{
    switch(route)
    {
        case HA_ROUTE_HS:
            hw_audio_set("STEREO_CH1 Mixer DAC3 Switch", 1);
            hw_audio_set("STEREO_CH2 Mixer DAC3 Switch", 1);
            hw_audio_set("Stereo Ch1 Volume", 55);
            hw_audio_set("Stereo Ch2 Volume", 55);
            break;

        case HA_ROUTE_ECHOLOOP_HS:
            hw_audio_set("STEREO_CH1 Mixer DAC3 Switch", 1);
            hw_audio_set("STEREO_CH2 Mixer DAC3 Switch", 1);
            hw_audio_set("MIC Mux", 2);
            break;

        case HA_ROUTE_SPK:
            hw_audio_set("MONO Mixer DAC3 Switch", 1);
            hw_audio_set("Mono Volume", 55);
            break;

        case HA_ROUTE_ECHOLOOP_SPK:
            hw_audio_set("BEAR Mixer DAC3 Switch", 1);
            hw_audio_set("MIC Mux", 1);
            break;

        case HA_ROUTE_CALL_HS:
        case HA_ROUTE_CALL_SPK:
            hw_audio_set("STEREO_CH2 Mixer AUX2 Switch", 0);
            hw_audio_set("STEREO_CH1 Mixer AUX1 Switch", 1);
            hw_audio_set("STEREO_CH2 Mixer AUX2inv Switch", 1);
            hw_audio_set("LINE_OUT Mixer MICP Switch", 1);
            hw_audio_set("BEAR Mixer AUX1 Switch", 1);
            hw_audio_set("BEAR Mixer AUX2inv Switch", 1);
            hw_audio_set("MIC Mux", 2);

            hw_audio_set("Stereo Ch1 Volume", 55);
            hw_audio_set("Stereo Ch2 Volume", 55);
            hw_audio_set("AUX1 Volume", 0);
            hw_audio_set("AUX2 Volume", 0);
            hw_audio_set("Bear Volume", 55);
            hw_audio_set("Mic Volume", 2);

            hw_audio_set("MONO Mixer AUX1 Switch", 1);
            hw_audio_set("MONO Mixer AUX2inv Switch", 1);
            hw_audio_set("Mono Volume", 55);
            break;

        case HA_ROUTE_CALL_RCV:
            hw_audio_set("LINE_OUT Mixer MICP Switch", 1);
            hw_audio_set("BEAR Mixer AUX1 Switch", 1);
            hw_audio_set("BEAR Mixer AUX2inv Switch", 1);
            hw_audio_set("MIC Mux", 1);

            hw_audio_set("AUX1 Volume", 0);
            hw_audio_set("AUX2 Volume", 0);
            hw_audio_set("Bear Volume", 55);
            hw_audio_set("Mic Volume", 5);
            break;

        case HA_ROUTE_FM_HS:
            hw_audio_set("STEREO_CH2 Mixer AUX2inv Switch", 0);
            hw_audio_set("STEREO_CH1 Mixer AUX1 Switch", 1);
            hw_audio_set("STEREO_CH2 Mixer AUX2 Switch", 1);
            hw_audio_set("FM Switch Allow", 1);

            hw_audio_set("FM Volume", 15);
            hw_audio_set("Stereo Ch1 Volume", 55);
            hw_audio_set("Stereo Ch2 Volume", 55);
            hw_audio_set("AUX1 Volume", 0);
            hw_audio_set("AUX2 Volume", 0);
            break;

        case HA_ROUTE_FM_SPK:
            hw_audio_set("MONO Mixer AUX1 Switch", 1);
            hw_audio_set("MONO Mixer AUX2 Switch", 1);
            hw_audio_set("FM Switch Allow", 1);

            hw_audio_set("FM Volume", 15);
            hw_audio_set("Mono Volume", 55);
            hw_audio_set("AUX1 Volume", 0);
            hw_audio_set("AUX2 Volume", 0);
            break;

        default: break;
    }
}

int hw_audio_set(const char *name, int value)
{
    static snd_ctl_t *handle = NULL;
    int err;

    snd_ctl_elem_id_t *id;
    snd_ctl_elem_info_t *info;
    snd_ctl_elem_value_t *control;

    snd_ctl_elem_id_alloca(&id);
    snd_ctl_elem_info_alloca(&info);
    snd_ctl_elem_value_alloca(&control);
	
    snd_ctl_elem_id_set_interface(id, SND_CTL_ELEM_IFACE_MIXER);
    snd_ctl_elem_id_set_name(id, name);
    snd_ctl_elem_info_set_id(info, id);

    if (handle == NULL &&
        (err = snd_ctl_open(&handle, "default", 0)) < 0)
    {
        LOGE("Control default open error: %s\n", snd_strerror(err));
        return 0;
    }

    int ret = snd_ctl_elem_info(handle, info);
    if (ret < 0) 
    {
        LOGE("Cannot find the given element from control default ret=%d %s ", 
                ret, snd_strerror(ret));
        return 0;
    }

    snd_ctl_elem_type_t type = snd_ctl_elem_info_get_type(info);
    snd_ctl_elem_info_get_id(info, id);
    snd_ctl_elem_value_set_id(control, id);

    switch (type) {
        case SND_CTL_ELEM_TYPE_BOOLEAN:
            snd_ctl_elem_value_set_boolean(control, 0, value);
            break;
        case SND_CTL_ELEM_TYPE_INTEGER:
            snd_ctl_elem_value_set_integer(control, 0, value);
            break;
        case SND_CTL_ELEM_TYPE_INTEGER64:
            snd_ctl_elem_value_set_integer64(control, 0, value);
            break;
        case SND_CTL_ELEM_TYPE_ENUMERATED:
            snd_ctl_elem_value_set_enumerated(control, 0, value);
            break;
        case SND_CTL_ELEM_TYPE_BYTES:
            snd_ctl_elem_value_set_byte(control, 0, value);
            break;
        default:
            break;
    }

    ret = snd_ctl_elem_write(handle, control);
    if(ret <0)
    {
        LOGE("Control default load error: %s\n", snd_strerror(ret));
    }

    return 1;
}

