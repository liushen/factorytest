
#ifndef _HW_LED_H_
#define _HW_LED_H_

#define HL_DEV_LCD_BRIGHT       "/sys/class/backlight/micco-bl/brightness"
#define HL_DEV_LCD_BRIGHT_MAX   "/sys/class/backlight/micco-bl/max_brightness"
#define HL_DEV_KP_BACKLIGHT     "/sys/class/backlight/micco-kp-bl/brightness"
#define HL_DEV_KP_BACKLIGHT_MAX "/sys/class/backlight/micco-kp-bl/max_brightness"
#define HL_DEV_FLASH_LIGHT      "/sys/class/timed_output/flashlight/enable"
#define HL_DEV_BALL_BRIGHT      "/sys/class/leds/jogball-backlight/brightness"
#define HL_DEV_LED_R_BRIGHT     "/sys/class/leds/red/brightness"
#define HL_DEV_LED_G_BRIGHT     "/sys/class/leds/green/brightness"
#define HL_DEV_LED_B_BRIGHT     "/sys/class/leds/blue/brightness"

#define HL_DEV_LED_FULL         255

enum
{
    HL_DEVICE_LCD = 0,
    HL_DEVICE_KEYBOARD,
    HL_DEVICE_FLASH_LIGHT,
    HL_DEVICE_TRACKBALL,
    HL_DEVICE_LED_R,
    HL_DEVICE_LED_G,
    HL_DEVICE_LED_B,
};

void hw_led_set(int device, int status);

int  hw_led_get(int device);

int  hw_led_get_range(int device, int *min, int *max);

#endif/*_HW_LED_H_*/
