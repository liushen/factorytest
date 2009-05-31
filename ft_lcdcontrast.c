#include "ft_lcdcontrast.h"
#include "gui/ft_button.h"
#include "gui/ft_draw.h"
#include "hw/hw_led.h"
#include <string.h>

extern FTColor ft_color_b;

static void ft_lcdcontrast_draw(FTWidget *widget)
{
    FTWindow *window = (FTWindow *)widget;
    FTWidget *w;
    FTDrawGC gc;
    FTRect rect;

    FBSurface *s = widget->surface;

    rect.x = 0;
    rect.y = 0;
    rect.width  = s->width;
    rect.height = s->height;

    gc = widget->gc;
    gc.foreground = ft_color_b;

    ft_draw_box(s, &rect, &gc, 1);

    FTList *iter = window->children;

    for (; iter; iter = iter->next)
    {
        w = (FTWidget *)iter->data;

        w->draw(w);
    }

    memcpy(window->buffer, s->buffer, s->size);
}

static void ft_lcdcontrast_adjust(FTButton *button, void *data)
{
    static int contrast = 0;

    if (button->text[strlen(button->text)-1] == '<')
    {
        if (contrast < 0xf8)
            contrast += 8;

        hw_led_set(HL_DEVICE_LCD, contrast);
    }
    else
    {
        if (contrast >= 0x8)
            contrast -= 8;

        hw_led_set(HL_DEVICE_LCD, --contrast);
    }
}

FTWindow *ft_lcdcontrast_new()
{
    FTWindow *window;
    FTWidget *widget;
    FTButton *button;

    window = ft_window_new();
    widget = (FTWidget *)window;

    button = ft_button_new("       <<");
    ft_button_set_handler(button, ft_lcdcontrast_adjust, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("       >>");
    ft_button_set_handler(button, ft_lcdcontrast_adjust, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    widget->draw = ft_lcdcontrast_draw;

    hw_led_set(HL_DEVICE_LCD, 0);

    return window;
}

