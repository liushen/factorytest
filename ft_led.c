#include "ft_led.h"
#include "ft_config.h"
#include "gui/ft_button.h"
#include "hw/hw_led.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

typedef struct _FLContext FLContext;

struct _FLContext
{
    int result;
};

static FLContext fl_context;

extern FTColor ft_color_r;
extern FTColor ft_color_g;
extern FTColor ft_color_w;

static FTWindow *ft_lcm_window_new();

static void on_ok_handler(FTButton *button, void *data)
{
    fl_context.result = FT_STATUS_OK;

    ft_window_close(data);
}

static void on_fail_handler(FTButton *button, void *data)
{
    fl_context.result = FT_STATUS_FAIL;

    ft_window_close(data);
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
    char key[32];

    sprintf(key, "%d", FT_ITEM_LED);

    ft_config_set_int(key, fl_context.result);
    ft_window_destroy(widget);
}

FTWindow *ft_led_new()
{
    FTWindow *window;
    FTWidget *widget;
    FTButton *button;
    FBSurface *s;
    FTRect *rect;

    window = ft_window_new();
    widget = (FTWidget *)window;

    s = widget->surface;

    button = ft_button_new("Keyboard LED");
    ft_button_set_handler(button, on_keyboard_led_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Trackball");
    ft_button_set_handler(button, on_trackball_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Flashlight");
    ft_button_set_handler(button, on_flash_light_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("RGB");
    ft_button_set_handler(button, on_rgb_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Main LCM BL");
    ft_button_set_handler(button, on_lcm_bl_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("       OK");

    rect = &((FTWidget *)button)->rect;
    rect->x = FT_WIDGET_SPACING;
    rect->y = s->height - FT_WIDGET_HEIGHT - FT_WIDGET_SPACING;
    rect->width = s->width / 2 - FT_WIDGET_SPACING;
    rect->height = FT_WIDGET_HEIGHT;

    ft_button_set_color(button, &ft_color_g);
    ft_button_set_handler(button, on_ok_handler, window);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("      FAIL");

    rect = &((FTWidget *)button)->rect;
    rect->x = s->width / 2 + FT_WIDGET_SPACING;
    rect->y = s->height - FT_WIDGET_HEIGHT - FT_WIDGET_SPACING;
    rect->width = s->width / 2 - FT_WIDGET_SPACING;
    rect->height = FT_WIDGET_HEIGHT;

    ft_button_set_color(button, &ft_color_r);
    ft_button_set_handler(button, on_fail_handler, window);
    ft_window_add_child(window, (FTWidget *)button);

    fl_context.result = FT_STATUS_NORMAL;
    widget->destroy = ft_led_destroy;

    ft_window_show(window);

    return window;
}

