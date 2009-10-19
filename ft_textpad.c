#include "ft_textpad.h"
#include "ft_config.h"
#include "gui/ft_button.h"
#include "gui/ft_status_button.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FT_MAX_LINE     (10)

typedef struct _FTContext FTContext;

struct _FTContext
{
    char   *text;
    int     id;
    int     center;
    int     result;
};

static FTContext ft_context;

extern FTColor ft_color_r;
extern FTColor ft_color_g;

static void on_ok_handler(FTButton *button, void *data)
{
    ft_context.result = FT_STATUS_OK;

    ft_window_close(data);
}

static void on_fail_handler(FTButton *button, void *data)
{
    ft_context.result = FT_STATUS_FAIL;

    ft_window_close(data);
}

static char **ft_textpad_split(const char *text, int *len)
{
    char **lines = NULL;
    char *p = NULL;
    int i = 0;

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

    char **lines = NULL;
    int len = 0, i;

    gc.foreground = widget->gc.background;
    gc.background = widget->gc.background;

    ft_window_draw(widget);

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

void ft_textpad_destroy(FTWidget *widget)
{
    if (ft_context.id != -1)
    {
        char key[32];

        sprintf(key, "%d", ft_context.id);
        ft_config_set_int(key, ft_context.result);
    }

    ft_window_destroy(widget);
}

FTWindow *ft_textpad_new(const char *text, int center)
{
    FTWindow *window = ft_window_new();
    FTWidget *widget = (FTWidget *)window;
    FBSurface *s = widget->surface;
    FTButton *button = NULL;
    FTRect *rect = NULL;

    button = ft_button_new("       OK");

    rect = &((FTWidget *)button)->rect;
    rect->x = FT_WIDGET_SPACING;
    rect->y = s->height - FT_WIDGET_HEIGHT - FT_WIDGET_SPACING;
    rect->width = s->width / 2 - FT_WIDGET_SPACING;
    rect->height = FT_WIDGET_HEIGHT;

    ft_button_set_color(button, &ft_color_g);
    ft_button_set_handler(button, on_ok_handler, window);
    ft_window_add_child(window, (FTWidget *)button);

    button = ft_button_new("      FAIL");

    rect = &((FTWidget *)button)->rect;
    rect->x = s->width / 2 + FT_WIDGET_SPACING;
    rect->y = s->height - FT_WIDGET_HEIGHT - FT_WIDGET_SPACING;
    rect->width = s->width / 2 - FT_WIDGET_SPACING;
    rect->height = FT_WIDGET_HEIGHT;

    ft_button_set_color(button, &ft_color_r);
    ft_button_set_handler(button, on_fail_handler, window);
    ft_window_add_child(window, (FTWidget *)button);

    widget->draw = ft_textpad_draw;
    widget->destroy = ft_textpad_destroy;

    free(ft_context.text);

    ft_context.id = -1;
    ft_context.text = strdup(text);
    ft_context.center = center;
    ft_context.result = FT_STATUS_NORMAL;

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

void ft_textpad_set_id(FTWindow *textpad, int test_id)
{
    ft_context.id = test_id;
}

