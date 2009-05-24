#include "ft_window.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct _FWContext FWContext;

struct _FWContext
{
    FTList *windows;
};

static FWContext fw_context;

static void ft_window_event_handler(FTEvent *event, void *data);

FTWindow *ft_window_new()
{
    FTWindow *window = malloc(sizeof(FTWindow));
    FTWidget *widget = (FTWidget *)window;

    memset(window, 0, sizeof(window));

    ft_widget_init_default(widget);

    widget->type = FW_TYPE_WINDOW;
    widget->draw = ft_window_draw;

    widget->rect.x = 0;
    widget->rect.y = 0;
    widget->rect.width = widget->surface->width;
    widget->rect.height = widget->surface->height;

    window->buffer = malloc(widget->surface->size);

    window->handler = ft_window_event_handler;
    window->data = window;

    return window;
}

void ft_window_layout(FTWindow *window)
{
    FTWidget *w = (FTWidget *)window;
    FBSurface *s = w->surface;

    FTRect rect;
    int i = 0;

    FTList *iter = window->children;

    for (; iter; iter = iter->next, i++)
    {
        w = (FTWidget *)iter->data;

        if (!w->rect.width || !w->rect.height)
        {
            w->rect.x = (i % 2) ? (s->width / 2 + FT_FONT_W) : FT_FONT_W;
            w->rect.y = (i / 2) * (FT_FONT_H + FT_FONT_W * 3) + FT_FONT_W;

            w->rect.width = s->width / 2 - FT_FONT_W * 2;
            w->rect.height = FT_FONT_H + FT_FONT_W * 2;
        }
    }
}

void ft_window_draw(FTWidget *widget)
{
    FTWindow *window = (FTWindow *)widget;
    FTWidget *w;

    FTList *iter = window->children;

    for (; iter; iter = iter->next)
    {
        w = (FTWidget *)iter->data;

        w->draw(w);
    }

    memcpy(window->buffer, widget->surface->buffer, widget->surface->size);

    ft_event_set_key_handler(window->handler, window->data);

    window->focus = ft_window_get_focus(window);
}

int ft_window_add_child(FTWindow *window, FTWidget *widget)
{
    assert(widget != NULL);

    window->children = ft_list_append(window->children, widget);

    ft_window_layout(window);

    return FT_SUCCESS;
}

FTWidget *ft_window_get_focus(FTWindow *window)
{
    FTList *iter = window->children;
    FTWidget *widget = NULL;

    if (!iter)
        return NULL;

    for (; iter; iter = iter->next)
    {
        widget = (FTWidget *)iter->data;

        if (widget->focus) break;
    }

    return widget;
}

void ft_window_move_focus(FTWindow *window, int orient)
{
    FTList *iter = window->children;
    FTWidget *widget = NULL;

    if (!iter)
        return;

    if (orient > 0)
    {
        for (; iter; iter = iter->next)
        {
            widget = (FTWidget *)iter->data;

            if (widget->focus) break;
        }

        if (iter && iter->next)
            iter = iter->next;
        else
            iter = window->children;
    }
    else
    {
        iter = ft_list_last(iter);

        for (; iter; iter = iter->prev)
        {
            widget = (FTWidget *)iter->data;

            if (widget->focus) break;
        }

        if (iter && iter->prev)
            iter = iter->prev;
        else
            iter = ft_list_last(window->children);
    }

    if (widget) 
        ft_widget_unset_focus(widget);

    window->focus = (FTWidget *)iter->data;
    ft_widget_set_focus(window->focus);
}

void ft_window_close(FTWindow *window)
{
}

static void ft_window_event_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;

    if (event->type == FE_KEY_RELEASE)
    {
        FTKeyEvent *e = (FTKeyEvent *)event;

        switch (e->key)
        {
            case FT_KEY_DIAL:
                ft_window_move_focus(window, 1);
                break;

            case FT_KEY_END:
                ft_window_move_focus(window, -1);
                break;
        }
    }
}

