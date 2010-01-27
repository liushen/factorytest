#undef LOG_TAG
#define LOG_TAG "Factory"

#include "ft_fm.h"
#include "ft_config.h"
#include "ft_textpad.h"
#include "gui/ft_button.h"
#include "hw/hw_fm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cutils/log.h>

static void on_fm_freq_handler(FTButton *button, void *data)
{
    FTWindow *window = (FTWindow *)data;

    char *freq = strdup(button->text);
    char *space = strchr(freq, ' ');

    if (space)
        *space = '\0';

    hw_fm_set_freq(atof(freq) * 1000);
    ft_textpad_set_text(window, freq);
    free(freq);
}

static void ft_fm_destroy(FTWidget *widget)
{
    ft_textpad_destroy(widget);
    hw_fm_stop();
}

FTWindow *ft_fm_new()
{
    FTWindow *window;
    FTWidget *widget;
    FTButton *button;

    window = ft_textpad_new("98.7", 1);
    widget = (FTWidget *)window;

    button = ft_button_new("98.7 MHz");
    ft_button_set_handler(button, on_fm_freq_handler, window);
    ft_window_add(window, (FTWidget *)button, 0);

    button = ft_button_new("94.2 MHz");
    ft_button_set_handler(button, on_fm_freq_handler, window);
    ft_window_add(window, (FTWidget *)button, 1);

    button = ft_button_new("100.4 MHz");
    ft_button_set_handler(button, on_fm_freq_handler, window);
    ft_window_add(window, (FTWidget *)button, 2);

    button = ft_button_new("102.5 MHz");
    ft_button_set_handler(button, on_fm_freq_handler, window);
    ft_window_add(window, (FTWidget *)button, 3);

    widget->destroy = ft_fm_destroy;
    ft_textpad_set_id(window, FT_ITEM_FM);

    hw_fm_start();
    hw_fm_set_freq(98.7 * 1000);

    return window;
}

