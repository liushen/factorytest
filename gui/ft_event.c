#include "ft_event.h"
#include "ft_list.h"

typedef struct _FEContext FEContext;

struct _FEContext
{
    FTList         *events;
    FEKeyHandler    key_func;
    void           *key_data;
};

static FEContext fe_context;

FTEvent *ft_event_get()
{
    return NULL;
}

FTEvent *ft_event_get_last()
{
    return NULL;
}

void ft_event_put(FTEvent *e)
{
    if (e->type < FE_MOUSE_EVENT)
    {
        if (fe_context.key_func)
        {
            FTKeyEvent *event = (FTKeyEvent *)e;

            fe_context.key_func(event, fe_context.key_data);
        }
    }
    else
    {
        //TODO
    }
}

void ft_event_set_key_handler(FEKeyHandler func, void *data)
{
    fe_context.key_func = func;
    fe_context.key_data = data;
}

void ft_event_clean()
{
    return;
}

