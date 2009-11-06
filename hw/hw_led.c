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

        case HL_DEVICE_TRACKBALL:
            hw_file_write(HL_DEV_BALL_BRIGHT, buf);
            break;

        case HL_DEVICE_CHARGE:
            hw_file_write(HL_DEV_CHARGE_BRIGHT, buf);
            break;

        case HL_DEVICE_FLASH_LIGHT:
            hw_file_write(HL_DEV_FLASH_LIGHT, buf);
            break;

        default:break;
    }
}

int hw_led_get(int device)
{
    char *text = NULL;
    int value = 0;

    switch (device)
    {
        case HL_DEVICE_LCD:
            text = hw_file_read(HL_DEV_LCD_BRIGHT, BUF_LEN_MAX);
            break;
            
        case HL_DEVICE_KEYBOARD:
            text = hw_file_read(HL_DEV_KP_BACKLIGHT, BUF_LEN_MAX);
            break;

        case HL_DEVICE_TRACKBALL:
            text = hw_file_read(HL_DEV_BALL_BRIGHT, BUF_LEN_MAX);
            break;

        case HL_DEVICE_CHARGE:
            text = hw_file_read(HL_DEV_CHARGE_BRIGHT, BUF_LEN_MAX);
            break;

        case HL_DEVICE_FLASH_LIGHT:
            text = hw_file_read(HL_DEV_FLASH_LIGHT, BUF_LEN_MAX);
            break;

        default:break;
    }

    if (text != NULL)
    {
        value = atoi(text);
        free(text);
    }

    return value;
}

int hw_led_get_range(int device, int *min, int *max)
{
    char *text = NULL;

    switch (device)
    {
        case HL_DEVICE_LCD:
            text = hw_file_read(HL_DEV_LCD_BRIGHT_MAX, 0);
            break;

        case HL_DEVICE_KEYBOARD:
            text = hw_file_read(HL_DEV_KP_BACKLIGHT_MAX, 0);
            break;

        case HL_DEVICE_TRACKBALL:
            text = hw_file_read(HL_DEV_BALL_BRIGHT_MAX, 0);
            break;

        default: break;
    }

    if (text == NULL)
        return 0;

    if (max)
        *max = atoi(text);

    if (min)
        *min = 0;

    free(text);

    return 1;
}

