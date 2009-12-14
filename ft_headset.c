#include "ft_headset.h"
#include "ft_textpad.h"
#include "hw/hw_comm.h"
#include "gui/ft_button.h"

#include <stdio.h>
#include <signal.h>

typedef struct _FHContext FHContext;

struct _FHContext
{
    FTWindow   *window;
    FTButton   *button_hs;
    FTButton   *button_key;
    int         key;
};

static FHContext fh_context;

extern FTColor ft_color_r;
extern FTColor ft_color_g;

static void update_status(FTWindow *window)
{
    int state = hw_headset_get_state();
    char headset[128] = {0};

    sprintf(headset, "Headset: %s\nKey: %s", 
            state ? "On-Line" : "Off-Line", 
            fh_context.key ? "DOWN" : "UP");

    ft_button_set_color(fh_context.button_hs, 
            state ? &ft_color_g : &ft_color_r);
    ft_button_set_color(fh_context.button_key, 
            fh_context.key ? &ft_color_g : &ft_color_r);

    ft_textpad_set_text(window, headset);
}

static void ft_headset_destroy(FTWidget *widget)
{
    fh_context.window = NULL;
    ft_textpad_destroy(widget);
}

static void ft_headset_event_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;

    if (event->type == FE_KEY_PRESS || event->type == FE_KEY_RELEASE)
    {
        FTKeyEvent *ke = (FTKeyEvent *)event;

        if (ke->key == FT_KEY_PHONE)
        {
            fh_context.key = (event->type == FE_KEY_PRESS) ? 1 : 0;
            update_status(window);
        }
    }

    ft_window_event_handler(event, data);
}

static void ft_headset_on_timer(int signal)
{
    FTWindow *window = fh_context.window;

    if (signal == SIGALRM && window)
    {
        update_status(window);
        alarm(1); 
    }
}

FTWindow *ft_headset_new()
{
    FTWindow *window;
    FTButton *button;
    FTWidget *widget;

    window = ft_textpad_new("", 1);
    widget = (FTWidget *)window;

    fh_context.button_hs = ft_button_new("HEADSET");
    ft_window_add(window, (FTWidget *)fh_context.button_hs, 0);

    fh_context.button_key = ft_button_new("KEY");
    ft_window_add(window, (FTWidget *)fh_context.button_key, 1);

    widget->destroy = ft_headset_destroy;
    widget->handler = ft_headset_event_handler;
    widget->data = window;
    fh_context.window = window;
    fh_context.key = hw_headset_get_key();

    update_status(window);

    signal(SIGALRM, ft_headset_on_timer);
    alarm(1); 

    return window;
}

