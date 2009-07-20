#include "hw_audio.h"
#include <stdlib.h>
#include <unistd.h>

#define HA_PLAY_EXEC    "alsa_aplay"

void hw_audio_play(int device, const char *file)
{
    if (device == HA_DEVICE_HANDSET)
    {
        system("alsa_amixer cset name='STEREO_CH1 Mixer DAC3 Switch' 1");
        system("alsa_amixer cset name='STEREO_CH2 Mixer DAC3 Switch' 1");
    }
    else
    {
        system("alsa_amixer cset name='MONO Mixer DAC3 Switch' 1");
    }

    if (vfork() == 0)
    {
        execlp(HA_PLAY_EXEC, HA_PLAY_EXEC, file, NULL);
        _exit(0);
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
        if (vfork() == 0)
        {
            execlp(HA_PLAY_EXEC, HA_PLAY_EXEC, "echoloop", NULL);
            _exit(0);
        }
    }
    else
    {
        system("killall -KILL "HA_PLAY_EXEC);
    }
}

