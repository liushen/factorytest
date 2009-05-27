#include "hw_comm.h"
#include <stdio.h>
#include <string.h>

#define VER_LEN_MAX     (255)
#define ADC_LEN_MAX     (255)

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

int hw_get_camera_nr()
{
    //TODO
    return 0;
}

void hw_vibrator_set(int status)
{
    //TODO
}

