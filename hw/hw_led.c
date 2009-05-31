#include "hw_led.h"
#include <stdio.h>
#include <stdlib.h>

#define BUF_LEN_MAX     (128)

void hw_led_set(int device, int status)
{
    char exec[BUF_LEN_MAX];

    switch (device)
    {
        case HL_DEVICE_LCD:
            snprintf(exec, BUF_LEN_MAX, "echo %d > %s", status, HL_DEV_LCD_BRIGHT);
            system(exec);
            break;
            
        case HL_DEVICE_KEYBOARD:
            snprintf(exec, BUF_LEN_MAX, "echo %d > %s", status, HL_DEV_KP_BACKLIGHT);
            system(exec);
            break;

        default:break;
    }
}

