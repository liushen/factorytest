
#include "ft_draw.h"
#include "libfbx-font-8x16.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX(x, y)   ((x)>(y))?(x):(y)
#define MIN(x, y)   ((x)<(y))?(x):(y)

#define FB_MAKE_COLOR_16(r, g, b) \
    (((r) >> 3) << 11 | ((g) >> 2) << 5 | ((b) >> 3))

int ft_draw_box(FBSurface *s, FTRect *rect, FTDrawGC *gc, int fill)
{
    FTPoint a, b, c, d;

    if (fill)
    {
        a.x = rect->x;
        a.y = rect->y;

        b.x = rect->x + rect->width - 1;
        b.y = rect->y;

        for (; a.y < rect->y + rect->height; a.y++, b.y++)
        {
            ft_draw_line(s, &a, &b, gc);
        }
    }
    else
    {
        a.x = rect->x;
        a.y = rect->y;

        b.x = rect->x + rect->width - 1;
        b.y = rect->y;

        c.x = rect->x + rect->width  - 1;
        c.y = rect->y + rect->height - 1;

        d.x = rect->x;
        d.y = rect->y + rect->height - 1;

        ft_draw_line(s, &a, &b, gc);
        ft_draw_line(s, &b, &c, gc);
        ft_draw_line(s, &c, &d, gc);
        ft_draw_line(s, &d, &a, gc);
    }

    return FT_SUCCESS;
}

int ft_draw_line(FBSurface *s, FTPoint *from, FTPoint *to, FTDrawGC *gc)
{
    assert (from->x == to->x || from->y == to->y);

    FTPoint p;
    int a, b, i;

    if (from->x == to->x)
    {
        a = MIN(from->y, to->y);
        b = MAX(from->y, to->y);
        p.x = from->x;

        for (i = a; i <= b; i++)
        {
            p.y = i;
            ft_draw_point(s, &p, gc);
        }
    }
    else
    {
        a = MIN(from->x, to->x);
        b = MAX(from->x, to->x);
        p.y = from->y;

        for (i = a; i <= b; i++)
        {
            p.x = i;
            ft_draw_point(s, &p, gc);
        }
    }

    return FT_SUCCESS;
}

int ft_draw_point(FBSurface *s, FTPoint *point, FTDrawGC *gc)
{
    FTColor fg = gc->foreground;
    int p, c16;

    p = (point->x + point->y * s->width) * s->depth;

    c16 = FB_MAKE_COLOR_16(fg.r, fg.g, fg.b);

    s->buffer[p] = c16 & 0xFF;
    s->buffer[p+1] = c16 >> 8;

    return FT_SUCCESS;
}

int ft_draw_char(FBSurface *s, FTPoint *point, char c, FTDrawGC *gc)
{
    static char buffer[FT_FONT_W * FT_FONT_H / 8];

    FTPoint p;
    int i, j;

    for (i = 0; i < FT_FONT_H; i++)
    {
        char ch = fb_fontdata_8x16[c * sizeof(buffer) + i];

        for (j = 0; j < 8; j++)
        {
            if ((ch >> j) & 0x1)
            {
                p.x = point->x + 7 - j;
                p.y = point->y + i;

                ft_draw_point(s, &p, gc);
            }
        }
    }

    return FT_SUCCESS;
}

int ft_draw_text(FBSurface *s, FTPoint *point, const char *text, FTDrawGC *gc)
{
    FTPoint p = *point;
    int len, i;

    if (!text || !*text)
    {
        return FT_SUCCESS;
    }

    len = strlen(text);

    for (i = 0; i < len; i++)
    {
        p.x = point->x + i * FT_FONT_W;

        ft_draw_char(s, &p, text[i], gc);
    }

    return FT_SUCCESS;
}

int ft_point_in_box(FTPoint *point, FTRect *rect)
{
    if (point->x < rect->x || point->y < rect->y)
    {
        return 0;
    }

    if (point->x > rect->x + rect->width || 
        point->y > rect->y + rect->height)
    {
        return 0;
    }

    return 1;
}

void ft_draw_cleanup(FBSurface *s)
{
    FTDrawGC gc;

    FTRect rect = 
    {
        .x = 0,
        .y = 0,
        .width = s->width,
        .height = s->height,
    };

    memset(&gc, 0, sizeof(FTDrawGC));

    ft_draw_box(s, &rect, &gc, 1);
}

