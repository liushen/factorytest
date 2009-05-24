#include "ft_event.h"
#include "ft_list.h"

typedef struct _FEContext FEContext;

struct _FEContext
{
    FTList     *events;
    FEHandler   handler;
    void       *data;
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
        if (fe_context.handler)
        {
            fe_context.handler(e, fe_context.data);
        }
    }
    else
    {
        //TODO
    }
}

void ft_event_set_key_handler(FEHandler func, void *data)
{
    fe_context.handler = func;
    fe_context.data = data;
}

void ft_event_clean()
{
    return;
}

