#include "ft_led.h"
#include "ft_config.h"
#include "ft_textpad.h"
#include "gui/ft_button.h"
#include "hw/hw_led.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern FTColor ft_color_r;
extern FTColor ft_color_g;
extern FTColor ft_color_w;

static FTWindow *ft_lcm_window_new();

static void on_all_led_handler(FTButton *button, void *data)
{
    if (strcmp(button->text, "All LEDs") == 0)
    {
        int max = 0;

        ft_button_set_text(button, "All LEDs ");
        ft_button_set_color(button, &ft_color_g);

        hw_led_get_range(HL_DEVICE_KEYBOARD, NULL, &max);
        hw_led_set(HL_DEVICE_KEYBOARD, max ? max : 1);
        hw_led_set(HL_DEVICE_TRACKBALL, HL_DEV_LED_FULL);
        hw_led_set(HL_DEVICE_FLASH_LIGHT, -1);

        hw_led_set(HL_DEVICE_LED_R, HL_DEV_LED_FULL);
        usleep(200000);
        hw_led_set(HL_DEVICE_LED_G, HL_DEV_LED_FULL);
        usleep(200000);
        hw_led_set(HL_DEVICE_LED_B, HL_DEV_LED_FULL);
        usleep(200000);
    }
    else
    {
        ft_button_set_text(button, "All LEDs");
        ft_button_set_color(button, &ft_color_w);

        hw_led_set(HL_DEVICE_KEYBOARD, 0);
        hw_led_set(HL_DEVICE_TRACKBALL, 0);
        hw_led_set(HL_DEVICE_FLASH_LIGHT, 0);
        hw_led_set(HL_DEVICE_LED_R, 0);
        hw_led_set(HL_DEVICE_LED_G, 0);
        hw_led_set(HL_DEVICE_LED_B, 0);
    }
}

static void on_keyboard_led_handler(FTButton *button, void *data)
{
    int max = 0;

    hw_led_get_range(HL_DEVICE_KEYBOARD, NULL, &max);
    hw_led_set(HL_DEVICE_KEYBOARD, max ? max : 1);
    ft_button_set_color(button, &ft_color_g);

    usleep(200000);

    hw_led_set(HL_DEVICE_KEYBOARD, 0);
    ft_button_set_color(button, &ft_color_w);
}

static void on_trackball_handler(FTButton *button, void *data)
{
    hw_led_set(HL_DEVICE_TRACKBALL, HL_DEV_LED_FULL);
    ft_button_set_color(button, &ft_color_g);

    usleep(200000);

    hw_led_set(HL_DEVICE_TRACKBALL, 0);
    ft_button_set_color(button, &ft_color_w);
}

static void on_flash_light_handler(FTButton *button, void *data)
{
    hw_led_set(HL_DEVICE_FLASH_LIGHT, -1);
    ft_button_set_color(button, &ft_color_g);

    usleep(200000);

    hw_led_set(HL_DEVICE_FLASH_LIGHT, 0);
    ft_button_set_color(button, &ft_color_w);
}

static void on_rgb_handler(FTButton *button, void *data)
{
    ft_button_set_color(button, &ft_color_g);

    hw_led_set(HL_DEVICE_LED_R, HL_DEV_LED_FULL);
    usleep(200000);

    hw_led_set(HL_DEVICE_LED_G, HL_DEV_LED_FULL);
    usleep(200000);

    hw_led_set(HL_DEVICE_LED_B, HL_DEV_LED_FULL);
    usleep(200000);

    hw_led_set(HL_DEVICE_LED_R, 0);
    hw_led_set(HL_DEVICE_LED_G, 0);
    hw_led_set(HL_DEVICE_LED_B, 0);

    ft_button_set_color(button, &ft_color_w);
}

static void on_lcm_bl_handler(FTButton *button, void *data)
{
    int max = 0, value, step, i;

    if (!hw_led_get_range(HL_DEVICE_LCD, NULL, &max))
    {
        return;
    }

    ft_button_set_color(button, &ft_color_g);

    value = hw_led_get(HL_DEVICE_LCD);
    step = max / 8;

    for (i = 0; i < max; i += step)
    {
        hw_led_set(HL_DEVICE_LCD, i);

        usleep(100 * 1000);
    }

    hw_led_set(HL_DEVICE_LCD, value);
    ft_button_set_color(button, &ft_color_w);
}

static void ft_lcm_window_draw(FTWidget *widget)
{
}

static void ft_lcm_window_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;

    if (event->type == FE_KEY_RELEASE ||
        event->type == FE_MOUSE_RELEASE)
    {
        ft_window_close(window);
    }
}

static FTWindow *ft_lcm_window_new()
{
    FTWindow *window;
    FTWidget *widget;

    window = ft_window_new();
    widget = (FTWidget *)window;

    widget->draw = ft_lcm_window_draw;
    widget->handler = ft_lcm_window_handler;
    widget->data = window;

    return window;
}

void ft_led_destroy(FTWidget *widget)
{
    ft_textpad_destroy(widget);

    hw_led_set(HL_DEVICE_KEYBOARD, 0);
    hw_led_set(HL_DEVICE_TRACKBALL, 0);
    hw_led_set(HL_DEVICE_FLASH_LIGHT, 0);
    hw_led_set(HL_DEVICE_LED_R, 0);
    hw_led_set(HL_DEVICE_LED_G, 0);
    hw_led_set(HL_DEVICE_LED_B, 0);
}

FTWindow *ft_led_new()
{
    FTWindow *window;
    FTWidget *widget;
    FTButton *button, *all_button;

    window = ft_textpad_new(NULL, 0);
    widget = (FTWidget *)window;

    button = ft_button_new("Keyboard LED");
    ft_button_set_handler(button, on_keyboard_led_handler, NULL);
    ft_window_add(window, (FTWidget *)button, 0);

    button = ft_button_new("Trackball");
    ft_button_set_handler(button, on_trackball_handler, NULL);
    ft_window_add(window, (FTWidget *)button, 1);

    button = ft_button_new("Flashlight");
    ft_button_set_handler(button, on_flash_light_handler, NULL);
    ft_window_add(window, (FTWidget *)button, 2);

    button = ft_button_new("RGB");
    ft_button_set_handler(button, on_rgb_handler, NULL);
    ft_window_add(window, (FTWidget *)button, 3);

    button = ft_button_new("Main LCM BL");
    ft_button_set_handler(button, on_lcm_bl_handler, NULL);
    ft_window_add(window, (FTWidget *)button, 4);

    all_button = ft_button_new("All LEDs");
    ft_button_set_handler(all_button, on_all_led_handler, NULL);
    ft_window_add(window, (FTWidget *)all_button, 5);

    widget->destroy = ft_led_destroy;
    ft_window_show(window);

    ft_textpad_set_id(window, FT_ITEM_LED);

    return window;
}

