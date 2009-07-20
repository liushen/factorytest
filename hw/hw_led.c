#include "hw_led.h"
#include "hw_comm.h"
#include <stdio.h>
#include <stdlib.h>

#define BUF_LEN_MAX     (128)

void hw_led_set(int device, int status)
{
    char buf[BUF_LEN_MAX];

    sprintf(buf, "%d", status);

    switch (device)
    {
        case HL_DEVICE_LCD:
            hw_file_write(HL_DEV_LCD_BRIGHT, buf);
            break;
            
        case HL_DEVICE_KEYBOARD:
            hw_file_write(HL_DEV_KP_BACKLIGHT, buf);
            break;

        default:break;
    }
}

int hw_led_get_range(int device, int *min, int *max)
{
    char *text = NULL;

    if (device == HL_DEVICE_LCD)
        text = hw_file_read(HL_DEV_LCD_BRIGHT_MAX, 0);

    if (device == HL_DEVICE_KEYBOARD)
        text = hw_file_read(HL_DEV_KP_BACKLIGHT_MAX, 0);

    if (text)
    {
        if (max)
            *max = atoi(text);

        if (min)
            *min = 0;

        free(text);

        return 1;
    }

    return 0;
}

