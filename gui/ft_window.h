#include "ft_list.h"
#include "ft_widget.h"

#ifndef _FT_WINDOW_H_
#define _FT_WINDOW_H_

typedef struct _FTWindow FTWindow;

struct _FTWindow
{
    FTWidget    widget;
    FTWidget   *focus;
    FTList     *children;
    char       *buffer;
};

FTWindow *ft_window_new();

int ft_window_add_child(FTWindow *window, FTWidget *child);

void ft_window_draw(FTWidget *widget);

void ft_window_move_focus(FTWindow *window, int orient);

void ft_window_close(FTWindow *window);

#endif/*_FT_WINDOW_H_*/