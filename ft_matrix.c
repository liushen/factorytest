
#include "ft_matrix.h"
#include "ft_config.h"
#include <stdio.h>
#include <string.h>

#define FM_DEFAULT_ROW      4
#define FM_DEFAULT_COL      3
#define FM_DEFAULT_SIZE     60

static FTRect ft_matrix_get_rect(FTWidget *widget, int col, int row)
{
    FBSurface *s = widget->surface;

    FTRect rect = {.width = FM_DEFAULT_SIZE, .height = FM_DEFAULT_SIZE};

    int w_margin = (s->width  / FM_DEFAULT_COL - FM_DEFAULT_SIZE) / 2;
    int h_margin = (s->height / FM_DEFAULT_ROW - FM_DEFAULT_SIZE) / 2;

    rect.x = (s->width  / FM_DEFAULT_COL) * col + w_margin;
    rect.y = (s->height / FM_DEFAULT_ROW) * row + h_margin;

    return rect;
}

static void ft_matrix_draw(FTWidget *widget)
{
    FBSurface *s = widget->surface;
    FTRect rect;
    int i, j;

    memset(s->buffer, 0, s->size);

    for (i = 0; i < FM_DEFAULT_COL; i++)
    {
        for (j = 0; j < FM_DEFAULT_ROW; j++)
        {
            rect = ft_matrix_get_rect(widget, i, j);

            ft_draw_box(s, &rect, &widget->gc, 0);
        }
    }
}

static void ft_matrix_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;
    FTWidget *widget = (FTWidget *)data;

    FBSurface *s = widget->surface;
    FTDrawGC gc;
    FTPoint point;
    FTRect rect;

    FTMouseEvent *me;
    FTKeyEvent *ke;
    char key[32];
    int i, j;

    static int num = FM_DEFAULT_COL * FM_DEFAULT_ROW;

    sprintf(key, "%d", FT_ITEM_MATRIX);

    if (event->type == FE_KEY_RELEASE)
    {
        ke = (FTKeyEvent *)event;

        if (ke->key == FT_KEY_BACK)
        {
            ft_config_set_int(key, FT_STATUS_FAIL);
            ft_window_close(window);
        }

        return;
    }

    if (event->type == FE_MOUSE_RELEASE)
    {
        me = (FTMouseEvent *)event;

        gc.foreground = widget->gc.background;
        gc.background = widget->gc.background;

        point.x = me->x;
        point.y = me->y;

        for (i = 0; i < FM_DEFAULT_COL; i++)
        {
            for (j = 0; j < FM_DEFAULT_ROW; j++)
            {
                rect = ft_matrix_get_rect(widget, i, j);

                if (ft_point_in_box(&point, &rect))
                {
                    ft_draw_box(s, &rect, &gc, 0);

                    if (--num <= 0)
                    {
                        num = FM_DEFAULT_COL * FM_DEFAULT_ROW;

                        ft_config_set_int(key, FT_STATUS_OK);
                        ft_window_close(window);
                    }

                    return;
                }
            }
        }
    }
}

FTWindow *ft_matrix_new()
{
    FTWindow *window = ft_window_new();
    FTWidget *widget = (FTWidget *)window;

    widget->draw = ft_matrix_draw;
    widget->handler = ft_matrix_handler;
    widget->data = window;

    return window;
}

