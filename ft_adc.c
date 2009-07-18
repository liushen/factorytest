#include "ft_adc.h"
#include "ft_textpad.h"
#include "hw/hw_comm.h"
#include <signal.h>

typedef struct _FAContext FAContext;

struct _FAContext
{
    FTWindow   *window;
};

static FAContext fa_context;

static void ft_adc_destroy(FTWidget *widget)
{
    FTWindow *window = (FTWindow *)widget;
    FTList *iter = window->children;

    for (; iter; iter = iter->next)
    {
        FTWidget *w = (FTWidget *)iter->data;

        w->destroy(w);
    }

    fa_context.window = NULL;

    free(window->buffer);
    free(window);
}

static void ft_adc_on_timer(int signal)
{
    FTWindow *window = fa_context.window;

    if (signal == SIGALRM && window)
    {
        const char *adc = hw_get_ADC();

        ft_textpad_set_text(window, adc);
        alarm(1); 
    }
}

FTWindow *ft_adc_new()
{
    FTWindow *window;
    FTWidget *widget;

    const char *adc = hw_get_ADC();

    window = ft_textpad_new(adc, 1);
    widget = (FTWidget *)window;

    widget->destroy = ft_adc_destroy;

    fa_context.window = window;
    ft_window_show(window);

    signal(SIGALRM, ft_adc_on_timer);
    alarm(1); 

    return window;
}

