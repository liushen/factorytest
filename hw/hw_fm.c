#include "hw_fm.h"
#include "hw_comm.h"
#include "hw_audio.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int hw_detect_fm()
{
    return access(HW_FW_DEVICE, F_OK) == 0;
}

int hw_fm_start()
{
    hw_audio_set_route(HA_ROUTE_FM_HS);

    return hw_file_write(HW_FW_DEVICE, "start");
}

int hw_fm_stop()
{
    return hw_file_write(HW_FW_DEVICE, "stop");
}

int hw_fm_set_freq(int freq)
{
    char buf[16];

    sprintf(buf, "%d", freq);

    return hw_file_write(HW_FW_DEVICE, buf);
}

int hw_fm_get_freq()
{
    int freq = 0;

    char *text = hw_file_read(HW_FW_DEVICE, 16);

    if (text != NULL)
    {
        freq = atoi(text);
        free(text);
    }

    return freq;
}

