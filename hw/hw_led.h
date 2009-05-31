
#ifndef _HW_LED_H_
#define _HW_LED_H_

#define HL_DEV_LCD_BRIGHT   "/sys/class/backlight/broncho-backlight/brightness"
#define HL_DEV_KP_BACKLIGHT "/sys/class/flashlight/broncho-kp_backligh/power"

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
