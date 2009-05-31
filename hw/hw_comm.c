#include "hw_comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VER_LEN_MAX     (255)
#define ADC_LEN_MAX     (255)
#define BUF_LEN_MAX     (128)

const char *hw_get_version()
{
    static char version[VER_LEN_MAX];

    //TODO
    snprintf(version, VER_LEN_MAX, "Version No: ******\nSerial No: ******");

    return version;
}

const char *hw_get_ADC()
{
    static char adc[ADC_LEN_MAX];

    //TODO
    snprintf(adc, ADC_LEN_MAX, "VBAT: 0.00V\nVAUX: 0.00V\nCurrent: 0.00A");

    return adc;
}

void hw_vibrator_set(int status)
{
    char exec[BUF_LEN_MAX];

    snprintf(exec, BUF_LEN_MAX, "echo %d > %s", status, HW_DEV_VIBRATOR);
    system(exec);
}

int hw_get_camera_nr()
{
    //TODO
    return 0;
}

int hw_detect_bluetooth()
{
    //TODO
    return 0;
}

int hw_detect_wifi()
{
    //TODO
    return 0;
}

int hw_detect_fm()
{
    //TODO
    return 0;
}

int hw_detect_gps()
{
    //TODO
    return 0;
}
