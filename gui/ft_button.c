#include "ft_button.h"
#include "ft_draw.h"
#include <stdlib.h>

static void ft_button_draw(FTWidget *widget);
static void ft_button_destroy(FTWidget *widget);
static void ft_button_event_handler(FTEvent *event, void *data);

FTButton *ft_button_new(const char *text)
{
    FTButton *button = malloc(sizeof(FTButton));
    FTWidget *widget = (FTWidget *)button;

    memset(button, 0, sizeof(FTButton));

    ft_widget_init_default(widget);

    widget->type = FW_TYPE_BUTTON;
    widget->draw = ft_button_draw;
    widget->destroy = ft_button_destroy;
    widget->handler = ft_button_event_handler;
    widget->data = widget;

    button->text = strdup(text);

    return button;
}

void ft_button_set_handler(FTButton *button, FBHandler handler, void *data)
{
    button->handler = handler;
    button->data    = data;
}

void ft_button_set_text(FTButton *button, const char *text)
{
    free(button->text);

    button->text = strdup(text);

    ft_button_draw((FTWidget *)button);
}

static void ft_button_draw(FTWidget *widget)
{
    FTButton *button = (FTButton *)widget;
    FTPoint point;

    if (!widget->visible)
        return;

    ft_widget_draw(widget);

    point.x = widget->rect.x + FT_FONT_W;
    point.y = widget->rect.y + FT_FONT_W;

    ft_draw_text(widget->surface, &point, button->text, &widget->gc);
}

static void ft_button_destroy(FTWidget *widget)
{
    FTButton *button = (FTButton *)widget;
    FTDrawGC gc;

    gc.foreground = widget->gc.background;
    gc.background = widget->gc.background;

    ft_draw_box(widget->surface, &widget->rect, &gc, 1);

    free(button->text);
    free(button);
}

static void ft_button_event_handler(FTEvent *event, void *data)
{
    FTButton *button = (FTButton *)data;

    if (button->handler)
    {
        button->handler(button, button->data);
    }
}

