#include "ft_headset.h"
#include "ft_textpad.h"
#include "hw/hw_comm.h"
#include <stdio.h>
#include <signal.h>

typedef struct _FHContext FHContext;

struct _FHContext
{
    FTWindow   *window;
};

static FHContext fh_context;

static void update_status(FTWindow *window)
{
    char headset[128] = {0};

    sprintf(headset, "Headset: %s\nKey: %s", 
            hw_headset_get_state() ? "IN" : "OUT", 
            hw_headset_get_key() ? "DOWN" : "UP");

    ft_textpad_set_text(window, headset);
}

static void ft_headset_destroy(FTWidget *widget)
{
    fh_context.window = NULL;
    ft_textpad_destroy(widget);
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
    FTWidget *widget;

    window = ft_textpad_new("", 1);
    widget = (FTWidget *)window;

    widget->destroy = ft_headset_destroy;
    fh_context.window = window;

    update_status(window);

    signal(SIGALRM, ft_headset_on_timer);
    alarm(1); 

    return window;
}

