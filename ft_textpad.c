#include "ft_textpad.h"
#include <stdlib.h>
#include <string.h>

#define FT_MAX_LINE     (10)

typedef struct _FTContext FTContext;

struct _FTContext
{
    char   *text;
    int     center;
};

static FTContext ft_context;

static char **ft_textpad_split(const char *text, int *len)
{
    char **lines;
    char *p;
    int i;

    lines = calloc(FT_MAX_LINE + 1, sizeof(char *));

    p = strchr(text, '\n');

    if (!p)
    {
        *lines = strdup(text);
        *len = 1;

        return lines;
    }

    for (i = 0; i < FT_MAX_LINE; i++)
    {
        if (!p)
        {
            lines[i] = strdup(text);
            break;
        }

        lines[i] = malloc(p - text + 1);

        strncpy(lines[i], text, p - text);
        lines[i][p - text] = '\0';
        text = p + 1;

        p = strchr(text, '\n');
    }

    if (len)
       *len = i + 1;

    return lines;
}

static void ft_textpad_draw(FTWidget *widget)
{
    FBSurface *s = widget->surface;
    FTDrawGC gc = widget->gc;
    FTPoint point;

    char **lines;
    int len = 0, i;

    gc.foreground = widget->gc.background;
    gc.background = widget->gc.background;

    ft_draw_box(s, &widget->rect, &gc, 1);

    if (ft_context.text == NULL)
        return;

    point.x = FT_FONT_W;
    point.y = FT_FONT_W;

    lines = ft_textpad_split(ft_context.text, &len);

    if (ft_context.center)
        point.y = s->height / 2 - FT_FONT_H * len;

    for (i = 0; i < len; i++, point.y += FT_FONT_H * 2)
    {
        if (ft_context.center)
            point.x = (s->width - FT_FONT_W * strlen(lines[i])) / 2;

        ft_draw_text(s, &point, lines[i], &widget->gc);
    }

    for (i = 0; lines[i]; i++)
        free(lines[i]);

    free(lines);
}

FTWindow *ft_textpad_new(const char *text, int center)
{
    FTWindow *window = ft_window_new();
    FTWidget *widget = (FTWidget *)window;

    widget->draw = ft_textpad_draw;

    free(ft_context.text);

    ft_context.center = center;
    ft_context.text = strdup(text);

    return window;
}

void ft_textpad_set_text(FTWindow *textpad, const char *text)
{
    free(ft_context.text);

    ft_context.text = strdup(text);

    ft_textpad_draw((FTWidget *)textpad);
}

void ft_textpad_set_color(FTWindow *textpad, FTColor *color)
{
    FTWidget *w = (FTWidget *)textpad;

    w->gc.foreground = *color;

    ft_textpad_draw(w);
}

