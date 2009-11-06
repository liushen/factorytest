
#ifndef _HW_LED_H_
#define _HW_LED_H_

#define HL_DEV_LCD_BRIGHT       "/sys/class/backlight/micco-bl/brightness"
#define HL_DEV_LCD_BRIGHT_MAX   "/sys/class/backlight/micco-bl/max_brightness"
#define HL_DEV_KP_BACKLIGHT     "/sys/class/backlight/micco-kp-bl/brightness"
#define HL_DEV_KP_BACKLIGHT_MAX "/sys/class/backlight/micco-kp-bl/max_brightness"
#define HL_DEV_BALL_BRIGHT      "/sys/class/backlight/pxa3xx_pwm_bl2/brightness"
#define HL_DEV_BALL_BRIGHT_MAX  "/sys/class/backlight/pxa3xx_pwm_bl2/max_brightness"
#define HL_DEV_CHARGE_BRIGHT    "/sys/class/leds/micco-leds/brightness"
#define HL_DEV_FLASH_LIGHT      "/sys/class/timed_output/flashlight/enable"

enum
{
    HL_DEVICE_LCD = 0,
    HL_DEVICE_KEYBOARD,
    HL_DEVICE_TRACKBALL,
    HL_DEVICE_CHARGE,
    HL_DEVICE_FLASH_LIGHT
};

enum
{
    HL_COLOR_RED = 1,
    HL_COLOR_GREEN,
    HL_COLOR_BLUE
};

void hw_led_set(int device, int status);

int  hw_led_get(int device);

int  hw_led_get_range(int device, int *min, int *max);

#endif/*_HW_LED_H_*/
