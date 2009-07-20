#include "ft_keyboard.h"
#include "ft_type.h"
#include "gui/ft_button.h"

typedef struct _FKContext FKContext;

static const char *fk_all_labels[] = 
{
    "Back", "Send", "End", "OK", "Menu", 
    "Volume -", "Volume +", "Power"
};

static const int fk_all_keys[] = 
{
    FT_KEY_BACK, FT_KEY_SEND, FT_KEY_END, FT_KEY_OK, FT_KEY_MENU,
    FT_KEY_VOL_SUB, FT_KEY_VOL_ADD, FT_KEY_POWER
};

struct _FKContext
{
    FTWidget *buttons[FT_N_ELEMENTS(fk_all_keys)];
};

static FKContext fk_context;

static int ft_keyboard_get_index(int key)
{
    int i;

    for (i = 0; i < FT_N_ELEMENTS(fk_all_keys); i++)
    {
        if (fk_all_keys[i] == key)
            return i;
    }

    return -1;
}

static void ft_keyboard_handler(FTEvent *event, void *data)
{
    FTWindow *window = (FTWindow *)data;
    FTKeyEvent *ke;
    int id, i;

    if (event->type != FE_KEY_RELEASE)
        return;

    ke = (FTKeyEvent *)event;
    id = ft_keyboard_get_index(ke->key);

    if (id > -1)
    {
        ft_widget_set_visible(fk_context.buttons[id], 0);

        for (i = 0; i < FT_N_ELEMENTS(fk_context.buttons); i++)
        {
            if (fk_context.buttons[i]->visible)
                return;
        }

        ft_window_close(window);
    }
}

FTWindow *ft_keyboard_new()
{
    FTWindow *window;
    FTWidget *widget;
    FTWidget **buttons;
    int i;

    window = ft_window_new();
    widget = (FTWidget *)window;
    buttons = fk_context.buttons;

    for (i = 0; i < FT_N_ELEMENTS(fk_all_labels); i++)
    {
        buttons[i] = (FTWidget *)ft_button_new(fk_all_labels[i]);

        ft_window_add_child(window, buttons[i]);
    }

    widget->handler = ft_keyboard_handler;
    widget->data = window;

    return window;
}

