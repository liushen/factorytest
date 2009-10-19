#include "ft_lcdcolor.h"
#include "ft_config.h"
#include "gui/ft_list.h"
#include "gui/ft_button.h"

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

typedef struct _FLContext FLContext;

struct _FLContext
{
    FTList *windows;
    int     result;
};

static FLContext fl_context;

extern FTColor ft_color_r;
extern FTColor ft_color_g;

static const char *color_descs[] = {"Red", "Green", "Blue", "White", "Black"};

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

static void ft_color_window_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;

    if (event->type == FE_KEY_RELEASE ||
        event->type == FE_MOUSE_RELEASE)
    {
        fl_context.windows = ft_list_delete(fl_context.windows, window);
        ft_window_close(window);
    }
}

static void ft_color_window_timer(int signal)
{
    FTList *list = fl_context.windows;

    if (signal == SIGALRM && list)
    {
        FTWindow *window = list->data;

        fl_context.windows = ft_list_delete(list, window);
        ft_window_close(window);
    }
}

static FTWindow *ft_color_window_new(FTDrawGC *gc)
{
    FTWindow *window;
    FTWidget *widget;

    window = ft_window_new();
    widget = (FTWidget *)window;

    widget->gc = *gc;
    widget->draw = ft_color_window_draw;

    widget->handler = ft_color_window_handler;
    widget->data = window;

    fl_context.windows = ft_list_append(fl_context.windows, window);

    signal(SIGALRM, ft_color_window_timer);
    alarm(1); 

    return window;
}

static FTDrawGC ft_lcdcolor_parse(const char *color)
{
    FTDrawGC gc;
    int i;

    const FTColor colors[FT_N_ELEMENTS(color_descs)] = 
    {
        {0xff, 0, 0, 0},
        {0, 0xff, 0, 0},
        {0, 0, 0xff, 0},
        {0xff, 0xff, 0xff, 0},
        {0, 0, 0, 0}
    };

    memset(&gc, 0, sizeof(FTDrawGC));

    for (i = 0; i < FT_N_ELEMENTS(color_descs); i++)
    {
        if (strcmp(color, color_descs[i]) == 0)
        {
            gc.foreground = colors[i];
            break;
        }
    }

    return gc;
}

static void ft_lcdcolor_demo(FTButton *button, void *data)
{
    FTWindow *window;
    FTDrawGC gc;
    int i;

    for (i = 0; i < FT_N_ELEMENTS(color_descs); i++)
    {
        gc = ft_lcdcolor_parse(color_descs[i]);

        window = ft_color_window_new(&gc);
        ft_window_show(window);

        usleep(1000 * 1000);
    }
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

void ft_lcdcolor_destroy(FTWidget *widget)
{
    char key[32];

    sprintf(key, "%d", FT_ITEM_LCD);

    ft_config_set_int(key, fl_context.result);
    ft_window_destroy(widget);
}

FTWindow *ft_lcdcolor_new()
{
    FTWindow *window;
    FTWidget *widget;
    FTButton *button;
    FBSurface *s;
    FTRect *rect;

    window = ft_window_new();
    widget = (FTWidget *)window;

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

    s = widget->surface;

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

    widget->destroy = ft_lcdcolor_destroy;

    return window;
}

