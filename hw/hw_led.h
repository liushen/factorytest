
#ifndef _HW_LED_H_
#define _HW_LED_H_

enum
{
    HL_DEVICE_LCD = 0,
    HL_DEVICE_KEYBOARD,
    HL_DEVICE_TRACKBALL
};

enum
{
    HL_COLOR_RED = 1,
    HL_COLOR_GREEN,
    HL_COLOR_BLUE
};

void hw_led_set(int device, int status);

#endif/*_HW_LED_H_*/
