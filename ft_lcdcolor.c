#include "ft_lcdcolor.h"
#include "gui/ft_button.h"
#include <string.h>

static void ft_color_window_draw(FTWidget *widget)
{
    FBSurface *s = widget->surface;
    FTRect rect;
    
    rect.x = 0;
    rect.y = 0;
    rect.width  = s->width;
    rect.height = s->height;

    ft_draw_box(s, &rect, &widget->gc, 1);
}

static FTWindow *ft_color_window_new(FTDrawGC *gc)
{
    FTWindow *window;
    FTWidget *widget;

    window = ft_window_new();
    widget = (FTWidget *)window;

    widget->gc = *gc;
    widget->draw = ft_color_window_draw;

    return window;
}

static FTDrawGC ft_lcdcolor_parse(const char *color)
{
    FTDrawGC gc;
    int i;

    const char *descs[] = {"Red", "Green", "Blue", "White", "Black"};

    const FTColor colors[FT_N_ELEMENTS(descs)] = 
    {
        {0xff, 0, 0, 0},
        {0, 0xff, 0, 0},
        {0, 0, 0xff, 0},
        {0xff, 0xff, 0xff, 0},
        {0, 0, 0, 0}
    };

    memset(&gc, 0, sizeof(FTDrawGC));

    for (i = 0; i < FT_N_ELEMENTS(descs); i++)
    {
        if (strcmp(color, descs[i]) == 0)
        {
            gc.foreground = colors[i];
            break;
        }
    }

    return gc;
}

static void ft_lcdcolor_demo(FTButton *button, void *data)
{
}

static void ft_lcdcolor_display(FTButton *button, void *data)
{
    FTWidget *widget = (FTWidget *)button;
    FTWindow *window;
    FTDrawGC gc;

    gc = ft_lcdcolor_parse(button->text);

    window = ft_color_window_new(&gc);
    ft_window_show(window);
}

FTWindow *ft_lcdcolor_new()
{
    FTWindow *window;
    FTButton *button;

    window = ft_window_new();

    button = ft_button_new("Auto display");
    ft_button_set_handler(button, ft_lcdcolor_demo, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Red");
    ft_button_set_handler(button, ft_lcdcolor_display, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Green");
    ft_button_set_handler(button, ft_lcdcolor_display, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Blue");
    ft_button_set_handler(button, ft_lcdcolor_display, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("White");
    ft_button_set_handler(button, ft_lcdcolor_display, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("Black");
    ft_button_set_handler(button, ft_lcdcolor_display, NULL);
    ft_window_add_child(window, (FTWidget *)button);

    return window;
}

