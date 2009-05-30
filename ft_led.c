#include "ft_led.h"
#include "gui/ft_button.h"
#include <string.h>

static FTWindow *ft_lcm_window_new();

static void on_keyboard_led_handler(FTButton *button, void *data)
{
}

static void on_status_led_r_handler(FTButton *button, void *data)
{
}

static void on_status_led_g_handler(FTButton *button, void *data)
{
}

static void on_status_led_b_handler(FTButton *button, void *data)
{
}

static void on_lcm_bl_handler(FTButton *button, void *data)
{
    FTWindow *window = ft_lcm_window_new();
    ft_window_show(window);
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

FTWindow *ft_led_new()
{
    FTWindow *window;
    FTWidget *widget;
    FTButton *button;

    window = ft_window_new();
    widget = (FTWidget *)window;

    button = ft_button_new("Keyboard LED");
    ft_button_set_handler(button, on_keyboard_led_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Status LED R");
    ft_button_set_handler(button, on_status_led_r_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Status LED G");
    ft_button_set_handler(button, on_status_led_g_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Status LED B");
    ft_button_set_handler(button, on_status_led_b_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Main LCM BL");
    ft_button_set_handler(button, on_lcm_bl_handler, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    ft_window_show(window);

    return window;
}

