#include "hw_audio.h"
#include <stdlib.h>
#include <unistd.h>

#define HA_PLAY_EXEC    "alsa_aplay"

void hw_audio_play(int device, const char *file)
{
    if (device == HA_DEVICE_HANDSET)
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

        if (device == HA_DEVICE_HANDSET)
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
        case HA_ROUTE_ECHOLOOP_HS:
            system("alsa_amixer cset name='STEREO_CH1 Mixer DAC3 Switch' 1");
            system("alsa_amixer cset name='STEREO_CH2 Mixer DAC3 Switch' 1");
            system("alsa_amixer cset name='MIC Mux' 2");

            system("alsa_amixer cset name='Stereo Ch1 Volume' 50");
            system("alsa_amixer cset name='Stereo Ch2 Volume' 50");
            break;

        case HA_ROUTE_SPK:
        case HA_ROUTE_ECHOLOOP_SPK:
            system("alsa_amixer cset name='MONO Mixer DAC3 Switch' 1");
            system("alsa_amixer cset name='MIC Mux' 1");
            system("alsa_amixer cset name='Mono Volume' 50");
            break;

        case HA_ROUTE_CALL_HS:
        case HA_ROUTE_CALL_SPK:
            system("alsa_amixer cset name='STEREO_CH2 Mixer AUX2 Switch' 0");
            system("alsa_amixer cset name='STEREO_CH1 Mixer AUX1 Switch' 1");
            system("alsa_amixer cset name='STEREO_CH2 Mixer AUX2inv Switch' 1");
            system("alsa_amixer cset name='LINE_OUT Mixer MICP Switch' 1");
            system("alsa_amixer cset name='BEAR Mixer AUX1 Switch' 1");
            system("alsa_amixer cset name='BEAR Mixer AUX2inv Switch' 1");
            system("alsa_amixer cset name='MIC Mux' 2");

            system("alsa_amixer cset name='Stereo Ch1 Volume' 55");
            system("alsa_amixer cset name='Stereo Ch2 Volume' 55");
            system("alsa_amixer cset name='AUX1 Volume' 3");
            system("alsa_amixer cset name='AUX2 Volume' 3");
            system("alsa_amixer cset name='Bear Volume' 63");
            system("alsa_amixer cset name='Mic Volume' 2");

            /* Speaker on */
            system("alsa_amixer cset name='MONO Mixer AUX1 Switch' 1");
            system("alsa_amixer cset name='MONO Mixer AUX2inv Switch' 1");
            system("alsa_amixer cset name='Mono Volume' 60");
            break;

        case HA_ROUTE_CALL_RCV:
            system("alsa_amixer cset name='LINE_OUT Mixer MICP Switch' 1");
            system("alsa_amixer cset name='BEAR Mixer AUX1 Switch' 1");
            system("alsa_amixer cset name='BEAR Mixer AUX2inv Switch' 1");
            system("alsa_amixer cset name='MIC Mux' 1");

            system("alsa_amixer cset name='AUX1 Volume' 3");
            system("alsa_amixer cset name='AUX2 Volume' 3");
            system("alsa_amixer cset name='Bear Volume' 63");
            system("alsa_amixer cset name='Mic Volume' 5");
            break;

        case HA_ROUTE_FM_HS:
            system("alsa_amixer cset name='STEREO_CH2 Mixer AUX2inv Switch' 0");
            system("alsa_amixer cset name='STEREO_CH1 Mixer AUX1 Switch' 1");
            system("alsa_amixer cset name='STEREO_CH2 Mixer AUX2 Switch' 1");
            system("alsa_amixer cset name='FM Switch Allow' 1");

            system("alsa_amixer cset name='FM Volume' 15");
            system("alsa_amixer cset name='Stereo Ch1 Volume' 63");
            system("alsa_amixer cset name='Stereo Ch2 Volume' 63");
            system("alsa_amixer cset name='AUX1 Volume' 3");
            system("alsa_amixer cset name='AUX2 Volume' 3");
            break;

        case HA_ROUTE_FM_SPK:
            system("alsa_amixer cset name='MONO Mixer AUX1 Switch' 1");
            system("alsa_amixer cset name='MONO Mixer AUX2 Switch' 1");
            system("alsa_amixer cset name='FM Switch Allow' 1");

            system("alsa_amixer cset name='FM Volume' 15");
            system("alsa_amixer cset name='Mono Volume' 63");
            system("alsa_amixer cset name='AUX1 Volume' 3");
            system("alsa_amixer cset name='AUX2 Volume' 3");
            break;

        default: break;
    }
}

